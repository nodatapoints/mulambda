#ifndef REPR_H
#define REPR_H

#include "parser.h"

typedef struct {
    enum ExprType type;
    union {
        struct App app;
        char freeVar;
        struct LabelledFunc {
            Expr *func;
            char label;
        } func;
    } data;
} LabelledExpr;

void printExprAST(const Expr* expr);
void printExpr(const Expr* expr);

static inline void _printExprASTIndent(const Expr *expr, int indent);
static inline char nextLabel(const Expr *func);
static void _prepareFuncLabels(Expr *expr);

#endif  // REPR_H
