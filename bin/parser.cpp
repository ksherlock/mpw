/*
** 2000-05-29
**
** The author disclaims copyright to this source code.  In place of
** a legal notice, here is a blessing:
**
**    May you do good and not evil.
**    May you find forgiveness for yourself and forgive others.
**    May you share freely, never taking more than you give.
**
*************************************************************************
** Driver template for the LEMON parser generator.
**
** The "lemon" program processes an LALR(1) input grammar file, then uses
** this template to construct a parser.  The "lemon" program inserts text
** at each "%%" line.  Also, any "P-a-r-s-e" identifer prefix (without the
** interstitial "-" characters) contained in this template is changed into
** the value of the %name directive from the grammar.  Otherwise, the content
** of this template is copied straight through into the generate parser
** source file.
**
** The following is the concatenation of all %include directives from the
** input grammar file:
*/
#include <stdio.h>
#include <assert.h>
/************ Begin %include sections from the grammar ************************/
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

#line 58 "parser.c"
/**************** End of %include directives **********************************/
/* These constants specify the various numeric values for terminal symbols
** in a format understandable to "makeheaders".  This section is blank unless
** "lemon" is run with the "-m" command-line option.
***************** Begin makeheaders token definitions *************************/
/**************** End makeheaders token definitions ***************************/

