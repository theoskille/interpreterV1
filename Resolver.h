#include "Expr.h"
#include "Interpreter.h"
#include "Stmt.h"
#include <vector>
#include <unordered_map>
#include <string>

class Resolver : public VoidExprVisitor, public StmtVisitor<void> {
    private:
        Interpreter& interpreter;
        std::vector<std::unordered_map<std::string, bool>> scopes;

    public:
        Resolver(Interpreter& interpreter);
        
        // Statement visitors
        void visitBlock(Block* stmt) override;
        void visitExpression(Expression* stmt) override;
        void visitFunction(Function* stmt) override;
        void visitIf(If* stmt) override;
        void visitPrint(Print* stmt) override;
        void visitReturn(Return* stmt) override;
        void visitVar(Var* stmt) override;
        void visitWhile(While* stmt) override;
        
        // Expression visitors
        void visitAssign(Assign* expr) override;
        void visitBinary(Binary* expr) override;
        void visitCall(Call* expr) override;
        void visitGrouping(Grouping* expr) override;
        void visitLiteralExpr(LiteralExpr* expr) override;
        void visitLogical(Logical* expr) override;
        void visitUnary(Unary* expr) override;
        void visitVariable(Variable* expr) override;

        void resolve(const std::vector<std::shared_ptr<Stmt>>& statements);
        void resolve(Stmt* stmt);
        void resolve(Expr* expr);

    private:
        void beginScope();
        void endScope();
        void declare(const Token& name);
        void define(const Token& name);
        void resolveLocal(Expr* expr, const Token& name);
};