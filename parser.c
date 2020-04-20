#include "parser.h"

static char getsymbol(FILE *fp) {
    char c;
    while (isspace(c = fgetc(fp)));
    return c;
}

static const Expr* getExprList(FILE * const fp, const Expr *vars[], int * const parenLevel) {
    int currentLevel = *parenLevel;
    const Expr *expr, *arg;
    Expr *app;

    expr = getExpr(fp, vars, parenLevel);
    if (!expr) {
        if (*parenLevel < currentLevel) RAISE(NULL, "empty expression\n");
        return NULL;
    }
    if (*parenLevel < currentLevel) return expr;

    while (1) {
        arg = getExpr(fp, vars, parenLevel);
        if (!arg)
            if (*parenLevel < currentLevel)
                return expr;
        else
            return NULL;

        if (*parenLevel < currentLevel && 0 < currentLevel)
            return expr;

        MALLOC(app)
        app->type = APP;
        app->data.app.func = expr;
        app->data.app.arg = arg;

        expr = app;
    }
    return expr;
}

Expr* parse(FILE * const fp) {
    int parenLevel = 0;

    const Expr *vars[256];
    for (int i = 0; i < 256; ++i)
        vars[i] = NULL;

    const Expr *root = getExprList(fp, &vars[0], &parenLevel);
    if (!root && parenLevel > -1)
        return NULL;

    return (Expr*) root;
}

static const Expr* getFunction(FILE * const fp, const Expr *vars[], int * const parenLevel) {
    char c;
    const Expr *prev, *body;
    Expr *func;

    c = getsymbol(fp);
    if (c == EOF) RAISE(NULL, "unexpected EOF in function\n")
    if (!isalpha(c)) RAISE(NULL, "invalid parameter: %c\n", c)

    if (getsymbol(fp) != DOT_SYMBOL) RAISE(NULL, "expected %c\n", DOT_SYMBOL)

    MALLOC(func)

    prev = vars[c];
    vars[c] = func;

    body = getExprList(fp, vars, parenLevel);
    if (!body) return NULL;

    vars[c] = prev;

    func->type = FUNC;
    func->data.func = (Expr*) body;

    return func;
}

static const Expr* getVar(const Expr *vars[], char c) {
    Expr *var;
    MALLOC(var)
    if (!isalpha(c)) RAISE(NULL, "invalid character: %c\n", c)

    if (!vars[c]) {
        var->type = FREE;
        var->data.freeVar = c;
    } else {
        var->type = BOUND;
        var->data.func = (Expr*) vars[c];
    }
    return var;
}

static const Expr* getExpr(FILE * const fp, const Expr *vars[], int * const parenLevel) {
    char c = getsymbol(fp);
    switch (c) {
        case LAMBDA_SYMBOL:
            return getFunction(fp, vars, parenLevel);

        case EOF:
            if (*parenLevel > 0) RAISE(NULL, "expected %c\n", CLOSEPAREN_SYMBOL)
            --(*parenLevel);
            return NULL;

        case OPENPAREN_SYMBOL:
            ++(*parenLevel);
            return getExprList(fp, vars, parenLevel);

        case CLOSEPAREN_SYMBOL:
            if (*parenLevel == 0) RAISE(NULL, "unexpected %c\n", CLOSEPAREN_SYMBOL)
            --(*parenLevel);
            return NULL;

        default:
            return getVar(vars, c);
    }
}

void freeExpr(const Expr * const expr) {
    switch (expr->type) {
        case FUNC:
            freeExpr(expr->data.func);
            break;
        case APP:
            freeExpr(expr->data.app.func);
            freeExpr(expr->data.app.arg);
        case BOUND:
        case FREE:
            break;
        case NULL_T:
            RAISE(, "cant free null\n")
    }
    P_DEBUG("freeing %p\n", (void*) expr)
    free((void*) expr);
}
