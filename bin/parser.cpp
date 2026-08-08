/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
#line 6 "parser.lemon"

#include <stdbool.h> 
#include <stdint.h>
#include <string.h>
#include <cassert>
#include <cstddef>
#include "debugger.h"

#include <toolbox/mm.h>

using Debug::Token;

#ifdef __cplusplus
extern "C" {
#endif

uint32_t cpuGetSR();
uint32_t cpuGetPC();
uint32_t cpuGetAReg(unsigned);
uint32_t cpuGetDReg(unsigned);

#ifdef __cplusplus
}
#endif


#undef NDEBUG

#line 38 "parser.c"
/* Next is all token values, in a form suitable for use by makeheaders.
** This section will be null unless lemon is run with the -m switch.
*/
/* 
** These constants (all generated automatically by the parser generator)
** specify the various kinds of tokens (terminals) that the parser
** understands. 
**
** Each symbol here is a terminal symbol in the grammar.
*/
/* Make sure the INTERFACE macro is defined.
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/* The next thing included is series of defines which control
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 terminals
**                       and nonterminals.  "int" is used otherwise.
**    YYNOCODE           is a number of type YYCODETYPE which corresponds
**                       to no legal terminal or nonterminal number.  This
**                       number is used to fill in empty slots of the hash 
**                       table.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       have fall-back values which should be used if the
**                       original value of the token will not parse.
**    YYACTIONTYPE       is the data type used for storing terminal
**                       and nonterminal numbers.  "unsigned char" is
**                       used if there are fewer than 250 rules and
**                       states combined.  "int" is used otherwise.
**    ParseTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 59
#define YYACTIONTYPE unsigned char
#define ParseTOKENTYPE  Token 
typedef union {
  ParseTOKENTYPE yy0;
  int yy117;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define ParseARG_SDECL  Debug::Command *command ;
#define ParseARG_PDECL , Debug::Command *command 
#define ParseARG_FETCH  Debug::Command *command  = yypParser->command 
#define ParseARG_STORE yypParser->command  = command 
#define YYNSTATE 157
#define YYNRULE 70
#define YYERRORSYMBOL 52
#define YYERRSYMDT yy117
#define YY_NO_ACTION      (YYNSTATE+YYNRULE+2)
#define YY_ACCEPT_ACTION  (YYNSTATE+YYNRULE+1)
#define YY_ERROR_ACTION   (YYNSTATE+YYNRULE)

/* Next are that tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N < YYNSTATE                  Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   YYNSTATE <= N < YYNSTATE+YYNRULE   Reduce by rule N-YYNSTATE.
**
**   N == YYNSTATE+YYNRULE              A syntax error has occurred.
**
**   N == YYNSTATE+YYNRULE+1            The parser accepts its input.
**
**   N == YYNSTATE+YYNRULE+2            No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as
**
**      yy_action[ yy_shift_ofst[S] + X ]
**
** If the index value yy_shift_ofst[S]+X is out of range or if the value
** yy_lookahead[yy_shift_ofst[S]+X] is not equal to X or if yy_shift_ofst[S]
** is equal to YY_SHIFT_USE_DFLT, it means that the action is not in the table
** and that yy_default[S] should be used instead.  
**
** The formula above is for computing the action when the lookahead is
** a terminal symbol.  If the lookahead is a non-terminal (as occurs after
** a reduce action) then the yy_reduce_ofst[] array is used in place of
** the yy_shift_ofst[] array and YY_REDUCE_USE_DFLT is used in place of
** YY_SHIFT_USE_DFLT.
**
** The following are the tables generated in this section:
**
**  yy_action[]        A single table containing all actions.
**  yy_lookahead[]     A table containing the lookahead for each entry in
**                     yy_action.  Used to detect hash collisions.
**  yy_shift_ofst[]    For each state, the offset into yy_action for
**                     shifting terminals.
**  yy_reduce_ofst[]   For each state, the offset into yy_action for
**                     shifting non-terminals after a reduce.
**  yy_default[]       Default action for each state.
*/
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    42,   38,   37,  113,   97,   40,   41,   94,   20,    3,
 /*    10 */    87,   91,    1,    5,    2,   16,    4,   14,   92,  116,
 /*    20 */     6,   98,   23,   25,   17,   18,   19,   21,   22,   88,
 /*    30 */    99,  149,  102,   83,   86,   27,   89,  130,   84,    7,
 /*    40 */    36,   35,   34,   33,   31,   32,   26,   28,   29,   30,
 /*    50 */    23,   25,   17,   18,   19,   21,   22,   43,    9,  121,
 /*    60 */    85,   82,  101,   96,   95,   44,   54,  100,  131,  122,
 /*    70 */    15,   24,  153,   90,   75,   93,    7,   36,   35,   34,
 /*    80 */    33,   31,   32,   26,   28,   29,   30,   23,   25,   17,
 /*    90 */    18,   19,   21,   22,  118,  120,  106,   17,   18,   19,
 /*   100 */    21,   22,  228,   45,  100,  131,  122,   13,   12,    7,
 /*   110 */    36,   35,   34,   33,   31,   32,   26,   28,   29,   30,
 /*   120 */    23,   25,   17,   18,   19,   21,   22,  143,  155,  110,
 /*   130 */     8,    7,   36,   35,   34,   33,   31,   32,   26,   28,
 /*   140 */    29,   30,   23,   25,   17,   18,   19,   21,   22,   36,
 /*   150 */    35,   34,   33,   31,   32,   26,   28,   29,   30,   23,
 /*   160 */    25,   17,   18,   19,   21,   22,   81,  134,    7,   36,
 /*   170 */    35,   34,   33,   31,   32,   26,   28,   29,   30,   23,
 /*   180 */    25,   17,   18,   19,   21,   22,  144,  105,  139,  135,
 /*   190 */     7,   36,   35,   34,   33,   31,   32,   26,   28,   29,
 /*   200 */    30,   23,   25,   17,   18,   19,   21,   22,  107,  109,
 /*   210 */   117,  111,    7,   36,   35,   34,   33,   31,   32,   26,
 /*   220 */    28,   29,   30,   23,   25,   17,   18,   19,   21,   22,
 /*   230 */    11,  114,  151,  130,    7,   36,   35,   34,   33,   31,
 /*   240 */    32,   26,   28,   29,   30,   23,   25,   17,   18,   19,
 /*   250 */    21,   22,   10,  130,  108,  130,    7,   36,   35,   34,
 /*   260 */    33,   31,   32,   26,   28,   29,   30,   23,   25,   17,
 /*   270 */    18,   19,   21,   22,  130,  130,  123,  130,    7,   36,
 /*   280 */    35,   34,   33,   31,   32,   26,   28,   29,   30,   23,
 /*   290 */    25,   17,   18,   19,   21,   22,  130,  130,  142,  130,
 /*   300 */     7,   36,   35,   34,   33,   31,   32,   26,   28,   29,
 /*   310 */    30,   23,   25,   17,   18,   19,   21,   22,  130,  130,
 /*   320 */   119,  130,    7,   36,   35,   34,   33,   31,   32,   26,
 /*   330 */    28,   29,   30,   23,   25,   17,   18,   19,   21,   22,
 /*   340 */   130,  130,  148,  130,    7,   36,   35,   34,   33,   31,
 /*   350 */    32,   26,   28,   29,   30,   23,   25,   17,   18,   19,
 /*   360 */    21,   22,  130,  130,  154,  130,    7,   36,   35,   34,
 /*   370 */    33,   31,   32,   26,   28,   29,   30,   23,   25,   17,
 /*   380 */    18,   19,   21,   22,  130,  130,  156,  130,    7,   36,
 /*   390 */    35,   34,   33,   31,   32,   26,   28,   29,   30,   23,
 /*   400 */    25,   17,   18,   19,   21,   22,  130,  130,  115,  130,
 /*   410 */     7,   36,   35,   34,   33,   31,   32,   26,   28,   29,
 /*   420 */    30,   23,   25,   17,   18,   19,   21,   22,  130,  130,
 /*   430 */   112,  130,    7,   36,   35,   34,   33,   31,   32,   26,
 /*   440 */    28,   29,   30,   23,   25,   17,   18,   19,   21,   22,
 /*   450 */   130,  130,  104,  130,    7,   36,   35,   34,   33,   31,
 /*   460 */    32,   26,   28,   29,   30,   23,   25,   17,   18,   19,
 /*   470 */    21,   22,   35,   34,   33,   31,   32,   26,   28,   29,
 /*   480 */    30,   23,   25,   17,   18,   19,   21,   22,  130,   80,
 /*   490 */   130,    7,   36,   35,   34,   33,   31,   32,   26,   28,
 /*   500 */    29,   30,   23,   25,   17,   18,   19,   21,   22,   34,
 /*   510 */    33,   31,   32,   26,   28,   29,   30,   23,   25,   17,
 /*   520 */    18,   19,   21,   22,   33,   31,   32,   26,   28,   29,
 /*   530 */    30,   23,   25,   17,   18,   19,   21,   22,  130,  130,
 /*   540 */   129,   31,   32,   26,   28,   29,   30,   23,   25,   17,
 /*   550 */    18,   19,   21,   22,  130,   42,   38,   39,  130,  130,
 /*   560 */    40,   41,  103,  130,   42,   38,   39,  130,  130,   40,
 /*   570 */    41,  150,  124,  131,  122,   42,   38,   39,  130,  130,
 /*   580 */    40,   41,  140,  130,  132,  147,  130,  145,  141,  130,
 /*   590 */    27,  130,  130,  132,  147,  130,  145,  141,  130,   27,
 /*   600 */   147,  130,  145,  141,  132,  147,  130,  145,  141,  130,
 /*   610 */    27,  130,  130,  130,   42,   38,   39,  130,  130,   40,
 /*   620 */    41,  146,  130,   42,   38,   39,  130,  130,   40,   41,
 /*   630 */   152,  127,  131,  122,   42,   38,   39,  130,  130,   40,
 /*   640 */    41,  137,  130,  132,  147,  130,  145,  141,  130,   27,
 /*   650 */   130,  130,  132,  147,  130,  145,  141,  130,   27,  130,
 /*   660 */   130,  130,  130,  132,  147,  130,  145,  141,  130,   27,
 /*   670 */   130,  130,  130,   42,   38,   39,  130,  130,   40,   41,
 /*   680 */    26,   28,   29,   30,   23,   25,   17,   18,   19,   21,
 /*   690 */    22,   62,  100,  131,  122,   53,  100,  131,  122,  125,
 /*   700 */   131,  122,  132,  147,  130,  145,  141,  130,   27,  130,
 /*   710 */   130,   77,  100,  131,  122,   57,  100,  131,  122,  133,
 /*   720 */   100,  131,  122,  130,  136,  100,  131,  122,   47,  100,
 /*   730 */   131,  122,  130,   58,  100,  131,  122,   59,  100,  131,
 /*   740 */   122,   66,  100,  131,  122,   60,  100,  131,  122,   65,
 /*   750 */   100,  131,  122,   48,  100,  131,  122,   56,  100,  131,
 /*   760 */   122,   64,  100,  131,  122,   51,  100,  131,  122,   49,
 /*   770 */   100,  131,  122,   74,  100,  131,  122,   73,  100,  131,
 /*   780 */   122,   61,  100,  131,  122,   72,  100,  131,  122,   69,
 /*   790 */   100,  131,  122,   63,  100,  131,  122,   70,  100,  131,
 /*   800 */   122,   68,  100,  131,  122,   67,  100,  131,  122,  138,
 /*   810 */   100,  131,  122,   78,  100,  131,  122,   79,  100,  131,
 /*   820 */   122,   55,  100,  131,  122,   52,  100,  131,  122,   46,
 /*   830 */   100,  131,  122,   76,  100,  131,  122,   50,  100,  131,
 /*   840 */   122,   71,  100,  131,  122,  126,  131,  122,   19,   21,
 /*   850 */    22,  128,  131,  122,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    14,   15,   16,   21,   56,   19,   20,   43,   22,   23,
 /*    10 */    24,   25,   26,   27,   28,   29,   30,   31,   52,   21,
 /*    20 */    34,   35,   12,   13,   14,   15,   16,   17,   18,   43,
 /*    30 */    44,   50,   46,   47,   48,   49,   37,   51,   39,    1,
 /*    40 */     2,    3,    4,    5,    6,    7,    8,    9,   10,   11,
 /*    50 */    12,   13,   14,   15,   16,   17,   18,   49,   45,   21,
 /*    60 */    37,   38,   39,   40,   41,   42,   54,   55,   56,   57,
 /*    70 */    32,   33,   21,   37,   36,   39,    1,    2,    3,    4,
 /*    80 */     5,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*    90 */    15,   16,   17,   18,   21,   21,   21,   14,   15,   16,
 /*   100 */    17,   18,   53,   54,   55,   56,   57,   32,   33,    1,
 /*   110 */     2,    3,    4,    5,    6,    7,    8,    9,   10,   11,
 /*   120 */    12,   13,   14,   15,   16,   17,   18,   21,   21,   21,
 /*   130 */    45,    1,    2,    3,    4,    5,    6,    7,    8,    9,
 /*   140 */    10,   11,   12,   13,   14,   15,   16,   17,   18,    2,
 /*   150 */     3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
 /*   160 */    13,   14,   15,   16,   17,   18,   36,   21,    1,    2,
 /*   170 */     3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
 /*   180 */    13,   14,   15,   16,   17,   18,   21,   21,   21,   21,
 /*   190 */     1,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*   200 */    11,   12,   13,   14,   15,   16,   17,   18,   21,   21,
 /*   210 */    21,   21,    1,    2,    3,    4,    5,    6,    7,    8,
 /*   220 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*   230 */    45,   21,   21,   58,    1,    2,    3,    4,    5,    6,
 /*   240 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   250 */    17,   18,   45,   58,   21,   58,    1,    2,    3,    4,
 /*   260 */     5,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*   270 */    15,   16,   17,   18,   58,   58,   21,   58,    1,    2,
 /*   280 */     3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
 /*   290 */    13,   14,   15,   16,   17,   18,   58,   58,   21,   58,
 /*   300 */     1,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*   310 */    11,   12,   13,   14,   15,   16,   17,   18,   58,   58,
 /*   320 */    21,   58,    1,    2,    3,    4,    5,    6,    7,    8,
 /*   330 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*   340 */    58,   58,   21,   58,    1,    2,    3,    4,    5,    6,
 /*   350 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   360 */    17,   18,   58,   58,   21,   58,    1,    2,    3,    4,
 /*   370 */     5,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*   380 */    15,   16,   17,   18,   58,   58,   21,   58,    1,    2,
 /*   390 */     3,    4,    5,    6,    7,    8,    9,   10,   11,   12,
 /*   400 */    13,   14,   15,   16,   17,   18,   58,   58,   21,   58,
 /*   410 */     1,    2,    3,    4,    5,    6,    7,    8,    9,   10,
 /*   420 */    11,   12,   13,   14,   15,   16,   17,   18,   58,   58,
 /*   430 */    21,   58,    1,    2,    3,    4,    5,    6,    7,    8,
 /*   440 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*   450 */    58,   58,   21,   58,    1,    2,    3,    4,    5,    6,
 /*   460 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   470 */    17,   18,    3,    4,    5,    6,    7,    8,    9,   10,
 /*   480 */    11,   12,   13,   14,   15,   16,   17,   18,   58,   36,
 /*   490 */    58,    1,    2,    3,    4,    5,    6,    7,    8,    9,
 /*   500 */    10,   11,   12,   13,   14,   15,   16,   17,   18,    4,
 /*   510 */     5,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*   520 */    15,   16,   17,   18,    5,    6,    7,    8,    9,   10,
 /*   530 */    11,   12,   13,   14,   15,   16,   17,   18,   58,   58,
 /*   540 */    50,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*   550 */    15,   16,   17,   18,   58,   14,   15,   16,   58,   58,
 /*   560 */    19,   20,   21,   58,   14,   15,   16,   58,   58,   19,
 /*   570 */    20,   21,   55,   56,   57,   14,   15,   16,   58,   58,
 /*   580 */    19,   20,   21,   58,   43,   44,   58,   46,   47,   58,
 /*   590 */    49,   58,   51,   43,   44,   58,   46,   47,   58,   49,
 /*   600 */    44,   51,   46,   47,   43,   44,   58,   46,   47,   58,
 /*   610 */    49,   58,   51,   58,   14,   15,   16,   58,   58,   19,
 /*   620 */    20,   21,   58,   14,   15,   16,   58,   58,   19,   20,
 /*   630 */    21,   55,   56,   57,   14,   15,   16,   58,   58,   19,
 /*   640 */    20,   21,   58,   43,   44,   58,   46,   47,   58,   49,
 /*   650 */    58,   51,   43,   44,   58,   46,   47,   58,   49,   58,
 /*   660 */    51,   58,   58,   43,   44,   58,   46,   47,   58,   49,
 /*   670 */    58,   51,   58,   14,   15,   16,   58,   58,   19,   20,
 /*   680 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   690 */    18,   54,   55,   56,   57,   54,   55,   56,   57,   55,
 /*   700 */    56,   57,   43,   44,   58,   46,   47,   58,   49,   58,
 /*   710 */    51,   54,   55,   56,   57,   54,   55,   56,   57,   54,
 /*   720 */    55,   56,   57,   58,   54,   55,   56,   57,   54,   55,
 /*   730 */    56,   57,   58,   54,   55,   56,   57,   54,   55,   56,
 /*   740 */    57,   54,   55,   56,   57,   54,   55,   56,   57,   54,
 /*   750 */    55,   56,   57,   54,   55,   56,   57,   54,   55,   56,
 /*   760 */    57,   54,   55,   56,   57,   54,   55,   56,   57,   54,
 /*   770 */    55,   56,   57,   54,   55,   56,   57,   54,   55,   56,
 /*   780 */    57,   54,   55,   56,   57,   54,   55,   56,   57,   54,
 /*   790 */    55,   56,   57,   54,   55,   56,   57,   54,   55,   56,
 /*   800 */    57,   54,   55,   56,   57,   54,   55,   56,   57,   54,
 /*   810 */    55,   56,   57,   54,   55,   56,   57,   54,   55,   56,
 /*   820 */    57,   54,   55,   56,   57,   54,   55,   56,   57,   54,
 /*   830 */    55,   56,   57,   54,   55,   56,   57,   54,   55,   56,
 /*   840 */    57,   54,   55,   56,   57,   55,   56,   57,   16,   17,
 /*   850 */    18,   55,   56,   57,
};
#define YY_SHIFT_USE_DFLT (-37)
#define YY_SHIFT_MAX 102
static const short yy_shift_ofst[] = {
 /*     0 */   -14,  600,  609,  561,  541,  550,  659,  659,  659,  659,
 /*    10 */   659,  659,  659,  659,  659,  659,  659,  659,  659,  659,
 /*    20 */   659,  659,  659,  659,  659,  659,  659,  659,  659,  659,
 /*    30 */   659,  659,  659,  659,  659,  659,  659,  620,  659,  659,
 /*    40 */   659,  659,  659,  556,  -36,   38,   75,  167,  130,  108,
 /*    50 */   233,  490,  453,  431,  409,  365,  343,  321,  299,  277,
 /*    60 */   255,  387,  211,  189,  147,  469,  505,  519,  535,  672,
 /*    70 */   672,   10,   10,   10,   10,   23,   83,   83,  832,  832,
 /*    80 */    -1,   36,   -2,   13,   51,   73,   74,  106,   85,  107,
 /*    90 */   146,  165,  166,  168,  187,  188,  190,  -19,  -18,  185,
 /*   100 */     8,  210,  207,
};
#define YY_REDUCE_USE_DFLT (-53)
#define YY_REDUCE_MAX 44
static const short yy_reduce_ofst[] = {
 /*     0 */    49,  661,  703,  683,  641,  637,   12,  707,  691,  679,
 /*    10 */   739,  727,  715,  783,  775,  771,  767,  763,  759,  755,
 /*    20 */   674,  670,  665,  657,  699,  779,  787,  711,  719,  723,
 /*    30 */   731,  735,  743,  747,  751,  687,  695,  796,  644,  796,
 /*    40 */   576,  790,  517,  -52,  -34,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   227,  227,  227,  227,  227,  227,  227,  227,  227,  227,
 /*    10 */   227,  227,  227,  227,  227,  227,  227,  227,  227,  227,
 /*    20 */   227,  227,  227,  227,  227,  227,  227,  227,  227,  227,
 /*    30 */   227,  227,  227,  227,  227,  227,  227,  227,  227,  227,
 /*    40 */   227,  227,  227,  227,  227,  227,  227,  227,  227,  227,
 /*    50 */   227,  227,  227,  227,  227,  227,  227,  227,  227,  227,
 /*    60 */   227,  227,  227,  227,  212,  211,  210,  209,  208,  206,
 /*    70 */   207,  202,  205,  204,  203,  227,  201,  200,  196,  195,
 /*    80 */   227,  227,  227,  226,  227,  227,  227,  227,  223,  227,
 /*    90 */   227,  227,  227,  227,  227,  227,  227,  227,  227,  224,
 /*   100 */   194,  227,  225,  171,  172,  188,  173,  187,  174,  186,
 /*   110 */   175,  185,  176,  177,  182,  189,  181,  190,  178,  191,
 /*   120 */   192,  157,  214,  193,  215,  216,  217,  218,  219,  220,
 /*   130 */   221,  222,  223,  199,  180,  183,  198,  158,  197,  159,
 /*   140 */   160,  226,  161,  162,  163,  225,  164,  224,  165,  213,
 /*   150 */   166,  167,  168,  184,  169,  179,  170,
};
#define YY_SZ_ACTTAB (int)(sizeof(yy_action)/sizeof(yy_action[0]))