/* The next sections is a series of control #defines.
** various aspects of the generated parser.
**    YYCODETYPE         is the data type used to store the integer codes
**                       that represent terminal and non-terminal symbols.
**                       "unsigned char" is used if there are fewer than
**                       256 symbols.  Larger types otherwise.
**    YYNOCODE           is a number of type YYCODETYPE that is not used for
**                       any terminal or nonterminal symbol.
**    YYFALLBACK         If defined, this indicates that one or more tokens
**                       (also known as: "terminal symbols") have fall-back
**                       values which should be used if the original symbol
**                       would not parse.  This permits keywords to sometimes
**                       be used as identifiers, for example.
**    YYACTIONTYPE       is the data type used for "action codes" - numbers
**                       that indicate what to do in response to the next
**                       token.
**    ParseTOKENTYPE     is the data type used for minor type for terminal
**                       symbols.  Background: A "minor type" is a semantic
**                       value associated with a terminal or non-terminal
**                       symbols.  For example, for an "ID" terminal symbol,
**                       the minor type might be the name of the identifier.
**                       Each non-terminal can have a different minor type.
**                       Terminal symbols all have the same minor type, though.
**                       This macros defines the minor type for terminal 
**                       symbols.
**    YYMINORTYPE        is the data type used for all minor types.
**                       This is typically a union of many types, one of
**                       which is ParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    ParseARG_SDECL     A static variable declaration for the %extra_argument
**    ParseARG_PDECL     A parameter declaration for the %extra_argument
**    ParseARG_STORE     Code to store %extra_argument into yypParser
**    ParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YY_MAX_SHIFT       Maximum value for shift actions
**    YY_MIN_SHIFTREDUCE Minimum value for shift-reduce actions
**    YY_MAX_SHIFTREDUCE Maximum value for shift-reduce actions
**    YY_MIN_REDUCE      Maximum value for reduce actions
**    YY_ERROR_ACTION    The yy_action[] code for syntax error
**    YY_ACCEPT_ACTION   The yy_action[] code for accept
**    YY_NO_ACTION       The yy_action[] code for no-op
*/
#ifndef INTERFACE
# define INTERFACE 1
#endif
/************* Begin control #defines *****************************************/
#define YYCODETYPE unsigned char
#define YYNOCODE 59
#define YYACTIONTYPE unsigned short int
#define ParseTOKENTYPE  Token 
typedef union {
  int yyinit;
  ParseTOKENTYPE yy0;
  int yy117;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define ParseARG_SDECL  Debug::Command *command ;
#define ParseARG_PDECL , Debug::Command *command 
#define ParseARG_FETCH  Debug::Command *command  = yypParser->command 
#define ParseARG_STORE yypParser->command  = command 
#define YYERRORSYMBOL 52
#define YYERRSYMDT yy117
#define YYNSTATE             140
#define YYNRULE              70
#define YY_MAX_SHIFT         139
#define YY_MIN_SHIFTREDUCE   157
#define YY_MAX_SHIFTREDUCE   226
#define YY_MIN_REDUCE        227
#define YY_MAX_REDUCE        296
#define YY_ERROR_ACTION      297
#define YY_ACCEPT_ACTION     298
#define YY_NO_ACTION         299
/************* End control #defines *******************************************/

/* Define the yytestcase() macro to be a no-op if is not already defined
** otherwise.
**
** Applications can choose to define yytestcase() in the %include section
** to a macro that can assist in verifying code coverage.  For production
** code the yytestcase() macro should be turned off.  But it is useful
** for testing.
*/
#ifndef yytestcase
# define yytestcase(X)
#endif


/* Next are the tables used to determine what action to take based on the
** current state and lookahead token.  These tables are used to implement
** functions that take a state number and lookahead value and return an
** action integer.  
**
** Suppose the action integer is N.  Then the action is determined as
** follows
**
**   0 <= N <= YY_MAX_SHIFT             Shift N.  That is, push the lookahead
**                                      token onto the stack and goto state N.
**
**   N between YY_MIN_SHIFTREDUCE       Shift to an arbitrary state then
**     and YY_MAX_SHIFTREDUCE           reduce by rule N-YY_MIN_SHIFTREDUCE.
**
**   N between YY_MIN_REDUCE            Reduce by rule N-YY_MIN_REDUCE
**     and YY_MAX_REDUCE
**
**   N == YY_ERROR_ACTION               A syntax error has occurred.
**
**   N == YY_ACCEPT_ACTION              The parser accepts its input.
**
**   N == YY_NO_ACTION                  No such action.  Denotes unused
**                                      slots in the yy_action[] table.
**
** The action table is constructed as a single large table named yy_action[].
** Given state S and lookahead X, the action is computed as either:
**
**    (A)   N = yy_action[ yy_shift_ofst[S] + X ]
**    (B)   N = yy_default[S]
**
** The (A) formula is preferred.  The B formula is used instead if:
**    (1)  The yy_shift_ofst[S]+X value is out of range, or
**    (2)  yy_lookahead[yy_shift_ofst[S]+X] is not equal to X, or
**    (3)  yy_shift_ofst[S] equal YY_SHIFT_USE_DFLT.
** (Implementation note: YY_SHIFT_USE_DFLT is chosen so that
** YY_SHIFT_USE_DFLT+X will be out of range for all possible lookaheads X.
** Hence only tests (1) and (2) need to be evaluated.)
**
** The formulas above are for computing the action when the lookahead is
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
**
*********** Begin parsing tables **********************************************/
#define YY_ACTTAB_COUNT (849)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    42,   41,   37,  119,  128,   39,   40,  126,   15,    5,
 /*    10 */   110,  108,    4,    3,    2,   14,    1,   13,  263,    6,
 /*    20 */    10,   93,   30,   29,   35,   34,   33,   32,   31,   83,
 /*    30 */    91,  262,   89,   87,   85,   17,  118,  221,  116,   18,
 /*    40 */    19,   20,   21,   22,   24,   23,   28,   27,   26,   25,
 /*    50 */    30,   29,   35,   34,   33,   32,   31,  261,   84,  139,
 /*    60 */   138,  136,  134,  132,  130,   44,   54,  120,  292,  284,
 /*    70 */    36,   16,    7,  124,   75,  122,   18,   19,   20,   21,
 /*    80 */    22,   24,   23,   28,   27,   26,   25,   30,   29,   35,
 /*    90 */    34,   33,   32,   31,  260,    8,   97,  298,   45,  120,
 /*   100 */   292,  284,   35,   34,   33,   32,   31,   12,   11,   18,
 /*   110 */    19,   20,   21,   22,   24,   23,   28,   27,   26,   25,
 /*   120 */    30,   29,   35,   34,   33,   32,   31,  259,    9,   82,
 /*   130 */    92,   18,   19,   20,   21,   22,   24,   23,   28,   27,
 /*   140 */    26,   25,   30,   29,   35,   34,   33,   32,   31,  247,
 /*   150 */   246,   86,  107,   18,   19,   20,   21,   22,   24,   23,
 /*   160 */    28,   27,   26,   25,   30,   29,   35,   34,   33,   32,
 /*   170 */    31,  245,  244,   88,  109,   18,   19,   20,   21,   22,
 /*   180 */    24,   23,   28,   27,   26,   25,   30,   29,   35,   34,
 /*   190 */    33,   32,   31,  243,  242,   90,  115,   18,   19,   20,
 /*   200 */    21,   22,   24,   23,   28,   27,   26,   25,   30,   29,
 /*   210 */    35,   34,   33,   32,   31,  241,  240,   94,  117,   18,
 /*   220 */    19,   20,   21,   22,   24,   23,   28,   27,   26,   25,
 /*   230 */    30,   29,   35,   34,   33,   32,   31,  239,  238,   95,
 /*   240 */   213,   18,   19,   20,   21,   22,   24,   23,   28,   27,
 /*   250 */    26,   25,   30,   29,   35,   34,   33,   32,   31,  237,
 /*   260 */   236,   96,   43,   18,   19,   20,   21,   22,   24,   23,
 /*   270 */    28,   27,   26,   25,   30,   29,   35,   34,   33,   32,
 /*   280 */    31,  235,  234,   98,  121,   18,   19,   20,   21,   22,
 /*   290 */    24,   23,   28,   27,   26,   25,   30,   29,   35,   34,
 /*   300 */    33,   32,   31,  233,  232,  100,  123,   18,   19,   20,
 /*   310 */    21,   22,   24,   23,   28,   27,   26,   25,   30,   29,
 /*   320 */    35,   34,   33,   32,   31,  231,  230,  101,  125,   18,
 /*   330 */    19,   20,   21,   22,   24,   23,   28,   27,   26,   25,
 /*   340 */    30,   29,   35,   34,   33,   32,   31,  229,  228,  103,
 /*   350 */   127,   18,   19,   20,   21,   22,   24,   23,   28,   27,
 /*   360 */    26,   25,   30,   29,   35,   34,   33,   32,   31,  253,
 /*   370 */   250,  105,  129,   18,   19,   20,   21,   22,   24,   23,
 /*   380 */    28,   27,   26,   25,   30,   29,   35,   34,   33,   32,
 /*   390 */    31,  254,  249,  111,  131,   18,   19,   20,   21,   22,
 /*   400 */    24,   23,   28,   27,   26,   25,   30,   29,   35,   34,
 /*   410 */    33,   32,   31,  258,  257,  113,  133,   18,   19,   20,
 /*   420 */    21,   22,   24,   23,   28,   27,   26,   25,   30,   29,
 /*   430 */    35,   34,   33,   32,   31,   19,   20,   21,   22,   24,
 /*   440 */    23,   28,   27,   26,   25,   30,   29,   35,   34,   33,
 /*   450 */    32,   31,   80,  135,   18,   19,   20,   21,   22,   24,
 /*   460 */    23,   28,   27,   26,   25,   30,   29,   35,   34,   33,
 /*   470 */    32,   31,   18,   19,   20,   21,   22,   24,   23,   28,
 /*   480 */    27,   26,   25,   30,   29,   35,   34,   33,   32,   31,
 /*   490 */    24,   23,   28,   27,   26,   25,   30,   29,   35,   34,
 /*   500 */    33,   32,   31,  220,  289,  292,  284,   81,   33,   32,
 /*   510 */    31,   20,   21,   22,   24,   23,   28,   27,   26,   25,
 /*   520 */    30,   29,   35,   34,   33,   32,   31,   21,   22,   24,
 /*   530 */    23,   28,   27,   26,   25,   30,   29,   35,   34,   33,
 /*   540 */    32,   31,   22,   24,   23,   28,   27,   26,   25,   30,
 /*   550 */    29,   35,   34,   33,   32,   31,   42,   41,   38,  256,
 /*   560 */   255,   39,   40,   99,  252,   42,   41,   38,  251,  248,
 /*   570 */    39,   40,  102,  288,  292,  284,   42,   41,   38,  137,
 /*   580 */   227,   39,   40,  104,  229,  223,  224,  229,  225,  226,
 /*   590 */   229,   17,  229,  221,  223,  224,  229,  225,  226,  229,
 /*   600 */    17,  224,  221,  225,  226,  223,  224,  229,  225,  226,
 /*   610 */   229,   17,  229,  221,  229,   42,   41,   38,  229,  229,
 /*   620 */    39,   40,  106,  229,   42,   41,   38,  229,  229,   39,
 /*   630 */    40,  112,  287,  292,  284,   42,   41,   38,  229,  229,
 /*   640 */    39,   40,  114,  229,  223,  224,  229,  225,  226,  229,
 /*   650 */    17,  229,  221,  223,  224,  229,  225,  226,  229,   17,
 /*   660 */   229,  221,  229,  229,  223,  224,  229,  225,  226,  229,
 /*   670 */    17,  229,  221,  229,   42,   41,   38,  229,  229,   39,
 /*   680 */    40,   28,   27,   26,   25,   30,   29,   35,   34,   33,
 /*   690 */    32,   31,   56,  120,  292,  284,   57,  120,  292,  284,
 /*   700 */   286,  292,  284,  223,  224,  229,  225,  226,  229,   17,
 /*   710 */   229,  221,   58,  120,  292,  284,   59,  120,  292,  284,
 /*   720 */    47,  120,  292,  284,  229,   48,  120,  292,  284,   49,
 /*   730 */   120,  292,  284,  229,   50,  120,  292,  284,   51,  120,
 /*   740 */   292,  284,   52,  120,  292,  284,   53,  120,  292,  284,
 /*   750 */    46,  120,  292,  284,   55,  120,  292,  284,   60,  120,
 /*   760 */   292,  284,   61,  120,  292,  284,   62,  120,  292,  284,
 /*   770 */    64,  120,  292,  284,   65,  120,  292,  284,   66,  120,
 /*   780 */   292,  284,   67,  120,  292,  284,   68,  120,  292,  284,
 /*   790 */    69,  120,  292,  284,   70,  120,  292,  284,   71,  120,
 /*   800 */   292,  284,   72,  120,  292,  284,   73,  120,  292,  284,
 /*   810 */    74,  120,  292,  284,   76,  120,  292,  284,   77,  120,
 /*   820 */   292,  284,  269,  120,  292,  284,  268,  120,  292,  284,
 /*   830 */   267,  120,  292,  284,   78,  120,  292,  284,   79,  120,
 /*   840 */   292,  284,   63,  120,  292,  284,  285,  292,  284,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */    14,   15,   16,   56,   43,   19,   20,   52,   22,   23,
 /*    10 */    24,   25,   26,   27,   28,   29,   30,   31,    0,   45,
 /*    20 */    34,   35,   12,   13,   14,   15,   16,   17,   18,   43,
 /*    30 */    44,    0,   46,   47,   48,   49,   37,   51,   39,    1,
 /*    40 */     2,    3,    4,    5,    6,    7,    8,    9,   10,   11,
 /*    50 */    12,   13,   14,   15,   16,   17,   18,    0,   21,   21,
 /*    60 */    37,   38,   39,   40,   41,   42,   54,   55,   56,   57,
 /*    70 */    32,   33,   45,   37,   36,   39,    1,    2,    3,    4,
 /*    80 */     5,    6,    7,    8,    9,   10,   11,   12,   13,   14,
 /*    90 */    15,   16,   17,   18,    0,   45,   21,   53,   54,   55,
 /*   100 */    56,   57,   14,   15,   16,   17,   18,   32,   33,    1,
 /*   110 */     2,    3,    4,    5,    6,    7,    8,    9,   10,   11,
 /*   120 */    12,   13,   14,   15,   16,   17,   18,    0,   45,   21,
 /*   130 */    21,    1,    2,    3,    4,    5,    6,    7,    8,    9,
 /*   140 */    10,   11,   12,   13,   14,   15,   16,   17,   18,    0,
 /*   150 */     0,   21,   21,    1,    2,    3,    4,    5,    6,    7,
 /*   160 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   170 */    18,    0,    0,   21,   21,    1,    2,    3,    4,    5,
 /*   180 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   190 */    16,   17,   18,    0,    0,   21,   21,    1,    2,    3,
 /*   200 */     4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
 /*   210 */    14,   15,   16,   17,   18,    0,    0,   21,   21,    1,
 /*   220 */     2,    3,    4,    5,    6,    7,    8,    9,   10,   11,
 /*   230 */    12,   13,   14,   15,   16,   17,   18,    0,    0,   21,
 /*   240 */    50,    1,    2,    3,    4,    5,    6,    7,    8,    9,
 /*   250 */    10,   11,   12,   13,   14,   15,   16,   17,   18,    0,
 /*   260 */     0,   21,   49,    1,    2,    3,    4,    5,    6,    7,
 /*   270 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   280 */    18,    0,    0,   21,   21,    1,    2,    3,    4,    5,
 /*   290 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   300 */    16,   17,   18,    0,    0,   21,   21,    1,    2,    3,
 /*   310 */     4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
 /*   320 */    14,   15,   16,   17,   18,    0,    0,   21,   21,    1,
 /*   330 */     2,    3,    4,    5,    6,    7,    8,    9,   10,   11,
 /*   340 */    12,   13,   14,   15,   16,   17,   18,    0,    0,   21,
 /*   350 */    21,    1,    2,    3,    4,    5,    6,    7,    8,    9,
 /*   360 */    10,   11,   12,   13,   14,   15,   16,   17,   18,    0,
 /*   370 */     0,   21,   21,    1,    2,    3,    4,    5,    6,    7,
 /*   380 */     8,    9,   10,   11,   12,   13,   14,   15,   16,   17,
 /*   390 */    18,    0,    0,   21,   21,    1,    2,    3,    4,    5,
 /*   400 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   410 */    16,   17,   18,    0,    0,   21,   21,    1,    2,    3,
 /*   420 */     4,    5,    6,    7,    8,    9,   10,   11,   12,   13,
 /*   430 */    14,   15,   16,   17,   18,    2,    3,    4,    5,    6,
 /*   440 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   450 */    17,   18,   36,   21,    1,    2,    3,    4,    5,    6,
 /*   460 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   470 */    17,   18,    1,    2,    3,    4,    5,    6,    7,    8,
 /*   480 */     9,   10,   11,   12,   13,   14,   15,   16,   17,   18,
 /*   490 */     6,    7,    8,    9,   10,   11,   12,   13,   14,   15,
 /*   500 */    16,   17,   18,   50,   55,   56,   57,   36,   16,   17,
 /*   510 */    18,    3,    4,    5,    6,    7,    8,    9,   10,   11,
 /*   520 */    12,   13,   14,   15,   16,   17,   18,    4,    5,    6,
 /*   530 */     7,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   540 */    17,   18,    5,    6,    7,    8,    9,   10,   11,   12,
 /*   550 */    13,   14,   15,   16,   17,   18,   14,   15,   16,    0,
 /*   560 */     0,   19,   20,   21,    0,   14,   15,   16,    0,    0,
 /*   570 */    19,   20,   21,   55,   56,   57,   14,   15,   16,   21,
 /*   580 */     0,   19,   20,   21,   58,   43,   44,   58,   46,   47,
 /*   590 */    58,   49,   58,   51,   43,   44,   58,   46,   47,   58,
 /*   600 */    49,   44,   51,   46,   47,   43,   44,   58,   46,   47,
 /*   610 */    58,   49,   58,   51,   58,   14,   15,   16,   58,   58,
 /*   620 */    19,   20,   21,   58,   14,   15,   16,   58,   58,   19,
 /*   630 */    20,   21,   55,   56,   57,   14,   15,   16,   58,   58,
 /*   640 */    19,   20,   21,   58,   43,   44,   58,   46,   47,   58,
 /*   650 */    49,   58,   51,   43,   44,   58,   46,   47,   58,   49,
 /*   660 */    58,   51,   58,   58,   43,   44,   58,   46,   47,   58,
 /*   670 */    49,   58,   51,   58,   14,   15,   16,   58,   58,   19,
 /*   680 */    20,    8,    9,   10,   11,   12,   13,   14,   15,   16,
 /*   690 */    17,   18,   54,   55,   56,   57,   54,   55,   56,   57,
 /*   700 */    55,   56,   57,   43,   44,   58,   46,   47,   58,   49,
 /*   710 */    58,   51,   54,   55,   56,   57,   54,   55,   56,   57,
 /*   720 */    54,   55,   56,   57,   58,   54,   55,   56,   57,   54,
 /*   730 */    55,   56,   57,   58,   54,   55,   56,   57,   54,   55,
 /*   740 */    56,   57,   54,   55,   56,   57,   54,   55,   56,   57,
 /*   750 */    54,   55,   56,   57,   54,   55,   56,   57,   54,   55,
 /*   760 */    56,   57,   54,   55,   56,   57,   54,   55,   56,   57,
 /*   770 */    54,   55,   56,   57,   54,   55,   56,   57,   54,   55,
 /*   780 */    56,   57,   54,   55,   56,   57,   54,   55,   56,   57,
 /*   790 */    54,   55,   56,   57,   54,   55,   56,   57,   54,   55,
 /*   800 */    56,   57,   54,   55,   56,   57,   54,   55,   56,   57,
 /*   810 */    54,   55,   56,   57,   54,   55,   56,   57,   54,   55,
 /*   820 */    56,   57,   54,   55,   56,   57,   54,   55,   56,   57,
 /*   830 */    54,   55,   56,   57,   54,   55,   56,   57,   54,   55,
 /*   840 */    56,   57,   54,   55,   56,   57,   55,   56,   57,
};
#define YY_SHIFT_USE_DFLT (849)
#define YY_SHIFT_COUNT    (139)
#define YY_SHIFT_MIN      (-39)
#define YY_SHIFT_MAX      (673)
static const short yy_shift_ofst[] = {
 /*     0 */   -14,  542,  551,  562,  601,  610,  660,  660,  660,  660,
 /*    10 */   660,  660,  660,  660,  660,  660,  660,  660,  660,  660,
 /*    20 */   660,  660,  660,  660,  660,  660,  660,  660,  660,  660,
 /*    30 */   660,  660,  660,  660,  660,  660,  660,  621,  660,  660,
 /*    40 */   660,  660,  660,  557,  -39,   38,   75,  108,  130,  152,
 /*    50 */   174,  196,  218,  240,  262,  284,  306,  328,  350,  372,
 /*    60 */   394,  416,  453,  471,  433,  508,  523,  537,  484,  673,
 /*    70 */   673,   10,   10,   10,   10,   23,   88,   88,  492,  492,
 /*    80 */    -1,   36,   18,  -26,   31,   37,   57,   27,   94,   50,
 /*    90 */   127,   83,  149,  109,  150,  171,  172,  193,  194,  215,
 /*   100 */   216,  237,  238,  259,  260,  281,  282,  303,  131,  304,
 /*   110 */   153,  325,  326,  347,  348,  369,  175,  370,  197,  190,
 /*   120 */   213,  391,  263,  392,  285,  413,  307,  414,  329,  559,
 /*   130 */   351,  560,  373,  564,  395,  568,  432,  569,  558,  580,
};
#define YY_REDUCE_USE_DFLT (-54)
#define YY_REDUCE_COUNT (44)
#define YY_REDUCE_MIN   (-53)
#define YY_REDUCE_MAX   (791)
static const short yy_reduce_ofst[] = {
 /*     0 */    44,   12,  638,  642,  658,  662,  666,  671,  675,  680,
 /*    10 */   684,  688,  692,  696,  700,  704,  708,  712,  716,  720,
 /*    20 */   724,  728,  732,  736,  740,  744,  748,  752,  756,  760,
 /*    30 */   764,  768,  772,  776,  780,  784,  788,  449,  449,  518,
 /*    40 */   577,  645,  791,  -53,  -45,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   297,  297,  297,  297,  297,  297,  297,  297,  297,  297,
 /*    10 */   297,  297,  297,  297,  297,  297,  297,  297,  297,  297,
 /*    20 */   297,  297,  297,  297,  297,  297,  297,  297,  297,  297,
 /*    30 */   297,  297,  297,  297,  297,  297,  297,  297,  297,  297,
 /*    40 */   297,  297,  297,  297,  297,  297,  297,  297,  297,  297,
 /*    50 */   297,  297,  297,  297,  297,  297,  297,  297,  297,  297,
 /*    60 */   297,  297,  297,  297,  282,  281,  280,  279,  278,  277,
 /*    70 */   276,  275,  274,  273,  272,  297,  271,  270,  266,  265,
 /*    80 */   297,  297,  297,  293,  297,  297,  297,  296,  297,  295,
 /*    90 */   297,  294,  297,  297,  297,  297,  297,  297,  297,  297,
 /*   100 */   297,  297,  297,  297,  297,  297,  297,  297,  297,  297,
 /*   110 */   297,  297,  297,  297,  297,  297,  297,  297,  297,  297,
 /*   120 */   264,  297,  297,  297,  297,  297,  297,  297,  297,  297,
 /*   130 */   297,  297,  297,  297,  297,  297,  297,  297,  297,  297,
};
/********** End of lemon-generated parsing tables *****************************/

