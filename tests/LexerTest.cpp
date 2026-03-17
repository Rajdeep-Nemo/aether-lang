#include <gtest/gtest.h>
#include "../src/Lexer/lexer.hpp"
#include "../src/Lexer/token.hpp"
#include <vector>
#include <string>

class LexerTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code if needed
    }
    
    static std::vector<Token> tokenize(const std::string& source) {
        init_scanner(source);
        std::vector<Token> tokens;
        
        while (true) {
            Token token = scan_token();
            tokens.push_back(token);
            if (token.type == TokenType::END_OF_FILE || token.type == TokenType::ERROR_TOKEN) {
                break;
            }
        }
        return tokens;
    }
};

// Test single character tokens
TEST_F(LexerTest, SingleCharacterTokens) {
    auto tokens = tokenize("(){}[],;?.");
    
    EXPECT_EQ(tokens[0].type, TokenType::LEFT_PAREN);
    EXPECT_EQ(tokens[1].type, TokenType::RIGHT_PAREN);
    EXPECT_EQ(tokens[2].type, TokenType::LEFT_BRACE);
    EXPECT_EQ(tokens[3].type, TokenType::RIGHT_BRACE);
    EXPECT_EQ(tokens[4].type, TokenType::LEFT_BRACKET);
    EXPECT_EQ(tokens[5].type, TokenType::RIGHT_BRACKET);
    EXPECT_EQ(tokens[6].type, TokenType::COMMA);
    EXPECT_EQ(tokens[7].type, TokenType::SEMICOLON);
    EXPECT_EQ(tokens[8].type, TokenType::QUESTION);
    EXPECT_EQ(tokens[9].type, TokenType::DOT);
    EXPECT_EQ(tokens[10].type, TokenType::END_OF_FILE);
}

// Test arithmetic operators
TEST_F(LexerTest, ArithmeticOperators) {
    auto tokens = tokenize("+-*/%");
    
    EXPECT_EQ(tokens[0].type, TokenType::PLUS);
    EXPECT_EQ(tokens[1].type, TokenType::MINUS);
    EXPECT_EQ(tokens[2].type, TokenType::STAR);
    EXPECT_EQ(tokens[3].type, TokenType::SLASH);
    EXPECT_EQ(tokens[4].type, TokenType::PERCENT);
    EXPECT_EQ(tokens[5].type, TokenType::END_OF_FILE);
}

// Test assignment operators
TEST_F(LexerTest, AssignmentOperators) {
    auto tokens = tokenize("= += -= *= /= %= == !=");
    
    EXPECT_EQ(tokens[0].type, TokenType::EQUAL);
    EXPECT_EQ(tokens[1].type, TokenType::PLUS_EQUAL);
    EXPECT_EQ(tokens[2].type, TokenType::MINUS_EQUAL);
    EXPECT_EQ(tokens[3].type, TokenType::STAR_EQUAL);
    EXPECT_EQ(tokens[4].type, TokenType::SLASH_EQUAL);
    EXPECT_EQ(tokens[5].type, TokenType::PERCENT_EQUAL);
    EXPECT_EQ(tokens[6].type, TokenType::EQUAL_EQUAL);
    EXPECT_EQ(tokens[7].type, TokenType::BANG_EQUAL);
    EXPECT_EQ(tokens[8].type, TokenType::END_OF_FILE);
}

// Test comparison operators
TEST_F(LexerTest, ComparisonOperators) {
    auto tokens = tokenize("< <= > >= <= <");
    
    EXPECT_EQ(tokens[0].type, TokenType::LESS);
    EXPECT_EQ(tokens[1].type, TokenType::LESS_EQUAL);
    EXPECT_EQ(tokens[2].type, TokenType::GREATER);
    EXPECT_EQ(tokens[3].type, TokenType::GREATER_EQUAL);
    EXPECT_EQ(tokens[4].type, TokenType::LESS_EQUAL);
    EXPECT_EQ(tokens[5].type, TokenType::LESS);
    EXPECT_EQ(tokens[6].type, TokenType::END_OF_FILE);
}

