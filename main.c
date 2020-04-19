#include "parser.h"

int main(int argc, const char *argv[]) {
    if (argc < 2) RAISE(1, "error: missing filename\n");
    FILE * const fp = fopen(argv[1], "r");
    if (!fp) RAISE(1, "error: can't open file: %s\n", argv[1]);

    Expr *root = parse(fp);
    fclose(fp);

#ifndef DEBUG
    printExpr(root);
#endif
    freeExpr(root);
    return 0;
}
