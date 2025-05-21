#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#include "Token.h"
#include "TokenType.h"
#include "Expr.h"
#include "Stmt.h"

// Custom exception for Parser errors
class ParseError : public std::runtime_error {
public:
    ParseError(const std::string& message) : std::runtime_error(message) {}
};

class Parser {
    private:
        int current = 0;
        std::vector<Token> tokens;

        // Stmt parsing methods
        std::shared_ptr<Stmt> declaration();
        std::shared_ptr<Stmt> varDeclaration();
        std::shared_ptr<Expr> expression();
        std::shared_ptr<Stmt> statement();
        std::shared_ptr<Stmt> printStatement();
        std::shared_ptr<Stmt> expressionStatement();
        std::vector<std::shared_ptr<Stmt>> block();
        std::shared_ptr<Stmt> ifStatement();
        std::shared_ptr<Stmt> whileStatement();
        std::shared_ptr<Stmt> forStatement();

        // Recursive descent parsing methods
        std::shared_ptr<Expr> assignment();
        std::shared_ptr<Expr> logicalOr();
        std::shared_ptr<Expr> logicalAnd();
        std::shared_ptr<Expr> equality();
        std::shared_ptr<Expr> comparison();
        std::shared_ptr<Expr> term();
        std::shared_ptr<Expr> factor();
        std::shared_ptr<Expr> unary();
        std::shared_ptr<Expr> primary();

        // Helper methods
        bool match(std::initializer_list<TokenType> types);
        bool check(TokenType type);
        Token advance();
        bool isAtEnd();
        Token peek();
        Token previous();
        
        // Error handling
        ParseError error(Token token, std::string message);
        void synchronize(); // Method to recover from errors
        Token consume(TokenType type, std::string message);
    
    public:
        Parser(std::vector<Token>& tokens);
        std::vector<std::shared_ptr<Stmt>> parse(); // Main parsing method
};

#endif // PARSER_H 