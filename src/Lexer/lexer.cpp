#include "lexer.hpp"
#include "token.hpp"
#include <cassert>
#include <format>
#include <fstream>
#include <iostream>
#include <memory>
// Function to initialize our scanner
void init_scanner(std::string src) {
    scanner.source = std::move(src);
    scanner.start = scanner.source.c_str();
    scanner.current = scanner.source.c_str();
    scanner.line = 1;
}
// Function that checks if we read the complete file or not
static bool is_at_end() {
    if (*scanner.current == '\0') {
        return true;
    }
    return false;
}
// Function that moves the pointer 'current' forward
static char advance() {
    if (is_at_end()) {
        return '\0';
    }
    return *scanner.current++;
}
// Function to check the next character
static char peek() {
    return *scanner.current;
}
// Function to check the second next character
static char peek_next() {
    if (*scanner.current == '\0') {
        return '\0';
    }
    return scanner.current[1];
}
// Function to skip whitespace characters
void skip_whitespace() {
    while (true) {
        switch (peek()) {
        case ' ':
        case '\t':
        case '\r':
            advance();
            break;
        case '\n':
            scanner.line++;
            advance();
            break;
        case '/':
            if (peek_next() == '/') {
                // Consumes the two '/' before entering loop
                advance();
                advance();
                while (!is_at_end() && peek() != '\n') {
                    advance();
                }
            } else {
                return;
            }
            break;
        default:
            return;
        }
    }
}
// Function to create a token
Token create_token(const TokenType token_type) {
    Token token;
    token.type = token_type;
    token.lexeme = std::string(scanner.start, scanner.current - scanner.start);
    token.line = scanner.line;
    return token;
}
// Function for error reporting
Token error_token(const char *message) {
    Token token;
    token.type = TokenType::ERROR_TOKEN;
    token.lexeme = std::string(message);
    token.line = scanner.line;
    return token;
}
// Function to read input file into a buffer
std::string read_file(const char *path) {
    // Opens the file
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    // Checks if the file exists
    if (!file) {
        std::cerr << "Could not open file " << path << '\n';
        std::exit(1);
    }
    // fileSize contains the size of the file
    const std::streamsize file_size = file.tellg();
    // Move the cursor back to start
    file.seekg(0, std::ios::beg);
    // Checks if fileSize is non-negative, means size is found without any error
    if (file_size < 0) {
        std::cerr << "Could not determine size of file " << path << '\n';
        std::exit(1);
    }
    // Buffer to store the file contents
    std::string buffer(file_size, '\0');
    file.read(buffer.data(), file_size);
    // Checks if reading was successful
    if (file.gcount() < file_size) {
        std::cerr << "Could not read file " << path << '\n';
        std::exit(1);
    }
    // Returns the buffer
    return buffer;
}
// Helper function to evaluate conditional advances - '!=' , '=='
bool match(const char expected) {
    if (is_at_end())
        return false;
    if (*scanner.current != expected)
        return false;
    scanner.current++;
    return true;
}
// Helper function to check if it is a digit
bool is_digit(const char c) {
    return c >= '0' && c <= '9';
}
// Helper function to check if it is an alphabet or underscore
bool is_alpha(const char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}
// Helper function to check if it is a character literal (Inside single quotes)
static Token is_char_literal() {
    // 1. Check for empty literal ''
    if (is_at_end()) {
        return error_token("Unterminated character literal.");
    }
    if (peek() == '\'') {
        return error_token("Empty character literal.");
    }
    // 2. Handle Escape Sequence
    if (peek() == '\\') {
        advance(); // consume '\'
        switch (peek()) {
        case '\'':
        case '"':
        case '\\':
        case 'n':
        case '{':
        case '}':
        case 't':
        case 'r':
        case '0':
            advance(); // consume the valid escaped char
            break;
        default:
            return error_token("Invalid escape sequence in character literal.");
        }
    }
    // 3. Handle Regular Character
    else {
        if (is_at_end()) {
            return error_token("Unterminated character literal.");
        }
        advance();
    }
    // 4. Ensure it closes correctly immediately after the character
    if (is_at_end()) {
        return error_token("Unterminated character literal.");
    }
    if (peek() != '\'') {
        return error_token("Character literal must contain exactly one character.");
    }
    // Consume the closing '
    if (is_at_end()) {
        return error_token("Unterminated character literal.");
    }
    advance();
    return create_token(TokenType::CHAR_LITERAL);
}
// Helper function to check if it is a string literal (Inside double quotes)
static Token is_string_literal() {
    while (peek() != '"' && !is_at_end()) {
        // If newline then line count is increased, thus allowing multiline string
        if (peek() == '\n') {
            scanner.line++;
            advance();
        }
        // To check escape sequence
        else if (peek() == '\\') {
            advance();
            if (is_at_end()) {
                return error_token("Unterminated string after escape.");
            }
            switch (peek()) {
                // Specified escape character list
            case '\'':
            case '"':
            case '\\':
            case 'n':
            case '{':
            case '}':
            case 't':
            case 'r':
            case '0':
                advance(); // Valid escape sequence, consume the char
                break;
            default:
                return error_token("Invalid escape sequence.");
            }
        } else {
            advance();
        }
    }
    if (is_at_end()) {
        return error_token("Unterminated string");
    }
    // Consume the closing quote
    advance();
    return create_token(TokenType::STRING_LITERAL);
}
// Helper function to check if it is a number literal (Integer literal of float literal)
static Token is_number_literal() {
    // Flag to check if an integer or float
    bool isFloat = false;
    // Consume digits
    while (is_digit(peek())) {
        advance();
    }
    // If a dot is found and the next character is also a digit it is considered a float
    if (peek() == '.' && is_digit(peek_next())) {
        isFloat = true;
        advance();
        while (is_digit(peek())) {
            advance();
        }
    }
    // Create a token based on flag
    return create_token(isFloat ? TokenType::FLOAT_LITERAL : TokenType::INT_LITERAL);
}
// Helper to check keyword
TokenType check_keyword(const int start, const int length, const char *rest, const TokenType type) {
    if (scanner.current - scanner.start == start + length &&
        std::string_view(scanner.start + start, length) == rest) {
        return type;
    }
    return TokenType::IDENTIFIER;
}
// Function to check the identifier type
static TokenType identifier_type() {
    switch (scanner.start[0]) {
        // Check break, bool
    case 'b':
        if (scanner.current - scanner.start > 1) {
            switch (scanner.start[1]) {
            case 'o':
                return check_keyword(2, 2, "ol", TokenType::BOOL);
            case 'r':
                return check_keyword(2, 3, "eak", TokenType::BREAK);
            default:;
            }
        }
        break;
        // Check char , const, continue
    case 'c':
        if (scanner.current - scanner.start > 1) {
            switch (scanner.start[1]) {
            case 'h':
                return check_keyword(2, 2, "ar", TokenType::CHAR);
            case 'o':
                if (scanner.current - scanner.start > 2 && scanner.start[2] == 'n') {
                    if (scanner.current - scanner.start > 3) {
                        switch (scanner.start[3]) {
                        case 's':
                            return check_keyword(4, 1, "t", TokenType::CONST);
                        case 't':
                            return check_keyword(4, 4, "inue", TokenType::CONTINUE);
                        default:;
                        }
                    }
                }
                break;
            default:;
            }
        }
        break;
        // Check do
    case 'd':
        return check_keyword(1, 1, "o", TokenType::DO);
        // Check else, enum
    case 'e':
        if (scanner.current - scanner.start > 1) {
            switch (scanner.start[1]) {
            case 'l':
                return check_keyword(2, 2, "se", TokenType::ELSE);
            case 'n':
                return check_keyword(2, 2, "um", TokenType::ENUM);
            default:;
            }
        }
        break;
        // Check false, fn, for, f32, f64
    case 'f':
        if (scanner.current - scanner.start > 1) {
            switch (scanner.start[1]) {
            case 'n':
                if (scanner.current - scanner.start == 2)
                    return TokenType::FN;
                break;
            case 'a':
                return check_keyword(2, 3, "lse", TokenType::FALSE);
            case 'o':
                return check_keyword(2, 1, "r", TokenType::FOR);
            case '3':
                return check_keyword(2, 1, "2", TokenType::F32);
            case '6':
                return check_keyword(2, 1, "4", TokenType::F64);
            default:;
            }
        }
        break;
        // Check if, in, i8, i16, i32, i64
    case 'i':
        if (scanner.current - scanner.start > 1) {
            switch (scanner.start[1]) {
            case 'f':
                if (scanner.current - scanner.start == 2)
                    return TokenType::IF;
                break;
            case 'n':
                if (scanner.current - scanner.start == 2)
                    return TokenType::IN;
                break;
            case '8':
                if (scanner.current - scanner.start == 2)
                    return TokenType::I8;
                break;
            case '1':
                return check_keyword(2, 1, "6", TokenType::I16);
            case '3':
                return check_keyword(2, 1, "2", TokenType::I32);
            case '6':
                return check_keyword(2, 1, "4", TokenType::I64);
            default:;
            }
        }
        break;
        // Check loop and let
    case 'l':
        if (scanner.current - scanner.start > 1) {
            switch (scanner.start[1]) {
            case 'o':
                return check_keyword(2, 2, "op", TokenType::LOOP);
            case 'e':
                return check_keyword(2, 1, "t", TokenType::LET);
            default:;
            }
        }
        break;
        // Check match
    case 'm':
        return check_keyword(1, 4, "atch", TokenType::MATCH);
        // Check NIL
    case 'N':
        return check_keyword(1, 2, "IL", TokenType::NIL_LITERAL);
        // Check return
    case 'r':
        return check_keyword(1, 5, "eturn", TokenType::RETURN);
        // Check string, struct, static
    case 's':
        if (scanner.current - scanner.start > 1) {
            switch (scanner.start[1]) {
            case 't':
                if (scanner.current - scanner.start > 2 && scanner.start[2] == 'r') {
                    if (scanner.current - scanner.start > 3) {
                        switch (scanner.start[3]) {
                        case 'i':
                            return check_keyword(4, 2, "ng", TokenType::STRING);
                        case 'u':
                            return check_keyword(4, 2, "ct", TokenType::STRUCT);
                        default:;
                        }
                    }
                }
                if (scanner.current - scanner.start > 2 && scanner.start[2] == 'a') {
                    if (scanner.current - scanner.start > 3) {
                        switch (scanner.start[3]) {
                        case 't':
                            return check_keyword(4, 2, "ic", TokenType::STATIC);
                        default:;
                        }
                    }
                }
            default:;
            }
        }
        break;
        // Check true
    case 't':
        return check_keyword(1, 3, "rue", TokenType::TRUE);
        // Check u8, u16, u32, u64, union
    case 'u':
        if (scanner.current - scanner.start > 1) {
            switch (scanner.start[1]) {
            case '8':
                if (scanner.current - scanner.start == 2)
                    return TokenType::U8;
                break;
            case '1':
                return check_keyword(2, 1, "6", TokenType::U16);
            case '3':
                return check_keyword(2, 1, "2", TokenType::U32);
            case '6':
                return check_keyword(2, 1, "4", TokenType::U64);
            case 'n':
                return check_keyword(2, 3, "ion", TokenType::UNION);
            default:;
            }
        }
        break;
        // Check void
    case 'v':
        return check_keyword(1, 3, "oid", TokenType::VOID);
        // Check while
    case 'w':
        return check_keyword(1, 4, "hile", TokenType::WHILE);

    default:;
    }
    return TokenType::IDENTIFIER;
}
// Helper function to check if it is an identifier (checks keywords as well)
static Token is_identifier() {
    while (is_alpha(peek()) || is_digit(peek())) {
        advance();
    }
    return create_token(identifier_type());
}
// Function to evaluate tokens
Token scan_token() {
    skip_whitespace();
    scanner.start = scanner.current;
    // If the pointer hits '\0' the program stops
    if (is_at_end())
        return create_token(TokenType::END_OF_FILE);

    switch (const char c = advance()) {
        // Single-character delimiters
    case '(':
        return create_token(TokenType::LEFT_PAREN);
    case ')':
        return create_token(TokenType::RIGHT_PAREN);
    case '{':
        return create_token(TokenType::LEFT_BRACE);
    case '}':
        return create_token(TokenType::RIGHT_BRACE);
    case '[':
        return create_token(TokenType::LEFT_BRACKET);
    case ']':
        return create_token(TokenType::RIGHT_BRACKET);
    case ',':
        return create_token(TokenType::COMMA);
    case ':':
        return create_token(TokenType::COLON);
    case ';':
        return create_token(TokenType::SEMICOLON);
    case '?':
        return create_token(TokenType::QUESTION);
    case '.':
        return create_token(match('.') ? TokenType::DOT_DOT : TokenType::DOT);
        // Arithmetic & Assignment Operators
    case '+':
        return create_token(match('=') ? TokenType::PLUS_EQUAL : TokenType::PLUS);
    case '*':
        return create_token(match('=') ? TokenType::STAR_EQUAL : TokenType::STAR);
    case '/':
        return create_token(match('=') ? TokenType::SLASH_EQUAL : TokenType::SLASH);
    case '%':
        return create_token(match('=') ? TokenType::PERCENT_EQUAL : TokenType::PERCENT);
        // Minus, Arrow, and Minus-Equal
    case '-':
        if (match('>'))
            return create_token(TokenType::ARROW);
        if (match('='))
            return create_token(TokenType::MINUS_EQUAL);
        return create_token(TokenType::MINUS);
        // Comparison & Assignment
    case '=':
        return create_token(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
    case '!':
        return create_token(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        // Less than, Less than equal, Left shift
    case '<':
        if (match('<'))
            return create_token(TokenType::LEFT_SHIFT);
        if (match('='))
            return create_token(TokenType::LESS_EQUAL);
        return create_token(TokenType::LESS);
        // Greater than, Greater equal, Right shift
    case '>':
        if (match('>'))
            return create_token(TokenType::RIGHT_SHIFT);
        if (match('='))
            return create_token(TokenType::GREATER_EQUAL);
        return create_token(TokenType::GREATER);
    case '&':
        return create_token(match('&') ? TokenType::AND : TokenType::BIT_AND);
    case '|':
        return create_token(match('|') ? TokenType::OR : TokenType::BIT_OR);
    case '^':
        return create_token(TokenType::BIT_XOR);
    case '~':
        return create_token(TokenType::BIT_NOT);

    case '\'':
        return is_char_literal();
    case '"':
        return is_string_literal();
    default:
        if (is_digit(c))
            return is_number_literal();
        if (is_alpha(c))
            return is_identifier();
        return error_token("Unexpected character.");
    }
}
// Function to manage the process
bool run_file(const char *path) {
    std::ifstream test(path);
    if (!test) {
        std::cerr << "File not found: " << path << '\n';
        return false;
    }
    init_scanner(read_file(path));
    return true;
}
