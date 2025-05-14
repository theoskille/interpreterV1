#ifndef LOX_H
#define LOX_H

#include <string>
#include "Token.h"

class Lox {
private:
    static bool hadError;
    static std::string readFile(const std::string& path);
    static void report(int line, std::string where, std::string message);

public:
    static void run(std::string source);
    static void runPrompt();
    static void runFile(std::string path);
    static void error(int line, std::string message);
    static void error(Token token, std::string message);
};

#endif // LOX_H 