/*
  Copyright information from the original package:
*/
/*
  The authors of this software is Rob Pike.
		Copyright (c) 2002 by Lucent Technologies.
  Permission to use, copy, modify, and distribute this software for any
  purpose without fee is hereby granted, provided that this entire notice
  is included in all copies of any software which is or includes a copy
  or modification of this software and in all copies of the supporting
  documentation for such software.
  THIS SOFTWARE IS BEING PROVIDED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
  WARRANTY.  IN PARTICULAR, NEITHER THE AUTHORS NOR LUCENT TECHNOLOGIES MAKE ANY
  REPRESENTATION OR WARRANTY OF ANY KIND CONCERNING THE MERCHANTABILITY
  OF THIS SOFTWARE OR ITS FITNESS FOR ANY PARTICULAR PURPOSE.


  This is a Unix port of the Plan 9 regular expression library.

  Please send comments about the packaging
  to Russ Cox <rsc@swtch.com>.

  ----

  This software is also made available under the Lucent Public License
  version 1.02; see http://plan9.bell-labs.com/plan9dist/license.html
*/
/*
  This software was packaged for Unix by Russ Cox.
  Please send comments to rsc@swtch.com.

  http://swtch.com/plan9port/unix
*/

#include <stdlib.h>
#include <setjmp.h>
#include <unistd.h>
#include "regexp9.h"

#define nil 0
#define exits(x) exit(x && *x ? 1 : 0)

/****************************************************
 * Routines from rune.c, runestrchr.c and utfrune.c *
 ****************************************************/

#include <stdarg.h>
#include <string.h>

enum
{
	Bit1	= 7,
	Bitx	= 6,
	Bit2	= 5,
	Bit3	= 4,
	Bit4	= 3,

	T1	= ((1<<(Bit1+1))-1) ^ 0xFF,	/* 0000 0000 */
	Tx	= ((1<<(Bitx+1))-1) ^ 0xFF,	/* 1000 0000 */
	T2	= ((1<<(Bit2+1))-1) ^ 0xFF,	/* 1100 0000 */
	T3	= ((1<<(Bit3+1))-1) ^ 0xFF,	/* 1110 0000 */
	T4	= ((1<<(Bit4+1))-1) ^ 0xFF,	/* 1111 0000 */

	Rune1	= (1<<(Bit1+0*Bitx))-1,		/* 0000 0000 0111 1111 */
	Rune2	= (1<<(Bit2+1*Bitx))-1,		/* 0000 0111 1111 1111 */
	Rune3	= (1<<(Bit3+2*Bitx))-1,		/* 1111 1111 1111 1111 */

	Maskx	= (1<<Bitx)-1,			/* 0011 1111 */
	Testx	= Maskx ^ 0xFF,			/* 1100 0000 */

	Bad	= Runeerror
};

int
chartorune(Rune *rune, const char *str)
{
	int c, c1, c2;
	long l;

	/*
	 * one character sequence
	 *	00000-0007F => T1
	 */
	c = *(uchar*)str;
	if(c < Tx) {
		*rune = c;
		return 1;
	}

	/*
	 * two character sequence
	 *	0080-07FF => T2 Tx
	 */
	c1 = *(uchar*)(str+1) ^ Tx;
	if(c1 & Testx)
		goto bad;
	if(c < T3) {
		if(c < T2)
			goto bad;
		l = ((c << Bitx) | c1) & Rune2;
		if(l <= Rune1)
			goto bad;
		*rune = l;
		return 2;
	}

	/*
	 * three character sequence
	 *	0800-FFFF => T3 Tx Tx
	 */
	c2 = *(uchar*)(str+2) ^ Tx;
	if(c2 & Testx)
		goto bad;
	if(c < T4) {
		l = ((((c << Bitx) | c1) << Bitx) | c2) & Rune3;
		if(l <= Rune2)
			goto bad;
		*rune = l;
		return 3;
	}

	/*
	 * bad decoding
	 */
bad:
	*rune = Bad;
	return 1;
}

Rune*
runestrchr(const Rune *s, Rune c)
{
	Rune c0 = c;
	Rune c1;

	if(c == 0) {
		while(*s++)
			;
		return (Rune*)s-1;
	}

	while((c1 = *s++))
		if(c1 == c0)
			return (Rune*)s-1;
	return 0;
}

char*
utfrune(char *s, long c)
{
	long c1;
	Rune r;
	int n;

	if(c < Runesync)		/* not part of utf sequence */
		return strchr(s, c);

	for(;;) {
		c1 = *(uchar*)s;
		if(c1 < Runeself) {	/* one byte rune */
			if(c1 == 0)
				return 0;
			if(c1 == c)
				return s;
			s++;
			continue;
		}
		n = chartorune(&r, s);
		if(r == c)
			return s;
		s += n;
	}
}

