: IMMEDIATE  lw @ cfa 1 - dup c@ 1 or swap c! ;
: over >r dup r> swap ;
: 2dup over over ;

: if ' 0branch , here 0  , ; IMMEDIATE
: else ' branch , here 0 , swap here swap !  ; IMMEDIATE
: then here swap ! ; IMMEDIATE
: endif ' then execute ; IMMEDIATE

: repeat here ; IMMEDIATE
: until  ' 0branch , , ; IMMEDIATE

: do  ' swap , ' >r , ' >r ,  here ; IMMEDIATE

: loop 
        ' r> , 
        ' lit , 1 , 
        ' + , 
        ' dup ,     
        ' r@ , 
        ' < , 
        ' not , 
        '  swap , 
        ' >r , 
        ' 0branch , ,
        ' r> , 
        ' drop ,
        ' r> , 
        ' drop ,
 ;  IMMEDIATE

: for 
      ' swap ,
      ' >r , 
      ' >r , 
here  ' r> , 
      ' r> , 
      ' 2dup , 
      ' >r , 
      ' >r , 
      ' < ,  
      ' 0branch ,  
here    0 , 
       swap ; IMMEDIATE
	   
: endfor 
      ' r> , 
      ' lit , 1 ,   
        ' + , 
       ' >r , 
   ' branch , 
            ,  here swap ! 
       ' r> , 
     ' drop , 
       ' r> , 
     ' drop ,  
	 
;  IMMEDIATE
