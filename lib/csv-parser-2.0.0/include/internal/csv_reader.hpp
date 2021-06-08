/** @file
 *  @brief Defines functionality needed for basic CSV parsing
 */

#pragma once

#include <algorithm>
#include <deque>
#include <fstream>
#include <iterator>
#include <memory>
#include <mutex>
#include <thread>
#include <sstream>
#include <string>
#include <vector>

#include "../external/mio.hpp"
#include "constants.hpp"
#include "data_type.h"
#include "csv_format.hpp"
#include "csv_reader_internals.hpp"
#include "compatibility.hpp"
#include "raw_csv_data.hpp"

/** The all encompassing namespace */
namespace csv {
    /** Stuff that is generally not of interest to end-users */
    namespace internals {
        std::string format_row(const std::vector<std::string>& row, csv::string_view delim = ", ");

        std::vector<std::string> _get_col_names( csv::string_view head, const CSVFormat format = CSVFormat::guess_csv());
    }

    std::vector<std::string> get_col_names(
        csv::string_view filename,
        const CSVFormat format = CSVFormat::guess_csv());

    /** Guess the delimiter used by a delimiter-separated values file */
    CSVGuessResult guess_format(csv::string_view filename,
        const std::vector<char>& delims = { ',', '|', '\t', ';', '^', '~' });

    /** @class CSVReader
     *  @brief Main class for parsing CSVs from files and in-memory sources
     *
     *  All rows are compared to the column names for length consistency
     *  - By default, rows that are too short or too long are dropped
     *  - Custom behavior can be defined by overriding bad_row_handler in a subclass
     */
    class CSVReader {
    public:
        /**
         * An input iterator capable of handling large files.
         * @note Created by CSVReader::begin() and CSVReader::end().
         *
         * @par Iterating over a file
         * @snippet tests/test_csv_iterator.cpp CSVReader Iterator 1
         *
         * @par Using with `<algorithm>` library
         * @snippet tests/test_csv_iterator.cpp CSVReader Iterator 2
         */
        class iterator {
        public:
            #ifndef DOXYGEN_SHOULD_SKIP_THIS
            using value_type = CSVRow;
            using difference_type = std::ptrdiff_t;
            using pointer = CSVRow * ;
            using reference = CSVRow & ;
            using iterator_category = std::input_iterator_tag;
            #endif

            iterator() = default;
            iterator(CSVReader* reader) : daddy(reader) {};
            iterator(CSVReader*, CSVRow&&);

            /** Access the CSVRow held by the iterator */
            CONSTEXPR reference operator*() { return this->row; }

            /** Return a pointer to the CSVRow the iterator has stopped at */
            CONSTEXPR pointer operator->() { return &(this->row); }

            iterator& operator++();   /**< Pre-increment iterator */
            iterator operator++(int); /**< Post-increment ierator */
            iterator& operator--();

            /** Returns true if iterators were constructed from the same CSVReader
             *  and point to the same row
             */
            CONSTEXPR bool operator==(const iterator& other) const noexcept {
                return (this->daddy == other.daddy) && (this->i == other.i);
            }

            CONSTEXPR bool operator!=(const iterator& other) const noexcept { return !operator==(other); }
        private:
            CSVReader * daddy = nullptr;  // Pointer to parent
            CSVRow row;                   // Current row
            size_t i = 0;               // Index of current row
        };

        /** @name Constructors
         *  Constructors for iterating over large files and parsing in-memory sources.
         */
         ///@{
        CSVReader(csv::string_view filename, CSVFormat format = CSVFormat::guess_csv());
        CSVReader(CSVFormat format = CSVFormat());
        ///@}

        CSVReader(const CSVReader&) = delete; // No copy constructor
        CSVReader(CSVReader&&) = default;     // Move constructor
        CSVReader& operator=(const CSVReader&) = delete; // No copy assignment
        CSVReader& operator=(CSVReader&& other) = default;
        ~CSVReader() {
            if (this->read_csv_worker.joinable()) {
                this->read_csv_worker.join();
            }
        }

        /** @name Reading In-Memory Strings
         *  You can piece together incomplete CSV fragments by calling feed() on them
         *  before finally calling end_feed().
         *
         *  Alternatively, you can also use the parse() shorthand function for
         *  smaller strings.
         */
         ///@{
        /** @name Reading In-Memory Strings */
        ///@{
        void feed(csv::string_view in);
        void end_feed();
        ///@}

        /** @name Retrieving CSV Rows */
        ///@{
        bool read_row(CSVRow &row);
        iterator begin();
        HEDLEY_CONST iterator end() const noexcept;
        ///@}

        /** @name CSV Metadata */
        ///@{
        CSVFormat get_format() const;
        std::vector<std::string> get_col_names() const;
        int index_of(csv::string_view col_name) const;
        ///@}
        
        /** @name CSV Metadata: Attributes */
        ///@{
        constexpr bool empty() const noexcept { return this->size() == 0; }
        constexpr size_t size() const noexcept { return this->n_rows; }

        /** Returns true if the CSV was prefixed with a UTF-8 bom */
        constexpr bool utf8_bom() const noexcept { return this->_utf8_bom; }
        ///@}

    protected:
        using RowCollection = internals::ThreadSafeDeque<CSVRow>;

        /**
         * \defgroup csv_internal CSV Parser Internals
         * @brief Internals of CSVReader. Only maintainers and those looking to
         *        extend the parser should read this.
         * @{
         */
         /** Returns true if we have reached end of file */
        constexpr bool eof() const noexcept { return this->mmap_eof; };

        /** Sets this reader's column names and associated data */
        void set_col_names(const std::vector<std::string>&);

        /** @name CSV Settings **/
        ///@{
        CSVFormat _format;
        ///@}

        /** @name Parser State */
        ///@{
        /** Pointer to a object containing column information */
        internals::ColNamesPtr col_names = std::make_shared<internals::ColNames>();

        /** Helper class which actually does the parsing */
        internals::BasicCSVParser parser = internals::BasicCSVParser(this->col_names);

        /** Queue of parsed CSV rows */
        RowCollection records;

        /** The number of columns in this CSV */
        size_t n_cols = 0;

        /** How many rows (minus header) have been parsed so far */
        size_t n_rows = 0;

        /** Set to true if UTF-8 BOM was detected */
        bool _utf8_bom = false;
        ///@}

        /** @name Multi-Threaded File Reading Functions */
        ///@{
        void feed_map(mio::mmap_source&& source);
        bool read_csv(size_t bytes = internals::ITERATION_CHUNK_SIZE);
        ///@}

        /**@}*/ // End of parser internals

    private:
        /** Whether or not an attempt to find Unicode BOM has been made */
        bool unicode_bom_scan = false;

        /** Whether or not rows before header were trimmed */
        bool header_trimmed = false;

        /** @name Multi-Threaded File Reading: Flags and State */
        ///@{
        std::string _filename = "";
        size_t file_size;
        bool mmap_eof = true;
        size_t mmap_pos = 0;
        std::thread read_csv_worker;
        ///@}

        void trim_utf8_bom(csv::string_view in);
        void trim_header();

        /** Set parse and whitespace flags */
        void set_parse_flags(const CSVFormat& format);
    };
}