#ifndef EVAL_H
#define EVAL_H

#include "parser.h"
#include "string.h"

#define MAX_PASSES 1024

Expr* eval(Expr * const expr);
Expr* expand(Expr * const expr, int * const n);
#endif  // EVAL_H
