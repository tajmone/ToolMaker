/*----------------------------------------------------------------------*\

  lmkErr.c

  Parser error handler 

\*----------------------------------------------------------------------*/

/* %%IMPORT */

#include "lmk_i.h"
#include "lmkScan.h"
#include "tmkCommon.h"
extern lmkContext lmkCtxt;
/* END %%IMPORT */

#include "lmkList.h"
#include "lmkCommon.h"

#define MaxTokens 5


static char insStr[101] = "";	/* Inserted symbol strings */
static int insToks = 0;

static char delStr[201] = "";	/* Deleted symbol strings */
static int delToks = 0;

/*-----------------------------------------------------------------------------
 * lmkRPoi - Recovery point, output a message indicating the position.
 *-----------------------------------------------------------------------------
 */
void lmkRPoi(token)
    LmkToken *token;		/* IN the restart symbol */
{
    if (delToks > MaxTokens) {
       /* Output a recovery point error message
       */
       lmkLog(&(token->srcp), 100, sevINF, "");
    }/*if*/

    /* Clear both token print strings */
    insStr[0] = '\0';
    delStr[0] = '\0';

    insToks = 0;
    delToks = 0;
}/*lmkRPoi()*/


/*-----------------------------------------------------------------------------
 * lmkISym - A symbol is to be inserted, collect it for later output, and
 *	    construct the requested token for use by the parser.
 *-----------------------------------------------------------------------------
 */
void lmkISym(code, symString, printString, token)
     int code;	/* IN terminal code number */
    char *symString;			/* IN terminal string */
    char *printString;			/* IN the terminals print symbol */
    LmkToken *token;		/* OUT the created scanner symbol */
{
    if (insToks < MaxTokens) {
	/* Concatenate the token string
	 */
	if (insToks > 0) strcat(insStr, " ");
	if (code == 0) strcat(insStr, "Unknown Token");
	else if (code == 1) strcat(insStr, "End Of File");
	else if (*printString != '\0') strcat(insStr, printString);
	else strcat(insStr, symString);
    } else if (insToks == MaxTokens) {
	strcat(insStr, " ...");
    }/*if*/
    insToks++;
    /* Make the requested token */
    token->code = code;
}/*lmkISym()*/


/*-----------------------------------------------------------------------------
 * lmkDSym - The indicated symbol is deleted by the parser, collect its string
 *	    for later output.
 *-----------------------------------------------------------------------------
 */
void lmkDSym(token, symString, printString)
    LmkToken *token;		/* IN terminal */
    char *symString;			/* IN terminal string */
    char *printString;			/* IN terminals print string */
{
    if (delToks < MaxTokens) {
	/* Concatenate the symbol strings */
	if (delToks > 0) strcat(delStr, " ");
	if (token->code == 0) strcat(delStr, "Unknown Token");
	else if (token->code == 1) strcat(delStr, "End Of File");
	else if (*printString != '\0') strcat(delStr, printString);
	else strcat(delStr, symString);
    } else if (delToks == MaxTokens) {
	strcat(delStr, " ...");
    }/*if*/
    delToks++;
}/*lmkDSym()*/


/*-----------------------------------------------------------------------------
 * lmkMess - An error message should be output, symbol indicates point of error.
 *-----------------------------------------------------------------------------
 * Method:	1 = Symbol(s) insertion	       Message:	% inserted
 *		2 = Symbol(s) deletion			% deleted
 *		3 = Symbol(s) replacement		% replaced by %
 *	        4 = Stack backup			Malformed phrase
 *		5 = Halted				%. System halted
 *
 * Code:	1 = Unknown token (error token from scanner)
 *		2 = Syntax error
 *		3 = Parse stack overflow
 *		4 = Table error
 *
 * Severity:	1 = Warning
 *		2 = Error (repairable)
 *		3 = Fatal error
 *		4 = System error & Limit error
 *-----------------------------------------------------------------------------
 */
void lmkMess(sym, method, code, severity)
    LmkToken *sym;	/* IN error token */
    int method;			/* IN recovery method */
    int code;			/* IN error classification */
    int severity;		/* IN error severity code */
{
    lmkSev sev;

    switch (severity) {
    case 1: sev = sevWAR; break;
    case 2: sev = sevERR; break;
    case 3: sev = sevFAT; break;
    case 4: sev = sevSYS; break;
    default: sev = sevSYS; break;
    }

    switch (code) {

    case 1:
	/* Unknown symbol, deleted */
	lmkLog(&(sym->srcp), 102, sev, "Unknown Token");
	break;

    case 2:
	/* Syntax Error */
	switch (method) {

	case 1:
	    /* Insert */
	    lmkLog(&(sym->srcp), 101, sev, insStr);
	    break;

	case 2:
	    /* Delete */
	    lmkLog(&(sym->srcp), 102, sev, delStr);
	    break;

	case 3:
	    /* Replace */
	    delStr[strlen(delStr)+1] = '\0';
	    delStr[strlen(delStr)] = lmkSEPARATOR; /* Separator */
	    strcat(delStr, insStr);
	    lmkLog(&(sym->srcp), 103, sev, delStr);
	    break;

	case 4:
	    /* Stack backed up */
	    lmkLog(&(sym->srcp), 104, sev, "");
	    break;

	case 5:
	    /* Syntax error, system halted */
	    lmkLog(&(sym->srcp), 105, sev, "");
	    break;
	}
	break;

    case 3:
	/* Parse stack overflow */
	lmkLog(&(sym->srcp), 106, sev, "");
	break;

    case 4:
	/* Parse table error */
	lmkLog(&(sym->srcp), 107, sev, "");
	break;
    }

    if (method == 5) {
	/* System halted, output informational message */
	lmkLog(&(sym->srcp), 108, sevINF, "");
    }
}/*lmkMess()*/