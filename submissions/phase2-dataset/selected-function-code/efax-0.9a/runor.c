int runor ( short *a, int na, short *b, int nb, short *c, int *pels )
{
  register short la, lb ;
  int ia, ib, ic, np=0 ;
  short tmp [ MAXRUNS ] ;

  if ( ! c ) c = tmp ;

  la = a [ ia = 0 ] ;
  lb = b [ ib = 0 ] ;
  c [ ic = 0 ] = 0 ;

  while ( 1 ) {
    if ( la <= lb ) {			  /* select shorter sub-run */
      if ( ( ( ia | ib ) ^ ic ) & 1 )	  /* OR of subruns same colour as c? */
	c [ ++ic ] = la ;		  /* no, new output run */
      else 
	c [ ic ] += la ;		  /* yes, add it */
      lb -= la ;			  /* align subruns */
      if ( ++ia >= na ) break ;		  /* done */
      la = a [ ia ] ;			  /* get new subrun */
    } else {				  /* same for line b ... */
      if ( ( ( ia | ib ) ^ ic ) & 1 ) 
	c [ ++ic ] = lb ;
      else 
	c [ ic ] += lb ;
      la -= lb ;
      if ( ++ib >= nb ) break ;
      lb = b [ ib ] ;
    }
  }

  if ( ia < na )
    while ( 1 ) {
      if ( ( ia ^ ic ) & 1 )	  
	c [ ++ic ] = la ;		  
      else 
	c [ ic ] += la ;		  
      if ( ++ia >= na ) break ;		  
      la = a [ ia ] ;			  
    } 
  else
    while ( 1 ) {
      if ( ( ib ^ ic ) & 1 ) 
	c [ ++ic ] = lb ;
      else 
	c [ ic ] += lb ;
      if ( ++ib >= nb ) break ;
      lb = b [ ib ] ;
    }

  if ( c == tmp ) for ( ia=0 ; ia <= ic ; ia++ ) np += a[ia] = c[ia] ;

  if ( pels ) *pels = np ;

  return ic + 1 ;
}  
