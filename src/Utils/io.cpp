#include "io.hpp"
#include <iostream>
#include <charconv>
#include <cstdint>
#include <format>
#include <string>
#include <cctype>

// 1. TRIM FUNCTIONS -----------------------------------------------------
// --------------------------------------------------------------------
// Trim leading white spaces
void trim_leading_spaces(std::string &str)
{
    size_t i = 0;
    while (i < str.size() && isspace(static_cast<unsigned char>(str[i])))
    {
        i = i + 1;
    }
    str.erase(0 , i);
}
// Trim trailing white spaces
void trim_trailing_spaces(std::string &str)
{
    while (!str.empty() && isspace(static_cast<unsigned char>(str.back())))
    {
        str.pop_back();
    }
}
// Combines both trimming functions as one
void trim(std::string &str)
{
    trim_leading_spaces(str);
    trim_trailing_spaces(str);
}

// 2. STRING INPUT FUNCTIONS -----------------------------------------------
// ----------------------------------------------------------------------
// Takes string input with prompt
std::string input_str(const std::string &prompt)
{
    std::string input;
    std::cout << prompt << std::flush;
    std::getline(std::cin , input);
    return input;
}
// Takes string input
std::string input_str()
{
    return input_str("");
}

// 3. CHAR INPUT FUNCTIONS -----------------------------------------------
// ----------------------------------------------------------------------
// Takes char input
char input_char()
{
    std::string input;
    std::getline(std::cin , input);
    if (input.empty())
    {
        std::cerr << "Empty input." << std::endl;
        std::exit(1);
    }
    if (input.length() > 1)
    {
        std::cerr << "Multiple characters found." << std::endl;
        std::exit(1);
    }
    return input.at(0);
}
// Takes char input with prompt
char input_char(const std::string &prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin , input);
    if (input.empty())
    {
        std::cerr << "Empty input." << std::endl;
        std::exit(1);
    }
    if (input.length() > 1)
    {
        std::cerr << "Multiple characters found." << std::endl;
        std::exit(1);
    }
    return input.at(0);
}

// 4. BOOL INPUT FUNCTIONS -----------------------------------------------
// ----------------------------------------------------------------------
// Takes bool input
bool input_bool()
{
    std::string input;
    std::getline(std::cin , input);
    trim(input);
    if (input.empty())
    {
        std::cerr << "Empty input." << std::endl;
        std::exit(1);
    }
    if (input.length() > 5)
    {
        std::cerr << "Invalid value: '" << input <<"', value accepted: true / false" << std::endl;
        std::exit(1);
    }
    if (input == "true")
    {
        return true;
    }
    if (input == "false")
    {
        return false;
    }
    std::cerr << "Invalid value: '" << input <<"', value accepted: true / false" << std::endl;
    std::exit(1);
}
// Takes bool input with prompt
bool input_bool(const std::string &prompt)
{
    std::string input;
    std::cout << prompt;
    std::getline(std::cin , input);
    trim(input);
    if (input.empty())
    {
        std::cerr << "Empty input." << std::endl;
        std::exit(1);
    }
    if (input.length() > 5)
    {
        std::cerr << "Invalid value: '" << input <<"' Value accepted: true / false" << std::endl;
        std::exit(1);
    }
    if (input == "true")
    {
        return true;
    }
    if (input == "false")
    {
        return false;
    }
    std::cerr << "Invalid value: '" << input <<"' Value accepted: true / false" << std::endl;
    std::exit(1);
}

// 5. UNSIGNED INT INPUT FUNCTIONS --------------------------------------------------------
// ------------------------------------------------------------------------------------------
// Template function
template <typename T>
bool parse_uint(const std::string &str , T &out)
{
    static_assert(std::is_unsigned_v<T> && std::is_integral_v<T> , "parse_uint only supports unsigned integer types");

    std::string copy = str;
    trim(copy);
    if (copy.empty())
    {
        return false;
    }
    T temp;
    auto [ptr , ec] = std::from_chars(copy.data() , copy.data() + copy.size() , temp , 10);
    if (ec == std::errc() && ptr == copy.data() + copy.size())
    {
        out = temp;
        return true;
    }
    return false;
}