/************
 * regaux.c *
 ************/

/*
 *  save a new match in mp
 */
static void
_renewmatch(Resub *mp, int ms, Resublist *sp)
{
	int i;

	if(mp==0 || ms<=0)
		return;
	if(mp[0].s.sp==0 || sp->m[0].s.sp<mp[0].s.sp ||
	   (sp->m[0].s.sp==mp[0].s.sp && sp->m[0].e.ep>mp[0].e.ep)){
		for(i=0; i<ms && i<NSUBEXP; i++)
			mp[i] = sp->m[i];
		for(; i<ms; i++)
			mp[i].s.sp = mp[i].e.ep = 0;
	}
}

/*
 * Note optimization in _renewthread:
 * 	*lp must be pending when _renewthread called; if *l has been looked
 *		at already, the optimization is a bug.
 */
static Relist*
_renewthread(Relist *lp,	/* _relist to add to */
	Reinst *ip,		/* instruction to add */
	int ms,
	Resublist *sep)		/* pointers to subexpressions */
{
	Relist *p;

	for(p=lp; p->inst; p++){
		if(p->inst == ip){
			if(sep->m[0].s.sp < p->se.m[0].s.sp){
				if(ms > 1)
					p->se = *sep;
				else
					p->se.m[0] = sep->m[0];
			}
			return 0;
		}
	}
	p->inst = ip;
	if(ms > 1)
		p->se = *sep;
	else
		p->se.m[0] = sep->m[0];
	(++p)->inst = 0;
	return p;
}

/*
 * same as renewthread, but called with
 * initial empty start pointer.
 */
static Relist*
_renewemptythread(Relist *lp,	/* _relist to add to */
	Reinst *ip,		/* instruction to add */
	int ms,
	char *sp)		/* pointers to subexpressions */
{
	Relist *p;

	for(p=lp; p->inst; p++){
		if(p->inst == ip){
			if(sp < p->se.m[0].s.sp) {
				if(ms > 1)
					memset(&p->se, 0, sizeof(p->se));
				p->se.m[0].s.sp = sp;
			}
			return 0;
		}
	}
	p->inst = ip;
	if(ms > 1)
		memset(&p->se, 0, sizeof(p->se));
	p->se.m[0].s.sp = sp;
	(++p)->inst = 0;
	return p;
}

static Relist*
_rrenewemptythread(Relist *lp,	/* _relist to add to */
	Reinst *ip,		/* instruction to add */
	int ms,
	Rune *rsp)		/* pointers to subexpressions */
{
	Relist *p;

	for(p=lp; p->inst; p++){
		if(p->inst == ip){
			if(rsp < p->se.m[0].s.rsp) {
				if(ms > 1)
					memset(&p->se, 0, sizeof(p->se));
				p->se.m[0].s.rsp = rsp;
			}
			return 0;
		}
	}
	p->inst = ip;
	if(ms > 1)
		memset(&p->se, 0, sizeof(p->se));
	p->se.m[0].s.rsp = rsp;
	(++p)->inst = 0;
	return p;
}

/*************
 * regcomp.c *
 *************/

#define	TRUE	1
#define	FALSE	0

/*
 * Parser Information
 */
typedef
struct Node
{
	Reinst*	first;
	Reinst*	last;
}Node;

#define	NSTACK	20
static	Node	andstack[NSTACK];
static	Node	*andp;
static	int	atorstack[NSTACK];
static	int*	atorp;
static	int	cursubid;		/* id of current subexpression */
static	int	subidstack[NSTACK];	/* parallel to atorstack */
static	int*	subidp;
static	int	lastwasand;	/* Last token was operand */
static	int	nbra;
static	char*	exprp;		/* pointer to next character in source expression */
static	int	lexdone;
static	int	nclass;
static	Reclass*classp;
static	Reinst*	freep;
static	int	errors;
static	Rune	yyrune;		/* last lex'd rune */
static	Reclass*yyclassp;	/* last lex'd class */

/* predeclared crap */
static	void	operator(int);
static	void	pushand(Reinst*, Reinst*);
static	void	pushator(int);
static	void	evaluntil(int);
static	int	bldcclass(void);

static jmp_buf regkaboom;

static	void
rcerror(char *s)
{
	errors++;
	regerror9(s);
	longjmp(regkaboom, 1);
}

static	Reinst*
newinst(int t)
{
	freep->type = t;
	freep->u2.left = 0;
	freep->u1.right = 0;
	return freep++;
}