// Test bitwise and logical operators
TEST_F(LexerTest, BitwiseAndLogicalOperators) {
    auto tokens = tokenize("& | ^ ~ && || ! << >>");
    
    EXPECT_EQ(tokens[0].type, TokenType::BIT_AND);
    EXPECT_EQ(tokens[1].type, TokenType::BIT_OR);
    EXPECT_EQ(tokens[2].type, TokenType::BIT_XOR);
    EXPECT_EQ(tokens[3].type, TokenType::BIT_NOT);
    EXPECT_EQ(tokens[4].type, TokenType::AND);
    EXPECT_EQ(tokens[5].type, TokenType::OR);
    EXPECT_EQ(tokens[6].type, TokenType::BANG);
    EXPECT_EQ(tokens[7].type, TokenType::LEFT_SHIFT);
    EXPECT_EQ(tokens[8].type, TokenType::RIGHT_SHIFT);
    EXPECT_EQ(tokens[9].type, TokenType::END_OF_FILE);
}

// Test special operators
TEST_F(LexerTest, SpecialOperators) {
    auto tokens = tokenize("-> . ..");
    
    EXPECT_EQ(tokens[0].type, TokenType::ARROW);
    EXPECT_EQ(tokens[1].type, TokenType::DOT);
    EXPECT_EQ(tokens[2].type, TokenType::DOT_DOT);
    EXPECT_EQ(tokens[3].type, TokenType::END_OF_FILE);
}

// Test integer literals
TEST_F(LexerTest, IntegerLiterals) {
    auto tokens = tokenize("0 42 123456789");
    
    EXPECT_EQ(tokens[0].type, TokenType::INT_LITERAL);
    EXPECT_EQ(tokens[0].lexeme, "0");
    EXPECT_EQ(tokens[1].type, TokenType::INT_LITERAL);
    EXPECT_EQ(tokens[1].lexeme, "42");
    EXPECT_EQ(tokens[2].type, TokenType::INT_LITERAL);
    EXPECT_EQ(tokens[2].lexeme, "123456789");
    EXPECT_EQ(tokens[3].type, TokenType::END_OF_FILE);
}

// Test float literals
TEST_F(LexerTest, FloatLiterals) {
    auto tokens = tokenize("3.14 0.5 123.4565");
    EXPECT_EQ(tokens[0].type, TokenType::FLOAT_LITERAL);
    EXPECT_EQ(tokens[0].lexeme, "3.14");
    EXPECT_EQ(tokens[1].type, TokenType::FLOAT_LITERAL);
    EXPECT_EQ(tokens[1].lexeme, "0.5");
    EXPECT_EQ(tokens[2].type, TokenType::FLOAT_LITERAL);
    EXPECT_EQ(tokens[2].lexeme, "123.4565");
    EXPECT_EQ(tokens[3].type, TokenType::END_OF_FILE);
}

// Test string literals
TEST_F(LexerTest, StringLiterals) {
    auto tokens = tokenize("\"hello\" \"world\" \"\" \"hello\\\\world\"\"");
    
    EXPECT_EQ(tokens[0].type, TokenType::STRING_LITERAL);
    EXPECT_EQ(tokens[0].lexeme, "\"hello\"");
    EXPECT_EQ(tokens[1].type, TokenType::STRING_LITERAL);
    EXPECT_EQ(tokens[1].lexeme, "\"world\"");
    EXPECT_EQ(tokens[2].type, TokenType::STRING_LITERAL);
    EXPECT_EQ(tokens[2].lexeme, "\"\"");
    EXPECT_EQ(tokens[3].type, TokenType::STRING_LITERAL);
    EXPECT_EQ(tokens[3].lexeme, "\"hello\\\\world\"\"");
    EXPECT_EQ(tokens[4].type, TokenType::END_OF_FILE);
}

// Test string literals with escape sequences
TEST_F(LexerTest, StringLiteralsWithEscapes) {
    auto tokens = tokenize("\"hello\\nworld\" \"\\t\\\\\\\"\"\"");
    
    EXPECT_EQ(tokens[0].type, TokenType::STRING_LITERAL);
    EXPECT_EQ(tokens[0].lexeme, "\"hello\\nworld\"");
    EXPECT_EQ(tokens[1].type, TokenType::STRING_LITERAL);
    EXPECT_EQ(tokens[1].lexeme, "\"\\t\\\\\\\"\"\"");
    EXPECT_EQ(tokens[2].type, TokenType::END_OF_FILE);
}

// Test multiline string literals
TEST_F(LexerTest, MultilineStringLiterals) {
    auto tokens = tokenize("\"hello\\nworld\"\"\n\"test\"");
    
    EXPECT_EQ(tokens[0].type, TokenType::STRING_LITERAL);
    EXPECT_EQ(tokens[0].lexeme, "\"hello\\nworld\"");
    EXPECT_EQ(tokens[1].type, TokenType::STRING_LITERAL);
    EXPECT_EQ(tokens[1].lexeme, "\"test\"");
    EXPECT_EQ(tokens[2].type, TokenType::END_OF_FILE);
}