/* The next table maps tokens (terminal symbols) into fallback tokens.  
** If a construct like the following:
** 
**      %fallback ID X Y Z.
**
** appears in the grammar, then ID becomes a fallback token for X, Y,
** and Z.  Whenever one of the tokens X, Y, or Z is input to the parser
** but it does not parse, the type of the token is changed to ID and
** the parse is retried before an error is thrown.
**
** This feature can be used, for example, to cause some keywords in a language
** to revert to identifiers if they keyword does not apply in the context where
** it appears.
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
**
** After the "shift" half of a SHIFTREDUCE action, the stateno field
** actually contains the reduce action for the second half of the
** SHIFTREDUCE.
*/
struct yyStackEntry {
  YYACTIONTYPE stateno;  /* The state-number, or reduce action in SHIFTREDUCE */
  YYCODETYPE major;      /* The major token value.  This is the code
                         ** number for the token at this stack level */
  YYMINORTYPE minor;     /* The user-supplied minor token value.  This
                         ** is the value of the token  */
};
typedef struct yyStackEntry yyStackEntry;

/* The state of the parser is completely contained in an instance of
** the following structure */
struct yyParser {
  yyStackEntry *yytos;          /* Pointer to top element of the stack */
#ifdef YYTRACKMAXSTACKDEPTH
  int yyhwm;                    /* High-water mark of the stack */
#endif
#ifndef YYNOERRORRECOVERY
  int yyerrcnt;                 /* Shifts left before out of the error */
#endif
  ParseARG_SDECL                /* A place to hold %extra_argument */
#if YYSTACKDEPTH<=0
  int yystksz;                  /* Current side of the stack */
  yyStackEntry *yystack;        /* The parser's stack */
  yyStackEntry yystk0;          /* First stack entry */
#else
  yyStackEntry yystack[YYSTACKDEPTH];  /* The parser's stack */
  yyStackEntry *yystackEnd;            /* Last entry in the stack */
#endif
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


#if YYSTACKDEPTH<=0
/*
** Try to increase the size of the parser stack.  Return the number
** of errors.  Return 0 on success.
*/
static int yyGrowStack(yyParser *p){
  int newSize;
  int idx;
  yyStackEntry *pNew;

  newSize = p->yystksz*2 + 100;
  idx = p->yytos ? (int)(p->yytos - p->yystack) : 0;
  if( p->yystack==&p->yystk0 ){
    pNew = malloc(newSize*sizeof(pNew[0]));
    if( pNew ) pNew[0] = p->yystk0;
  }else{
    pNew = realloc(p->yystack, newSize*sizeof(pNew[0]));
  }
  if( pNew ){
    p->yystack = pNew;
    p->yytos = &p->yystack[idx];
#ifndef NDEBUG
    if( yyTraceFILE ){
      fprintf(yyTraceFILE,"%sStack grows from %d to %d entries.\n",
              yyTracePrompt, p->yystksz, newSize);
    }
#endif
    p->yystksz = newSize;
  }
  return pNew==0; 
}
#endif

/* Datatype of the argument to the memory allocated passed as the
** second argument to ParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* Initialize a new parser that has already been allocated.
*/
void ParseInit(void *yypParser){
  yyParser *pParser = (yyParser*)yypParser;
#ifdef YYTRACKMAXSTACKDEPTH
  pParser->yyhwm = 0;
#endif
#if YYSTACKDEPTH<=0
  pParser->yytos = NULL;
  pParser->yystack = NULL;
  pParser->yystksz = 0;
  if( yyGrowStack(pParser) ){
    pParser->yystack = &pParser->yystk0;
    pParser->yystksz = 1;
  }
#endif
#ifndef YYNOERRORRECOVERY
  pParser->yyerrcnt = -1;
#endif
  pParser->yytos = pParser->yystack;
  pParser->yystack[0].stateno = 0;
  pParser->yystack[0].major = 0;
#if YYSTACKDEPTH>0
  pParser->yystackEnd = &pParser->yystack[YYSTACKDEPTH-1];
#endif
}

#ifndef Parse_ENGINEALWAYSONSTACK
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
void *ParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ) ParseInit(pParser);
  return pParser;
}
#endif /* Parse_ENGINEALWAYSONSTACK */


