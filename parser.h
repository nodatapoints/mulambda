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

#define _MALLOC(X) { \
    X = (Expr*) malloc(sizeof(Expr)); \
    if (!(X)) RAISE(NULL, "could not allocate memory") \
    (X)->type = NULL_T; }

#ifdef DEBUG
#   define MALLOC(X) { _MALLOC(X); printf("debug: allocated %p\n", (void*) (X)); }
#   define P_DEBUG(...) printf(__VA_ARGS__);
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

const Expr* parse(FILE *fp);
void freeExpr(const Expr* expr);
void printExpr(const Expr* expr);

static char getsymbol(FILE *fp);
static const Expr* getExprList(FILE *fp, const Expr **vars, int* parenLevel);
static const Expr* getExpr(FILE * const fp, const Expr ** const vars, int * const parenLevel);
static const Expr* getFunction(FILE * const fp, const Expr ** const vars, int * const parenLevel);
static const Expr* getVar(const Expr ** const vars, char c);
static const Expr* getExpr(FILE * const fp, const Expr ** const vars, int * const parenLevel);
static inline void _printExprIndent(const Expr* const expr, int indent);

#endif  // PARSER_H
