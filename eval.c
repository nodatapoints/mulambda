#include "eval.h"

static int rebind(Expr * const expr, const Expr * const old, const Expr * new) {
    P_DEBUG("rebinding %p to %p in %p \n", (void*) old, (void*) new, (void*) expr)
    if (old == new)
        return 0;

    switch (expr->type) {
        case NULL_T:
            RAISE(-1, "cant rebind null\n");
        case FREE:
            return 0;
        case FUNC:
            return rebind((Expr*) expr->data.func, old, new);
        case APP:
            return rebind((Expr*) expr->data.app.func, old, new)
                + rebind((Expr*) expr->data.app.arg, old, new);
        case BOUND:
            if (expr->data.func == old) {
                expr->data.func = new;
                return 1;
            }
            return 0;
    }
}

static Expr* copyExpr(Expr * const target, const Expr * const src) {
    P_DEBUG("copy %p -> %p\n", (void*) src, (void*) target)
    Expr *func, *arg;
    target->type = src->type;
    switch (src->type) {
        case NULL_T:
            RAISE(NULL, "cant copy null\n");
        case FREE:
        case BOUND:
            memcpy(target, src, sizeof(Expr));
            break;
        case FUNC:
            MALLOC(func)
            target->data.func = copyExpr(func, src->data.func);
            rebind((Expr*) target->data.func, src, target);
            break;
        case APP:
            MALLOC(func)
            target->data.app.func = copyExpr(func, src->data.app.func);
            MALLOC(arg)
            target->data.app.arg = copyExpr(arg, src->data.app.arg);
            break;
    }
    return target;
}

static int apply(Expr * const expr, const Expr * const func, const Expr * const arg) {
    switch (expr->type) {
        case NULL_T:
            RAISE(-1, "cant replace null\n");
        case FREE:
            return 0;
        case FUNC:
            return apply((Expr*) expr->data.func, func, arg);
        case APP:
            return apply((Expr*) expr->data.app.func, func, arg)
               + apply((Expr*) expr->data.app.arg, func, arg);
        case BOUND:
            if (expr->data.func == func) {
                copyExpr(expr, arg);
                return 1;
            }
            return 0;
    }
}

Expr* expand(Expr * const expr, int * const n) {
    Expr *ret;
    switch (expr->type) {
        case NULL_T: RAISE(NULL, "cant evaluate null\n");
        case BOUND:
        case FREE:
            return expr;
        case FUNC:
            expr->data.func = expand((Expr*) expr->data.func, n);
            return expr;
        case APP:
            if (expr->data.app.func->type == FUNC) {
                apply((Expr*) expr->data.app.func->data.func,
                    expr->data.app.func,
                    expr->data.app.arg);

                ret = (Expr*) expr->data.app.func->data.func;
#ifdef DEBUG
    printf("debug: freeing %p\n", (void*) expr->data.app.func);
    printf("debug: freeing %p\n", (void*) expr);
#endif
                free((void*) expr->data.app.func);
                freeExpr(expr->data.app.arg);
                free((void*) expr);

                ++(*n);
                return ret;
            }
            expr->data.app.func = expand((Expr*) expr->data.app.func, n);
            expr->data.app.arg = expand((Expr*) expr->data.app.arg, n);

            return expr;
    }
}

Expr* eval(Expr *expr) {
    int changes, total = 0, passes = -1;
    do {

#ifdef DEBUG
        printExpr(root);
        printf("-------------------------------------------------\n");
#endif

        changes = 0;
        expr = expand((Expr*) expr, &changes);
        if (!expr) return NULL;
        total += changes;
        ++passes;
        if (passes > MAX_PASSES)
            RAISE(NULL, "exceeded expansion limit (%d)\n", MAX_PASSES);

#ifdef DEBUG
        printf("done\ndissolved %d functions in %d passes\n", total, passes);
#endif

    } while (changes);
    return expr;
}
