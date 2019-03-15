( FIRST PART )

( 1 and 2 - just read)

( 3 TASK )
( words to check if even )
: is_num_even 2 % not ;
: is_num_odd 2 % ;

( 4 TASK)

( words for prime check )
: is_it_one 1 - not ; ( same as 1 = )
: is_less_four 3 - 0 > not ; ( same as 4 < )
: sq dup * ; ( n*n )

( C code for primary check - round brackets are replaces with square )
( int is_prime [unsigned long num] {
	int del;
	if [num <=1] {return 0;}
	if [num <=3] {return 1;}
	if [num % 2] == 0 {return 0;}
	for [del=2; del*del < num; del++] {
		if [num % del == 0] return 0;
	}
	return 1;
} )


( check if prime; ret - 1 if prime, 0 - if not )
: is_num_prime ( n )
    dup ( n n )
    1 > if ( check if it is not 0 or 1 )
      dup 4 < not if ( 2 - is exception - is primary, 3 is primary too )
       dup is_num_odd if ( if even - not primary else - check further )
        2 ( pointer )
        repeat
         1 + ( pointer increment )
	( check according to algorithm ) 
         2dup sq < if drop 1 exit then ( primary)
         2dup % not if drop 0 exit then ( not primary)
	( continue loop ) 0 
        until 
       else 0 then 
     else 1 then 
   else 0 then ;

( 5 TASK )

: is_num_prime_allot 
    is_num_prime
    1 ( byte ) allot ( of memory )
    swap over ( a b -> b a b ) 
    c! dup c@ ." Result " .  ."  Memory cell " . ;

( 6 TASK )

 ( Just reading  )

( 7 TASK )




( SECOND PART )

( m" Rogalenko" string-hash
.
375
m" rogalenko" string-hash
.
61884  )

( VARIANT 0 )

: collatz ( n ) 
   dup . 32 emit ( space )
   repeat
     ( if even - div 2 )
     dup is_num_even if
       2 /
     else
      ( eles - 3n + 1  )
      3 * 1 +
     endif
     dup . 32 emit
    ( if 1 - we finished )
     dup 1 > not
   until
  ( make stack free )
   drop ;

     	















