#include "Scanner.h"
#include "Lox.h"
using namespace std;

const unordered_map<string, TokenType> Scanner::keywords = {
    {"and",    AND},
    {"class",  CLASS},
    {"else",   ELSE},
    {"false",  FALSE},
    {"for",    FOR},
    {"fun",    FUN},
    {"if",     IF},
    {"nil",    NIL},
    {"or",     OR},
    {"print",  PRINT},
    {"return", RETURN},
    {"super",  SUPER},
    {"this",   THIS},
    {"true",   TRUE},
    {"var",    VAR},
    {"while",  WHILE}
};

Scanner::Scanner(string source) {
    this->source = source;
}

bool Scanner::isAtEnd() {
    return current >= source.length();
}

vector<Token> Scanner::scanTokens() {
    while(!isAtEnd()) {
        start = current;
        scanToken();
    }
    tokens.push_back(Token(EOF_TOKEN, "", Literal(), line));
    return tokens;
}

void Scanner::scanToken() {
    char c = advance();
    switch(c) {
        case '(': addToken(LEFT_PAREN); break;
        case ')': addToken(RIGHT_PAREN); break;
        case '{': addToken(LEFT_BRACE); break;
        case '}': addToken(RIGHT_BRACE); break;
        case ',': addToken(COMMA); break;
        case '.': addToken(DOT); break;
        case '-': addToken(MINUS); break;
        case '+': addToken(PLUS); break;
        case ';': addToken(SEMICOLON); break;
        case '*': addToken(STAR); break;
        case '!':
            addToken(match('=') ? BANG_EQUAL : BANG);
            break;
        case '=':
            addToken(match('=') ? EQUAL_EQUAL : EQUAL);
            break;
        case '<':
            addToken(match('=') ? LESS_EQUAL : LESS);
            break;
        case '>':
            addToken(match('=') ? GREATER_EQUAL : GREATER);
            break;
        case '"': handleString(); break;
        case '/':
            if(match('/')) {
                while(peek() != '\n' && !isAtEnd())
                    advance();
            } else {
                addToken(SLASH);
            }
            break;
        case ' ':
        case '\r':
        case '\t':
            break;
        case '\n':
            line++;
            break;
        default:
            if (isDigit(c)) {
                handleNumber();
            } else if(isAlpha(c)) {
                handleIdentifier();
            } else {
                Lox::error(line, "Unexpected character.");
            }
            break;
    }
}

char Scanner::advance() {
    return source[current++];
}

void Scanner::addToken(TokenType type) {
    addToken(type, Literal());
}

void Scanner::addToken(TokenType type, Literal literal) {
    string text = source.substr(start, current - start);
    tokens.push_back(Token(type, text, literal, line));
} 

bool Scanner::match(char expected) {
    if(isAtEnd())
        return false;
    if(source[current] != expected)
        return false;

    current++;
    return true;
}

char Scanner::peek() {
    if(isAtEnd())
        return '\0';
    return source[current];
}

char Scanner::peekNext() {
    if(current + 1 >= source.length())
        return '\0';
    return source[current + 1];
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Scanner::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

void Scanner::handleString() {
    while(peek() != '"' && !isAtEnd()) {
        if(peek() == '\n')
            line++;
        advance();
    }

    if(isAtEnd()) {
        Lox::error(line, "Unterminated string.");
        return;
    }

    advance(); // closing ".

    string value = source.substr(start + 1, current - start - 2);
    addToken(STRING, Literal(value));
}

void Scanner::handleNumber() {
    while(isDigit(peek()))
        advance();

    if(peek() == '.' && isDigit(peekNext())) {
        advance();
        while(isDigit(peek()))
            advance();
    }

    string numberStr = source.substr(start, current - start);
    double value = stod(numberStr);
    addToken(NUMBER, Literal(value));
}

void Scanner::handleIdentifier() {
    while(isAlphaNumeric(peek()))
        advance();

    string text = source.substr(start, current - start);
    TokenType type = IDENTIFIER;
    if(keywords.count(text) > 0)
        type = keywords.at(text);
    addToken(type);
}