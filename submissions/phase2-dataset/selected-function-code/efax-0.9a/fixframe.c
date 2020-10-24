{
  int i;

  if ( *buf == 0xc0 || *buf == 0xc8 ) {
    for ( i=n; i >= 1 ; i-- ) 
      buf[i]=buf[i-1] ;
    buf[i] = 0xff ;
    msg ("W HDLC frame missing initial 0xff" ) ;
    n++ ;
  }

  if ( buf[1] == 0x03 || buf[1] == 0x13 ) {
    for ( i=0 ; i < n ; i++ ) 
      buf[i]=normalbits[buf[i]] ;
    msg ("W bit-reversed HDLC frame, reversing bit order" ) ;
    f->ibitorder = f->ibitorder == normalbits ? reversebits : normalbits ;
  }

  return n ;
}
