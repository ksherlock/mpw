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
** at each "%%" line.  Also, any "P-a-r-s-e" identifier prefix (without the
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
#line 6 "template_parser.lemon"

	
	#include <string>
	#include <stdlib.h>
	#include <cassert>
	#include <cstddef>

	#include "template.h"

	using namespace Debug;
#line 40 "template_parser.c"
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
**    TemplateParseTOKENTYPE     is the data type used for minor type for terminal
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
**                       which is TemplateParseTOKENTYPE.  The entry in the union
**                       for terminal symbols is called "yy0".
**    YYSTACKDEPTH       is the maximum depth of the parser's stack.  If
**                       zero the stack is dynamically sized using realloc()
**    TemplateParseARG_SDECL     A static variable declaration for the %extra_argument
**    TemplateParseARG_PDECL     A parameter declaration for the %extra_argument
**    TemplateParseARG_STORE     Code to store %extra_argument into yypParser
**    TemplateParseARG_FETCH     Code to extract %extra_argument from yypParser
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
#define YYNOCODE 36
#define YYACTIONTYPE unsigned char
#define TemplateParseTOKENTYPE void*
typedef union {
  int yyinit;
  TemplateParseTOKENTYPE yy0;
  int yy32;
  FieldEntry * yy51;
} YYMINORTYPE;
#ifndef YYSTACKDEPTH
#define YYSTACKDEPTH 100
#endif
#define TemplateParseARG_SDECL  Debug::TemplateParseInfo *info ;
#define TemplateParseARG_PDECL , Debug::TemplateParseInfo *info 
#define TemplateParseARG_FETCH  Debug::TemplateParseInfo *info  = yypParser->info 
#define TemplateParseARG_STORE yypParser->info  = info 
#define YYNSTATE             26
#define YYNRULE              34
#define YY_MAX_SHIFT         25
#define YY_MIN_SHIFTREDUCE   50
#define YY_MAX_SHIFTREDUCE   83
#define YY_MIN_REDUCE        84
#define YY_MAX_REDUCE        117
#define YY_ERROR_ACTION      118
#define YY_ACCEPT_ACTION     119
#define YY_NO_ACTION         120
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
#define YY_ACTTAB_COUNT (61)
static const YYACTIONTYPE yy_action[] = {
 /*     0 */    59,   86,   59,  119,    5,   10,   19,   79,    3,  100,
 /*    10 */    24,   13,   24,   13,  101,   70,   23,   70,   23,    2,
 /*    20 */    87,  108,    6,   17,   22,   19,   73,    3,   62,   64,
 /*    30 */     7,   63,   65,    8,   20,   92,   12,  109,  110,   14,
 /*    40 */    79,   25,   68,    4,   69,   55,   83,   15,   83,   18,
 /*    50 */     9,   57,   16,   54,   11,   51,   21,    1,   94,   71,
 /*    60 */    50,
};
static const YYCODETYPE yy_lookahead[] = {
 /*     0 */     2,   23,    2,   24,   25,    7,   28,   11,   30,   34,
 /*    10 */    12,   13,   12,   13,   34,   17,   18,   17,   18,   22,
 /*    20 */    23,    0,    1,    8,    3,   28,   19,   30,   14,   15,
 /*    30 */    16,   14,   15,   16,    5,   32,   33,   26,   27,   29,
 /*    40 */    11,   28,   16,   30,   16,    6,   20,   31,   20,   29,
 /*    50 */     2,   10,    9,    6,    2,    6,    2,    4,    2,   19,
 /*    60 */     2,
};
#define YY_SHIFT_USE_DFLT (61)
#define YY_SHIFT_COUNT    (25)
#define YY_SHIFT_MIN      (-4)
#define YY_SHIFT_MAX      (58)
static const signed char yy_shift_ofst[] = {
 /*     0 */    61,   -4,   29,   -2,    0,   21,   -4,   26,   28,   15,
 /*    10 */     7,   15,   14,   17,   39,   48,   41,   43,   47,   52,
 /*    20 */    49,   53,   54,   40,   56,   58,
};
#define YY_REDUCE_USE_DFLT (-26)
#define YY_REDUCE_COUNT (11)
#define YY_REDUCE_MIN   (-25)
#define YY_REDUCE_MAX   (20)
static const signed char yy_reduce_ofst[] = {
 /*     0 */   -21,   -3,  -22,    3,    3,   11,   13,  -25,  -20,   10,
 /*    10 */    16,   20,
};
static const YYACTIONTYPE yy_default[] = {
 /*     0 */   111,  112,  112,  114,  114,  118,  112,  116,  116,   90,
 /*    10 */   106,   90,  118,   95,  118,  118,  118,  118,  118,  118,
 /*    20 */   118,  118,  118,  118,  115,  118,
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
  TemplateParseARG_SDECL                /* A place to hold %extra_argument */
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
 /*   0 */ "typedef ::= TYPEDEF type IDENTIFIER",
 /*   1 */ "struct ::= STRUCT IDENTIFIER LBRACE struct_fields RBRACE SEMI",
 /*   2 */ "struct_fields ::= struct_fields struct_field",
 /*   3 */ "struct_fields ::= struct_field",
 /*   4 */ "struct_field ::= type IDENTIFIER array_count SEMI",
 /*   5 */ "struct_field ::= opt_volatile TEMPLATE opt_star IDENTIFIER array_count SEMI",
 /*   6 */ "array_count ::=",
 /*   7 */ "array_count ::= LBRACKET INTEGER RBRACKET",
 /*   8 */ "type ::= opt_volatile typecode",
 /*   9 */ "type ::= opt_volatile IDENTIFIER",
 /*  10 */ "typecode ::= SIGNED",
 /*  11 */ "typecode ::= UNSIGNED",
 /*  12 */ "typecode ::= opt_signed CHAR",
 /*  13 */ "typecode ::= UNSIGNED CHAR",
 /*  14 */ "typecode ::= opt_signed SHORT",
 /*  15 */ "typecode ::= UNSIGNED SHORT",
 /*  16 */ "typecode ::= opt_signed LONG opt_int",
 /*  17 */ "typecode ::= UNSIGNED LONG opt_int",
 /*  18 */ "typecode ::= opt_signed LONG LONG",
 /*  19 */ "typecode ::= UNSIGNED LONG LONG",
 /*  20 */ "typecode ::= TYPECODE",
 /*  21 */ "typecode ::= VOID STAR",
 /*  22 */ "opt_star ::=",
 /*  23 */ "opt_star ::= STAR",
 /*  24 */ "start ::= templates",
 /*  25 */ "templates ::= templates struct",
 /*  26 */ "templates ::= templates typedef",
 /*  27 */ "templates ::=",
 /*  28 */ "opt_volatile ::=",
 /*  29 */ "opt_volatile ::= VOLATILE",
 /*  30 */ "opt_signed ::=",
 /*  31 */ "opt_signed ::= SIGNED",
 /*  32 */ "opt_int ::=",
 /*  33 */ "opt_int ::= INT",
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
** second argument to TemplateParseAlloc() below.  This can be changed by
** putting an appropriate #define in the %include section of the input
** grammar.
*/
#ifndef YYMALLOCARGTYPE
# define YYMALLOCARGTYPE size_t
#endif

/* Initialize a new parser that has already been allocated.
*/
void TemplateParseInit(void *yypParser){
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

#ifndef TemplateParse_ENGINEALWAYSONSTACK
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
void *TemplateParseAlloc(void *(*mallocProc)(YYMALLOCARGTYPE)){
  yyParser *pParser;
  pParser = (yyParser*)(*mallocProc)( (YYMALLOCARGTYPE)sizeof(yyParser) );
  if( pParser ) TemplateParseInit(pParser);
  return pParser;
}
#endif /* TemplateParse_ENGINEALWAYSONSTACK */


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
  TemplateParseARG_FETCH;
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
void TemplateParseFinalize(void *p){
  yyParser *pParser = (yyParser*)p;
  while( pParser->yytos>pParser->yystack ) yy_pop_parser_stack(pParser);
#if YYSTACKDEPTH<=0
  if( pParser->yystack!=&pParser->yystk0 ) free(pParser->yystack);
#endif
}

#ifndef TemplateParse_ENGINEALWAYSONSTACK
/* 
** Deallocate and destroy a parser.  Destructors are called for
** all stack elements before shutting the parser down.
**
** If the YYPARSEFREENEVERNULL macro exists (for example because it
** is defined in a %include section of the input grammar) then it is
** assumed that the input pointer is never NULL.
*/
void TemplateParseFree(
  void *p,                    /* The parser to be deleted */
  void (*freeProc)(void*)     /* Function used to reclaim memory */
){
#ifndef YYPARSEFREENEVERNULL
  if( p==0 ) return;
#endif
  TemplateParseFinalize(p);
  (*freeProc)(p);
}
#endif /* TemplateParse_ENGINEALWAYSONSTACK */

/*
** Return the peak depth of the stack for a parser.
*/
#ifdef YYTRACKMAXSTACKDEPTH
int TemplateParseStackPeak(void *p){
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
   TemplateParseARG_FETCH;
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
   TemplateParseARG_STORE; /* Suppress warning about unused %extra_argument var */
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
  TemplateParseTOKENTYPE yyMinor        /* The minor token to shift in */
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
  { 27, -3 },
  { 26, -6 },
  { 22, -2 },
  { 22, -1 },
  { 23, -4 },
  { 23, -6 },
  { 29, 0 },
  { 29, -3 },
  { 28, -2 },
  { 28, -2 },
  { 32, -1 },
  { 32, -1 },
  { 32, -2 },
  { 32, -2 },
  { 32, -2 },
  { 32, -2 },
  { 32, -3 },
  { 32, -3 },
  { 32, -3 },
  { 32, -3 },
  { 32, -1 },
  { 32, -2 },
  { 31, 0 },
  { 31, -1 },
  { 24, -1 },
  { 25, -2 },
  { 25, -2 },
  { 25, 0 },
  { 30, 0 },
  { 30, -1 },
  { 33, 0 },
  { 33, -1 },
  { 34, 0 },
  { 34, -1 },
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
  TemplateParseARG_FETCH;
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
      case 0: /* typedef ::= TYPEDEF type IDENTIFIER */
#line 29 "template_parser.lemon"
{
	CreateTypedef((std::string *)yymsp[0].minor.yy0, yymsp[-1].minor.yy32, info);
}
#line 863 "template_parser.c"
        break;
      case 1: /* struct ::= STRUCT IDENTIFIER LBRACE struct_fields RBRACE SEMI */
#line 34 "template_parser.lemon"
{
	CreateTemplate((std::string *)yymsp[-4].minor.yy0, yymsp[-2].minor.yy51, info);
}
#line 870 "template_parser.c"
        break;
      case 2: /* struct_fields ::= struct_fields struct_field */
#line 38 "template_parser.lemon"
{
	// reverse order?
	yymsp[0].minor.yy51->next = yymsp[-1].minor.yy51;
	yylhsminor.yy51 = yymsp[0].minor.yy51;
}
#line 879 "template_parser.c"
  yymsp[-1].minor.yy51 = yylhsminor.yy51;
        break;
      case 3: /* struct_fields ::= struct_field */
#line 44 "template_parser.lemon"
{
	yylhsminor.yy51 = yymsp[0].minor.yy51;
}
#line 887 "template_parser.c"
  yymsp[0].minor.yy51 = yylhsminor.yy51;
        break;
      case 4: /* struct_field ::= type IDENTIFIER array_count SEMI */
#line 49 "template_parser.lemon"
{
	FieldEntry *e = (FieldEntry *)calloc(sizeof(FieldEntry), 1);

	e->name = (std::string *)yymsp[-2].minor.yy0;
	e->type = yymsp[-3].minor.yy32;
	e->count = yymsp[-1].minor.yy32;
	yylhsminor.yy51 = e;
}
#line 900 "template_parser.c"
  yymsp[-3].minor.yy51 = yylhsminor.yy51;
        break;
      case 5: /* struct_field ::= opt_volatile TEMPLATE opt_star IDENTIFIER array_count SEMI */
#line 58 "template_parser.lemon"
{
	FieldEntry *e = (FieldEntry *)calloc(sizeof(FieldEntry), 1);

	e->name = (std::string *)yymsp[-2].minor.yy0;
	e->type = yymsp[-3].minor.yy32 ? kStructPtr : kStruct;
	e->tmpl = (Template)yymsp[-4].minor.yy0;
	e->count = yymsp[-1].minor.yy32;

	yymsp[-5].minor.yy51 = e;
}
#line 915 "template_parser.c"
        break;
      case 6: /* array_count ::= */
      case 22: /* opt_star ::= */ yytestcase(yyruleno==22);
#line 70 "template_parser.lemon"
{ yymsp[1].minor.yy32 = 0; }
#line 921 "template_parser.c"
        break;
      case 7: /* array_count ::= LBRACKET INTEGER RBRACKET */
#line 71 "template_parser.lemon"
{ 
	int i = (int)(ptrdiff_t)yymsp[-1].minor.yy0;
	if (i == 0) {
		fprintf(stderr, "Template error: line %u: 0-sized arrays are not allowed.\n", 
			info->LineNumber);
		i = 1;
	}
	yymsp[-2].minor.yy32 = i; 
}
#line 934 "template_parser.c"
        break;
      case 8: /* type ::= opt_volatile typecode */
#line 82 "template_parser.lemon"
{ yymsp[-1].minor.yy32 = yymsp[0].minor.yy32; }
#line 939 "template_parser.c"
        break;
      case 9: /* type ::= opt_volatile IDENTIFIER */
#line 85 "template_parser.lemon"
{

	// ugh, Lemon will blindly replace text within a string.
	fprintf(stderr, "Template error: line %u: %s is not a known type.\n", 
		info->LineNumber, ((std::string *)yymsp[0].minor.yy0)->c_str());

	yymsp[-1].minor.yy32 = 'i';
}
#line 951 "template_parser.c"
        break;
      case 10: /* typecode ::= SIGNED */
#line 100 "template_parser.lemon"
{ yymsp[0].minor.yy32 = kSInt32; }
#line 956 "template_parser.c"
        break;
      case 11: /* typecode ::= UNSIGNED */
#line 101 "template_parser.lemon"
{yymsp[0].minor.yy32 = kUInt32; }
#line 961 "template_parser.c"
        break;
      case 12: /* typecode ::= opt_signed CHAR */
#line 103 "template_parser.lemon"
{ yymsp[-1].minor.yy32 = kSInt8; }
#line 966 "template_parser.c"
        break;
      case 13: /* typecode ::= UNSIGNED CHAR */
#line 104 "template_parser.lemon"
{ yymsp[-1].minor.yy32 = kUInt8; }
#line 971 "template_parser.c"
        break;
      case 14: /* typecode ::= opt_signed SHORT */
#line 106 "template_parser.lemon"
{ yymsp[-1].minor.yy32 = kSInt16; }
#line 976 "template_parser.c"
        break;
      case 15: /* typecode ::= UNSIGNED SHORT */
#line 107 "template_parser.lemon"
{ yymsp[-1].minor.yy32 = kUInt16; }
#line 981 "template_parser.c"
        break;
      case 16: /* typecode ::= opt_signed LONG opt_int */
#line 109 "template_parser.lemon"
{ yymsp[-2].minor.yy32 = kSInt32; }
#line 986 "template_parser.c"
        break;
      case 17: /* typecode ::= UNSIGNED LONG opt_int */
#line 110 "template_parser.lemon"
{ yymsp[-2].minor.yy32 = kUInt32; }
#line 991 "template_parser.c"
        break;
      case 18: /* typecode ::= opt_signed LONG LONG */
#line 112 "template_parser.lemon"
{ yymsp[-2].minor.yy32 = kSInt64; }
#line 996 "template_parser.c"
        break;
      case 19: /* typecode ::= UNSIGNED LONG LONG */
#line 113 "template_parser.lemon"
{ yymsp[-2].minor.yy32 = kUInt64; }
#line 1001 "template_parser.c"
        break;
      case 20: /* typecode ::= TYPECODE */
#line 115 "template_parser.lemon"
{ yylhsminor.yy32 = (int)(ptrdiff_t)yymsp[0].minor.yy0; }
#line 1006 "template_parser.c"
  yymsp[0].minor.yy32 = yylhsminor.yy32;
        break;
      case 21: /* typecode ::= VOID STAR */
#line 118 "template_parser.lemon"
{ yymsp[-1].minor.yy32 = kVoidPtr; }
#line 1012 "template_parser.c"
        break;
      case 23: /* opt_star ::= STAR */
#line 128 "template_parser.lemon"
{ yymsp[0].minor.yy32 = 1; }
#line 1017 "template_parser.c"
        break;
      default:
      /* (24) start ::= templates */ yytestcase(yyruleno==24);
      /* (25) templates ::= templates struct */ yytestcase(yyruleno==25);
      /* (26) templates ::= templates typedef */ yytestcase(yyruleno==26);
      /* (27) templates ::= */ yytestcase(yyruleno==27);
      /* (28) opt_volatile ::= */ yytestcase(yyruleno==28);
      /* (29) opt_volatile ::= VOLATILE */ yytestcase(yyruleno==29);
      /* (30) opt_signed ::= */ yytestcase(yyruleno==30);
      /* (31) opt_signed ::= SIGNED */ yytestcase(yyruleno==31);
      /* (32) opt_int ::= */ yytestcase(yyruleno==32);
      /* (33) opt_int ::= INT */ yytestcase(yyruleno==33);
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
  TemplateParseARG_FETCH;
#ifndef NDEBUG
  if( yyTraceFILE ){
    fprintf(yyTraceFILE,"%sFail!\n",yyTracePrompt);
  }
#endif
  while( yypParser->yytos>yypParser->yystack ) yy_pop_parser_stack(yypParser);
  /* Here code is inserted which will be executed whenever the
  ** parser fails */
/************ Begin %parse_failure code ***************************************/
/************ End %parse_failure code *****************************************/
  TemplateParseARG_STORE; /* Suppress warning about unused %extra_argument variable */
}
#endif /* YYNOERRORRECOVERY */

/*
** The following code executes when a syntax error first occurs.
*/
static void yy_syntax_error(
  yyParser *yypParser,           /* The parser */
  int yymajor,                   /* The major type of the error token */
  TemplateParseTOKENTYPE yyminor         /* The minor type of the error token */
){
  TemplateParseARG_FETCH;
#define TOKEN yyminor
/************ Begin %syntax_error code ****************************************/
/************ End %syntax_error code ******************************************/
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
#ifndef YYNOERRORRECOVERY
  yypParser->yyerrcnt = -1;
#endif
  assert( yypParser->yytos==yypParser->yystack );
  /* Here code is inserted which will be executed whenever the
  ** parser accepts */
/*********** Begin %parse_accept code *****************************************/
/*********** End %parse_accept code *******************************************/
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
  TemplateParseARG_STORE;

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