/* The next table maps tokens into fallback tokens.  If a construct
** like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammer, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
*/
#ifdef YYFALLBACK
static const YYCODETYPE yyFallback[] = {
};
#endif /* YYFALLBACK */

/* The following structure represents a single element of the
** parser's stack.  Information stored includes:
**
**   +  The state number for the parser at this level of the stack.
**
**   +  The value of the token stored at this level of the stack.
**      (In other words, the "major" token.)
**
**   +  The semantic value stored at this level of the stack.  This is
**      the information used by the action routines in the grammar.
**      It is sometimes called the "minor" token.
*/
struct yyStackEntry {
  int stateno;       /* The state-number */
  int major;         /* The major token value.  This is the code
                     ** number for the token at this stack level */
  YYMINORTYPE minor; /* The user-supplied minor token value.  This
                     ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  int yyidx;                    /* Index of top element in stack */
  int yyerrcnt;                 /* Shifts left before out of the error */
  ParseARG_SDECL                /* A place to hold %extra_argument */
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
};
typedef struct yyParser yyParser;

#ifndef NDEBUG
#include <stdio.h>
static FILE *yyTraceFILE = 0;
static char *yyTracePrompt = 0;
#endif /* NDEBUG */

#ifndef NDEBUG
/* 
** Turn parser tracing on by giving a stream to which to write the trace
** and a prompt to preface each trace message.  Tracing is turned off
** by making either argument NULL 
**
** Inputs:
** <ul>
** <li> A FILE* to which trace output should be written.
**      If NULL, then tracing is turned off.
** <li> A prefix string written at the beginning of every
**      line of trace output.  If NULL, then tracing is
**      turned off.
** </ul>
**
** Outputs:
** None.
*/
void ParseTrace(FILE *TraceFILE, char *zTracePrompt){
  yyTraceFILE = TraceFILE;
  yyTracePrompt = zTracePrompt;
  if( yyTraceFILE==0 ) yyTracePrompt = 0;
  else if( yyTracePrompt==0 ) yyTraceFILE = 0;
}
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing shifts, the names of all terminals and nonterminals
** are required.  The following table supplies these names */
static const char *const yyTokenName[] = { 
  "$",             "PIPEPIPE",      "AMPAMP",        "PIPE",        
  "CARET",         "AMP",           "EQEQ",          "BANGEQ",      
  "LT",            "LTEQ",          "GT",            "GTEQ",        
  "LTLT",          "GTGT",          "PLUS",          "MINUS",       
  "STAR",          "SLASH",         "PERCENT",       "BANG",        
  "TILDE",         "EOL",           "PRINT",         "BREAK",       
  "BACKTRACE",     "CONTINUE",      "TBREAK",        "RBREAK",      
  "WBREAK",        "RWBREAK",       "NEXT",          "DUMP",        
  "COLON",         "AT",            "LIST",          "STACKCRAWL",  
  "SEMI",          "SEMIH",         "SEMII",         "SEMIL",       
  "SEMIDATE",      "SEMIERROR",     "SEMIT",         "IDENTIFIER",  
  "DREGISTER",     "EQ",            "AREGISTER",     "XREGISTER",   
  "HELP",          "LPAREN",        "RPAREN",        "INTEGER",     
  "error",         "stmt",          "expr",          "unary",       
  "register",      "term",        
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "stmt ::= expr EOL",
 /*   1 */ "stmt ::= STAR EOL",
 /*   2 */ "stmt ::= PRINT expr EOL",
 /*   3 */ "stmt ::= BREAK EOL",
 /*   4 */ "stmt ::= BREAK expr EOL",
 /*   5 */ "stmt ::= BACKTRACE EOL",
 /*   6 */ "stmt ::= CONTINUE EOL",
 /*   7 */ "stmt ::= TBREAK EOL",
 /*   8 */ "stmt ::= TBREAK expr EOL",
 /*   9 */ "stmt ::= RBREAK EOL",
 /*  10 */ "stmt ::= RBREAK expr EOL",
 /*  11 */ "stmt ::= WBREAK EOL",
 /*  12 */ "stmt ::= WBREAK expr EOL",
 /*  13 */ "stmt ::= RWBREAK expr EOL",
 /*  14 */ "stmt ::= NEXT EOL",
 /*  15 */ "stmt ::= NEXT expr EOL",
 /*  16 */ "stmt ::= DUMP expr EOL",
 /*  17 */ "stmt ::= DUMP expr COLON expr EOL",
 /*  18 */ "stmt ::= DUMP expr AT expr EOL",
 /*  19 */ "stmt ::= LIST expr EOL",
 /*  20 */ "stmt ::= STACKCRAWL EOL",
 /*  21 */ "stmt ::= expr SEMI SEMIH EOL",
 /*  22 */ "stmt ::= expr COLON expr SEMI SEMIH EOL",
 /*  23 */ "stmt ::= expr AT expr SEMI SEMIH EOL",
 /*  24 */ "stmt ::= expr SEMI SEMII EOL",
 /*  25 */ "stmt ::= expr SEMI SEMIL EOL",
 /*  26 */ "stmt ::= expr AT expr SEMI SEMIL EOL",
 /*  27 */ "stmt ::= expr COLON expr SEMI SEMIL EOL",
 /*  28 */ "stmt ::= expr SEMI SEMIDATE EOL",
 /*  29 */ "stmt ::= expr SEMI SEMIERROR EOL",
 /*  30 */ "stmt ::= expr SEMI SEMIT IDENTIFIER EOL",
 /*  31 */ "stmt ::= expr SEMI SEMIT error EOL",
 /*  32 */ "stmt ::= DREGISTER EQ expr EOL",
 /*  33 */ "stmt ::= AREGISTER EQ expr EOL",
 /*  34 */ "stmt ::= XREGISTER EQ expr EOL",
 /*  35 */ "stmt ::= HELP EOL",
 /*  36 */ "stmt ::= IDENTIFIER EQ expr EOL",
 /*  37 */ "expr ::= unary",
 /*  38 */ "expr ::= expr PLUS expr",
 /*  39 */ "expr ::= expr MINUS expr",
 /*  40 */ "expr ::= expr STAR expr",
 /*  41 */ "expr ::= expr SLASH expr",
 /*  42 */ "expr ::= expr PERCENT expr",
 /*  43 */ "expr ::= expr LTLT expr",
 /*  44 */ "expr ::= expr GTGT expr",
 /*  45 */ "expr ::= expr LT expr",
 /*  46 */ "expr ::= expr LTEQ expr",
 /*  47 */ "expr ::= expr GT expr",
 /*  48 */ "expr ::= expr GTEQ expr",
 /*  49 */ "expr ::= expr EQEQ expr",
 /*  50 */ "expr ::= expr BANGEQ expr",
 /*  51 */ "expr ::= expr AMP expr",
 /*  52 */ "expr ::= expr CARET expr",
 /*  53 */ "expr ::= expr PIPE expr",
 /*  54 */ "expr ::= expr AMPAMP expr",
 /*  55 */ "expr ::= expr PIPEPIPE expr",
 /*  56 */ "expr ::= unary LPAREN register RPAREN",
 /*  57 */ "unary ::= term",
 /*  58 */ "unary ::= PLUS unary",
 /*  59 */ "unary ::= MINUS unary",
 /*  60 */ "unary ::= TILDE unary",
 /*  61 */ "unary ::= BANG unary",
 /*  62 */ "unary ::= STAR unary",
 /*  63 */ "term ::= LPAREN expr RPAREN",
 /*  64 */ "term ::= INTEGER",
 /*  65 */ "term ::= register",
 /*  66 */ "term ::= IDENTIFIER",
 /*  67 */ "register ::= DREGISTER",
 /*  68 */ "register ::= AREGISTER",
 /*  69 */ "register ::= XREGISTER",
};
#endif /* NDEBUG */

