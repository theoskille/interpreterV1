#include "AstPrinter.h"
#include <sstream>

using namespace std;

string AstPrinter::print(Expr* expr) {
    // Start the visitor pattern by calling accept on the expression
    return expr->accept(*this);
}

string AstPrinter::visitAssign(Assign* expr) {
    stringstream ss;
    ss << "(= " << expr->name.lexeme << " " << expr->value->accept(*this) << ")";
    return ss.str();
}

string AstPrinter::visitBinary(Binary* expr) {
    // TODO: Implement this method
    // Use the parenthesize helper to format the binary expression
    return parenthesize(expr->op.lexeme, expr->left.get(), expr->right.get());
}

string AstPrinter::visitGrouping(Grouping* expr) {
    // TODO: Implement this method
    // Use the parenthesize helper to format the grouping expression
    return parenthesize("group", expr->expression.get());
}

string AstPrinter::visitLiteralExpr(LiteralExpr* expr) {
    if (expr->value.isNull()) return "nil";
    return expr->value.toString();
}

string AstPrinter::visitUnary(Unary* expr) {
    // TODO: Implement this method
    // Use the parenthesize helper to format the unary expression
    return parenthesize(expr->op.lexeme, expr->right.get());
}

string AstPrinter::visitVariable(Variable* expr) {
    return expr->name.lexeme;
}

string AstPrinter::visitLogical(Logical* expr) {
    return parenthesize(expr->op.lexeme, expr->left.get(), expr->right.get());
}

string AstPrinter::parenthesize(const string& name, Expr* expr) {
    // TODO: Implement this method
    // Create a string that wraps the expression in parentheses with the name
    // e.g. "(name expr)"
    stringstream ss;
    ss << "(" << name << " ";
    ss << expr->accept(*this);
    ss << ")";
    return ss.str();
}

string AstPrinter::parenthesize(const string& name, Expr* expr1, Expr* expr2) {
    // TODO: Implement this method
    // Create a string that wraps both expressions in parentheses with the name
    // e.g. "(name expr1 expr2)"
    stringstream ss;
    ss << "(" << name << " ";
    ss << expr1->accept(*this) << " ";
    ss << expr2->accept(*this);
    ss << ")";
    return ss.str();
}