static	void
operand(int t)
{
	Reinst *i;

	if(lastwasand)
		operator(CAT);	/* catenate is implicit */
	i = newinst(t);

	if(t == CCLASS || t == NCCLASS)
		i->u1.cp = yyclassp;
	if(t == RUNE)
		i->u1.r = yyrune;

	pushand(i, i);
	lastwasand = TRUE;
}

static	void
operator(int t)
{
	if(t==RBRA && --nbra<0)
		rcerror("unmatched right paren");
	if(t==LBRA){
		if(++cursubid >= NSUBEXP)
			rcerror ("too many subexpressions");
		nbra++;
		if(lastwasand)
			operator(CAT);
	} else
		evaluntil(t);
	if(t != RBRA)
		pushator(t);
	lastwasand = FALSE;
	if(t==STAR || t==QUEST || t==PLUS || t==RBRA)
		lastwasand = TRUE;	/* these look like operands */
}

static	void
regerr2(char *s, int c)
{
	char buf[100];
	char *cp = buf;
	while(*s)
		*cp++ = *s++;
	*cp++ = c;
	*cp = '\0'; 
	rcerror(buf);
}

static	void
cant(char *s)
{
	char buf[100];
	strcpy(buf, "can't happen: ");
	strcat(buf, s);
	rcerror(buf);
}

static	void
pushand(Reinst *f, Reinst *l)
{
	if(andp >= &andstack[NSTACK])
		cant("operand stack overflow");
	andp->first = f;
	andp->last = l;
	andp++;
}

static	void
pushator(int t)
{
	if(atorp >= &atorstack[NSTACK])
		cant("operator stack overflow");
	*atorp++ = t;
	*subidp++ = cursubid;
}

static	Node*
popand(int op)
{
	Reinst *inst;

	if(andp <= &andstack[0]){
		regerr2("missing operand for ", op);
		inst = newinst(NOP);
		pushand(inst,inst);
	}
	return --andp;
}

static	int
popator(void)
{
	if(atorp <= &atorstack[0])
		cant("operator stack underflow");
	--subidp;
	return *--atorp;
}

static	void
evaluntil(int pri)
{
	Node *op1, *op2;
	Reinst *inst1, *inst2;

	while(pri==RBRA || atorp[-1]>=pri){
		switch(popator()){
		default:
			rcerror("unknown operator in evaluntil");
			break;
		case LBRA:		/* must have been RBRA */
			op1 = popand('(');
			inst2 = newinst(RBRA);
			inst2->u1.subid = *subidp;
			op1->last->u2.next = inst2;
			inst1 = newinst(LBRA);
			inst1->u1.subid = *subidp;
			inst1->u2.next = op1->first;
			pushand(inst1, inst2);
			return;
		case OR:
			op2 = popand('|');
			op1 = popand('|');
			inst2 = newinst(NOP);
			op2->last->u2.next = inst2;
			op1->last->u2.next = inst2;
			inst1 = newinst(OR);
			inst1->u1.right = op1->first;
			inst1->u2.left = op2->first;
			pushand(inst1, inst2);
			break;
		case CAT:
			op2 = popand(0);
			op1 = popand(0);
			op1->last->u2.next = op2->first;
			pushand(op1->first, op2->last);
			break;
		case STAR:
			op2 = popand('*');
			inst1 = newinst(OR);
			op2->last->u2.next = inst1;
			inst1->u1.right = op2->first;
			pushand(inst1, inst1);
			break;
		case PLUS:
			op2 = popand('+');
			inst1 = newinst(OR);
			op2->last->u2.next = inst1;
			inst1->u1.right = op2->first;
			pushand(op2->first, inst1);
			break;
		case QUEST:
			op2 = popand('?');
			inst1 = newinst(OR);
			inst2 = newinst(NOP);
			inst1->u2.left = inst2;
			inst1->u1.right = op2->first;
			op2->last->u2.next = inst2;
			pushand(inst1, inst2);
			break;
		}
	}
}