/*
** This function returns the symbolic name associated with a token
** value.
*/
const char *ParseTokenName(int tokenType){
#ifndef NDEBUG
  if( tokenType>0 && tokenType<(sizeof(yyTokenName)/sizeof(yyTokenName[0])) ){
    return yyTokenName[tokenType];
  }else{
    return "Unknown";
  }
#else
  return "";
#endif
}

/* 
** This function allocates a new parser.
** The only argument is a pointer to a function which works like
** malloc.
**
** Inputs:
** A pointer to the function used to allocate memory.
**
** Outputs:
** A pointer to a parser.  This pointer is used in subsequent calls
** to Parse and ParseFree.
*/
void *ParseAlloc(void *(*mallocProc)(size_t)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (size_t)sizeof(yyParser) );
  if( pParser ){
    pParser->yyidx = -1;
  }
  return pParser;
}

/* The following function deletes the value associated with a
** symbol.  The symbol can be either a terminal or nonterminal.
** "yymajor" is the symbol code, and "yypminor" is a pointer to
** the value.
*/
static void yy_destructor(YYCODETYPE yymajor, YYMINORTYPE *yypminor){
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are not used
    ** inside the C code.
    */
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
**
** Return the major token number for the symbol popped.
*/
static int yy_pop_parser_stack(yyParser *pParser){
  YYCODETYPE yymajor;
  yyStackEntry *yytos = &pParser->yystack[pParser->yyidx];

  if( pParser->yyidx<0 ) return 0;
#ifndef NDEBUG
  if( yyTraceFILE && pParser->yyidx>=0 ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yymajor = yytos->major;
  yy_destructor( yymajor, &yytos->minor);
  pParser->yyidx--;
  return yymajor;
}

/* 
** Deallocate and destroy a parser.  Destructors are all called for
** all stack elements before shutting the parser down.
**
** Inputs:
** <ul>
** <li>  A pointer to the parser.  This should be a pointer
**       obtained from ParseAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
  yyParser *pParser = (yyParser*)p;
  if( pParser==0 ) return;
  while( pParser->yyidx>=0 ) yy_pop_parser_stack(pParser);
  (*freeProc)((void*)pParser);
}

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yystack[pParser->yyidx].stateno;
 
  if( stateno>YY_SHIFT_MAX || (i = yy_shift_ofst[stateno])==YY_SHIFT_USE_DFLT ){
    return yy_default[stateno];
  }
  if( iLookAhead==YYNOCODE ){
    return YY_NO_ACTION;
  }
  i += iLookAhead;
  if( i<0 || i>=YY_SZ_ACTTAB || yy_lookahead[i]!=iLookAhead ){
    if( iLookAhead>0 ){
#ifdef YYFALLBACK
      int iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        return yy_find_shift_action(pParser, iFallback);
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( j>=0 && j<YY_SZ_ACTTAB && yy_lookahead[j]==YYWILDCARD ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
    }
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
**
** If the look-ahead token is YYNOCODE, then check to see if the action is
** independent of the look-ahead.  If it is, return the action, otherwise
** return YY_NO_ACTION.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  /* int stateno = pParser->yystack[pParser->yyidx].stateno; */
 
  if( stateno>YY_REDUCE_MAX ||
      (i = yy_reduce_ofst[stateno])==YY_REDUCE_USE_DFLT ){
    return yy_default[stateno];
  }
  if( iLookAhead==YYNOCODE ){
    return YY_NO_ACTION;
  }
  i += iLookAhead;
  if( i<0 || i>=YY_SZ_ACTTAB || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }else{
    return yy_action[i];
  }
}

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  YYMINORTYPE *yypMinor         /* Pointer ot the minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yyidx++;
  if( yypParser->yyidx>=YYSTACKDEPTH ){
     ParseARG_FETCH;
     yypParser->yyidx--;
#ifndef NDEBUG
     if( yyTraceFILE ){
       fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
     }
#endif
     while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
     /* Here code is inserted which will execute if the parser
     ** stack every overflows */
     ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
     return;
  }
  yytos = &yypParser->yystack[yypParser->yyidx];
  yytos->stateno = yyNewState;
  yytos->major = yyMajor;
  yytos->minor = *yypMinor;