/* The following function deletes the "minor type" or semantic value
** associated with a symbol.  The symbol can be either a terminal
** or nonterminal. "yymajor" is the symbol code, and "yypminor" is
** a pointer to the value to be deleted.  The code used to do the 
** deletions is derived from the %destructor and/or %token_destructor
** directives of the input grammar.
*/
static void yy_destructor(
  yyParser *yypParser,    /* The parser */
  YYCODETYPE yymajor,     /* Type code for object to destroy */
  YYMINORTYPE *yypminor   /* The object to be destroyed */
){
  ParseARG_FETCH;
  switch( yymajor ){
    /* Here is inserted the actions which take place when a
    ** terminal or non-terminal is destroyed.  This can happen
    ** when the symbol is popped from the stack during a
    ** reduce or during error processing or when a parser is 
    ** being destroyed before it is finished parsing.
    **
    ** Note: during a reduce, the only symbols destroyed are those
    ** which appear on the RHS of the rule, but which are *not* used
    ** inside the C code.
    */
/********* Begin destructor definitions ***************************************/
/********* End destructor definitions *****************************************/
    default:  break;   /* If no destructor action specified: do nothing */
  }
}

/*
** Pop the parser's stack once.
**
** If there is a destructor routine associated with the token which
** is popped from the stack, then call it.
*/
static void yy_pop_parser_stack(yyParser *pParser){
  yyStackEntry *yytos;
  assert( pParser->yytos!=0 );
  assert( pParser->yytos > pParser->yystack );
  yytos = pParser->yytos--;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sPopping %s\n",
      yyTracePrompt,
      yyTokenName[yytos->major]);
  }
