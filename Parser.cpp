#include "Parser.h"
#include <memory> 
#include <initializer_list> 
#include "Lox.h"

using namespace std;

Parser::Parser(vector<Token>& tokens) : current(0), tokens(tokens) {}

shared_ptr<Expr> Parser::expression() {
    return equality();
}

shared_ptr<Expr> Parser::equality() {
    shared_ptr<Expr> expr = comparison();
    while(match({BANG_EQUAL, EQUAL_EQUAL})) {
        Token op = previous();
        shared_ptr<Expr> right = comparison();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::comparison() {
    shared_ptr<Expr> expr = term(); 
    while(match({GREATER, GREATER_EQUAL, LESS, LESS_EQUAL})) {
        Token op = previous();
        shared_ptr<Expr> right = term();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::term() {
    shared_ptr<Expr> expr = factor();
    while(match({MINUS, PLUS})) {
        Token op = previous();
        shared_ptr<Expr> right = factor();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::factor() {
    shared_ptr<Expr> expr = unary();
    while(match({SLASH, STAR})) {
        Token op = previous();
        shared_ptr<Expr> right = unary();
        expr = make_shared<Binary>(expr, op, right);
    }

    return expr;
}

shared_ptr<Expr> Parser::unary() {
    if(match({BANG, MINUS})) {
        Token op = previous();
        shared_ptr<Expr> right = unary();
        return make_shared<Unary>(op, right);
    }

    return primary();
}

shared_ptr<Expr> Parser::primary() {
    if (match({FALSE})) {
        return make_shared<LiteralExpr>(Literal(false));
    }
    if (match({TRUE})) {
        return make_shared<LiteralExpr>(Literal(true));
    }
    if (match({NIL})) {
        return make_shared<LiteralExpr>(Literal());
    }
    if (match({NUMBER})) {
        double value = stod(previous().lexeme);
        return make_shared<LiteralExpr>(Literal(value));
    }
    if (match({STRING})) {
        return make_shared<LiteralExpr>(Literal(previous().lexeme));
    }

    if (match({LEFT_PAREN})) {
        shared_ptr<Expr> expr = expression();
        consume(RIGHT_PAREN, "Expect ')' after expression.");
        return make_shared<Grouping>(expr);
    }
    
    throw error(peek(), "Expect expression.");
}

bool Parser::match(initializer_list<TokenType> types) {
    for(TokenType type : types) {
        if(check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::check(TokenType type) {
    if(isAtEnd())
        return false;
    return peek().type == type;
}

Token Parser::advance() {
    if(!isAtEnd())
        current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == EOF_TOKEN;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

ParseError Parser::error(Token token, string message) {
    Lox::error(token, message);
    return ParseError(message);
}

Token Parser::consume(TokenType type, string message) {
    if (check(type)) {
        return advance();
    }
    
    throw error(peek(), message);
}

void Parser::synchronize() {
    advance();
    
    while (!isAtEnd()) {
        if (previous().type == SEMICOLON) return;
        
        switch (peek().type) {
            case CLASS:
            case FUN:
            case VAR:
            case FOR:
            case IF:
            case WHILE:
            case PRINT:
            case RETURN:
                return;
            default:
                break;
        }
        
        advance();
    }
}

shared_ptr<Expr> Parser::parse() {
    try {
        return expression();
    } catch (const ParseError& error) {
        return nullptr;
    }
}