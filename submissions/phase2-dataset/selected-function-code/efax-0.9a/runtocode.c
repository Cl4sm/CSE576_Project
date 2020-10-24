{
  uchar col = 0, *maxcodes = codes + MAXCODES ;
  t4tab *ctab = wtab, *p ;
  short rlen ;
  long x ;
  short shift ;

#define PUTCODE(p) { x = ( x << p->bits ) | p->code ;  shift += p->bits ; \
	while ( shift >= 0 ) { *codes++ = x >> shift ; shift -= 8 ; } }

  x = e->x ; shift = e->shift ;

  while ( nr-- > 0 ) {
    rlen = *runs++ ;
    if ( rlen > 63 ) {				/* make-up code */
      if ( rlen > MAXRUNLEN ) rlen = MAXRUNLEN ;
      p = ctab + 63 + ( rlen >> 6 ) ;
      if ( codes < maxcodes ) PUTCODE(p) ;
    }
    p = ctab + ( rlen & 0x3f ) ;		/* terminating code */
    if ( codes < maxcodes ) PUTCODE(p) ;
    ctab = ( col ^= 1 ) ? btab : wtab ;
  }  
  
  e->x = x ; e->shift = shift ;

  return codes ;
}
