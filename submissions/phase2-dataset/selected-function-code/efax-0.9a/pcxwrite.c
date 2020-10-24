{
  int c, n, runc ;
  FILE *f = of->f ;

  runc = *p++ ;
  n = 1 ;

  for ( nb-- ; nb > 0 ; nb-- ) {
    c = *p++ ;
    if ( c == runc && n < 63 ) { /* continue run */
      n++ ;
    } else {		/* terminate run */
      if ( n > 1 || ( ( runc & 0xc0 ) == 0xc0 ) ) /* output as run */
	putc ( n | 0xc0, f ) ;
      putc ( runc, f ) ;
      runc = c ;	/* start new run */
      n = 1 ;
    }
  }

  /* last run */

  if ( n > 1 || ( ( runc & 0xc0 ) == 0xc0 ) ) /* output as run */
    putc ( n | 0xc0, f ) ;
  putc ( runc, f ) ;

}