#ifndef NDEBUG
  if( yyTraceFILE && yypParser->yyidx>0 ){
    int i;
    fprintf(yyTraceFILE,"%sShift %d\n",yyTracePrompt,yyNewState);
    fprintf(yyTraceFILE,"%sStack:",yyTracePrompt);
    for(i=1; i<=yypParser->yyidx; i++)
      fprintf(yyTraceFILE," %s",yyTokenName[yypParser->yystack[i].major]);
    fprintf(yyTraceFILE,"\n");
  }
#endif
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;         /* Symbol on the left-hand side of the rule */
  unsigned char nrhs;     /* Number of right-hand side symbols in the rule */
} yyRuleInfo[] = {
  { 53, 2 },
  { 53, 2 },
  { 53, 3 },
  { 53, 2 },
  { 53, 3 },
  { 53, 2 },
  { 53, 2 },
  { 53, 2 },
  { 53, 3 },
  { 53, 2 },
  { 53, 3 },
  { 53, 2 },
  { 53, 3 },
  { 53, 3 },
  { 53, 2 },
  { 53, 3 },
  { 53, 3 },
  { 53, 5 },
  { 53, 5 },
  { 53, 3 },
  { 53, 2 },
  { 53, 4 },
  { 53, 6 },
  { 53, 6 },
  { 53, 4 },
  { 53, 4 },
  { 53, 6 },
  { 53, 6 },
  { 53, 4 },
  { 53, 4 },
  { 53, 5 },
  { 53, 5 },
  { 53, 4 },
  { 53, 4 },
  { 53, 4 },
  { 53, 2 },
  { 53, 4 },
  { 54, 1 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 3 },
  { 54, 4 },
  { 55, 1 },
  { 55, 2 },
  { 55, 2 },
  { 55, 2 },
  { 55, 2 },
  { 55, 2 },
  { 57, 3 },
  { 57, 1 },
  { 57, 1 },
  { 57, 1 },
  { 56, 1 },
  { 56, 1 },
  { 56, 1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  int yyruleno                 /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  YYMINORTYPE yygotominor;        /* The LHS of the rule reduced */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  yymsp = &yypParser->yystack[yypParser->yyidx];
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno>=0 
        && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    fprintf(yyTraceFILE, "%sReduce [%s].\n", yyTracePrompt,
      yyRuleName[yyruleno]);
  }
#endif /* NDEBUG */

#ifndef NDEBUG
  /* Silence complaints from purify about yygotominor being uninitialized
  ** in some cases when it is copied into the stack after the following
  ** switch.  yygotominor is uninitialized when a rule reduces that does
  ** not set the value of its left-hand side nonterminal.  Leaving the
  ** value of the nonterminal uninitialized is utterly harmless as long
  ** as the value is never used.  So really the only thing this code
  ** accomplishes is to quieten purify.  
  */
  memset(&yygotominor, 0, sizeof(yygotominor));
#endif

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
      case 0:
      case 2:
#line 62 "parser.lemon"
{
	Debug::Print(yymsp[-1].minor.yy0.intValue);
}
#line 918 "parser.c"
        break;
      case 1:
#line 67 "parser.lemon"
{
	Debug::PrintRegisters();
}
#line 925 "parser.c"
        break;
      case 3:
#line 77 "parser.lemon"
{
	Debug::Break();
}
#line 932 "parser.c"
        break;
      case 4:
#line 82 "parser.lemon"
{
	Debug::Break(yymsp[-1].minor.yy0.intValue);
}
#line 939 "parser.c"
        break;
      case 5:
#line 87 "parser.lemon"
{
	Debug::PrintBackTrace();
}
#line 946 "parser.c"
        break;
      case 6:
#line 92 "parser.lemon"
{
	command->action = Debug::cmdContinue;
	command->argc = 0;
}
#line 954 "parser.c"
        break;
      case 7:
#line 98 "parser.lemon"
{
	Debug::ToolBreak();
}
#line 961 "parser.c"
        break;
      case 8:
#line 103 "parser.lemon"
{
	Debug::ToolBreak(yymsp[-1].minor.yy0.intValue);
}
#line 968 "parser.c"
        break;
      case 9:
#line 108 "parser.lemon"
{
	Debug::ReadBreak();
}
#line 975 "parser.c"
        break;
      case 10:
#line 113 "parser.lemon"
{
	Debug::ReadBreak(yymsp[-1].minor.yy0.intValue);
}
#line 982 "parser.c"
        break;
      case 11:
#line 118 "parser.lemon"
{
	Debug::WriteBreak();
}
#line 989 "parser.c"
        break;
      case 12:
#line 123 "parser.lemon"
{
	Debug::WriteBreak(yymsp[-1].minor.yy0.intValue);
}
#line 996 "parser.c"
        break;
      case 13:
#line 129 "parser.lemon"
{
	Debug::ReadWriteBreak(yymsp[-1].minor.yy0.intValue);
}
#line 1003 "parser.c"
        break;
      case 14:
#line 135 "parser.lemon"
{
	command->action = Debug::cmdStep;
	command->argc = 0;
}
#line 1011 "parser.c"
        break;
      case 15:
#line 141 "parser.lemon"
{
	command->action = Debug::cmdStep;
	command->argc = 1;
	command->argv[0] = yymsp[-1].minor.yy0.intValue;
}
#line 1020 "parser.c"
        break;
      case 16:
#line 148 "parser.lemon"
{
	Debug::Dump(yymsp[-1].minor.yy0.intValue);
}
#line 1027 "parser.c"
        break;
      case 17:
#line 153 "parser.lemon"
{
	Debug::Dump(yymsp[-3].minor.yy0.intValue, yymsp[-1].minor.yy0.intValue - yymsp[-3].minor.yy0.intValue);
}
#line 1034 "parser.c"
        break;
      case 18:
#line 158 "parser.lemon"
{
	Debug::Dump(yymsp[-3].minor.yy0.intValue, yymsp[-1].minor.yy0.intValue);
}
#line 1041 "parser.c"
        break;
      case 19:
#line 163 "parser.lemon"
{
	Debug::List(yymsp[-1].minor.yy0.intValue);
}
#line 1048 "parser.c"
        break;
      case 20:
#line 168 "parser.lemon"
{
	Debug::StackCrawl();
}
#line 1055 "parser.c"
        break;
      case 21:
#line 173 "parser.lemon"
{
	Debug::Dump(yymsp[-3].minor.yy0.intValue);
}
#line 1062 "parser.c"
        break;
      case 22:
#line 178 "parser.lemon"
{
	Debug::Dump(yymsp[-5].minor.yy0.intValue, yymsp[-3].minor.yy0.intValue - yymsp[-5].minor.yy0.intValue);
}
#line 1069 "parser.c"
        break;
      case 23:
#line 183 "parser.lemon"
{
	Debug::Dump(yymsp[-5].minor.yy0.intValue, yymsp[-3].minor.yy0.intValue);
}
#line 1076 "parser.c"
        break;
      case 24:
#line 189 "parser.lemon"
{
	Debug::Info(yymsp[-3].minor.yy0.intValue);
}
#line 1083 "parser.c"
        break;
      case 25:
#line 195 "parser.lemon"
{
	Debug::List(yymsp[-3].minor.yy0.intValue);
}
#line 1090 "parser.c"
        break;
      case 26:
#line 200 "parser.lemon"
{
	Debug::List(yymsp[-5].minor.yy0.intValue, (int)yymsp[-3].minor.yy0.intValue);
}
#line 1097 "parser.c"
        break;
      case 27:
#line 205 "parser.lemon"
{
	Debug::List(yymsp[-5].minor.yy0.intValue, yymsp[-3].minor.yy0.intValue);
}
#line 1104 "parser.c"
        break;
      case 28:
#line 210 "parser.lemon"
{
	Debug::PrintDate(yymsp[-3].minor.yy0.intValue);
}
#line 1111 "parser.c"
        break;
      case 29:
#line 215 "parser.lemon"
{
	Debug::PrintError(yymsp[-3].minor.yy0.intValue);
}
#line 1118 "parser.c"
        break;
      case 30:
#line 222 "parser.lemon"
{
	Debug::ApplyTemplate(yymsp[-4].minor.yy0.intValue, *yymsp[-1].minor.yy0.stringValue);	
}
#line 1125 "parser.c"
        break;
      case 31:
#line 228 "parser.lemon"
{
	fprintf(stderr, "usage: expression ; t TemplateName\n");
}
#line 1132 "parser.c"
        break;
      case 32:
#line 234 "parser.lemon"
{
	Debug::SetDRegister(yymsp[-3].minor.yy0.intValue, yymsp[-1].minor.yy0.intValue);
}
#line 1139 "parser.c"
        break;
      case 33:
#line 239 "parser.lemon"
{
	Debug::SetARegister(yymsp[-3].minor.yy0.intValue, yymsp[-1].minor.yy0.intValue);
}
#line 1146 "parser.c"
        break;
      case 34:
#line 244 "parser.lemon"
{
	Debug::SetXRegister(yymsp[-3].minor.yy0.intValue, yymsp[-1].minor.yy0.intValue);
}
#line 1153 "parser.c"
        break;
      case 35:
#line 249 "parser.lemon"
{
	Debug::Help();
}
#line 1160 "parser.c"
        break;
      case 36:
#line 254 "parser.lemon"
{
	Debug::VariableSet(*yymsp[-3].minor.yy0.stringValue, yymsp[-1].minor.yy0.intValue);
}
#line 1167 "parser.c"
        break;
      case 37:
      case 57:
      case 58:
      case 64:
      case 65:
#line 258 "parser.lemon"
{ yygotominor.yy0 = yymsp[0].minor.yy0; }
#line 1176 "parser.c"
        break;
      case 38:
#line 259 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue + yymsp[0].minor.yy0.intValue); }
#line 1181 "parser.c"
        break;
      case 39:
#line 260 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue - yymsp[0].minor.yy0.intValue); }
#line 1186 "parser.c"
        break;
      case 40:
#line 261 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue * yymsp[0].minor.yy0.intValue); }
#line 1191 "parser.c"
        break;
      case 41:
#line 262 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue / yymsp[0].minor.yy0.intValue); }
#line 1196 "parser.c"
        break;
      case 42:
#line 263 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue % yymsp[0].minor.yy0.intValue); }
#line 1201 "parser.c"
        break;
      case 43:
#line 264 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue << yymsp[0].minor.yy0.intValue); }
#line 1206 "parser.c"
        break;
      case 44:
#line 265 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue >> yymsp[0].minor.yy0.intValue); }
#line 1211 "parser.c"
        break;
      case 45:
#line 266 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue < yymsp[0].minor.yy0.intValue); }
#line 1216 "parser.c"
        break;
      case 46:
#line 267 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue <= yymsp[0].minor.yy0.intValue); }
#line 1221 "parser.c"
        break;
      case 47:
#line 268 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue > yymsp[0].minor.yy0.intValue); }
#line 1226 "parser.c"
        break;
      case 48:
#line 269 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue >= yymsp[0].minor.yy0.intValue); }
#line 1231 "parser.c"
        break;
      case 49:
#line 270 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue == yymsp[0].minor.yy0.intValue); }
#line 1236 "parser.c"
        break;
      case 50:
#line 271 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue != yymsp[0].minor.yy0.intValue); }
#line 1241 "parser.c"
        break;
      case 51:
#line 272 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue & yymsp[0].minor.yy0.intValue); }
#line 1246 "parser.c"
        break;
      case 52:
#line 273 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue ^ yymsp[0].minor.yy0.intValue); }
#line 1251 "parser.c"
        break;
      case 53:
#line 274 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue | yymsp[0].minor.yy0.intValue); }
#line 1256 "parser.c"
        break;
      case 54:
#line 275 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue && yymsp[0].minor.yy0.intValue); }
#line 1261 "parser.c"
        break;
      case 55:
#line 276 "parser.lemon"
{ yygotominor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue || yymsp[0].minor.yy0.intValue); }
#line 1266 "parser.c"
        break;
      case 56:
