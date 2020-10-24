{
  int i, k ;
  t30tabst *p ;

  len = len > DCSLEN ? DCSLEN : len ;

  fif[0] = 0 ;
  fif[1] = ( isdis && t4tx ? 0x80 : 0 ) | 0x40 ;
  for ( i=2 ; i<len-1 ; i++ ) fif[i] = 0x01 ;       /* add extension bits */
  fif[i] = 0 ;

  checkcap ( c ) ;

  for ( i=0 ; i<NCAP ; i++ ) {
    p = t30tab + i ;
    if ( ( k = ( isdis ? p->captodis : p->captodcs ) [ c [ i ] ] ) == X )
      msg ( "E3mkdis: can't happen (invalid %s)", p->name ), k=0 ;
    if ( p->byte < len ) fif [ p->byte ] |= k << p->shift ;
  }
}