static	Reprog*
optimize(Reprog *pp)
{
	Reinst *inst, *target;
	int size;
	Reprog *npp;
	Reclass *cl;
	int diff;

	/*
	 *  get rid of NOOP chains
	 */
	for(inst=pp->firstinst; inst->type!=END; inst++){
		target = inst->u2.next;
		while(target->type == NOP)
			target = target->u2.next;
		inst->u2.next = target;
	}

	/*
	 *  The original allocation is for an area larger than
	 *  necessary.  Reallocate to the actual space used
	 *  and then relocate the code.
	 */
	size = sizeof(Reprog) + (freep - pp->firstinst)*sizeof(Reinst);
	npp = realloc(pp, size);
	if(npp==0 || npp==pp)
		return pp;
	diff = (char *)npp - (char *)pp;
	freep = (Reinst *)((char *)freep + diff);
	for(inst=npp->firstinst; inst<freep; inst++){
		switch(inst->type){
		case OR:
		case STAR:
		case PLUS:
		case QUEST:
			inst->u1.right = (void*)((char*)inst->u1.right + diff);
			break;
		case CCLASS:
		case NCCLASS:
			inst->u1.right = (void*)((char*)inst->u1.right + diff);
			cl = inst->u1.cp;
			cl->end = (void*)((char*)cl->end + diff);
			break;
		}
		inst->u2.left = (void*)((char*)inst->u2.left + diff);
	}
	npp->startinst = (void*)((char*)npp->startinst + diff);
	return npp;
}

#ifdef	DEBUG
static	void
dumpstack(void){
	Node *stk;
	int *ip;

	print("operators\n");
	for(ip=atorstack; ip<atorp; ip++)
		print("0%o\n", *ip);
	print("operands\n");
	for(stk=andstack; stk<andp; stk++)
		print("0%o\t0%o\n", stk->first->type, stk->last->type);
}

static	void
dump(Reprog *pp)
{
	Reinst *l;
	Rune *p;

	l = pp->firstinst;
	do{
		print("%d:\t0%o\t%d\t%d", l-pp->firstinst, l->type,
			l->u2.left-pp->firstinst, l->u1.right-pp->firstinst);
		if(l->type == RUNE)
			print("\t%C\n", l->u1.r);
		else if(l->type == CCLASS || l->type == NCCLASS){
			print("\t[");
			if(l->type == NCCLASS)
				print("^");
			for(p = l->u1.cp->spans; p < l->u1.cp->end; p += 2)
				if(p[0] == p[1])
					print("%C", p[0]);
				else
					print("%C-%C", p[0], p[1]);
			print("]\n");
		} else
			print("\n");
	}while(l++->type);
}
#endif

static	Reclass*
newclass(void)
{
	if(nclass >= NCLASS)
		regerr2("too many character classes; limit", NCLASS+'0');
	return &(classp[nclass++]);
}

static	int
nextc(Rune *rp)
{
	if(lexdone){
		*rp = 0;
		return 1;
	}
	exprp += chartorune(rp, exprp);
	if(*rp == '\\'){
		exprp += chartorune(rp, exprp);
		return 1;
	}
	if(*rp == 0)
		lexdone = 1;
	return 0;
}

static	int
lex(int literal, int dot_type)
{
	int quoted;

	quoted = nextc(&yyrune);
	if(literal || quoted){
		if(yyrune == 0)
			return END;
		return RUNE;
	}

	switch(yyrune){
	case 0:
		return END;
	case '*':
		return STAR;
	case '?':
		return QUEST;
	case '+':
		return PLUS;
	case '|':
		return OR;
	case '.':
		return dot_type;
	case '(':
		return LBRA;
	case ')':
		return RBRA;
	case '^':
		return BOL;
	case '$':
		return EOL;
	case '[':
		return bldcclass();
	}
	return RUNE;
}

static int
bldcclass(void)
{
	int type;
	Rune r[NCCRUNE];
	Rune *p, *ep, *np;
	Rune rune;
	int quoted;

	/* we have already seen the '[' */
	type = CCLASS;
	yyclassp = newclass();

	/* look ahead for negation */
	/* SPECIAL CASE!!! negated classes don't match \n */
	ep = r;
	quoted = nextc(&rune);
	if(!quoted && rune == '^'){
		type = NCCLASS;
		quoted = nextc(&rune);
		*ep++ = '\n';
		*ep++ = '\n';
	}

	/* parse class into a set of spans */
	for(; ep<&r[NCCRUNE];){
		if(rune == 0){
			rcerror("malformed '[]'");
			return 0;
		}
		if(!quoted && rune == ']')
			break;
		if(!quoted && rune == '-'){
			if(ep == r){
				rcerror("malformed '[]'");
				return 0;
			}
			quoted = nextc(&rune);
			if((!quoted && rune == ']') || rune == 0){
				rcerror("malformed '[]'");
				return 0;
			}
			*(ep-1) = rune;
		} else {
			*ep++ = rune;
			*ep++ = rune;
		}
		quoted = nextc(&rune);
	}

	/* sort on span start */
	for(p = r; p < ep; p += 2){
		for(np = p; np < ep; np += 2)
			if(*np < *p){
				rune = np[0];
				np[0] = p[0];
				p[0] = rune;
				rune = np[1];
				np[1] = p[1];
				p[1] = rune;
			}
	}

	/* merge spans */
	np = yyclassp->spans;
	p = r;
	if(r == ep)
		yyclassp->end = np;
	else {
		np[0] = *p++;
		np[1] = *p++;
		for(; p < ep; p += 2)
			if(p[0] <= np[1]){
				if(p[1] > np[1])
					np[1] = p[1];
			} else {
				np += 2;
				np[0] = p[0];
				np[1] = p[1];
			}
		yyclassp->end = np+2;
	}

	return type;
}

