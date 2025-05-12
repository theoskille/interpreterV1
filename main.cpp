#include "Lox.h"
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    if(argc > 2) {
        cout << "Usage: jlox [script]\n";
        exit(65);
    } else if(argc == 2) {
        Lox::runFile(argv[1]);
    } else {
        Lox::runPrompt();
    }
    return 0;
}