#endif
  yy_destructor(pParser, yytos->major, &yytos->minor);
}

/*
** Clear all secondary memory allocations from the parser
*/
void ParseFinalize(void *p){
  yyParser *pParser = (yyParser*)p;
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
}

#ifndef Parse_ENGINEALWAYSONSTACK
/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void ParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
#ifndef YYPARSEFREENEVERNULL
  if( p==0 ) return;
#endif
  ParseFinalize(p);
  (*freeProc)(p);
}
#endif /* Parse_ENGINEALWAYSONSTACK */

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int ParseStackPeak(void *p){
  yyParser *pParser = (yyParser*)p;
  return pParser->yyhwm;
}
#endif

/*
** Find the appropriate action for a parser given the terminal
** look-ahead token iLookAhead.
*/
static unsigned int yy_find_shift_action(
  yyParser *pParser,        /* The parser */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
  int stateno = pParser->yytos->stateno;
 
  if( stateno>=YY_MIN_REDUCE ) return stateno;
  assert( stateno <= YY_SHIFT_COUNT );
  do{
    i = yy_shift_ofst[stateno];
    assert( iLookAhead!=YYNOCODE );
    i += iLookAhead;
    if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
#ifdef YYFALLBACK
      YYCODETYPE iFallback;            /* Fallback token */
      if( iLookAhead<sizeof(yyFallback)/sizeof(yyFallback[0])
             && (iFallback = yyFallback[iLookAhead])!=0 ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE, "%sFALLBACK %s => %s\n",
             yyTracePrompt, yyTokenName[iLookAhead], yyTokenName[iFallback]);
        }
#endif
        assert( yyFallback[iFallback]==0 ); /* Fallback loop must terminate */
        iLookAhead = iFallback;
        continue;
      }
#endif
#ifdef YYWILDCARD
      {
        int j = i - iLookAhead + YYWILDCARD;
        if( 
#if YY_SHIFT_MIN+YYWILDCARD<0
          j>=0 &&
#endif
#if YY_SHIFT_MAX+YYWILDCARD>=YY_ACTTAB_COUNT
          j<YY_ACTTAB_COUNT &&
#endif
          yy_lookahead[j]==YYWILDCARD && iLookAhead>0
        ){
#ifndef NDEBUG
          if( yyTraceFILE ){
            fprintf(yyTraceFILE, "%sWILDCARD %s => %s\n",
               yyTracePrompt, yyTokenName[iLookAhead],
               yyTokenName[YYWILDCARD]);
          }
#endif /* NDEBUG */
          return yy_action[j];
        }
      }
#endif /* YYWILDCARD */
      return yy_default[stateno];
    }else{
      return yy_action[i];
    }
  }while(1);
}

/*
** Find the appropriate action for a parser given the non-terminal
** look-ahead token iLookAhead.
*/
static int yy_find_reduce_action(
  int stateno,              /* Current state number */
  YYCODETYPE iLookAhead     /* The look-ahead token */
){
  int i;
#ifdef YYERRORSYMBOL
  if( stateno>YY_REDUCE_COUNT ){
    return yy_default[stateno];
  }
#else
  assert( stateno<=YY_REDUCE_COUNT );
#endif
  i = yy_reduce_ofst[stateno];
  assert( i!=YY_REDUCE_USE_DFLT );
  assert( iLookAhead!=YYNOCODE );
  i += iLookAhead;
#ifdef YYERRORSYMBOL
  if( i<0 || i>=YY_ACTTAB_COUNT || yy_lookahead[i]!=iLookAhead ){
    return yy_default[stateno];
  }
#else
  assert( i>=0 && i<YY_ACTTAB_COUNT );
  assert( yy_lookahead[i]==iLookAhead );
#endif
  return yy_action[i];
}

/*
** The following routine is called if the stack overflows.
*/
static void yyStackOverflow(yyParser *yypParser){
   ParseARG_FETCH;
#ifndef NDEBUG
   if( yyTraceFILE ){
     fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
   }
#endif
   while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
   /* Here code is inserted which will execute if the parser
   ** stack every overflows */
/******** Begin %stack_overflow code ******************************************/
/******** End %stack_overflow code ********************************************/
   ParseARG_STORE; /* Suppress warning about unused %extra_argument var */
}

/*
** Print tracing information for a SHIFT action
*/
#ifndef NDEBUG
static void yyTraceShift(yyParser *yypParser, int yyNewState){
  if( yyTraceFILE ){
    if( yyNewState<YYNSTATE ){
      fprintf(yyTraceFILE,"%sShift '%s', go to state %d\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major],
         yyNewState);
    }else{
      fprintf(yyTraceFILE,"%sShift '%s'\n",
         yyTracePrompt,yyTokenName[yypParser->yytos->major]);
    }
  }
}
#else
# define yyTraceShift(X,Y)
#endif

/*
** Perform a shift action.
*/
static void yy_shift(
  yyParser *yypParser,          /* The parser to be shifted */
  int yyNewState,               /* The new state to shift in */
  int yyMajor,                  /* The major token to shift in */
  ParseTOKENTYPE yyMinor        /* The minor token to shift in */
){
  yyStackEntry *yytos;
  yypParser->yytos++;
#ifdef YYTRACKMAXSTACKDEPTH
  if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
    yypParser->yyhwm++;
    assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack) );
  }
#endif
#if YYSTACKDEPTH>0 
  if( yypParser->yytos>yypParser->yystackEnd ){
    yypParser->yytos--;
    yyStackOverflow(yypParser);
    return;
  }
#else
  if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz] ){
    if( yyGrowStack(yypParser) ){
      yypParser->yytos--;
      yyStackOverflow(yypParser);
      return;
    }
  }
#endif
  if( yyNewState > YY_MAX_SHIFT ){
    yyNewState += YY_MIN_REDUCE - YY_MIN_SHIFTREDUCE;
  }
  yytos = yypParser->yytos;
  yytos->stateno = (YYACTIONTYPE)yyNewState;
  yytos->major = (YYCODETYPE)yyMajor;
  yytos->minor.yy0 = yyMinor;
  yyTraceShift(yypParser, yyNewState);
}

/* The following table contains information about every rule that
** is used during the reduce.
*/
static const struct {
  YYCODETYPE lhs;       /* Symbol on the left-hand side of the rule */
  signed char nrhs;     /* Negative of the number of RHS symbols in the rule */
} yyRuleInfo[] = {
  { 53, -2 },
  { 53, -2 },
  { 53, -3 },
  { 53, -2 },
  { 53, -3 },
  { 53, -2 },
  { 53, -2 },
  { 53, -2 },
  { 53, -3 },
  { 53, -2 },
  { 53, -3 },
  { 53, -2 },
  { 53, -3 },
  { 53, -3 },
  { 53, -2 },
  { 53, -3 },
  { 53, -3 },
  { 53, -5 },
  { 53, -5 },
  { 53, -3 },
  { 53, -2 },
  { 53, -4 },
  { 53, -6 },
  { 53, -6 },
  { 53, -4 },
  { 53, -4 },
  { 53, -6 },
  { 53, -6 },
  { 53, -4 },
  { 53, -4 },
  { 53, -5 },
  { 53, -5 },
  { 53, -4 },
  { 53, -4 },
  { 53, -4 },
  { 53, -2 },
  { 53, -4 },
  { 54, -1 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -3 },
  { 54, -4 },
  { 55, -1 },
  { 55, -2 },
  { 55, -2 },
  { 55, -2 },
  { 55, -2 },
  { 55, -2 },
  { 57, -3 },
  { 57, -1 },
  { 57, -1 },
  { 57, -1 },
  { 56, -1 },
  { 56, -1 },
  { 56, -1 },
};

