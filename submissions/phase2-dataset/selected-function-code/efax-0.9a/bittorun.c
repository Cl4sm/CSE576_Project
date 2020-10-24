int bittorun ( uchar *bits, int n, short *runs )
{
  static uchar init=0, *rltab [ 256 ] ;
  register uchar *p, c, lastc = 0x00 ;
  short *runs0 = runs ;

  if ( ! init ) {		/* initialize pointer and run tables */
    int i = 0 ;
    for ( rltab[ 0 ] = p = byteruns ; *p ; p++ )
      if ( ! ( *p -= '0' ) && i < 255 ) 
	rltab [ ++i ] = p+1 ;
    init = 1 ;
  }

  *runs = 0 ;
  for ( ; n > 0 ; n-- ) {
    p = rltab [ c = *bits++ ] ;
    if ( ( lastc & 0x01 ) ? ! ( c & 0x80 ) : ( c & 0x80 ) )
      *(++runs) = *p++ ;		  /* new run */
    else 			  
      *runs += *p++ ;			  /* continue run */
    while ( *p ) 
      *(++runs) = *p++ ;
    lastc = c ;
  }

  return runs - runs0 + 1  ;
}