static	Reprog*
regcomp1(char *s, int literal, int dot_type)
{
	int token;
	Reprog *volatile pp;

	/* get memory for the program */
	pp = malloc(sizeof(Reprog) + 6*sizeof(Reinst)*strlen(s));
	if(pp == 0){
		regerror9("out of memory");
		return 0;
	}
	freep = pp->firstinst;
	classp = pp->class;
	errors = 0;

	if(setjmp(regkaboom))
		goto out;

	/* go compile the sucker */
	lexdone = 0;
	exprp = s;
	nclass = 0;
	nbra = 0;
	atorp = atorstack;
	andp = andstack;
	subidp = subidstack;
	lastwasand = FALSE;
	cursubid = 0;

	/* Start with a low priority operator to prime parser */
	pushator(START-1);
	while((token = lex(literal, dot_type)) != END){
		if((token&0300) == OPERATOR)
			operator(token);
		else
			operand(token);
	}

	/* Close with a low priority operator */
	evaluntil(START);

	/* Force END */
	operand(END);
	evaluntil(START);
#ifdef DEBUG
	dumpstack();
#endif
	if(nbra)
		rcerror("unmatched left paren");
	--andp;	/* points to first and only operand */
	pp->startinst = andp->first;
#ifdef DEBUG
	dump(pp);
#endif
	pp = optimize(pp);
#ifdef DEBUG
	print("start: %d\n", andp->first-pp->firstinst);
	dump(pp);
#endif
out:
	if(errors){
		free(pp);
		pp = 0;
	}
	return pp;
}

extern	Reprog*
regcomp9(char *s)
{
	return regcomp1(s, 0, ANY);
}

extern	Reprog*
regcomplit9(char *s)
{
	return regcomp1(s, 1, ANY);
}

extern	Reprog*
regcompnl9(char *s)
{
	return regcomp1(s, 0, ANYNL);
}

/*************
 * regexec.c *
 *************/

/*
 *  return	0 if no match
 *		>0 if a match
 *		<0 if we ran out of _relist space
 */