#line 282 "parser.lemon"
{
	uint32_t offset = yymsp[-3].minor.yy0.intValue;
	uint32_t value = yymsp[-1].minor.yy0.intValue;

	// offset is 16-bits.
	if (offset <= 0xffff)
	{
		if (offset & 0x8000)
			offset |= 0xffff0000;
	}

	yygotominor.yy0 = Token::Make(value + offset);
}
#line 1283 "parser.c"
        break;
      case 59:
#line 299 "parser.lemon"
{ yygotominor.yy0 = Token::Make(-yymsp[0].minor.yy0.intValue); }
#line 1288 "parser.c"
        break;
      case 60:
#line 300 "parser.lemon"
{ yygotominor.yy0 = Token::Make(~yymsp[0].minor.yy0.intValue); }
#line 1293 "parser.c"
        break;
      case 61:
#line 301 "parser.lemon"
{ yygotominor.yy0 = Token::Make(!yymsp[0].minor.yy0.intValue); }
#line 1298 "parser.c"
        break;
      case 62:
#line 302 "parser.lemon"
{ yygotominor.yy0 = Token::Make(Debug::ReadLong(yymsp[0].minor.yy0)); }
#line 1303 "parser.c"
        break;
      case 63:
#line 304 "parser.lemon"
{ yygotominor.yy0 = yymsp[-1].minor.yy0; }
#line 1308 "parser.c"
        break;
      case 66:
