#include "parser.h"

#define RAISE(...) {fprintf(stderr, "error: "); fprintf(stderr, __VA_ARGS__); return NULL; }

static char getsymbol(FILE *fp) {
    char c;
    while (isspace(c = fgetc(fp)));
    return c;
}

static const Expr* getExpr(FILE * const fp, Expr ** const p, const Expr ** const vars, int * const parenLevel);

const Expr* getExprList(FILE * const fp, Expr ** const p, const Expr ** const vars, int * const parenLevel) {
    int currentLevel = *parenLevel;
    const Expr *expr, *arg;
    Expr *app;

    expr = getExpr(fp, p, vars, parenLevel);
    if (expr == NULL) return NULL;
    if (expr == *p) RAISE("empty expression\n");

    ++ *p;
    while (currentLevel == *parenLevel) {
        app = (*p)++;
        arg = getExpr(fp, p, vars, parenLevel);
        if (arg == *p)
            return expr;

        if (arg == NULL)
            return NULL;

        app->type = APP;
        app->content.app.func = expr;
        app->content.app.arg = arg;

        expr = app;
    }
    return expr;
}

static const Expr* getFunction(FILE * const fp, Expr ** const p, const Expr ** const vars, int * const parenLevel) {
    char c;
    const Expr *prev, *body;
    Expr *func;

    c = getsymbol(fp);
    if (c == EOF) RAISE("unexpected EOF in function\n")
    if (!isalpha(c)) RAISE("invalid parameter: %c\n", c)

    if (getsymbol(fp) != DOT_SYMBOL) RAISE("expected %c\n", DOT_SYMBOL)

    prev = vars[c];
    vars[c] = func = (*p)++;

    body = getExprList(fp, p, vars, parenLevel);
    if (body == NULL) return NULL;

    vars[c] = prev;

    func->type = FUNC;
    func->content.func = body;

    return func;
}

static const Expr* getVar(Expr ** const p, const Expr ** const vars, char c) {
    Expr * const var = (*p)++;
    if (!isalpha(c)) RAISE("invalid character: %c\n", c)

    if (vars[c] == NULL) {
        var->type = FREE;
        var->content.freeVar = c;
    } else {
        var->type = BOUND;
        var->content.func = vars[c];
    }
    return var;
}

static const Expr* getExpr(FILE * const fp, Expr ** const p, const Expr ** const vars, int * const parenLevel) {
    char c;
    const Expr *next, *arg;

    switch (c = getsymbol(fp)) {
        case LAMBDA_SYMBOL:
            return getFunction(fp, p, vars, parenLevel);

        case EOF:
            if (*parenLevel > 0) RAISE("expected %c\n", CLOSEPAREN_SYMBOL)
            return *p;

        case OPENPAREN_SYMBOL:
            ++(*parenLevel);
            return getExprList(fp, p, vars, parenLevel);

        case CLOSEPAREN_SYMBOL:
            if (*parenLevel == 0) RAISE("unexpected %c\n", CLOSEPAREN_SYMBOL)
            --(*parenLevel);
            return *p;

        default:
            return getVar(p, vars, c);
    }
}

void printExpr(const Expr* const expr, int indent) {
    printf("%p: ", (void*) expr);
    for (int i = 0; i < indent; ++i) printf("  ");

    switch (expr->type) {
        case FREE:
            printf("free '%c'\n", expr->content.freeVar);
            break;

        case BOUND:
            printf("bound %p\n", (void*) expr->content.func);
            break;

        case FUNC:
            printf("func @ %p\n", (void*) expr->content.func);
            printExpr(expr->content.func, indent+1);
            break;

        case APP:
            printf("apply\n");
            printExpr(expr->content.app.func, indent+1);
            for (int i = 0; i < indent; ++i) printf("  ");
            printf("                to\n");
            printExpr(expr->content.app.arg, indent+1);
            break;

        default:
            printf("null\n");
            break;
    }
}

