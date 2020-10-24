char *tgets( TFILE *f, char *s, int len, int t )
{
  int i, n, c ;

  for ( i=n=0 ; 1 ; i++ ) {
    if ( ( c = tgetc ( f, t ) ) == EOF ) break ;
    if ( i == 0 ) msg ( "M-+ .%03d [", time_ms ( ) ) ;
    msg ( "M-+ %s", cname ( c ) ) ;
    if ( n > 0 && c == LF ) break ;
    if ( ! iscntrl ( c ) && n < len ) s[n++] = c ;
  }

  if ( n >= len ) msg ( "W- modem response overflow" ) ;
  s[ n < len ? n : len-1 ] = '\0' ;
  if ( i > 0 ) {
    if ( c == EOF ) msg ( "M- <...%.1f s>]", (float)t/10 ) ;
    else msg ( "M- ]" ) ;
  }

  return c == EOF ? 0 : s ;
}
