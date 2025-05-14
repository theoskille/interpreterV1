#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include <string>
#include "Expr.h"

class AstPrinter : public Visitor {
public:
    std::string print(Expr* expr);

    std::string visitBinary(Binary* expr) override;
    std::string visitGrouping(Grouping* expr) override;
    std::string visitLiteralExpr(LiteralExpr* expr) override;
    std::string visitUnary(Unary* expr) override;

private:
    std::string parenthesize(const std::string& name, Expr* expr);
    std::string parenthesize(const std::string& name, Expr* expr1, Expr* expr2);
};

#endif // AST_PRINTER_H 