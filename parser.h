#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

#define LAMBDA_SYMBOL 'L'
#define DOT_SYMBOL '.'
#define OPENPAREN_SYMBOL '('
#define CLOSEPAREN_SYMBOL ')'
#define COMMENT_SYMBOL '#'

#define RAISE(RET, ...) {fprintf(stderr, "error: "); fprintf(stderr, __VA_ARGS__); return RET; }

#define _MALLOC(X) { \
    X = (Expr*) malloc(sizeof(Expr)); \
    if (!(X)) RAISE(NULL, "could not allocate memory") \
    (X)->type = NULL_T; }

#ifdef DEBUG
#   define P_DEBUG(...) { printf("debug: "); printf(__VA_ARGS__); }
#   define MALLOC(X) { _MALLOC(X); P_DEBUG("allocated %p\n", (void*) (X)); }
#else
#   define MALLOC(X) _MALLOC(X)
#   define P_DEBUG(...) ;
#endif


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

Expr* parse(FILE *fp);
void freeExpr(const Expr* expr);

static char getsymbol(FILE *fp);
static const Expr* getExprList(FILE *fp, const Expr *vars[], int* parenLevel);
static const Expr* getExpr(FILE *fp, const Expr *vars[], int *parenLevel);
static const Expr* getFunction(FILE *fp, const Expr *vars[], int *parenLevel);
static const Expr* getVar(const Expr *vars[], char c);
static const Expr* getExpr(FILE *fp, const Expr *vars[], int *parenLevel);

#endif  // PARSER_H
