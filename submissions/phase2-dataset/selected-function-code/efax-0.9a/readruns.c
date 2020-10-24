int readruns ( IFILE *f, short *runs, int *pels )
{
  int err=0, c=EOF, n ;
  register int x ;
  dtab *tab, *t ;
  short shift ;
  short *p, *maxp, *q, len=0, npad=0 ;
  DECODER *d ;
  uchar reverse=f->page->revbits ;

  maxp = ( p = runs ) + MAXRUNS ;
  d = &f->d ;

  x = d->x ; shift = d->shift ; tab = d->tab ; /* restore decoder state */

  do {
    do {
      while ( shift < 0 ) { 
	if ( ( c = fgetc ( f->f ) ) == EOF )  {
	  x = ( x << 15 ) | 1 ; shift += 15 ;  /* EOL pad at EOF */
	  npad++ ;
	} else {
	  if ( reverse ) c = normalbits [ c & 0xff ] ;
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

  if ( npad > 1 ) msg ("W EOF before RTC" ) ;

  if ( p >= maxp ) msg ( "W run length buffer overflow" ) ;

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

  if ( c == EOF ) {
    if ( ferror ( f->f ) ) {
      err = -msg ("ES2error reading fax file:") ;
    } else { 
      err = -2 ;
    }
  }

  if ( pels ) *pels = len ;
  
  return err ? err : n ;
}
