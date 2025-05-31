#include "Lox.h"
#include <iostream>
#include <fstream>
#include <memory> 
#include "Scanner.h"
#include "Parser.h"
#include "Token.h"
#include "Expr.h"
#include "Stmt.h"
#include "AstPrinter.h"
#include "Interpreter.h"
#include "Resolver.h"
using namespace std;

// Initialize static members
bool Lox::hadError = false;
bool Lox::hadRuntimeError = false;

void Lox::run(string source) {
    Scanner scanner(source);
    vector<Token> tokens = scanner.scanTokens();
    
    // Print all tokens
    cout << "Tokens:" << endl;
    for(const Token& token : tokens) {
        cout << token << endl;
    }

    // Parse tokens into statements
    Parser parser(tokens);
    vector<shared_ptr<Stmt>> statements = parser.parse();

    // Stop if there was a parsing error
    if(hadError)
        return;

    // Create the interpreter
    Interpreter interpreter;
    
    // Run the resolver
    Resolver resolver(interpreter);
    resolver.resolve(statements);
    
    // Stop if there was a resolution error
    if(hadError)
        return;

    // Interpret the statements
    if (!statements.empty()) {
        cout << "\nInterpreted Result:" << endl;
        interpreter.interpret(statements);
    }
}

void Lox::runPrompt() {
    string input;
    while(true) {
        cout << "> ";
        getline(cin, input);
        if(input.empty() || input == "exit")
            break;
        run(input);
        hadError = false;
        hadRuntimeError = false;
    }
}

string Lox::readFile(const string& path) {
    ifstream file(path, ios::binary | ios::ate);
    
    if(!file.is_open()) {
        throw runtime_error("Unable to open file: " + path + "\n");
    }

    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    string content;
    content.resize(size);

    file.read(&content[0], size);

    return content;
}

void Lox::runFile(string path) {
    string content = readFile(path);
    run(content);
    if(hadError)
        exit(65);
    if(hadRuntimeError)
        exit(70);
}

void Lox::error(int line, string message) {
    report(line, "", message);
}

void Lox::error(Token token, string message) {
    if (token.type == EOF_TOKEN) {
        report(token.line, " at end", message);
    } else {
        report(token.line, " at '" + token.lexeme + "'", message);
    }
}

void Lox::runtimeError(const RuntimeError& error) {
    cerr << error.what() << "\n[line " << error.getToken().line << "]" << endl;
    hadRuntimeError = true;
}

void Lox::report(int line, string where, string message) {
    cerr << "[line " << line << "] Error" << where << ": " << message << "\n";
    hadError = true;
} 