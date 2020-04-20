#include "repr.h"

inline static void _printExprASTIndent(const Expr* const expr, int indent) {
    printf("%p: ", (void*) expr);
    for (int i = 0; i < indent; ++i) printf("  ");

    switch (expr->type) {
        case FREE:
            printf("free '%c'\n", expr->data.freeVar);
            break;

        case BOUND:
            printf("bound @ %p\n", (void*) expr->data.func);
            break;

        case FUNC:
            printf("func @ %p\n", (void*) expr->data.func);
            _printExprASTIndent(expr->data.func, indent+1);
            break;

        case APP:
            printf("apply\n");
            _printExprASTIndent(expr->data.app.func, indent+1);
            for (int i = 0; i < indent; ++i) printf("  ");
            printf("                to\n");
            _printExprASTIndent(expr->data.app.arg, indent+1);
            break;

        default:
            printf("null\n");
            break;
    }
}

void printExprAST(const Expr* const expr) {
    _printExprASTIndent(expr, 0);
}

static void _prepareFuncLabels(Expr *expr) {
    switch (expr->type) {
        case FREE:
        case BOUND:
        case NULL_T:
            return;
        case APP:
            _prepareFuncLabels((Expr*) expr->data.app.func);
            _prepareFuncLabels((Expr*) expr->data.app.arg);
            return;
        case FUNC:
            ((LabelledExpr*) expr)->data.func.label = 0;
            _prepareFuncLabels((Expr*) expr->data.func);
            return;
    }
}

static const char labelOrder[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

inline static int isAvailable(const Expr *expr, const char c) {
    switch (expr->type) {
        case FREE:
            return (expr->data.freeVar != c);
        case BOUND:
            return (((const LabelledExpr*) expr->data.func)->data.func.label != c);
        case NULL_T:
            return 0;
        case APP:
            return isAvailable(expr->data.app.func, c) && isAvailable(expr->data.app.arg, c);
        case FUNC:
            return isAvailable(expr->data.func, c);
    }
}

inline static char nextLabel(const Expr *func) {
    const char *c = &labelOrder[0];
    while (!isAvailable(func->data.func, *c)) ++c;
    return *c;
}

void printExpr(const Expr *expr) {
    switch (expr->type) {
        case FREE:
            putchar(expr->data.freeVar);
            return;
        case BOUND:
            putchar(((const LabelledExpr*) expr->data.func)->data.func.label);
            return;
        case NULL_T:
            putchar('?');
            return;
        case APP:
            if (expr->data.app.func->type == FUNC) {
                putchar(OPENPAREN_SYMBOL);
                printExpr(expr->data.app.func);
                putchar(CLOSEPAREN_SYMBOL);
            } else
                printExpr(expr->data.app.func);

            if (expr->data.app.arg->type == APP) {
                putchar(OPENPAREN_SYMBOL);
                printExpr(expr->data.app.arg);
                putchar(CLOSEPAREN_SYMBOL);
            } else
                printExpr(expr->data.app.arg);
            return;
        case FUNC:
            putchar(LAMBDA_SYMBOL);
            _prepareFuncLabels((Expr*) expr->data.func);
            ((LabelledExpr*) expr)->data.func.label = nextLabel(expr);
            putchar(((LabelledExpr*) expr)->data.func.label);
            putchar(DOT_SYMBOL);
            printExpr(expr->data.func);
            return;
    }
}