static int
regexec1(const Reprog *progp,	/* program to run */
	char *bol,	/* string to run machine on */
	Resub *mp,	/* subexpression elements */
	int ms,		/* number of elements at mp */
	Reljunk *j
)
{
	int flag=0;
	Reinst *inst;
	Relist *tlp;
	char *s;
	int i, checkstart;
	Rune r, *rp, *ep;
	int n;
	Relist* tl;		/* This list, next list */
	Relist* nl;
	Relist* tle;		/* ends of this and next list */
	Relist* nle;
	int match;
	char *p;

	match = 0;
	checkstart = j->starttype;
	if(mp)
		for(i=0; i<ms; i++) {
			mp[i].s.sp = 0;
			mp[i].e.ep = 0;
		}
	j->relist[0][0].inst = 0;
	j->relist[1][0].inst = 0;

	/* Execute machine once for each character, including terminal NUL */
	s = j->starts;
	do{
		/* fast check for first char */
		if(checkstart) {
			switch(j->starttype) {
			case RUNE:
				p = utfrune(s, j->startchar);
				if(p == 0 || s == j->eol)
					return match;
				s = p;
				break;
			case BOL:
				if(s == bol)
					break;
				p = utfrune(s, '\n');
				if(p == 0 || s == j->eol)
					return match;
				s = p+1;
				break;
			}
		}
		r = *(uchar*)s;
		if(r < Runeself)
			n = 1;
		else
			n = chartorune(&r, s);

		/* switch run lists */
		tl = j->relist[flag];
		tle = j->reliste[flag];
		nl = j->relist[flag^=1];
		nle = j->reliste[flag];
		nl->inst = 0;

		/* Add first instruction to current list */
		if(match == 0)
			_renewemptythread(tl, progp->startinst, ms, s);

		/* Execute machine until current list is empty */
		for(tlp=tl; tlp->inst; tlp++){	/* assignment = */
			for(inst = tlp->inst; ; inst = inst->u2.next){
				switch(inst->type){
				case RUNE:	/* regular character */
					if(inst->u1.r == r){
						if(_renewthread(nl, inst->u2.next, ms, &tlp->se)==nle)
							return -1;
					}
					break;
				case LBRA:
					tlp->se.m[inst->u1.subid].s.sp = s;
					continue;
				case RBRA:
					tlp->se.m[inst->u1.subid].e.ep = s;
					continue;
				case ANY:
					if(r != '\n')
						if(_renewthread(nl, inst->u2.next, ms, &tlp->se)==nle)
							return -1;
					break;
				case ANYNL:
					if(_renewthread(nl, inst->u2.next, ms, &tlp->se)==nle)
							return -1;
					break;
				case BOL:
					if(s == bol || *(s-1) == '\n')
						continue;
					break;
				case EOL:
					if(s == j->eol || r == 0 || r == '\n')
						continue;
					break;
				case CCLASS:
					ep = inst->u1.cp->end;
					for(rp = inst->u1.cp->spans; rp < ep; rp += 2)
						if(r >= rp[0] && r <= rp[1]){
							if(_renewthread(nl, inst->u2.next, ms, &tlp->se)==nle)
								return -1;
							break;
						}
					break;
				case NCCLASS:
					ep = inst->u1.cp->end;
					for(rp = inst->u1.cp->spans; rp < ep; rp += 2)
						if(r >= rp[0] && r <= rp[1])
							break;
					if(rp == ep)
						if(_renewthread(nl, inst->u2.next, ms, &tlp->se)==nle)
							return -1;
					break;
				case OR:
					/* evaluate right choice later */
					if(_renewthread(tlp, inst->u1.right, ms, &tlp->se) == tle)
						return -1;
					/* efficiency: advance and re-evaluate */
					continue;
				case END:	/* Match! */
					match = 1;
					tlp->se.m[0].e.ep = s;
					if(mp != 0)
						_renewmatch(mp, ms, &tlp->se);
					break;
				}
				break;
			}
		}
		if(s == j->eol)
			break;
		checkstart = j->starttype && nl->inst==0;
		s += n;
	}while(r);
	return match;
}

static int
regexec2(const Reprog *progp,	/* program to run */
	char *bol,	/* string to run machine on */
	Resub *mp,	/* subexpression elements */
	int ms,		/* number of elements at mp */
	Reljunk *j
)
{
	int rv;
	Relist *relist0, *relist1;

	/* mark space */
	relist0 = malloc(BIGLISTSIZE*sizeof(Relist));
	if(relist0 == nil)
		return -1;
	relist1 = malloc(BIGLISTSIZE*sizeof(Relist));
	if(relist1 == nil){
		free(relist1);
		return -1;
	}
	j->relist[0] = relist0;
	j->relist[1] = relist1;
	j->reliste[0] = relist0 + BIGLISTSIZE - 2;
	j->reliste[1] = relist1 + BIGLISTSIZE - 2;

	rv = regexec1(progp, bol, mp, ms, j);
	free(relist0);
	free(relist1);
	return rv;
}

extern int
regexec9(const Reprog *progp,	/* program to run */
	char *bol,	/* string to run machine on */
	Resub *mp,	/* subexpression elements */
	int ms)		/* number of elements at mp */
{
	Reljunk j;
	Relist relist0[LISTSIZE], relist1[LISTSIZE];
	int rv;

	/*
 	 *  use user-specified starting/ending location if specified
	 */
	j.starts = bol;
	j.eol = 0;
	if(mp && ms>0){
		if(mp->s.sp)
			j.starts = mp->s.sp;
		if(mp->e.ep)
			j.eol = mp->e.ep;
	}
	j.starttype = 0;
	j.startchar = 0;
	if(progp->startinst->type == RUNE && progp->startinst->u1.r < Runeself) {
		j.starttype = RUNE;
		j.startchar = progp->startinst->u1.r;
	}
	if(progp->startinst->type == BOL)
		j.starttype = BOL;

	/* mark space */
	j.relist[0] = relist0;
	j.relist[1] = relist1;
	j.reliste[0] = relist0 + nelem(relist0) - 2;
	j.reliste[1] = relist1 + nelem(relist1) - 2;

	rv = regexec1(progp, bol, mp, ms, &j);
	if(rv >= 0)
		return rv;
	rv = regexec2(progp, bol, mp, ms, &j);
	if(rv >= 0)
		return rv;
	return -1;
}

/************
 * regsub.c *
 ************/