// Test character literals
TEST_F(LexerTest, CharacterLiterals) {
    auto tokens = tokenize("'a' 'b' '1' '_' '\"'");
    
    EXPECT_EQ(tokens[0].type, TokenType::CHAR_LITERAL);
    EXPECT_EQ(tokens[0].lexeme, "'a'");
    EXPECT_EQ(tokens[1].type, TokenType::CHAR_LITERAL);
    EXPECT_EQ(tokens[1].lexeme, "'b'");
    EXPECT_EQ(tokens[2].type, TokenType::CHAR_LITERAL);
    EXPECT_EQ(tokens[2].lexeme, "'1'");
    EXPECT_EQ(tokens[3].type, TokenType::CHAR_LITERAL);
    EXPECT_EQ(tokens[3].lexeme, "'_'");
    EXPECT_EQ(tokens[4].type, TokenType::CHAR_LITERAL);
    EXPECT_EQ(tokens[4].lexeme, "'\"'");
    EXPECT_EQ(tokens[5].type, TokenType::END_OF_FILE);
}

// Test character literals with escape sequences
TEST_F(LexerTest, CharacterLiteralsWithEscapes) {
    auto tokens = tokenize("'\\n' '\\\\' '\\\"' '\\0'");
    
    EXPECT_EQ(tokens[0].type, TokenType::CHAR_LITERAL);
    EXPECT_EQ(tokens[0].lexeme, "'\\n'");
    EXPECT_EQ(tokens[1].type, TokenType::CHAR_LITERAL);
    EXPECT_EQ(tokens[1].lexeme, "'\\\\'");
    EXPECT_EQ(tokens[2].type, TokenType::CHAR_LITERAL);
    EXPECT_EQ(tokens[2].lexeme, "'\\\"'");
    EXPECT_EQ(tokens[3].type, TokenType::CHAR_LITERAL);
    EXPECT_EQ(tokens[3].lexeme, "'\\0'");
    EXPECT_EQ(tokens[4].type, TokenType::END_OF_FILE);
}

// Test identifiers
TEST_F(LexerTest, Identifiers) {
    auto tokens = tokenize("variable x123 _temp my_var");
    
    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].lexeme, "variable");
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].lexeme, "x123");
    EXPECT_EQ(tokens[2].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[2].lexeme, "_temp");
    EXPECT_EQ(tokens[3].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[3].lexeme, "my_var");
    EXPECT_EQ(tokens[4].type, TokenType::END_OF_FILE);
}

// Test control flow keywords
TEST_F(LexerTest, ControlFlowKeywords) {
    auto tokens = tokenize("if else do while for in loop break continue return");
    
    EXPECT_EQ(tokens[0].type, TokenType::IF);
    EXPECT_EQ(tokens[1].type, TokenType::ELSE);
    EXPECT_EQ(tokens[2].type, TokenType::DO);
    EXPECT_EQ(tokens[3].type, TokenType::WHILE);
    EXPECT_EQ(tokens[4].type, TokenType::FOR);
    EXPECT_EQ(tokens[5].type, TokenType::IN);
    EXPECT_EQ(tokens[6].type, TokenType::LOOP);
    EXPECT_EQ(tokens[7].type, TokenType::BREAK);
    EXPECT_EQ(tokens[8].type, TokenType::CONTINUE);
    EXPECT_EQ(tokens[9].type, TokenType::RETURN);
    EXPECT_EQ(tokens[10].type, TokenType::END_OF_FILE);
}

// Test function and storage keywords
TEST_F(LexerTest, FunctionAndStorageKeywords) {
    auto tokens = tokenize("fn static const match");
    
    EXPECT_EQ(tokens[0].type, TokenType::FN);
    EXPECT_EQ(tokens[1].type, TokenType::STATIC);
    EXPECT_EQ(tokens[2].type, TokenType::CONST);
    EXPECT_EQ(tokens[3].type, TokenType::MATCH);
    EXPECT_EQ(tokens[4].type, TokenType::END_OF_FILE);
}

// Test boolean literals
TEST_F(LexerTest, BooleanLiterals) {
    auto tokens = tokenize("true false");
    
    EXPECT_EQ(tokens[0].type, TokenType::TRUE);
    EXPECT_EQ(tokens[0].lexeme, "true");
    EXPECT_EQ(tokens[1].type, TokenType::FALSE);
    EXPECT_EQ(tokens[1].lexeme, "false");
    EXPECT_EQ(tokens[2].type, TokenType::END_OF_FILE);
}

