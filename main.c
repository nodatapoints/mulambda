#include "parser.h"

int main(int argc, const char *argv[]) {
    const Expr *vars[256];
    Expr space[2048];

    if (argc < 2) {
        fprintf(stderr, "error: missing filename\n");
        return 1;
    }

    FILE * const fp = fopen(argv[1], "r");
    if (fp == NULL) {
        fprintf(stderr, "error: can't open file: %s\n", argv[1]);
        return 1;
    }

    int parenLevel = 0;
    Expr *ptr = &space[0];

    const Expr *ret = getExprList(fp, &ptr, &vars[0], &parenLevel);
    fclose(fp);

    if (ret == NULL)
        return 1;

    printExpr(ret, 0);
    return 0;
}
