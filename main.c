#include "parser.h"

int main(int argc, const char *argv[]) {
    const Expr *vars[256];
    int parenLevel = 0;

    for (int i = 0; i < 256; ++i)
        vars[i] = NULL;

    if (argc < 2) RAISE(1, "error: missing filename\n");

    FILE * const fp = fopen(argv[1], "r");
    if (!fp) RAISE(1, "error: can't open file: %s\n", argv[1]);

    const Expr *root = getExprList(fp, &vars[0], &parenLevel);
    fclose(fp);

    if (!root && parenLevel > -1)
        return 1;

    printExpr(root);

    freeExpr(root);

    /*
    int changes, total = 0, passes = -1;
    do {
        changes = 0;
        root = reduce((Expr*) root, &changes);
        total += changes;
        ++passes;
        printf("----------------------------------\n");
        printExpr(root, 0);
    } while (changes && passes < 5);

    printf("\ndissolved %d functions in %d passes to\n\n", total, passes);
    */
    return 0;
}
