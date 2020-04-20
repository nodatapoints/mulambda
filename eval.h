#ifndef EVAL_H
#define EVAL_H

#include "parser.h"
#include "string.h"

#define MAX_PASSES 65536

Expr* eval(Expr * const expr);
Expr* expand(Expr * const expr, int * const n);
#endif  // EVAL_H
