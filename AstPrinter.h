#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include <string>
#include "Expr.h"

class AstPrinter : public ExprStringVisitor {
public:
    std::string print(Expr* expr);

    std::string visitAssign(Assign* expr) override;
    std::string visitBinary(Binary* expr) override;
    std::string visitGrouping(Grouping* expr) override;
    std::string visitLiteralExpr(LiteralExpr* expr) override;
    std::string visitUnary(Unary* expr) override;
    std::string visitVariable(Variable* expr) override;

private:
    std::string parenthesize(const std::string& name, Expr* expr);
    std::string parenthesize(const std::string& name, Expr* expr1, Expr* expr2);
};

#endif // AST_PRINTER_H 