// 8-BIT UNSIGNED INTEGER INPUT FUNCTIONS -------------------------------------------
// --------------------------------------------------------------------------
// Parses a string to 8-bit unsigned integer and returns true if success and false if failure
bool parse_u8(const std::string &str , uint8_t &out)
{
    return parse_uint(str , out);
}
// Parses a string and returns the parsed value, crashes on failure
uint8_t parse_u8(const std::string &str)
{
    std::string copy = str;
    trim(copy);
    if (copy.empty())
    {
        std::cerr << "Empty input." << std::endl;
        std::exit(1);
    }
    uint8_t value;
    auto [ptr , ec] = std::from_chars(copy.data() , copy.data() + copy.size() , value , 10);
    if (ec == std::errc::result_out_of_range) {
        std::cerr << "Out of range: " << copy << std::endl;
        std::exit(1);
    }
    if (ec == std::errc() &&  ptr == copy.data() + copy.size())
    {
        return value;
    }
    std::cerr << "Invalid input: '" << copy << "' is not a valid 8-bit unsigned integer." << std::endl;
    std::exit(1);
}
// Combines input_str() and parse_u8() with prompt
uint8_t input_u8(const std::string &str)
{
    return parse_u8(input_str(str));
}
// Combines input_str() and parse_u8() without prompt
uint8_t input_u8()
{
    return parse_u8(input_str());
}

// 16-BIT UNSIGNED INTEGER INPUT FUNCTIONS -------------------------------------------
// --------------------------------------------------------------------------
// Parses a string to 16-bit unsigned integer and returns true if success and false if failure
bool parse_u16(const std::string &str , uint16_t &out)
{
    return parse_uint(str , out);
}
// Parses a string and returns the parsed value, crashes on failure
uint16_t parse_u16(const std::string &str)
{
    std::string copy = str;
    trim(copy);
    if (copy.empty())
    {
        std::cerr << "Empty input." << std::endl;
        std::exit(1);
    }
    uint16_t value;
    auto [ptr , ec] = std::from_chars(copy.data() , copy.data() + copy.size() , value , 10);
    if (ec == std::errc::result_out_of_range) {
        std::cerr << "Out of range: " << copy << std::endl;
        std::exit(1);
    }
    if (ec == std::errc() &&  ptr == copy.data() + copy.size())
    {
        return value;
    }
    std::cerr << "Invalid input: '" << copy << "' is not a valid 16-bit unsigned integer." << std::endl;
    std::exit(1);
}
// Combines input_str() and parse_u16() with prompt
uint16_t input_u16(const std::string &str)
{
    return parse_u16(input_str(str));
}
// Combines input_str() and parse_u16() without prompt
uint16_t input_u16()
{
    return parse_u16(input_str());
}

// 32-BIT UNSIGNED INTEGER INPUT FUNCTIONS -------------------------------------------
// --------------------------------------------------------------------------
// Parses a string to 32-bit unsigned integer and returns true if success and false if failure
bool parse_u32(const std::string &str , uint32_t &out)
{
    return parse_uint(str , out);
}
// Parses a string and returns the parsed value, crashes on failure
uint32_t parse_u32(const std::string &str)
{
    std::string copy = str;
    trim(copy);
    if (copy.empty())
    {
        std::cerr << "Empty input." << std::endl;
        std::exit(1);
    }
    uint32_t value;
    auto [ptr , ec] = std::from_chars(copy.data() , copy.data() + copy.size() , value , 10);
    if (ec == std::errc::result_out_of_range) {
        std::cerr << "Out of range: " << copy << std::endl;
        std::exit(1);
    }
    if (ec == std::errc() &&  ptr == copy.data() + copy.size())
    {
        return value;
    }
    std::cerr << "Invalid input: '" << copy << "' is not a valid 32-bit unsigned integer." << std::endl;
    std::exit(1);
}
// Combines input_str() and parse_u32() with prompt
uint32_t input_u32(const std::string &str)
{
    return parse_u32(input_str(str));
}
// Combines input_str() and parse_u32() without prompt
uint32_t input_u32()
{
    return parse_u32(input_str());
}

// 64-BIT UNSIGNED INTEGER INPUT FUNCTIONS -------------------------------------------
// --------------------------------------------------------------------------
// Parses a string to 64-bit unsigned integer and returns true if success and false if failure
bool parse_u64(const std::string &str , uint64_t &out)
{
    return parse_uint(str , out);
}
// Parses a string and returns the parsed value, crashes on failure
uint64_t parse_u64(const std::string &str)
{
    std::string copy = str;
    trim(copy);
    if (copy.empty())
    {
        std::cerr << "Empty input." << std::endl;
        std::exit(1);
    }
    uint64_t value;
    auto [ptr , ec] = std::from_chars(copy.data() , copy.data() + copy.size() , value , 10);
    if (ec == std::errc::result_out_of_range) {
        std::cerr << "Out of range: " << copy << std::endl;
        std::exit(1);
    }
    if (ec == std::errc() &&  ptr == copy.data() + copy.size())
    {
        return value;
    }
    std::cerr << "Invalid input: '" << copy << "' is not a valid 64-bit unsigned integer." << std::endl;
    std::exit(1);
}
// Combines input_str() and parse_u64() with prompt
uint64_t input_u64(const std::string &str)
{
    return parse_u64(input_str(str));
}
// Combines input_str() and parse_u64() without prompt
uint64_t input_u64()
{
    return parse_u64(input_str());
}