static void yy_accept(yyParser*);  /* Forward Declaration */

/*
** Perform a reduce action and the shift that must immediately
** follow the reduce.
*/
static void yy_reduce(
  yyParser *yypParser,         /* The parser */
  unsigned int yyruleno        /* Number of the rule by which to reduce */
){
  int yygoto;                     /* The next state */
  int yyact;                      /* The next action */
  yyStackEntry *yymsp;            /* The top of the parser's stack */
  int yysize;                     /* Amount to pop the stack */
  ParseARG_FETCH;
  yymsp = yypParser->yytos;
#ifndef NDEBUG
  if( yyTraceFILE && yyruleno<(int)(sizeof(yyRuleName)/sizeof(yyRuleName[0])) ){
    yysize = yyRuleInfo[yyruleno].nrhs;
    fprintf(yyTraceFILE, "%sReduce [%s], go to state %d.\n", yyTracePrompt,
      yyRuleName[yyruleno], yymsp[yysize].stateno);
  }
#endif /* NDEBUG */

  /* Check that the stack is large enough to grow by a single entry
  ** if the RHS of the rule is empty.  This ensures that there is room
  ** enough on the stack to push the LHS value */
  if( yyRuleInfo[yyruleno].nrhs==0 ){
#ifdef YYTRACKMAXSTACKDEPTH
    if( (int)(yypParser->yytos - yypParser->yystack)>yypParser->yyhwm ){
      yypParser->yyhwm++;
      assert( yypParser->yyhwm == (int)(yypParser->yytos - yypParser->yystack));
    }
#endif
#if YYSTACKDEPTH>0 
    if( yypParser->yytos>=yypParser->yystackEnd ){
      yyStackOverflow(yypParser);
      return;
    }
#else
    if( yypParser->yytos>=&yypParser->yystack[yypParser->yystksz-1] ){
      if( yyGrowStack(yypParser) ){
        yyStackOverflow(yypParser);
        return;
      }
      yymsp = yypParser->yytos;
    }
#endif
  }

  switch( yyruleno ){
  /* Beginning here are the reduction cases.  A typical example
  ** follows:
  **   case 0:
  **  #line <lineno> <grammarfile>
  **     { ... }           // User supplied code
  **  #line <lineno> <thisfile>
  **     break;
  */
/********** Begin reduce actions **********************************************/
        YYMINORTYPE yylhsminor;
      case 0: /* stmt ::= expr EOL */
      case 2: /* stmt ::= PRINT expr EOL */ yytestcase(yyruleno==2);
#line 61 "parser.lemon"
{
	Debug::Print(yymsp[-1].minor.yy0.intValue);
}
#line 1142 "parser.c"
        break;
      case 1: /* stmt ::= STAR EOL */
#line 66 "parser.lemon"
{
	Debug::PrintRegisters();
}
#line 1149 "parser.c"
        break;
      case 3: /* stmt ::= BREAK EOL */
#line 76 "parser.lemon"
{
	Debug::Break();
}
#line 1156 "parser.c"
        break;
      case 4: /* stmt ::= BREAK expr EOL */
#line 81 "parser.lemon"
{
	Debug::Break(yymsp[-1].minor.yy0.intValue);
}
#line 1163 "parser.c"
        break;
      case 5: /* stmt ::= BACKTRACE EOL */
#line 86 "parser.lemon"
{
	Debug::PrintBackTrace();
}
#line 1170 "parser.c"
        break;
      case 6: /* stmt ::= CONTINUE EOL */
#line 91 "parser.lemon"
{
	command->action = Debug::cmdContinue;
	command->argc = 0;
}
#line 1178 "parser.c"
        break;
      case 7: /* stmt ::= TBREAK EOL */
#line 97 "parser.lemon"
{
	Debug::ToolBreak();
}
#line 1185 "parser.c"
        break;
      case 8: /* stmt ::= TBREAK expr EOL */
#line 102 "parser.lemon"
{
	Debug::ToolBreak(yymsp[-1].minor.yy0.intValue);
}
#line 1192 "parser.c"
        break;
      case 9: /* stmt ::= RBREAK EOL */
#line 107 "parser.lemon"
{
	Debug::ReadBreak();
}
#line 1199 "parser.c"
        break;
      case 10: /* stmt ::= RBREAK expr EOL */
#line 112 "parser.lemon"
{
	Debug::ReadBreak(yymsp[-1].minor.yy0.intValue);
}
#line 1206 "parser.c"
        break;
      case 11: /* stmt ::= WBREAK EOL */
#line 117 "parser.lemon"
{
	Debug::WriteBreak();
}
#line 1213 "parser.c"
        break;
      case 12: /* stmt ::= WBREAK expr EOL */
#line 122 "parser.lemon"
{
	Debug::WriteBreak(yymsp[-1].minor.yy0.intValue);
}
#line 1220 "parser.c"
        break;
      case 13: /* stmt ::= RWBREAK expr EOL */
#line 128 "parser.lemon"
{
	Debug::ReadWriteBreak(yymsp[-1].minor.yy0.intValue);
}
#line 1227 "parser.c"
        break;
      case 14: /* stmt ::= NEXT EOL */
#line 134 "parser.lemon"
{
	command->action = Debug::cmdStep;
	command->argc = 0;
}
#line 1235 "parser.c"
        break;
      case 15: /* stmt ::= NEXT expr EOL */
#line 140 "parser.lemon"
{
	command->action = Debug::cmdStep;
	command->argc = 1;
	command->argv[0] = yymsp[-1].minor.yy0.intValue;
}
#line 1244 "parser.c"
        break;
      case 16: /* stmt ::= DUMP expr EOL */
#line 147 "parser.lemon"
{
	Debug::Dump(yymsp[-1].minor.yy0.intValue);
}
#line 1251 "parser.c"
        break;
      case 17: /* stmt ::= DUMP expr COLON expr EOL */
#line 152 "parser.lemon"
{
	Debug::Dump(yymsp[-3].minor.yy0.intValue, yymsp[-1].minor.yy0.intValue - yymsp[-3].minor.yy0.intValue);
}
#line 1258 "parser.c"
        break;
      case 18: /* stmt ::= DUMP expr AT expr EOL */
#line 157 "parser.lemon"
{
	Debug::Dump(yymsp[-3].minor.yy0.intValue, yymsp[-1].minor.yy0.intValue);
}
#line 1265 "parser.c"
        break;
      case 19: /* stmt ::= LIST expr EOL */
#line 162 "parser.lemon"
{
	Debug::List(yymsp[-1].minor.yy0.intValue);
}
#line 1272 "parser.c"
        break;
      case 20: /* stmt ::= STACKCRAWL EOL */
#line 167 "parser.lemon"
{
	Debug::StackCrawl();
}
#line 1279 "parser.c"
        break;
      case 21: /* stmt ::= expr SEMI SEMIH EOL */
#line 172 "parser.lemon"
{
	Debug::Dump(yymsp[-3].minor.yy0.intValue);
}
#line 1286 "parser.c"
        break;
      case 22: /* stmt ::= expr COLON expr SEMI SEMIH EOL */
#line 177 "parser.lemon"
{
	Debug::Dump(yymsp[-5].minor.yy0.intValue, yymsp[-3].minor.yy0.intValue - yymsp[-5].minor.yy0.intValue);
}
#line 1293 "parser.c"
        break;
      case 23: /* stmt ::= expr AT expr SEMI SEMIH EOL */
#line 182 "parser.lemon"
{
	Debug::Dump(yymsp[-5].minor.yy0.intValue, yymsp[-3].minor.yy0.intValue);
}
#line 1300 "parser.c"
        break;
      case 24: /* stmt ::= expr SEMI SEMII EOL */
#line 188 "parser.lemon"
{
	Debug::Info(yymsp[-3].minor.yy0.intValue);
}
#line 1307 "parser.c"
        break;
      case 25: /* stmt ::= expr SEMI SEMIL EOL */
#line 194 "parser.lemon"
{
	Debug::List(yymsp[-3].minor.yy0.intValue);
}
#line 1314 "parser.c"
        break;
      case 26: /* stmt ::= expr AT expr SEMI SEMIL EOL */
#line 199 "parser.lemon"
{
	Debug::List(yymsp[-5].minor.yy0.intValue, (int)yymsp[-3].minor.yy0.intValue);
}
#line 1321 "parser.c"
        break;
      case 27: /* stmt ::= expr COLON expr SEMI SEMIL EOL */
#line 204 "parser.lemon"
{
	Debug::List(yymsp[-5].minor.yy0.intValue, yymsp[-3].minor.yy0.intValue);
}
#line 1328 "parser.c"
        break;
      case 28: /* stmt ::= expr SEMI SEMIDATE EOL */
#line 209 "parser.lemon"
{
	Debug::PrintDate(yymsp[-3].minor.yy0.intValue);
}
#line 1335 "parser.c"
        break;
      case 29: /* stmt ::= expr SEMI SEMIERROR EOL */
#line 214 "parser.lemon"
{
	Debug::PrintError(yymsp[-3].minor.yy0.intValue);
}
#line 1342 "parser.c"
        break;
      case 30: /* stmt ::= expr SEMI SEMIT IDENTIFIER EOL */
#line 221 "parser.lemon"
{
	Debug::ApplyTemplate(yymsp[-4].minor.yy0.intValue, *yymsp[-1].minor.yy0.stringValue);	
}
#line 1349 "parser.c"
        break;
      case 31: /* stmt ::= expr SEMI SEMIT error EOL */
#line 227 "parser.lemon"
{
	fprintf(stderr, "usage: expression ; t TemplateName\n");
}
#line 1356 "parser.c"
        break;
      case 32: /* stmt ::= DREGISTER EQ expr EOL */
#line 233 "parser.lemon"
{
	Debug::SetDRegister(yymsp[-3].minor.yy0.intValue, yymsp[-1].minor.yy0.intValue);
}
#line 1363 "parser.c"
        break;
      case 33: /* stmt ::= AREGISTER EQ expr EOL */
#line 238 "parser.lemon"
{
	Debug::SetARegister(yymsp[-3].minor.yy0.intValue, yymsp[-1].minor.yy0.intValue);
}
#line 1370 "parser.c"
        break;
      case 34: /* stmt ::= XREGISTER EQ expr EOL */
#line 243 "parser.lemon"
{
	Debug::SetXRegister(yymsp[-3].minor.yy0.intValue, yymsp[-1].minor.yy0.intValue);
}
#line 1377 "parser.c"
        break;
      case 35: /* stmt ::= HELP EOL */
#line 248 "parser.lemon"
{
	Debug::Help();
}
#line 1384 "parser.c"
        break;
      case 36: /* stmt ::= IDENTIFIER EQ expr EOL */
#line 253 "parser.lemon"
{
	Debug::VariableSet(*yymsp[-3].minor.yy0.stringValue, yymsp[-1].minor.yy0.intValue);
}
#line 1391 "parser.c"
        break;
      case 37: /* expr ::= unary */
      case 57: /* unary ::= term */ yytestcase(yyruleno==57);
      case 64: /* term ::= INTEGER */ yytestcase(yyruleno==64);
      case 65: /* term ::= register */ yytestcase(yyruleno==65);
#line 257 "parser.lemon"
{ yylhsminor.yy0 = yymsp[0].minor.yy0; }
#line 1399 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 38: /* expr ::= expr PLUS expr */
#line 258 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue + yymsp[0].minor.yy0.intValue); }
#line 1405 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 39: /* expr ::= expr MINUS expr */
#line 259 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue - yymsp[0].minor.yy0.intValue); }
#line 1411 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 40: /* expr ::= expr STAR expr */
#line 260 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue * yymsp[0].minor.yy0.intValue); }
#line 1417 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 41: /* expr ::= expr SLASH expr */
#line 261 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue / yymsp[0].minor.yy0.intValue); }
#line 1423 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 42: /* expr ::= expr PERCENT expr */
#line 262 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue % yymsp[0].minor.yy0.intValue); }
#line 1429 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 43: /* expr ::= expr LTLT expr */
#line 263 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue << yymsp[0].minor.yy0.intValue); }
#line 1435 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 44: /* expr ::= expr GTGT expr */
#line 264 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue >> yymsp[0].minor.yy0.intValue); }
#line 1441 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 45: /* expr ::= expr LT expr */
#line 265 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue < yymsp[0].minor.yy0.intValue); }
#line 1447 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 46: /* expr ::= expr LTEQ expr */
#line 266 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue <= yymsp[0].minor.yy0.intValue); }
#line 1453 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 47: /* expr ::= expr GT expr */
#line 267 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue > yymsp[0].minor.yy0.intValue); }
#line 1459 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 48: /* expr ::= expr GTEQ expr */
#line 268 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue >= yymsp[0].minor.yy0.intValue); }
#line 1465 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 49: /* expr ::= expr EQEQ expr */
#line 269 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue == yymsp[0].minor.yy0.intValue); }
#line 1471 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 50: /* expr ::= expr BANGEQ expr */
#line 270 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue != yymsp[0].minor.yy0.intValue); }
#line 1477 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 51: /* expr ::= expr AMP expr */
#line 271 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue & yymsp[0].minor.yy0.intValue); }
#line 1483 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 52: /* expr ::= expr CARET expr */
#line 272 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue ^ yymsp[0].minor.yy0.intValue); }
#line 1489 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 53: /* expr ::= expr PIPE expr */
#line 273 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue | yymsp[0].minor.yy0.intValue); }
#line 1495 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 54: /* expr ::= expr AMPAMP expr */
#line 274 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue && yymsp[0].minor.yy0.intValue); }
#line 1501 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 55: /* expr ::= expr PIPEPIPE expr */
#line 275 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(yymsp[-2].minor.yy0.intValue || yymsp[0].minor.yy0.intValue); }
#line 1507 "parser.c"
  yymsp[-2].minor.yy0 = yylhsminor.yy0;
        break;
      case 56: /* expr ::= unary LPAREN register RPAREN */
#line 281 "parser.lemon"
{
	uint32_t offset = yymsp[-3].minor.yy0.intValue;
	uint32_t value = yymsp[-1].minor.yy0.intValue;

	// offset is 16-bits.
	if (offset <= 0xffff)
	{
		if (offset & 0x8000)
			offset |= 0xffff0000;
	}

	yylhsminor.yy0 = Token::Make(value + offset);
}
#line 1525 "parser.c"
  yymsp[-3].minor.yy0 = yylhsminor.yy0;
        break;
      case 58: /* unary ::= PLUS unary */
#line 297 "parser.lemon"
{ yymsp[-1].minor.yy0 = yymsp[0].minor.yy0; }
#line 1531 "parser.c"
        break;
      case 59: /* unary ::= MINUS unary */
#line 298 "parser.lemon"
{ yymsp[-1].minor.yy0 = Token::Make(-yymsp[0].minor.yy0.intValue); }
#line 1536 "parser.c"
        break;
      case 60: /* unary ::= TILDE unary */
#line 299 "parser.lemon"
{ yymsp[-1].minor.yy0 = Token::Make(~yymsp[0].minor.yy0.intValue); }
#line 1541 "parser.c"
        break;
      case 61: /* unary ::= BANG unary */
#line 300 "parser.lemon"
{ yymsp[-1].minor.yy0 = Token::Make(!yymsp[0].minor.yy0.intValue); }
#line 1546 "parser.c"
        break;
      case 62: /* unary ::= STAR unary */
#line 301 "parser.lemon"
{ yymsp[-1].minor.yy0 = Token::Make(Debug::ReadLong(yymsp[0].minor.yy0)); }
#line 1551 "parser.c"
        break;
      case 63: /* term ::= LPAREN expr RPAREN */
#line 303 "parser.lemon"
{ yymsp[-2].minor.yy0 = yymsp[-1].minor.yy0; }
#line 1556 "parser.c"
        break;
      case 66: /* term ::= IDENTIFIER */
#line 309 "parser.lemon"
{
	// should throw/barf if undefined?
	yylhsminor.yy0 = Token::Make(Debug::VariableGet(*yymsp[0].minor.yy0.stringValue));
}
#line 1564 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 67: /* register ::= DREGISTER */
#line 315 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(cpuGetDReg(yymsp[0].minor.yy0)); }
#line 1570 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 68: /* register ::= AREGISTER */
#line 316 "parser.lemon"
{ yylhsminor.yy0 = Token::Make(cpuGetAReg(yymsp[0].minor.yy0)); }
#line 1576 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      case 69: /* register ::= XREGISTER */
#line 318 "parser.lemon"
{
	switch(yymsp[0].minor.yy0)
	{
	case 0: 
		yylhsminor.yy0 = Token::Make(cpuGetPC());
		break;
	case 1:
		yylhsminor.yy0 = Token::Make(cpuGetSR());
		break;
	default:
		yylhsminor.yy0 = Token::Make(0);
	}
}
#line 1594 "parser.c"
  yymsp[0].minor.yy0 = yylhsminor.yy0;
        break;
      default:
        break;
/********** End reduce actions ************************************************/
  };
  assert( yyruleno<sizeof(yyRuleInfo)/sizeof(yyRuleInfo[0]) );
  yygoto = yyRuleInfo[yyruleno].lhs;
  yysize = yyRuleInfo[yyruleno].nrhs;
  yyact = yy_find_reduce_action(yymsp[yysize].stateno,(YYCODETYPE)yygoto);

  /* There are no SHIFTREDUCE actions on nonterminals because the table
  ** generator has simplified them to pure REDUCE actions. */
  assert( !(yyact>YY_MAX_SHIFT && yyact<=YY_MAX_SHIFTREDUCE) );

  /* It is not possible for a REDUCE to be followed by an error */
  assert( yyact!=YY_ERROR_ACTION );

  if( yyact==YY_ACCEPT_ACTION ){
    yypParser->yytos += yysize;
    yy_accept(yypParser);
  }else{
    yymsp += yysize+1;
    yypParser->yytos = yymsp;
    yymsp->stateno = (YYACTIONTYPE)yyact;
    yymsp->major = (YYCODETYPE)yygoto;
    yyTraceShift(yypParser, yyact);
  }
}

