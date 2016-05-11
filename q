RAND(3)                    Linux Programmer's Manual                   RAND(3)



NNAAMMEE
       rand, rand_r, srand - pseudo-random number generator

SSYYNNOOPPSSIISS
       ##iinncclluuddee <<ssttddlliibb..hh>>

       iinntt rraanndd((vvooiidd));;

       iinntt rraanndd__rr((uunnssiiggnneedd iinntt **_s_e_e_d_p));;

       vvooiidd ssrraanndd((uunnssiiggnneedd iinntt _s_e_e_d));;

   Feature Test Macro Requirements for glibc (see ffeeaattuurree__tteesstt__mmaaccrrooss(7)):

       rraanndd__rr(): _POSIX_C_SOURCE >= 1 || _XOPEN_SOURCE || _POSIX_SOURCE

DDEESSCCRRIIPPTTIIOONN
       The  rraanndd()  function returns a pseudo-random integer in the range 0 to
       RRAANNDD__MMAAXX inclusive (i.e., the mathematical range [0, RRAANNDD__MMAAXX]).

       The ssrraanndd() function sets its argument as the seed for a  new  sequence
       of  pseudo-random  integers  to be returned by rraanndd().  These sequences
       are repeatable by calling ssrraanndd() with the same seed value.

       If no seed value is provided,  the  rraanndd()  function  is  automatically
       seeded with a value of 1.

       The function rraanndd() is not reentrant or thread-safe, since it uses hid‐
       den state that is modified on each call.  This might just be  the  seed
       value to be used by the next call, or it might be something more elabo‐
       rate.  In order to get reproducible behavior in a threaded application,
       this  state must be made explicit; this can be done using the reentrant
       function rraanndd__rr().

       Like rraanndd(), rraanndd__rr() returns a  pseudo-random  integer  in  the  range
       [0, RRAANNDD__MMAAXX].  The _s_e_e_d_p argument is a pointer to an _u_n_s_i_g_n_e_d _i_n_t that
       is used to store state between calls.  If rraanndd__rr() is called  with  the
       same  initial value for the integer pointed to by _s_e_e_d_p, and that value
       is not modified between calls, then  the  same  pseudo-random  sequence
       will result.

       The  value pointed to by the _s_e_e_d_p argument of rraanndd__rr() provides only a
       very small amount of state, so this function will be a weak pseudo-ran‐
       dom generator.  Try ddrraanndd4488__rr(3) instead.

RREETTUURRNN VVAALLUUEE
       The rraanndd() and rraanndd__rr() functions return a value between 0 and RRAANNDD__MMAAXX
       (inclusive).  The ssrraanndd() function returns no value.

CCOONNFFOORRMMIINNGG TTOO
       The functions rraanndd() and ssrraanndd() conform to  SVr4,  4.3BSD,  C89,  C99,
       POSIX.1-2001.     The   function   rraanndd__rr()   is   from   POSIX.1-2001.
       POSIX.1-2008 marks rraanndd__rr() as obsolete.

NNOOTTEESS
       The versions of rraanndd() and ssrraanndd() in the Linux C Library use the  same
       random number generator as rraannddoomm(3) and ssrraannddoomm(3), so the lower-order
       bits should be as random as the higher-order bits.  However,  on  older
       rraanndd()  implementations,  and  on  current implementations on different
       systems, the lower-order bits are much less  random  than  the  higher-
       order  bits.   Do  not use this function in applications intended to be
       portable when good randomness is needed.  (Use rraannddoomm(3) instead.)

EEXXAAMMPPLLEE
       POSIX.1-2001 gives the following example of an implementation of rraanndd()
       and  ssrraanndd(),  possibly  useful when one needs the same sequence on two
       different machines.

           static unsigned long next = 1;

           /* RAND_MAX assumed to be 32767 */
           int myrand(void) {
               next = next * 1103515245 + 12345;
               return((unsigned)(next/65536) % 32768);
           }

           void mysrand(unsigned seed) {
               next = seed;
           }

       The following program can be used to display the pseudo-random sequence
       produced by rraanndd() when given a particular seed.

           #include <stdlib.h>
           #include <stdio.h>

           int
           main(int argc, char *argv[])
           {
               int j, r, nloops;
               unsigned int seed;

               if (argc != 3) {
                   fprintf(stderr, "Usage: %s <seed> <nloops>\n", argv[0]);
                   exit(EXIT_FAILURE);
               }

               seed = atoi(argv[1]);
               nloops = atoi(argv[2]);

               srand(seed);
               for (j = 0; j < nloops; j++) {
                   r =  rand();
                   printf("%d\n", r);
               }

               exit(EXIT_SUCCESS);
           }

SSEEEE AALLSSOO
       ddrraanndd4488(3), rraannddoomm(3)

CCOOLLOOPPHHOONN
       This  page  is  part of release 3.54 of the Linux _m_a_n_-_p_a_g_e_s project.  A
       description of the project, and information about reporting  bugs,  can
       be found at http://www.kernel.org/doc/man-pages/.



                                  2010-10-01                           RAND(3)
