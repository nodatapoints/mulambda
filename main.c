#include "parser.h"
#include "eval.h"
#include "repr.h"

int main(int argc, const char *argv[]) {
    Expr *root = parse();
    if (!root) RAISE(2, "could not parse expression\n")

    root = eval(root);
    if (!root) RAISE(2, "could not evaluate expression\n")

    printExpr(root);
    printf("\n");

    freeExpr(root);

    return 0;
}