// 6. SIGNED INT INPUT FUNCTIONS-----------------------------------------------------------
// ------------------------------------------------------------------------------------------
// Template function
template <typename T>
bool parse_int(const std::string &str , T &out)
{
    std::string copy = str;
    trim(copy);
    if (copy.empty())
    {
        return false;
    }
    T temp;
    auto [ptr , ec] = std::from_chars(copy.data() , copy.data() + copy.size() , temp , 10);
    if (ec == std::errc() && ptr == copy.data() + copy.size())
    {
        out = temp;
        return true;
    }
    return false;
}

// 8-BIT INTEGER INPUT FUNCTIONS -------------------------------------------
// --------------------------------------------------------------------------
// Parses a string to 8-bit integer and returns true if success and false if failure
bool parse_i8(const std::string &str , int8_t &out)
{
    return parse_int(str , out);
}
// Parses a string and returns the parsed value, crashes on failure
int8_t parse_i8(const std::string &str)
{
    std::string copy = str;
    trim(copy);
    if (copy.empty())
    {
        std::cerr << "Empty input." << std::endl;
        std::exit(1);
    }
    int8_t value;
    auto [ptr , ec] = std::from_chars(copy.data() , copy.data() + copy.size() , value , 10);
    if (ec == std::errc::result_out_of_range) {
        std::cerr << "Out of range: " << copy << std::endl;
        std::exit(1);
    }
    if (ec == std::errc() &&  ptr == copy.data() + copy.size())
    {
        return value;
    }
    std::cerr << "Invalid input: '" << copy << "' is not a valid 8-bit integer." << std::endl;
    std::exit(1);
}
// Combines input_str() and parse_i8() with prompt
int8_t input_i8(const std::string &str)
{
    return parse_i8(input_str(str));
}
// Combines input_str() and parse_i8() without prompt
int8_t input_i8()
{
    return parse_i8(input_str());
}

// 16-BIT INTEGER INPUT FUNCTIONS -------------------------------------------
// --------------------------------------------------------------------------
// Parses a string to 16-bit integer and returns true if success and false if failure
bool parse_i16(const std::string &str , int16_t &out)
{
    return parse_int(str , out);
}
// Parses a string and returns the parsed value, crashes on failure
int16_t parse_i16(const std::string &str)
{
    std::string copy = str;
    trim(copy);
    if (copy.empty())
    {
        std::cerr << "Empty input." << std::endl;
        std::exit(1);
    }
    int16_t value;
    auto [ptr , ec] = std::from_chars(copy.data() , copy.data() + copy.size() , value , 10);
    if (ec == std::errc::result_out_of_range) {
        std::cerr << "Out of range: " << copy << std::endl;
        std::exit(1);
    }
    if (ec == std::errc() &&  ptr == copy.data() + copy.size())
    {
        return value;
    }
    std::cerr << "Invalid input: '" << copy << "' is not a valid 16-bit integer." << std::endl;
    std::exit(1);
}
// Combines input_str() and parse_i16() with prompt
int16_t input_i16(const std::string &str)
{
    return parse_i16(input_str(str));
}
// Combines input_str() and parse_i16() without prompt
int16_t input_i16()
{
    return parse_i16(input_str());
}

// 32-BIT INTEGER INPUT FUNCTIONS -------------------------------------------
// --------------------------------------------------------------------------
// Parses a string to 32-bit integer and returns true if success and false if failure
bool parse_i32(const std::string &str , int32_t &out)
{
    return parse_int(str , out);
}
// Parses a string and returns the parsed value, crashes on failure
int32_t parse_i32(const std::string &str)
{
    std::string copy = str;
    trim(copy);
    if (copy.empty())
    {
        std::cerr << "Empty input." << std::endl;
        std::exit(1);
    }
    int32_t value;
    auto [ptr , ec] = std::from_chars(copy.data() , copy.data() + copy.size() , value , 10);
    if (ec == std::errc::result_out_of_range) {
        std::cerr << "Out of range: " << copy << std::endl;
        std::exit(1);
    }
    if (ec == std::errc() &&  ptr == copy.data() + copy.size())
    {
        return value;
    }
    std::cerr << "Invalid input: '" << copy << "' is not a valid 32-bit integer." << std::endl;
    std::exit(1);
}
// Combines input_str() and parse_i32() with prompt
int32_t input_i32(const std::string &str)
{
    return parse_i32(input_str(str));
}
// Combines input_str() and parse_i32() without prompt
int32_t input_i32()
{
    return parse_i32(input_str());
}