// Test nil literal
TEST_F(LexerTest, NilLiteral) {
    auto tokens = tokenize("NIL");
    
    EXPECT_EQ(tokens[0].type, TokenType::NIL_LITERAL);
    EXPECT_EQ(tokens[0].lexeme, "NIL");
    EXPECT_EQ(tokens[1].type, TokenType::END_OF_FILE);
}

// Test integer type keywords
TEST_F(LexerTest, IntegerTypeKeywords) {
    auto tokens = tokenize("i8 i16 i32 i64 u8 u16 u32 u64");
    
    EXPECT_EQ(tokens[0].type, TokenType::I8);
    EXPECT_EQ(tokens[1].type, TokenType::I16);
    EXPECT_EQ(tokens[2].type, TokenType::I32);
    EXPECT_EQ(tokens[3].type, TokenType::I64);
    EXPECT_EQ(tokens[4].type, TokenType::U8);
    EXPECT_EQ(tokens[5].type, TokenType::U16);
    EXPECT_EQ(tokens[6].type, TokenType::U32);
    EXPECT_EQ(tokens[7].type, TokenType::U64);
    EXPECT_EQ(tokens[8].type, TokenType::END_OF_FILE);
}

// Test float type keywords
TEST_F(LexerTest, FloatTypeKeywords) {
    auto tokens = tokenize("f32 f64");
    
    EXPECT_EQ(tokens[0].type, TokenType::F32);
    EXPECT_EQ(tokens[1].type, TokenType::F64);
    EXPECT_EQ(tokens[2].type, TokenType::END_OF_FILE);
}

// Test basic type keywords
TEST_F(LexerTest, BasicTypeKeywords) {
    auto tokens = tokenize("char string bool void");
    
    EXPECT_EQ(tokens[0].type, TokenType::CHAR);
    EXPECT_EQ(tokens[1].type, TokenType::STRING);
    EXPECT_EQ(tokens[2].type, TokenType::BOOL);
    EXPECT_EQ(tokens[3].type, TokenType::VOID);
    EXPECT_EQ(tokens[4].type, TokenType::END_OF_FILE);
}

// Test composite type keywords
TEST_F(LexerTest, CompositeTypeKeywords) {
    auto tokens = tokenize("struct enum union");
    
    EXPECT_EQ(tokens[0].type, TokenType::STRUCT);
    EXPECT_EQ(tokens[1].type, TokenType::ENUM);
    EXPECT_EQ(tokens[2].type, TokenType::UNION);
    EXPECT_EQ(tokens[3].type, TokenType::END_OF_FILE);
}

// Test whitespace handling
TEST_F(LexerTest, WhitespaceHandling) {
    auto tokens = tokenize("   \t\r\n   x   \n   y\t\n");
    
    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].lexeme, "x");
    EXPECT_EQ(tokens[0].line, 2);
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].lexeme, "y");
    EXPECT_EQ(tokens[1].line, 3);
    EXPECT_EQ(tokens[2].type, TokenType::END_OF_FILE);
}

// Test line comments
TEST_F(LexerTest, LineComments) {
    auto tokens = tokenize("x // this is a comment\ny // another comment");
    
    EXPECT_EQ(tokens[0].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[0].lexeme, "x");
    EXPECT_EQ(tokens[1].type, TokenType::IDENTIFIER);
    EXPECT_EQ(tokens[1].lexeme, "y");
    EXPECT_EQ(tokens[2].type, TokenType::END_OF_FILE);
}

// Test error handling - unterminated string
TEST_F(LexerTest, UnterminatedString) {
    auto tokens = tokenize("\"unterminated string");
    
    EXPECT_EQ(tokens[0].type, TokenType::ERROR_TOKEN);
    EXPECT_EQ(tokens[0].lexeme, "Unterminated string");
}

// Test error handling - unterminated character literal
TEST_F(LexerTest, UnterminatedCharacterLiteral) {
    auto tokens = tokenize("'unterminated");
    
    EXPECT_EQ(tokens[0].type, TokenType::ERROR_TOKEN);
    EXPECT_EQ(tokens[0].lexeme, "Character literal must contain exactly one character.");
}

// Test error handling - empty character literal
TEST_F(LexerTest, EmptyCharacterLiteral) {
    auto tokens = tokenize("''");
    
    EXPECT_EQ(tokens[0].type, TokenType::ERROR_TOKEN);
    EXPECT_EQ(tokens[0].lexeme, "Empty character literal.");
}

