#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "../../Token.h"
#include "../../TokenType.h"
#include "../../Scanner.h"
#include "../../Parser.h"
#include "../../Expr.h"
#include "../../AstPrinter.h"
#include "../../Lox.h"

using namespace std;

// Forward declarations for test functions
void testScanner(const string& source);
void testParser(const string& source);
void testManualAst();

int main() {
    cout << "=== Lox Interpreter Test Runner ===" << endl;
    cout << "Select a test to run:" << endl;
    cout << "1. Test Scanner with input" << endl;
    cout << "2. Test Parser with input" << endl;
    cout << "3. Test Manual AST Creation" << endl;
    cout << "Enter your choice (1-3): ";
    
    int choice;
    cin >> choice;
    cin.ignore(); // Clear the newline
    
    string input;
    
    switch (choice) {
        case 1:
            cout << "Enter source code to tokenize: ";
            getline(cin, input);
            testScanner(input);
            break;
        case 2:
            cout << "Enter expression to parse: ";
            getline(cin, input);
            testParser(input);
            break;
        case 3:
            testManualAst();
            break;
        default:
            cout << "Invalid choice." << endl;
            return 1;
    }
    
    return 0;
}

void testScanner(const string& source) {
    Scanner scanner(source);
    vector<Token*> tokens = scanner.scanTokens();
    
    cout << "\nTokens:" << endl;
    for (const Token* token : tokens) {
        cout << *token << endl;
    }
    
    // Clean up
    for (Token* token : tokens) {
        delete token;
    }
}

void testParser(const string& source) {
    // First scan the source to get tokens
    Scanner scanner(source);
    vector<Token*> rawTokens = scanner.scanTokens();
    
    // Convert to the format needed by the parser
    vector<Token> tokens;
    for (const Token* token : rawTokens) {
        tokens.push_back(*token);
    }
    
    // Parse the tokens
    Parser parser(tokens);
    shared_ptr<Expr> expression = parser.parse();
    
    if (expression) {
        // Print the AST
        AstPrinter printer;
        cout << "\nAST: " << printer.print(expression.get()) << endl;
    } else {
        cout << "\nParsing failed." << endl;
    }
    
    // Clean up tokens
    for (Token* token : rawTokens) {
        delete token;
    }
}

void testManualAst() {
    // Create a manually constructed AST for testing
    // Example: (123 + 456) * 789
    
    // First create the literals and tokens
    auto leftValue = Literal(123.0);
    auto rightValue = Literal(456.0);
    auto multiplyValue = Literal(789.0);
    
    Token plus(PLUS, "+", Literal(), 1);
    Token multiply(STAR, "*", Literal(), 1);
    
    // Create the expressions
    auto left = make_shared<LiteralExpr>(leftValue);
    auto right = make_shared<LiteralExpr>(rightValue);
    auto sum = make_shared<Binary>(left, plus, right);
    
    auto multiplyLiteral = make_shared<LiteralExpr>(multiplyValue);
    auto product = make_shared<Binary>(sum, multiply, multiplyLiteral);
    
    // Print the AST
    AstPrinter printer;
    cout << "\nManual AST: " << printer.print(product.get()) << endl;
} 