// 64-BIT INTEGER INPUT FUNCTIONS -------------------------------------------
// --------------------------------------------------------------------------
// Parses a string to 64-bit integer and returns true if success and false if failure
bool parse_i64(const std::string &str , int64_t &out)
{
    return parse_int(str , out);
}
// Parses a string and returns the parsed value, crashes on failure
int64_t parse_i64(const std::string &str)
{
    std::string copy = str;
    trim(copy);
    if (copy.empty())
    {
        std::cerr << "Empty input." << std::endl;
        std::exit(1);
    }
    int64_t value;
    auto [ptr , ec] = std::from_chars(copy.data() , copy.data() + copy.size() , value , 10);
    if (ec == std::errc::result_out_of_range) {
        std::cerr << "Out of range: " << copy << std::endl;
        std::exit(1);
    }
    if (ec == std::errc() &&  ptr == copy.data() + copy.size())
    {
        return value;
    }
    std::cerr << "Invalid input: '" << copy << "' is not a valid 64-bit integer." << std::endl;
    std::exit(1);
}
// Combines input_str() and parse_i64() with prompt
int64_t input_i64(const std::string &str)
{
    return parse_i64(input_str(str));
}
// Combines input_str() and parse_i64() without prompt
int64_t input_i64()
{
    return parse_i64(input_str());
}

// 7. 32-BIT FLOATING POINT INPUT FUNCTIONS ------------------------------------
// --------------------------------------------------------------------------
// Parses a string to 32-bit floating point and returns true if success and false if failure
bool parse_f32(const std::string &str , float &out)
{
    std::string copy = str;
    trim(copy);
    if (copy.empty())
    {
        return false;
    }
    float temp;
    auto [ptr,ec] = std::from_chars(copy.data() , copy.data() + copy.size() , temp ,std::chars_format::general);
    if (ec == std::errc() && ptr == copy.data() + copy.size())
    {
        out = temp;
        return true;
    }
    return false;
}
// Parses a string and returns the parsed value, crashes on failure
float parse_f32(const std::string &str)
{
    std::string copy = str;
    trim(copy);
    if (copy.empty())
    {
        std::cerr << "Empty input." << std::endl;
        std::exit(1);
    }
    float value;
    auto [ptr , ec] = std::from_chars(copy.data() , copy.data() + copy.size() , value , std::chars_format::general);
    if (ec == std::errc::result_out_of_range) {
        std::cerr << "Out of range: '" << copy << "' is out of 32-bit floating point range." << std::endl;
        std::exit(1);
    }
    if (ec == std::errc() &&  ptr == copy.data() + copy.size())
    {
        return value;
    }
    std::cerr << "Invalid input: '" << copy << "' is not a valid 32-bit floating point." << std::endl;
    std::exit(1);
}
// Combines input_str() and parse_f32() with prompt
float input_f32(const std::string &str)
{
    return parse_f32(input_str(str));
}
// Combines input_str() and parse_f32() without prompt
float input_f32()
{
    return parse_f32(input_str());
}

// 8. 64-BIT FLOATING POINT INPUT FUNCTIONS ------------------------------------
// --------------------------------------------------------------------------
// Parses a string to 64-bit floating point and returns true if success and false if failure
bool parse_f64(const std::string &str , double &out)
{
    std::string copy = str;
    trim(copy);
    if (copy.empty())
    {
        return false;
    }
    double temp;
    auto [ptr,ec] = std::from_chars(copy.data() , copy.data() + copy.size() , temp ,std::chars_format::general);
    if (ec == std::errc() && ptr == copy.data() + copy.size())
    {
        out = temp;
        return true;
    }
    return false;
}
// Parses a string and returns the parsed value, crashes on failure
double parse_f64(const std::string &str)
{
    std::string copy = str;
    trim(copy);
    if (copy.empty())
    {
        std::cerr << "Empty input." << std::endl;
        std::exit(1);
    }
    double value;
    auto [ptr , ec] = std::from_chars(copy.data() , copy.data() + copy.size() , value , std::chars_format::general);
    if (ec == std::errc::result_out_of_range) {
        std::cerr << "Out of range: '" << copy << "' is out of 64-bit floating point range." << std::endl;
        std::exit(1);
    }
    if (ec == std::errc() &&  ptr == copy.data() + copy.size())
    {
        return value;
    }
    std::cerr << "Invalid input: '" << copy << "' is not a valid 64-bit floating point." << std::endl;
    std::exit(1);
}
// Combines input_str() and parse_f64() with prompt
double input_f64(const std::string &str)
{
    return parse_f64(input_str(str));
}
// Combines input_str() and parse_f64() without prompt
double input_f64()
{
    return parse_f64(input_str());
}