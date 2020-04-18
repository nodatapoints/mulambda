#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <ctype.h>

#define LAMBDA_SYMBOL 'L'
#define DOT_SYMBOL '.'
#define OPENPAREN_SYMBOL '('
#define CLOSEPAREN_SYMBOL ')'

struct Expr;

typedef struct Expr {
    enum ExprType {NULL_T = 0, APP, FUNC, BOUND, FREE} type;
    union {
        struct App {
            const struct Expr *arg, *func;
        } app;
        char freeVar;
        const struct Expr *func;
    } content;
} Expr;

const Expr* getExprList(FILE * const fp, Expr** const expr, const Expr ** const vars, int* const parenLevel);
void printExpr(const Expr* const expr, int indent);

#endif  // PARSER_H
