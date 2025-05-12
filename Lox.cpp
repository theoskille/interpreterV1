#include "Lox.h"
#include <iostream>
#include <fstream>
#include "Scanner.h"
#include "Token.h"
using namespace std;

// Initialize static member
bool Lox::hadError = false;

void Lox::run(string source) {
    Scanner scanner(source);
    vector<Token*> tokens = scanner.scanTokens();
    
    // Print all tokens
    for(const Token* token : tokens) {
        cout << *token << endl;
    }

    // Clean up tokens
    for(Token* token : tokens) {
        delete token;
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

void Lox::report(int line, string where, string message) {
    cerr << "[line " << line << "] Error" << where << ": " << message << "\n";
    hadError = true;
} 