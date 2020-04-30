/* The code in [[[ .. ]]] was taken over from ideas presented in
 * https://github.com/pfultz2/Cloak/wiki/C-Preprocessor-tricks,-tips,-and-idioms
 *
 * For a better understanding I kept the names near the original there.
 *
 * The rest in this file either is extremely trivial
 * or it is not based on  some days of experimentation.
 */

#define	EVAL(A...)	EVAL1(EVAL1(EVAL1(EVAL1(A))))
#define	EVAL1(A...)	EVAL2(EVAL2(EVAL2(EVAL2(A))))
#define	EVAL2(A...)	EVAL3(EVAL3(EVAL3(EVAL3(A))))
#define	EVAL3(A...)	EVAL4(EVAL4(EVAL4(EVAL4(A))))
#define	EVAL4(A...)	EVAL5(EVAL5(EVAL5(EVAL5(A))))
/* extend here if not enough	*/
#define	EVAL5(A...)	A

#define	CAT(A...)	CONS(A)
#define	CONS(A,B...)	A ## B
#define	HEAD(A,B...)	A
#define	TAIL(A,B...)	B
#define	SECOND(A,B,...)	B
#define	THIRD(A,B,C,...)	C
#define NOTHING()

/* We do not need HEAD5.  If you do, rather change the design.
 * Humans can immediately count 0,1,2,3 but not 4.  This is especially true for grouping with the eye.
 * Stopping at 4 allows us to immediately detect 4 as "something else than 0,1,2,3".
 */
#define	HEAD0(A...)
#define	TAIL0(A...)	A
#define	HEAD1(A,B...)	A
#define	TAIL1(A,B...)	B
#define	HEAD2(A,B,C...)	A,B
#define	TAIL2(A,B,C...)	C
#define	HEAD3(A,B,C,D...)	A,B,C
#define	TAIL3(A,B,C,D...)	D
#define	HEAD4(A,B,C,D,E...)	A,B,C,D
#define	TAIL4(A,B,C,D,E...)	E


/* [[[ */

/* 0 if CHECK(something) is not PROBE()
 * 1 if something expands to PROBE()
 * as long as something is normally free of any comma
 */
#define	CHECK(A...)	SECOND(A,0,)
#define	PROBE(A)	A,1,

/* Check for A starting with some paired parenteses
 * ()
 * Nice find there, might come useful sometime
 */
#define	PAREN(A)	CHECK(PAREN_ A)
#define	PAREN_(...)	PROBE()

/* Inverted match: gives 0 for any A which is not 0.  For A==0 it gives 1
 */
#define	NOT(A)		CHECK(CONS(NOT_,A))
#define	NOT_0		PROBE(~)

#define DEFER(A)	A NOTHING()
#define OBSTRUCT(A)	A DEFER(NOTHING)()

/* ]]] */

#define	IF(A)		CAT(IF_,BOOL(A))
#define	IF_1(A,B...)	A
#define	IF_0(A,B...)	B

#define	BOOL(A)		INV(NOT(A))	/* inverse of NOT	*/

#define	INV(A)		CONS(INV_,A)	/* like NOT, but A must be 0 or 1	*/
#define	INV_0		1
#define	INV_1		0

#define	WHEN(A)		IF(A)(TAIL0,HEAD0)	/* quick IF	*/
#define	UNLESS(A)	IF(A)(HEAD0,TAIL0)	/* quick ELSE	*/

/* Create a FOR loop with an auxiliary FN: (called FORGE for now)
 *
 * WARNING, THIS IS DEAD SLOW due to the EVAL, DO NOT USE EXTENSIVELY!
 * Also a bigger argument count might not evaluate,
 * in that case exted the EVALs above (makes it even slower).
 *
 * EVAL(FORGE(FN,ARGS...)) expands to:
 *
 * - Nothing, if ARGS... is empty, else:
 * - on the 1st loop: FN_0(ARGS...) FN_1(ARGS...) FORGE(FN,FN_2(ARGS))
 * - on the 2nd loop: FN_3(ARGS...) FN_1(ARGS...) FORGE(FN,FN_2(ARGS))
 * - end of nonempty: FN_4()
 *
 * Pseudo Shell-like Code:
 *
 * if a; then
 *	FN_0(a);		// output prefix
 *	while	FN_1(a);	// output processing
 *		a = FN_2(a);	// do the iteration
 *	do
 *		FN_3(a);	// output separator
 *	done;
 *	FN_4();			// output suffix
 * fi;
 *
 * Example to process 2 arguments each:
 *
 * #define FN_0(...)		(		// output prefix
 * #define FN_1(A,B,C...)	A B		// output 2 arguments without comma
 * #define FN_2(A,B,C...)	C		// skip 2 arguments on each iteration
 * #define FN_3(...)		,		// separator in between
 * #define FN_4(...)		)		// output suffix
 *
 * EVAL(FORGE(FN, a1,b1, a2,b2))
 */
#define FORGE(FN,A...)		FORGE_(FN,0,A)
#define FORGE_(FN,N,A...)	IF(NOTEMPTY(A))(FORGE_1,FORGE_##N)(FN,N,A)()(FN,3,IF(NOTEMPTY(A))(FN##_2,HEAD0)(A))
#define FORGE_0(FN,N,A...)	HEAD0
#define FORGE_1(FN,N,A...)	FN##_##N(A) FN##_1(A) OBSTRUCT(FORGE_2)
#define FORGE_2()		FORGE_
#define FORGE_3(FN,N,A...)	FN##_4() OBSTRUCT(FORGE_4)
#define	FORGE_4()		HEAD0

/* Check for nonempty arg "A..." as follows:
 *	If "A..." is "X", expand it to "0,X"
 *	If "A..." is "", expand it to just "0"
 * Put something behind which cannot be expanded:
 *	0,X	=>	0,X _
 *	0	=>	0 _
 * Now take the first arg (hence this does not contain a ","):
 *	0,X _	=>	0
 *	0 _	=>	0 _
 * With the help of a macro check if it can be expanded to PROBE():
 *	check0__0()	=>	PROBE()
 *	check0__0 _()	=>	check0__0 _()
 * Finally CHECK() for PROBE()
 */
#define	NOTEMPTY(A...)	CHECK0(NOTEMPTY_(A)CHECK0__)
#define	NOTEMPTY_(A...)	0,##A
#define	CHECK0(A...)	CHECK(CHECK0_(A)())
#define	CHECK0_(A,B...)	CONS(CHECK0__,A)
#define	CHECK0__0()	PROBE()

#define	EMPTY(A...)	NOT(NOTEMPTY(A))


#if 0

/* Following is probably wrong, DO NOT USE, leftovers from experiments
 *
 * Not ready yet:  Check for COMMA (COMMA(args) <=> 1: args start with ',')
 */


#define	PROBE0(X,A...)		PROBE X ()	/* ==0 arguments */
#define	PROBE1(A...)		PROBE2(1,##A)	/* >=1 arguments */
#define	PROBE2(A,B,...)		PROBE()		/* >=2 arguments */
#define	PROBE3(A,B,C,...)	PROBE()		/* >=3 arguments */

#define	ONE(A...)	CHECK(PROBE1(A))
#define	TWO(A...)	CHECK(PROBE2(A))
#define	THREE(A...)	CHECK(PROBE3(A))

#define	COMMA(A...)	CONS(COMMA_,CHECK(PROBE2(A)))(A)
#define	COMMA_0(A...)	0
#define	COMMA_1(A,B...)	EMPTY(A)

#endif