#line 310 "parser.lemon"
{
	// should throw/barf if undefined?
	yygotominor.yy0 = Token::Make(Debug::VariableGet(*yymsp[0].minor.yy0.stringValue));
}
#line 1316 "parser.c"
        break;
      case 67:
#line 316 "parser.lemon"
{ yygotominor.yy0 = Token::Make(cpuGetDReg(yymsp[0].minor.yy0)); }
#line 1321 "parser.c"
        break;
      case 68:
#line 317 "parser.lemon"
{ yygotominor.yy0 = Token::Make(cpuGetAReg(yymsp[0].minor.yy0)); }
#line 1326 "parser.c"
        break;
      case 69:
#line 319 "parser.lemon"
{
	switch(yymsp[0].minor.yy0)
	{
	case 0: 
		yygotominor.yy0 = Token::Make(cpuGetPC());
		break;
	case 1:
		yygotominor.yy0 = Token::Make(cpuGetSR());
		break;
	default:
		yygotominor.yy0 = Token::Make(0);
	}
}
#line 1343 "parser.c"
        break;
  };
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yypParser->yyidx -= yysize;
  yyact = yy_find_reduce_action(yymsp[-yysize].stateno,yygoto);
  if( yyact < YYNSTATE ){
#ifdef NDEBUG
    /* If we are not debugging and the reduce action popped at least
    ** one element off the stack, then we can push the new element back
    ** onto the stack here, and skip the stack overflow test in yy_shift().
    ** That gives a significant speed improvement. */
    if( yysize ){
      yypParser->yyidx++;
      yymsp -= yysize-1;
      yymsp->stateno = yyact;
      yymsp->major = yygoto;
      yymsp->minor = yygotominor;
    }else
#endif
    {
      yy_shift(yypParser,yyact,yygoto,&yygotominor);
    }
  }else if( yyact == YYNSTATE + YYNRULE + 1 ){
    yy_accept(yypParser);
  }
}