/*
** The following code executes when the parse fails
*/
#ifndef YYNOERRORRECOVERY
static void yy_parse_failed(
  yyParser *yypParser           /* The parser */
){
  ParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
#line 36 "parser.lemon"

	//fprintf(stderr,"I don't understand.\n");
	command->valid = false;
#line 1646 "parser.c"
/************ End %parse_failure code *****************************************/
  ParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  ParseTOKENTYPE yyminor         /* The minor type of the error token */
){
  ParseARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
/************ End %syntax_error code ******************************************/
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
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
#line 41 "parser.lemon"

	command->valid = true;
#line 1689 "parser.c"
/*********** End %parse_accept code *******************************************/
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
  unsigned int yyact;   /* The parser action. */
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  int yyendofinput;     /* True if we are at the end of input */
#endif
#ifdef YYERRORSYMBOL
  int yyerrorhit = 0;   /* True if yymajor has invoked an error */
#endif
  yyParser *yypParser;  /* The parser */

  yypParser = (yyParser*)yyp;
  assert( yypParser->yytos!=0 );
#if !defined(YYERRORSYMBOL) && !defined(YYNOERRORRECOVERY)
  yyendofinput = (yymajor==0);
#endif
  ParseARG_STORE;

#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sInput '%s'\n",yyTracePrompt,yyTokenName[yymajor]);
  }
