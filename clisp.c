#include <stdio.h>
#include <stdlib.h>

#include <editline/readline.h>
#include "mpc.h"

static char input[2048];

long eval_op(long x, char* op, long y) {

    if(strcmp(op, "+") == 0) {return x + y;}
    if (strcmp(op, "-") == 0 ) {return x - y; }
    if (strcmp(op, "*") == 0 ) {return x * y ;}
    if (strcmp(op, "/") == 0 ) {return x / y ;}
    return 0;
}

long eval(mpc_ast_t* t) {

    if (strstr(t->tag, "number")){
        return atoi(t->contents);

    }
 //Operator is always secondchild
    char* op = t->children[1]->contents;
    // Store third child in 'x'
    long x = eval(t->children[2]);

    // iterate remaining children and combining
    int i = 3;
    while(strstr(t->children[i]->tag, "expr")) {
        x = eval_op(x, op, eval(t->children[i]));
        i++;
    }
return x;
}



int main(int argc, char** argv) {
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Operator = mpc_new("operator");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Lispy = mpc_new("lispy");

    // Define Them
    mpca_lang (MPCA_LANG_DEFAULT,
    "                                      \
    number  : /-?[0-9]+/    ;                 \
    operator  :   '+' | '-' | '*' | '/' ;       \
    expr   : <number> | '(' <operator> <expr>+ ')'; \
     lispy : /^/ <operator> <expr>+ /$/;            \
    ",
       Number, Operator, Expr, Lispy);

    puts("clisp Version 0.0.0.2");
    puts("This software will soon be released under the terms of the GNU GPL V3 license.");
    puts("Press CTRL + C to Exit \n");

    // Main part of REPL (Read, Eval, Print, Loop):

    while(1) {
        char* input = readline("clisp> ");
        if (input == NULL) {                       
            break;
        }

      mpc_result_t r;
      if (mpc_parse("<stdin>", input, Lispy, &r)){
        long result = eval(r.output);
        printf("%li\n", result);
        mpc_ast_delete(r.output);
      } else {
        mpc_err_print(r.error);
        mpc_err_delete(r.error);
      }
     

        free(input);
    }   
    mpc_cleanup(4, Number, Operator, Expr, Lispy);
            return 0;
}