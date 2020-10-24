{
  int err=0, c=EOF, x, n ;
  dtab *tab, *t ;
  short shift ;
  short *p, *maxp, *q, len=0 ;
  uchar rd_state ;

  maxp = ( p = runs ) + MAXRUNS ;

  x = d->x ; shift = d->shift ; tab = d->tab ; /* restore decoder state */
  rd_state = f->rd_state ;

  do {
    do {
      while ( shift < 0 ) { 
	c = tgetd ( f, TO_CHAR ) ;

	rd_state = ( rd_state & rd_allowed[c] ) ?
	  ( ( rd_state & rd_nexts[c] ) ? rd_state <<= 1 : rd_state ) : 
	  RD_BEGIN ;

	if ( rd_state == RD_END )
	  msg ( "W+ modem response in data" ) ; 

	if ( c < 0 )  {
	  x = ( x << 15 ) | 1 ; shift += 15 ;  /* EOL pad at EOF */
	} else {
	  x = ( x <<  8 ) | c ; shift +=  8 ; 
	}
      }
      t = tab + ( ( x >> shift ) & 0x1ff ) ;
      tab = t->next ;
      shift -= t->bits ;
    } while ( ! t->code ) ;
    if ( p < maxp ) *p++ = t->code ;
  } while ( t->code != -1 ) ;

  d->x = x ; d->shift = shift ; d->tab = tab ; /* save state */
  f->rd_state = rd_state ;

  if ( p >= maxp ) msg ( "Wrun length buffer overflow" ) ;

  /* combine make-up and terminating codes and remove +1 offset
     in run lengths */

  n = p - runs - 1 ;
  for ( p = q = runs ; n-- > 0 ; )
    if ( *p > 64 && n-- > 0 ) {
      len += *q++ = p[0] + p[1] - 2 ;
      p+=2 ;
    } else {
      len += *q++ = *p++ - 1 ;
    }
  n = q - runs ;
  
  /* check for RTC and errors */

  if ( len )
    d->eolcnt = 0 ;
  else
    if ( ++(d->eolcnt) >= RTCEOL ) err = EOF ;

  if ( c < 0 ) err = - 2 ;

  if ( pels ) *pels = len ;
  
  return err ? err : n ;
}
