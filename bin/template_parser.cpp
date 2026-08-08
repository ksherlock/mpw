/* Driver template for the LEMON parser generator.
** The author disclaims copyright to this source code.
*/
/* First off, code is include which follows the "include" declaration
** in the input file. */
#include <stdio.h>
#line 6 "template_parser.lemon"

	
	#include <string>
	#include <stdlib.h>
	#include <cassert>
	#include <cstddef>

	#include "template.h"

	using namespace Debug;
#line 20 "template_parser.c"
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
**    TemplateParseTOKENTYPE     is the data type used for minor tokens given 
**                       directly to the parser from the tokenizer.
**    YYMINORTYPE        is the data type used for all minor tokens.
**                       This is typically a union of many types, one of
**                       which is TemplateParseTOKENTYPE.  The entry in the union
**                       for base tokens is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.
**    TemplateParseARG_SDECL     A static variable declaration for the %extra_argument
**    TemplateParseARG_PDECL     A parameter declaration for the %extra_argument
**    TemplateParseARG_STORE     Code to store %extra_argument into yypParser
**    TemplateParseARG_FETCH     Code to extract %extra_argument from yypParser
**    YYNSTATE           the combined number of states.
**    YYNRULE            the number of rules in the grammar
**    YYERRORSYMBOL      is the code number of the error symbol.  If not
**                       defined, then do no error processing.
*/
#define YYCODETYPE unsigned char
#define YYNOCODE 36
#define YYACTIONTYPE unsigned char
#define TemplateParseTOKENTYPE void*
typedef union {
  TemplateParseTOKENTYPE yy0;
  int yy32;
  FieldEntry * yy51;
  int yy71;
} YYMINORTYPE;
#define YYSTACKDEPTH 100
#define TemplateParseARG_SDECL  Debug::TemplateParseInfo *info ;
#define TemplateParseARG_PDECL , Debug::TemplateParseInfo *info 
#define TemplateParseARG_FETCH  Debug::TemplateParseInfo *info  = yypParser->info 
#define TemplateParseARG_STORE yypParser->info  = info 
#define YYNSTATE 50
#define YYNRULE 34
#define YYERRORSYMBOL 21
#define YYERRSYMDT yy71
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
 /*     0 */    36,   27,   36,   35,   29,   11,   43,   44,    8,   45,
 /*    10 */    16,   13,   16,   13,   46,   48,   17,   48,   17,    2,
 /*    20 */    39,   31,   30,    7,   34,   21,    6,    3,   18,   21,
 /*    30 */    24,    3,   15,   42,    4,   85,    5,   41,   47,   20,
 /*    40 */    37,   12,   41,   22,   14,   27,   32,   25,   38,   28,
 /*    50 */    66,   49,   19,    1,    9,   23,   10,   33,   71,   71,
 /*    60 */    71,   40,   71,   26,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     2,   11,    2,   26,   27,    7,   14,   15,   16,   34,
 /*    10 */    12,   13,   12,   13,   34,   17,   18,   17,   18,   22,
 /*    20 */    23,   14,   15,   16,   23,   28,    1,   30,    3,   28,
 /*    30 */    31,   30,   28,   16,   30,   24,   25,   20,   16,    5,
 /*    40 */    32,   33,   20,    8,   29,   11,   19,   29,    6,    2,
 /*    50 */     2,   19,    2,    4,    2,    9,    2,    6,   35,   35,
 /*    60 */    35,    6,   35,   10,
};
#define YY_SHIFT_USE_DFLT (-11)
#define YY_SHIFT_MAX 25
static const signed char yy_shift_ofst[] = {
 /*     0 */   -11,  -10,   34,   -2,    0,   25,  -10,   17,   22,   35,
 /*    10 */    35,   27,   -8,    7,   42,   47,   48,   32,   50,   49,
 /*    20 */    51,   52,   46,   53,   54,   55,
};
#define YY_REDUCE_USE_DFLT (-26)
#define YY_REDUCE_MAX 11
static const signed char yy_reduce_ofst[] = {
 /*     0 */    11,   -3,    1,    8,    8,  -23,    4,  -25,  -20,   15,
 /*    10 */    18,   -1,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */    53,   64,   64,   78,   78,   50,   64,   80,   80,   60,
 /*    10 */    60,   82,   84,   67,   84,   84,   79,   84,   84,   84,
 /*    20 */    84,   84,   84,   84,   84,   84,   61,   65,   54,   52,
 /*    30 */    71,   69,   83,   55,   56,   51,   63,   62,   58,   57,
 /*    40 */    59,   81,   75,   68,   70,   73,   72,   74,   76,   77,
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
  TemplateParseARG_SDECL                /* A place to hold %extra_argument */
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
void TemplateParseTrace(FILE *TraceFILE, char *zTracePrompt){
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
  "$",             "TYPEDEF",       "IDENTIFIER",    "STRUCT",      
  "LBRACE",        "RBRACE",        "SEMI",          "TEMPLATE",    
  "LBRACKET",      "INTEGER",       "RBRACKET",      "VOLATILE",    
  "SIGNED",        "UNSIGNED",      "CHAR",          "SHORT",       
  "LONG",          "TYPECODE",      "VOID",          "STAR",        
  "INT",           "error",         "struct_fields",  "struct_field",
  "start",         "templates",     "struct",        "typedef",     
  "type",          "array_count",   "opt_volatile",  "opt_star",    
  "typecode",      "opt_signed",    "opt_int",     
};
#endif /* NDEBUG */

#ifndef NDEBUG
/* For tracing reduce actions, the names of all rules are required.
*/
static const char *const yyRuleName[] = {
 /*   0 */ "start ::= templates",
 /*   1 */ "templates ::= templates struct",
 /*   2 */ "templates ::= templates typedef",
 /*   3 */ "templates ::=",
 /*   4 */ "typedef ::= TYPEDEF type IDENTIFIER",
 /*   5 */ "struct ::= STRUCT IDENTIFIER LBRACE struct_fields RBRACE SEMI",
 /*   6 */ "struct_fields ::= struct_fields struct_field",
 /*   7 */ "struct_fields ::= struct_field",
 /*   8 */ "struct_field ::= type IDENTIFIER array_count SEMI",
 /*   9 */ "struct_field ::= opt_volatile TEMPLATE opt_star IDENTIFIER array_count SEMI",
 /*  10 */ "array_count ::=",
 /*  11 */ "array_count ::= LBRACKET INTEGER RBRACKET",
 /*  12 */ "type ::= opt_volatile typecode",
 /*  13 */ "type ::= opt_volatile IDENTIFIER",
 /*  14 */ "opt_volatile ::=",
 /*  15 */ "opt_volatile ::= VOLATILE",
 /*  16 */ "typecode ::= SIGNED",
 /*  17 */ "typecode ::= UNSIGNED",
 /*  18 */ "typecode ::= opt_signed CHAR",
 /*  19 */ "typecode ::= UNSIGNED CHAR",
 /*  20 */ "typecode ::= opt_signed SHORT",
 /*  21 */ "typecode ::= UNSIGNED SHORT",
 /*  22 */ "typecode ::= opt_signed LONG opt_int",
 /*  23 */ "typecode ::= UNSIGNED LONG opt_int",
 /*  24 */ "typecode ::= opt_signed LONG LONG",
 /*  25 */ "typecode ::= UNSIGNED LONG LONG",
 /*  26 */ "typecode ::= TYPECODE",
 /*  27 */ "typecode ::= VOID STAR",
 /*  28 */ "opt_signed ::=",
 /*  29 */ "opt_signed ::= SIGNED",
 /*  30 */ "opt_int ::=",
 /*  31 */ "opt_int ::= INT",
 /*  32 */ "opt_star ::=",
 /*  33 */ "opt_star ::= STAR",
};
#endif /* NDEBUG */

/*
** This function returns the symbolic name associated with a token
** value.
*/
const char *TemplateParseTokenName(int tokenType){
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
** to TemplateParse and TemplateParseFree.
*/
void *TemplateParseAlloc(void *(*mallocProc)(size_t)){
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
**       obtained from TemplateParseAlloc.
** <li>  A pointer to a function used to reclaim memory obtained
**       from malloc.
** </ul>
*/
void TemplateParseFree(
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
     TemplateParseARG_FETCH;
     yypParser->yyidx--;
#ifndef NDEBUG
     if( yyTraceFILE ){
       fprintf(yyTraceFILE,"%sStack Overflow!\n",yyTracePrompt);
     }
#endif
     while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
     /* Here code is inserted which will execute if the parser
     ** stack every overflows */
     TemplateParseARG_STORE; /* Suppress warning about unused %extra_argument var */
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
  { 24, 1 },
  { 25, 2 },
  { 25, 2 },
  { 25, 0 },
  { 27, 3 },
  { 26, 6 },
  { 22, 2 },
  { 22, 1 },
  { 23, 4 },
  { 23, 6 },
  { 29, 0 },
  { 29, 3 },
  { 28, 2 },
  { 28, 2 },
  { 30, 0 },
  { 30, 1 },
  { 32, 1 },
  { 32, 1 },
  { 32, 2 },
  { 32, 2 },
  { 32, 2 },
  { 32, 2 },
  { 32, 3 },
  { 32, 3 },
  { 32, 3 },
  { 32, 3 },
  { 32, 1 },
  { 32, 2 },
  { 33, 0 },
  { 33, 1 },
  { 34, 0 },
  { 34, 1 },
  { 31, 0 },
  { 31, 1 },
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
  TemplateParseARG_FETCH;
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
      case 4:
#line 29 "template_parser.lemon"
{
	CreateTypedef((std::string *)yymsp[0].minor.yy0, yymsp[-1].minor.yy32, info);
}
#line 643 "template_parser.c"
        break;
      case 5:
#line 34 "template_parser.lemon"
{
	CreateTemplate((std::string *)yymsp[-4].minor.yy0, yymsp[-2].minor.yy51, info);
}
#line 650 "template_parser.c"
        break;
      case 6:
#line 38 "template_parser.lemon"
{
	// reverse order?
	yymsp[0].minor.yy51->next = yymsp[-1].minor.yy51;
	yygotominor.yy51 = yymsp[0].minor.yy51;
}
#line 659 "template_parser.c"
        break;
      case 7:
#line 44 "template_parser.lemon"
{
	yygotominor.yy51 = yymsp[0].minor.yy51;
}
#line 666 "template_parser.c"
        break;
      case 8:
#line 49 "template_parser.lemon"
{
	FieldEntry *e = (FieldEntry *)calloc(sizeof(FieldEntry), 1);

	e->name = (std::string *)yymsp[-2].minor.yy0;
	e->type = yymsp[-3].minor.yy32;
	e->count = yymsp[-1].minor.yy32;
	yygotominor.yy51 = e;
}
#line 678 "template_parser.c"
        break;
      case 9:
#line 58 "template_parser.lemon"
{
	FieldEntry *e = (FieldEntry *)calloc(sizeof(FieldEntry), 1);

	e->name = (std::string *)yymsp[-2].minor.yy0;
	e->type = yymsp[-3].minor.yy32 ? kStructPtr : kStruct;
	e->tmpl = (Template)yymsp[-4].minor.yy0;
	e->count = yymsp[-1].minor.yy32;

	yygotominor.yy51 = e;
}
#line 692 "template_parser.c"
        break;
      case 10:
      case 32:
#line 70 "template_parser.lemon"
{ yygotominor.yy32 = 0; }
#line 698 "template_parser.c"
        break;
      case 11:
#line 71 "template_parser.lemon"
{ 
	int i = (int)(ptrdiff_t)yymsp[-1].minor.yy0;
	if (i == 0) {
		fprintf(stderr, "Template error: line %u: 0-sized arrays are not allowed.\n", 
			info->LineNumber);
		i = 1;
	}
	yygotominor.yy32 = i; 
}
#line 711 "template_parser.c"
        break;
      case 12:
#line 82 "template_parser.lemon"
{ yygotominor.yy32 = yymsp[0].minor.yy32; }
#line 716 "template_parser.c"
        break;
      case 13:
#line 85 "template_parser.lemon"
{

	// ugh, Lemon will blindly replace text within a string.
	fprintf(stderr, "Template error: line %u: %s is not a known type.\n", 
		info->LineNumber, ((std::string *)yymsp[0].minor.yy0)->c_str());

	yygotominor.yy32 = 'i';
}
#line 728 "template_parser.c"
        break;
      case 16:
      case 22:
#line 100 "template_parser.lemon"
{ yygotominor.yy32 = kSInt32; }
#line 734 "template_parser.c"
        break;
      case 17:
#line 101 "template_parser.lemon"
{yygotominor.yy32 = kUInt32; }
#line 739 "template_parser.c"
        break;
      case 18:
#line 103 "template_parser.lemon"
{ yygotominor.yy32 = kSInt8; }
#line 744 "template_parser.c"
        break;
      case 19:
#line 104 "template_parser.lemon"
{ yygotominor.yy32 = kUInt8; }
#line 749 "template_parser.c"
        break;
      case 20:
#line 106 "template_parser.lemon"
{ yygotominor.yy32 = kSInt16; }
#line 754 "template_parser.c"
        break;
      case 21:
#line 107 "template_parser.lemon"
{ yygotominor.yy32 = kUInt16; }
#line 759 "template_parser.c"
        break;
      case 23:
#line 110 "template_parser.lemon"
{ yygotominor.yy32 = kUInt32; }
#line 764 "template_parser.c"
        break;
      case 24:
#line 112 "template_parser.lemon"
{ yygotominor.yy32 = kSInt64; }
#line 769 "template_parser.c"
        break;
      case 25:
#line 113 "template_parser.lemon"
{ yygotominor.yy32 = kUInt64; }
#line 774 "template_parser.c"
        break;
      case 26:
#line 115 "template_parser.lemon"
{ yygotominor.yy32 = (int)(ptrdiff_t)yymsp[0].minor.yy0; }
#line 779 "template_parser.c"
        break;
      case 27:
#line 118 "template_parser.lemon"
{ yygotominor.yy32 = kVoidPtr; }
#line 784 "template_parser.c"
        break;
      case 33:
#line 128 "template_parser.lemon"
{ yygotominor.yy32 = 1; }
#line 789 "template_parser.c"
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
  TemplateParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
  TemplateParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  YYMINORTYPE yyminor            /* The minor type of the error token */
){
  TemplateParseARG_FETCH;
#define TOKEN (yyminor.yy0)
  TemplateParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/*
** The following is executed when the parser accepts
*/
static void yy_accept(
  yyParser *yypParser           /* The parser */
){
  TemplateParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sAccept!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yyidx>=0 ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
  TemplateParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}

/* The main parser program.
** The first argument is a pointer to a structure obtained from
** "TemplateParseAlloc" which describes the current state of the parser.
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
void TemplateParse(
  void *yyp,                   /* The parser */
  int yymajor,                 /* The major token code number */
  TemplateParseTOKENTYPE yyminor       /* The value for the token */
  TemplateParseARG_PDECL               /* Optional %extra_argument parameter */
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
  TemplateParseARG_STORE;

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
