int mkcap ( uchar *fif, cap c, int dis ) 
{
  int err=0, i, j, k, len ;
  t30tabst *p ;

  len = dislen ( fif ) ;

  for ( i=0 ; i<NCAP ; i++ ) {
    p=t30tab+i ;
    if ( p->byte >= len ) {
      c [ i ] = 0 ;
    } else {
      j = ( fif [ p->byte ] >> p->shift ) & p->mask ;
      k = ( dis ? p->distocap : p->dcstocap ) [ j ] ;
      if ( k == X ) {
	c [ i ] = p->safeval ;
	err = msg("E3mkcap: bad %s field (%d) set to %d", 
		  p->name, j, c [ i ] ) ;
      } else { 
	c [ i ] = k ;
      }
    }
  }
  return err ;
}
