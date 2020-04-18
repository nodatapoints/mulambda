# μLambda
Minimal C99 lambda calculus interpreter

Grammar:
```
# all whitespace is silently ignored

variable ::= "a" | "b" | ... ;
function ::= "L" , variable , "." , expression ;
expression ::= variable | function | application | "(" , expression , ")" ;
application ::= expression , expression ;
```

