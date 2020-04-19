#include "parser.h"
#include "eval.h"
#include "repr.h"

int main(int argc, const char *argv[]) {
    if (argc < 2) RAISE(1, "missing filename\n");
    FILE * const fp = fopen(argv[1], "r");
    if (!fp) RAISE(1, "can't open file: %s\n", argv[1]);

    Expr *root = parse(fp);
    if (!root) RAISE(2, "could not parse expression\n")

    fclose(fp);

    root = eval(root);
    if (!root) RAISE(2, "could not evaluate expression\n")

    printExpr(root);
    printf("\n");

    freeExpr(root);

    return 0;
}