/*
** The following code executes when the parse fails
*/
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
#line 36 "parser.lemon"

	// fprintf(stderr,"I don't understand.\n");
	command->valid = false;
#line 1392 "parser.c"
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN (yyminor.yy0)
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
#line 41 "parser.lemon"

	command->valid = true;
#line 1428 "parser.c"
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "ParseAlloc" which describes the current state of the parser.
** The second argument is the major token number.  The third is
** the minor token.  The fourth optional argument is whatever the
** user wants (and specified in the grammar) and is available for
** use by the action routines.
**
** Inputs:
** <ul>
** <li> A pointer to the parser (an opaque structure.)
** <li> The major token number.
** <li> The minor token number.
** <li> An option argument of a grammar-specified type.
** </ul>
**
** Outputs:
** None.
*/
void Parse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  ParseTOKENTYPE yyminor       /* The value for the token */
  ParseARG_PDECL               /* Optional %extra_argument parameter */
){
  YYMINORTYPE yyminorunion;
  int yyact;            /* The parser action. */
  int yyendofinput;     /* True if we are at the end of input */
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
  yyParser *yypParser;  /* The parser */

  /* (re)initialize the parser, if necessary */
  yypParser = (yyParser*)yyp;
  if( yypParser->yyidx<0 ){
    /* if( yymajor==0 ) return; // not sure why this was here... */
    yypParser->yyidx = 0;
    yypParser->yyerrcnt = -1;
    yypParser->yystack[0].stateno = 0;
    yypParser->yystack[0].major = 0;
  }
  yyminorunion.yy0 = yyminor;
  yyendofinput = (yymajor==0);
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput %s\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,yymajor);
    if( yyact<YYNSTATE ){
      yy_shift(yypParser,yyact,yymajor,&yyminorunion);
      yypParser->yyerrcnt--;
      if( yyendofinput && yypParser->yyidx>=0 ){
        yymajor = 0;
      }else{
        yymajor = YYNOCODE;
      }
    }else if( yyact < YYNSTATE + YYNRULE ){
      yy_reduce(yypParser,yyact-YYNSTATE);
    }else if( yyact == YY_ERROR_ACTION ){
      int yymx;
#ifndef NDEBUG
      if( yyTraceFILE ){
        fprintf(yyTraceFILE,"%sSyntax Error!\n",yyTracePrompt);
      }
#endif
#ifdef YYERRORSYMBOL
      /* A syntax error has occurred.
      ** The response to an error depends upon whether or not the
      ** grammar defines an error token "ERROR".  
      **
      ** This is what we do if the grammar does define ERROR:
      **
      **  * Call the %syntax_error function.
      **
      **  * Begin popping the stack until we enter a state where
      **    it is legal to shift the error symbol, then shift
      **    the error symbol.
      **
      **  * Set the error count to three.
      **
      **  * Begin accepting and shifting new tokens.  No new error
      **    processing will occur until three tokens have been
      **    shifted successfully.
      **
      */
      if( yypParser->yyerrcnt<0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yymx = yypParser->yystack[yypParser->yyidx].major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yymajor,&yyminorunion);
        yymajor = YYNOCODE;
      }else{
         while(
          yypParser->yyidx >= 0 &&
          yymx != YYERRORSYMBOL &&
          (yyact = yy_find_reduce_action(
                        yypParser->yystack[yypParser->yyidx].stateno,
                        YYERRORSYMBOL)) >= YYNSTATE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yyidx < 0 || yymajor==0 ){
          yy_destructor(yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          YYMINORTYPE u2;
          u2.YYERRSYMDT = 0;
          yy_shift(yypParser,yyact,YYERRORSYMBOL,&u2);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#else  /* YYERRORSYMBOL is not defined */
      /* This is what we do if the grammar does not define ERROR:
      **
      **  * Report an error message, and throw away the input token.
      **
      **  * If the input token is $, then fail the parse.
      **
      ** As before, subsequent error messages are suppressed until
      ** three input tokens have been successfully shifted.
      */
      if( yypParser->yyerrcnt<=0 ){
        yy_syntax_error(yypParser,yymajor,yyminorunion);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
      }
      yymajor = YYNOCODE;
#endif
    }else{
      yy_accept(yypParser);
      yymajor = YYNOCODE;
    }
  }while( yymajor!=YYNOCODE && yypParser->yyidx>=0 );
  return;
}
