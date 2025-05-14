#include "Lox.h"
#include <iostream>
#include <fstream>
#include <memory> 
#include "Scanner.h"
#include "Parser.h"
#include "Token.h"
#include "Expr.h"
#include "AstPrinter.h"
using namespace std;

// Initialize static member
bool Lox::hadError = false;

void Lox::run(string source) {
    Scanner scanner(source);
    vector<Token> tokens = scanner.scanTokens();
    
    // Print all tokens
    cout << "Tokens:" << endl;
    for(const Token& token : tokens) {
        cout << token << endl;
    }

    // Parse tokens into an abstract syntax tree
    Parser parser(tokens);
    shared_ptr<Expr> expression = parser.parse();

    if(hadError)
        return;

    // Print the syntax tree if parsing was successful
    if (expression) {
        cout << "\nAbstract Syntax Tree:" << endl;
        AstPrinter printer;
        cout << printer.print(expression.get()) << endl;
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

void Lox::report(int line, string where, string message) {
    cerr << "[line " << line << "] Error" << where << ": " << message << "\n";
    hadError = true;
} 