#endif

  do{
    yyact = yy_find_shift_action(yypParser,(YYCODETYPE)yymajor);
    if( yyact <= YY_MAX_SHIFTREDUCE ){
      yy_shift(yypParser,yyact,yymajor,yyminor);
#ifndef YYNOERRORRECOVERY
      yypParser->yyerrcnt--;
#endif
      yymajor = YYNOCODE;
    }else if( yyact <= YY_MAX_REDUCE ){
      yy_reduce(yypParser,yyact-YY_MIN_REDUCE);
    }else{
      assert( yyact == YY_ERROR_ACTION );
      yyminorunion.yy0 = yyminor;
#ifdef YYERRORSYMBOL
      int yymx;
#endif
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
        yy_syntax_error(yypParser,yymajor,yyminor);
      }
      yymx = yypParser->yytos->major;
      if( yymx==YYERRORSYMBOL || yyerrorhit ){
#ifndef NDEBUG
        if( yyTraceFILE ){
          fprintf(yyTraceFILE,"%sDiscard input token %s\n",
             yyTracePrompt,yyTokenName[yymajor]);
        }
#endif
        yy_destructor(yypParser, (YYCODETYPE)yymajor, &yyminorunion);
        yymajor = YYNOCODE;
      }else{
        while( yypParser->yytos >= yypParser->yystack
            && yymx != YYERRORSYMBOL
            && (yyact = yy_find_reduce_action(
                        yypParser->yytos->stateno,
                        YYERRORSYMBOL)) >= YY_MIN_REDUCE
        ){
          yy_pop_parser_stack(yypParser);
        }
        if( yypParser->yytos < yypParser->yystack || yymajor==0 ){
          yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
          yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
          yypParser->yyerrcnt = -1;
#endif
          yymajor = YYNOCODE;
        }else if( yymx!=YYERRORSYMBOL ){
          yy_shift(yypParser,yyact,YYERRORSYMBOL,yyminor);
        }
      }
      yypParser->yyerrcnt = 3;
      yyerrorhit = 1;
#elif defined(YYNOERRORRECOVERY)
      /* If the YYNOERRORRECOVERY macro is defined, then do not attempt to
      ** do any kind of error recovery.  Instead, simply invoke the syntax
      ** error routine and continue going as if nothing had happened.
      **
      ** Applications can set this macro (for example inside %include) if
      ** they intend to abandon the parse upon the first syntax error seen.
      */
      yy_syntax_error(yypParser,yymajor, yyminor);
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      yymajor = YYNOCODE;
      
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
        yy_syntax_error(yypParser,yymajor, yyminor);
      }
      yypParser->yyerrcnt = 3;
      yy_destructor(yypParser,(YYCODETYPE)yymajor,&yyminorunion);
      if( yyendofinput ){
        yy_parse_failed(yypParser);
#ifndef YYNOERRORRECOVERY
        yypParser->yyerrcnt = -1;
#endif
      }
      yymajor = YYNOCODE;
#endif
    }
  }while( yymajor!=YYNOCODE && yypParser->yytos>yypParser->yystack );
#ifndef NDEBUG
  if( yyTraceFILE ){
    yyStackEntry *i;
    char cDiv = '[';
    fprintf(yyTraceFILE,"%sReturn. Stack=",yyTracePrompt);
    for(i=&yypParser->yystack[1]; i<=yypParser->yytos; i++){
      fprintf(yyTraceFILE,"%c%s", cDiv, yyTokenName[i->major]);
      cDiv = ' ';
    }
    if (cDiv == '[') fprintf(yyTraceFILE,"[");
    fprintf(yyTraceFILE,"]\n");
  }
#endif
  return;
}