/* substitute into one string using the matches from the last regexec() */
extern	void
regsub9(char *sp,	/* source string */
	char *dp,	/* destination string */
	int dlen,
	Resub *mp,	/* subexpression elements */
	int ms)		/* number of elements pointed to by mp */
{
	char *ssp, *ep;
	int i;

	ep = dp+dlen-1;
	while(*sp != '\0'){
		if(*sp == '\\'){
			switch(*++sp){
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				i = *sp-'0';
				if(mp[i].s.sp != 0 && mp!=0 && ms>i)
					for(ssp = mp[i].s.sp;
					     ssp < mp[i].e.ep;
					     ssp++)
						if(dp < ep)
							*dp++ = *ssp;
				break;
			case '\\':
				if(dp < ep)
					*dp++ = '\\';
				break;
			case '\0':
				sp--;
				break;
			default:
				if(dp < ep)
					*dp++ = *sp;
				break;
			}
		}else if(*sp == '&'){				
			if(mp[0].s.sp != 0 && mp!=0 && ms>0)
			if(mp[0].s.sp != 0)
				for(ssp = mp[0].s.sp;
				     ssp < mp[0].e.ep; ssp++)
					if(dp < ep)
						*dp++ = *ssp;
		}else{
			if(dp < ep)
				*dp++ = *sp;
		}
		sp++;
	}
	*dp = '\0';
}

/**************
 * regerror.c *
 **************/

void
regerror9(char *s)
{
	char buf[132];

	strcpy(buf, "regerror: ");
	strcat(buf, s);
	strcat(buf, "\n");
	write(2, buf, strlen(buf));
	exits("regerr");
}

/**************
 * rregexec.c *
 **************/

/*
 *  return	0 if no match
 *		>0 if a match
 *		<0 if we ran out of _relist space
 */
static int
rregexec1(const Reprog *progp,	/* program to run */
	Rune *bol,		/* string to run machine on */
	Resub *mp,		/* subexpression elements */
	int ms,			/* number of elements at mp */
	Reljunk *j)
{
	int flag=0;
	Reinst *inst;
	Relist *tlp;
	Rune *s;
	int i, checkstart;
	Rune r, *rp, *ep;
	Relist* tl;		/* This list, next list */
	Relist* nl;
	Relist* tle;		/* ends of this and next list */
	Relist* nle;
	int match;
	Rune *p;

	match = 0;
	checkstart = j->startchar;
	if(mp)
		for(i=0; i<ms; i++) {
			mp[i].s.rsp = 0;
			mp[i].e.rep = 0;
		}
	j->relist[0][0].inst = 0;
	j->relist[1][0].inst = 0;

	/* Execute machine once for each character, including terminal NUL */
	s = j->rstarts;
	do{

		/* fast check for first char */
		if(checkstart) {
			switch(j->starttype) {
			case RUNE:
				p = runestrchr(s, j->startchar);
				if(p == 0 || p == j->reol)
					return match;
				s = p;
				break;
			case BOL:
				if(s == bol)
					break;
				p = runestrchr(s, '\n');
				if(p == 0 || s == j->reol)
					return match;
				s = p+1;
				break;
			}
		}

		r = *s;

		/* switch run lists */
		tl = j->relist[flag];
		tle = j->reliste[flag];
		nl = j->relist[flag^=1];
		nle = j->reliste[flag];
		nl->inst = 0;

		/* Add first instruction to current list */
		_rrenewemptythread(tl, progp->startinst, ms, s);

		/* Execute machine until current list is empty */
		for(tlp=tl; tlp->inst; tlp++){
			for(inst=tlp->inst; ; inst = inst->u2.next){
				switch(inst->type){
				case RUNE:	/* regular character */
					if(inst->u1.r == r)
						if(_renewthread(nl, inst->u2.next, ms, &tlp->se)==nle)
							return -1;
					break;
				case LBRA:
					tlp->se.m[inst->u1.subid].s.rsp = s;
					continue;
				case RBRA:
					tlp->se.m[inst->u1.subid].e.rep = s;
					continue;
				case ANY:
					if(r != '\n')
						if(_renewthread(nl, inst->u2.next, ms, &tlp->se)==nle)
							return -1;
					break;
				case ANYNL:
					if(_renewthread(nl, inst->u2.next, ms, &tlp->se)==nle)
							return -1;
					break;
				case BOL:
					if(s == bol || *(s-1) == '\n')
						continue;
					break;
				case EOL:
					if(s == j->reol || r == 0 || r == '\n')
						continue;
					break;
				case CCLASS:
					ep = inst->u1.cp->end;
					for(rp = inst->u1.cp->spans; rp < ep; rp += 2)
						if(r >= rp[0] && r <= rp[1]){
							if(_renewthread(nl, inst->u2.next, ms, &tlp->se)==nle)
								return -1;
							break;
						}
					break;
				case NCCLASS:
					ep = inst->u1.cp->end;
					for(rp = inst->u1.cp->spans; rp < ep; rp += 2)
						if(r >= rp[0] && r <= rp[1])
							break;
					if(rp == ep)
						if(_renewthread(nl, inst->u2.next, ms, &tlp->se)==nle)
							return -1;
					break;
				case OR:
					/* evaluate right choice later */
					if(_renewthread(tlp, inst->u1.right, ms, &tlp->se) == tle)
						return -1;
					/* efficiency: advance and re-evaluate */
					continue;
				case END:	/* Match! */
					match = 1;
					tlp->se.m[0].e.rep = s;
					if(mp != 0)
						_renewmatch(mp, ms, &tlp->se);
					break;
				}
				break;
			}
		}
		if(s == j->reol)
			break;
		checkstart = j->startchar && nl->inst==0;
		s++;
	}while(r);
	return match;
}

