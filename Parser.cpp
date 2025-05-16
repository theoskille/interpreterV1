#include "Parser.h"
#include <memory> 
#include <initializer_list> 
#include "Lox.h"

using namespace std;

Parser::Parser(vector<Token>& tokens) : current(0), tokens(tokens) {}

shared_ptr<Stmt> Parser::declaration() {
    try {
        if(match({VAR}))
            return varDeclaration();
        return statement();
    } catch (ParseError error) {
        synchronize();
        return nullptr;
    }
}

shared_ptr<Stmt> Parser::statement() {
    if (match({PRINT}))
        return printStatement();
    if(match({LEFT_BRACE}))
        return make_shared<Block>(block());
    return expressionStatement();
}

shared_ptr<Stmt> Parser::printStatement() {
    shared_ptr<Expr> value = expression();
    consume(SEMICOLON, "Expect ';' after value.");
    return make_shared<Print>(value);
}

shared_ptr<Stmt> Parser::varDeclaration() {
    Token name = consume(IDENTIFIER, "Expect variable name.");
    shared_ptr<Expr> initializer = nullptr;
    if(match({EQUAL})) {
        initializer = expression();
    }

    consume(SEMICOLON, "Expect ';' after variable declaration");
    return make_shared<Var>(name, initializer);
}

shared_ptr<Stmt> Parser::expressionStatement() {
    shared_ptr<Expr> expr = expression();
    consume(SEMICOLON, "Expect ';' after expression.");
    return make_shared<Expression>(expr);
}

vector<shared_ptr<Stmt>> Parser::block() {
    vector<shared_ptr<Stmt>> statements;

    while(!check(RIGHT_BRACE) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(RIGHT_BRACE, "Expect '}' after block.");
    return statements;
}

shared_ptr<Expr> Parser::expression() {
    return assignment();
}

shared_ptr<Expr> Parser::assignment() {
    shared_ptr<Expr> expr = equality();

    if(match({EQUAL})) {
        Token equals = previous();
        shared_ptr<Expr> value = assignment();
        //check if instance of Variable expression. if it is get the name and return a new Assign Expression with the name and value
        if(Variable* var = dynamic_cast<Variable*>(expr.get())) {
            Token name = var->name;
            return make_shared<Assign>(name, value);
        }

        error(equals, "Invalid assignment target.");
    }

    return expr;
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
    if (match({IDENTIFIER})) {
        return make_shared<Variable>(previous());
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

vector<shared_ptr<Stmt>> Parser::parse() {
    vector<shared_ptr<Stmt>> statements;
    while(!isAtEnd()) {
        statements.push_back(declaration());
    }
    return statements;
}