// Test error handling - invalid escape sequence in character
TEST_F(LexerTest, InvalidEscapeSequenceInCharacter) {
    auto tokens = tokenize("'\\x'");
    
    EXPECT_EQ(tokens[0].type, TokenType::ERROR_TOKEN);
    EXPECT_EQ(tokens[0].lexeme, "Invalid escape sequence in character literal.");
}

// Test error handling - invalid escape sequence in string
TEST_F(LexerTest, InvalidEscapeSequenceInString) {
    auto tokens = tokenize("\"hello\\xworld\"");
    
    EXPECT_EQ(tokens[0].type, TokenType::ERROR_TOKEN);
    EXPECT_EQ(tokens[0].lexeme, "Invalid escape sequence.");
}

// Test error handling - unexpected character
TEST_F(LexerTest, UnexpectedCharacter) {
    auto tokens = tokenize("$");
    
    EXPECT_EQ(tokens[0].type, TokenType::ERROR_TOKEN);
    EXPECT_EQ(tokens[0].lexeme, "Unexpected character.");
}

// Test complex expression
TEST_F(LexerTest, ComplexExpression) {
    auto tokens = tokenize("fn main() -> i32 { let x: i32 = 42 + 3.14; return x; }");
    
    // Verify key tokens in the expression
    bool found_fn = false, found_main = false, found_left_paren = false;
    bool found_right_paren = false, found_arrow = false, found_i32 = false;
    bool found_left_brace = false, found_let = false, found_x = false;
    bool found_colon = false, found_equal = false, found_int = false;
    bool found_plus = false, found_float = false, found_semicolon = false;
    bool found_return = false, found_right_brace = false;
    
    for (const auto& token : tokens) {
        if (token.type == TokenType::FN) found_fn = true;
        if (token.type == TokenType::IDENTIFIER && token.lexeme == "main") found_main = true;
        if (token.type == TokenType::LEFT_PAREN) found_left_paren = true;
        if (token.type == TokenType::RIGHT_PAREN) found_right_paren = true;
        if (token.type == TokenType::ARROW) found_arrow = true;
        if (token.type == TokenType::I32) found_i32 = true;
        if (token.type == TokenType::LEFT_BRACE) found_left_brace = true;
        if (token.type == TokenType::IDENTIFIER && token.lexeme == "let") found_let = true;
        if (token.type == TokenType::IDENTIFIER && token.lexeme == "x") found_x = true;
        if (token.type == TokenType::COLON) found_colon = true;
        if (token.type == TokenType::EQUAL) found_equal = true;
        if (token.type == TokenType::INT_LITERAL) found_int = true;
        if (token.type == TokenType::PLUS) found_plus = true;
        if (token.type == TokenType::FLOAT_LITERAL) found_float = true;
        if (token.type == TokenType::SEMICOLON) found_semicolon = true;
        if (token.type == TokenType::RETURN) found_return = true;
        if (token.type == TokenType::RIGHT_BRACE) found_right_brace = true;
    }
    
    EXPECT_TRUE(found_fn);
    EXPECT_TRUE(found_main);
    EXPECT_TRUE(found_left_paren);
    EXPECT_TRUE(found_right_paren);
    EXPECT_TRUE(found_arrow);
    EXPECT_TRUE(found_i32);
    EXPECT_TRUE(found_left_brace);
    EXPECT_TRUE(found_let);
    EXPECT_TRUE(found_x);
    EXPECT_TRUE(found_colon);
    EXPECT_TRUE(found_equal);
    EXPECT_TRUE(found_int);
    EXPECT_TRUE(found_plus);
    EXPECT_TRUE(found_float);
    EXPECT_TRUE(found_semicolon);
    EXPECT_TRUE(found_return);
    EXPECT_TRUE(found_right_brace);
}

// Test edge case - empty input
TEST_F(LexerTest, EmptyInput) {
    auto tokens = tokenize("");
    
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::END_OF_FILE);
    EXPECT_EQ(tokens[0].line, 1);
}

// Test edge case - only whitespace
TEST_F(LexerTest, OnlyWhitespace) {
    auto tokens = tokenize("   \t\n   \r\n   ");
    
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::END_OF_FILE);
    EXPECT_EQ(tokens[0].line, 3);
}

// Test edge case - only comments
TEST_F(LexerTest, OnlyComments) {
    auto tokens = tokenize("// line 1\n// line 2\n// line 3");
    
    EXPECT_EQ(tokens.size(), 1);
    EXPECT_EQ(tokens[0].type, TokenType::END_OF_FILE);
    EXPECT_EQ(tokens[0].line, 4);
}