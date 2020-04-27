# CPP tricks

The C Preprocessor is very mighty.

This here basically implements a `NOTEMPTY` macro and on top of that a `FOR` macro,
which is able to iterate over a variable number of arguments.

> This is based on the `EVAL` feature, which is very CPU intensitive!

Note that this is based on ideas presented elsewhere.  Please see comments for details.

You probably need a modern C18 `GCC`, as ellipsis variables are used heavily.


## Usage

	git submodule add https://github.com/hilbix/cpptricks.git
	ln -s --relative cpptricks/cpptricks.h h/

In your source then:

	#include "h/cpptricks.h"


## Example

```
/* A,B,C,D,..   => A B , C D , ..       */
#define Otwo(A...)              EVAL(FOR(Otwo,A))
#define Otwo_0(A,B,C...)
#define Otwo_1(A,B,C...)        A B
#define Otwo_2(A,B,C...)        C
#define Otwo_3(...)             ,
#define Otwo_4()

/* A,B,C,D,..   => B , D , ..   */
#define O2nd(A...)              EVAL(FOR(O2nd,A))
#define O2nd_0(A,B,C...)
#define O2nd_1(A,B,C...)        B
#define O2nd_2(A,B,C...)        C
#define O2nd_3(...)             ,
#define O2nd_4()

/* Create our own checked wrappers from the prototypes
 * void Ofunctionname()
 * has the same calling signature as
 * int functioname()
 * but OOPSes when an error occurs.
 */
#define O(A,B...)       static void O##A(Otwo(B)) { if (A(O2nd(B))) OOPS(#A); }
```

You can use this to create some standard wrapper routines,
which stay very closely to the real function definition prototype:

```
int sigemptyset (sigset_t * set);
O(  sigemptyset, sigset_t *,set);
```

```
int sigaction( int signum, const struct sigaction * act, struct sigaction * oldact);
O(  sigaction, int,signum, const struct sigaction *,act, struct sigaction *,oldact);
```

For this all you have to do is to stick to the `type,name` idiom.  `typedef` is your friend.


## FAQ

WTF why?

- Because I needed it.

License?

- My part of the code is free as in free beer, free speech and free baby.
- But I'm not sure about the code references used to assemble these sources.

Standard?

- No standard.  Probably C18.  Perhaps GCC only.

Contact?

- Open Issue on GH
- Eventually I listen

Contrib?

- Send PR on GH
- Eventually I listen
- Waive all Copyright, else I will not pull!

Names?

- Warning!  The names might change to some better scheme
- However currently I have no good idea
- `git` is your friend, `submodules` can keep it compatible.