static int
rregexec2(const Reprog *progp,	/* program to run */
	Rune *bol,	/* string to run machine on */
	Resub *mp,	/* subexpression elements */
	int ms,		/* number of elements at mp */
	Reljunk *j
)
{
	Relist relist0[5*LISTSIZE], relist1[5*LISTSIZE];

	/* mark space */
	j->relist[0] = relist0;
	j->relist[1] = relist1;
	j->reliste[0] = relist0 + nelem(relist0) - 2;
	j->reliste[1] = relist1 + nelem(relist1) - 2;

	return rregexec1(progp, bol, mp, ms, j);
}

extern int
rregexec9(const Reprog *progp,	/* program to run */
	Rune *bol,	/* string to run machine on */
	Resub *mp,	/* subexpression elements */
	int ms)		/* number of elements at mp */
{
	Reljunk j;
	Relist relist0[LISTSIZE], relist1[LISTSIZE];
	int rv;

	/*
 	 *  use user-specified starting/ending location if specified
	 */
	j.rstarts = bol;
	j.reol = 0;
	if(mp && ms>0){
		if(mp->s.sp)
			j.rstarts = mp->s.rsp;
		if(mp->e.ep)
			j.reol = mp->e.rep;
	}
	j.starttype = 0;
	j.startchar = 0;
	if(progp->startinst->type == RUNE && progp->startinst->u1.r < Runeself) {
		j.starttype = RUNE;
		j.startchar = progp->startinst->u1.r;
	}
	if(progp->startinst->type == BOL)
		j.starttype = BOL;

	/* mark space */
	j.relist[0] = relist0;
	j.relist[1] = relist1;
	j.reliste[0] = relist0 + nelem(relist0) - 2;
	j.reliste[1] = relist1 + nelem(relist1) - 2;

	rv = rregexec1(progp, bol, mp, ms, &j);
	if(rv >= 0)
		return rv;
	rv = rregexec2(progp, bol, mp, ms, &j);
	if(rv >= 0)
		return rv;
	return -1;
}

/*************
 * rregsub.c *
 *************/

/* substitute into one string using the matches from the last regexec() */
extern	void
rregsub9(Rune *sp,	/* source string */
	Rune *dp,	/* destination string */
	int dlen,
	Resub *mp,	/* subexpression elements */
	int ms)		/* number of elements pointed to by mp */
{
	Rune *ssp, *ep;
	int i;

	ep = dp+(dlen/sizeof(Rune))-1;
	while(*sp != '\0'){
		if(*sp == '\\'){
			switch(*++sp){
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				i = *sp-'0';
				if(mp[i].s.rsp != 0 && mp!=0 && ms>i)
					for(ssp = mp[i].s.rsp;
					     ssp < mp[i].e.rep;
					     ssp++)
						if(dp < ep)
							*dp++ = *ssp;
				break;
			case '\\':
				if(dp < ep)
					*dp++ = '\\';
				break;
			case '\0':
				sp--;
				break;
			default:
				if(dp < ep)
					*dp++ = *sp;
				break;
			}
		}else if(*sp == '&'){				
			if(mp[0].s.rsp != 0 && mp!=0 && ms>0)
			if(mp[0].s.rsp != 0)
				for(ssp = mp[0].s.rsp;
				     ssp < mp[0].e.rep; ssp++)
					if(dp < ep)
						*dp++ = *ssp;
		}else{
			if(dp < ep)
				*dp++ = *sp;
		}
		sp++;
	}
	*dp = '\0';
}

