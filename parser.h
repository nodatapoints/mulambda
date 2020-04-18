#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define LAMBDA_SYMBOL 'L'
#define DOT_SYMBOL '.'
#define OPENPAREN_SYMBOL '('
#define CLOSEPAREN_SYMBOL ')'

#define RAISE(RET, ...) {fprintf(stderr, "error: "); fprintf(stderr, __VA_ARGS__); return RET; }

struct Expr;

typedef struct Expr {
    enum ExprType {NULL_T = 0, APP, FUNC, BOUND, FREE} type;
    union {
        struct App {
            const struct Expr *arg, *func;
        } app;
        char freeVar;
        const struct Expr *func;
    } data;
} Expr;

const Expr* getExprList(FILE *fp, const Expr **vars, int* parenLevel);
void freeExpr(const Expr* expr);
void printExpr(const Expr* expr);

#endif  // PARSER_H
