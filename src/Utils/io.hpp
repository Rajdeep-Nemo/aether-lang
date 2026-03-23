#ifndef IO_HPP
#define IO_HPP

#include <string>
#include <cstdint>

// ============================================================================
// INPUT FUNCTIONS LIBRARY
// ============================================================================
// A comprehensive set of C++20 functions for safe, validated user input
// handling with support for all integer and floating-point types.
// ============================================================================

// ============================================================================
// 1. TRIM FUNCTIONS
// ============================================================================
/// Removes leading whitespace from a string
/// @param str The string to modify (modified in-place)
void trim_leading_spaces(std::string &str);

/// Removes trailing whitespace from a string
/// @param str The string to modify (modified in-place)
void trim_trailing_spaces(std::string &str);

/// Removes both leading and trailing whitespace from a string
/// @param str The string to modify (modified in-place)
void trim(std::string &str);

// ============================================================================
// 2. STRING INPUT FUNCTIONS
// ============================================================================
/// Reads a line of text from standard input with an optional prompt
/// @param prompt The text to display before reading (default: empty)
/// @return The input string (may contain spaces)
std::string input_str(const std::string &prompt);

/// Reads a line of text from standard input
/// @return The input string (may contain spaces)
std::string input_str();

// ============================================================================
// 3. CHARACTER INPUT FUNCTIONS
// ============================================================================
/// Reads a single character from input and validates exactly one character
/// Exits with code 1 on empty input or multiple characters
/// @return A single character
char input_char();

/// Reads a single character from input with a prompt
/// Exits with code 1 on empty input or multiple characters
/// @param prompt The text to display before reading
/// @return A single character
char input_char(const std::string &prompt);

// ============================================================================
// 4. BOOLEAN INPUT FUNCTIONS
// ============================================================================
/// Reads a boolean value (accepts "true" or "false", case-sensitive)
/// Exits with code 1 on invalid input
/// @return true if input is "true", false if input is "false"
bool input_bool();

/// Reads a boolean value with a prompt
/// Exits with code 1 on invalid input
/// @param prompt The text to display before reading
/// @return true if input is "true", false if input is "false"
bool input_bool(const std::string &prompt);

// ============================================================================
// 5. UNSIGNED INTEGER FUNCTIONS
// ============================================================================

// --- 8-bit Unsigned Integer (0 to 255) ---
/// Attempts to parse a string to uint8_t
/// @param str The string to parse
/// @param out The output variable (only modified on success)
/// @return true if parsing succeeded, false otherwise
bool parse_u8(const std::string &str, uint8_t &out);

/// Parses a string to uint8_t, exits on failure
/// @param str The string to parse
/// @return The parsed value
uint8_t parse_u8(const std::string &str);

/// Reads uint8_t from input with prompt
/// @param str The prompt to display
/// @return The parsed value
uint8_t input_u8(const std::string &str);

/// Reads uint8_t from input without prompt
/// @return The parsed value
uint8_t input_u8();

// --- 16-bit Unsigned Integer (0 to 65535) ---
/// Attempts to parse a string to uint16_t
bool parse_u16(const std::string &str, uint16_t &out);

/// Parses a string to uint16_t, exits on failure
uint16_t parse_u16(const std::string &str);

/// Reads uint16_t from input with prompt
uint16_t input_u16(const std::string &str);

/// Reads uint16_t from input without prompt
uint16_t input_u16();

// --- 32-bit Unsigned Integer (0 to 4294967295) ---
/// Attempts to parse a string to uint32_t
bool parse_u32(const std::string &str, uint32_t &out);

/// Parses a string to uint32_t, exits on failure
uint32_t parse_u32(const std::string &str);

/// Reads uint32_t from input with prompt
uint32_t input_u32(const std::string &str);

/// Reads uint32_t from input without prompt
uint32_t input_u32();

// --- 64-bit Unsigned Integer (0 to 18446744073709551615) ---
/// Attempts to parse a string to uint64_t
bool parse_u64(const std::string &str, uint64_t &out);

/// Parses a string to uint64_t, exits on failure
uint64_t parse_u64(const std::string &str);

/// Reads uint64_t from input with prompt
uint64_t input_u64(const std::string &str);

/// Reads uint64_t from input without prompt
uint64_t input_u64();

// ============================================================================
// 6. SIGNED INTEGER FUNCTIONS
// ============================================================================

// --- 8-bit Signed Integer (-128 to 127) ---
/// Attempts to parse a string to int8_t
bool parse_i8(const std::string &str, int8_t &out);

/// Parses a string to int8_t, exits on failure
int8_t parse_i8(const std::string &str);

/// Reads int8_t from input with prompt
int8_t input_i8(const std::string &str);

/// Reads int8_t from input without prompt
int8_t input_i8();

// --- 16-bit Signed Integer (-32768 to 32767) ---
/// Attempts to parse a string to int16_t
bool parse_i16(const std::string &str, int16_t &out);

/// Parses a string to int16_t, exits on failure
int16_t parse_i16(const std::string &str);

/// Reads int16_t from input with prompt
int16_t input_i16(const std::string &str);

/// Reads int16_t from input without prompt
int16_t input_i16();

// --- 32-bit Signed Integer (-2147483648 to 2147483647) ---
/// Attempts to parse a string to int32_t
bool parse_i32(const std::string &str, int32_t &out);

/// Parses a string to int32_t, exits on failure
int32_t parse_i32(const std::string &str);

/// Reads int32_t from input with prompt
int32_t input_i32(const std::string &str);

/// Reads int32_t from input without prompt
int32_t input_i32();

// --- 64-bit Signed Integer (-9223372036854775808 to 9223372036854775807) ---
/// Attempts to parse a string to int64_t
bool parse_i64(const std::string &str, int64_t &out);

/// Parses a string to int64_t, exits on failure
int64_t parse_i64(const std::string &str);

/// Reads int64_t from input with prompt
int64_t input_i64(const std::string &str);

/// Reads int64_t from input without prompt
int64_t input_i64();

// ============================================================================
// 7. FLOATING-POINT FUNCTIONS
// ============================================================================

// --- 32-bit Floating Point (single precision) ---
/// Attempts to parse a string to float
/// @param str The string to parse (supports standard decimal and scientific notation)
/// @param out The output variable (only modified on success)
/// @return true if parsing succeeded, false otherwise
bool parse_f32(const std::string &str, float &out);

/// Parses a string to float, exits on failure
/// @param str The string to parse
/// @return The parsed value
float parse_f32(const std::string &str);

/// Reads float from input with prompt
/// @param str The prompt to display
/// @return The parsed value
float input_f32(const std::string &str);

/// Reads float from input without prompt
/// @return The parsed value
float input_f32();

// --- 64-bit Floating Point (double precision) ---
/// Attempts to parse a string to double
/// @param str The string to parse (supports standard decimal and scientific notation)
/// @param out The output variable (only modified on success)
/// @return true if parsing succeeded, false otherwise
bool parse_f64(const std::string &str, double &out);

/// Parses a string to double, exits on failure
/// @param str The string to parse
/// @return The parsed value
double parse_f64(const std::string &str);

/// Reads double from input with prompt
/// @param str The prompt to display
/// @return The parsed value
double input_f64(const std::string &str);

/// Reads double from input without prompt
/// @return The parsed value
double input_f64();

#endif // IO_HPP
