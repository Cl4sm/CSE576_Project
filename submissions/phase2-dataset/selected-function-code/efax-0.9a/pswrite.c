{
  int i, j, l ;
  static unsigned char last [ MAXBITS ] ;
  
  l=i=0 ;

  if ( ! f || ! buf || n<0 ) {
    msg ( "E2 can't happen (pswrite)" ) ;
    return ;
  }

  for ( j=0 ; j<n && buf[j]==last[j] && f->pslines ; j++ ) ;
  if ( j == n ) {		/* repeat line */
    hexputc ( f->f, 0 ) ;
    l=i=n ;
  }

  while ( i<n ) {

    for ( j=i ; j<n && buf[j]==last[j] && j-i<127 && f->pslines ; j++ ) ;
    if ( j-i > 2 ) {		/* skip */
      hexputs ( f->f, buf+l, i-l ) ;
      hexputc ( f->f, j-i + 127 ) ; 
      l=i=j ;
    } else {
      for ( j=i ; j<n && buf[j]==buf[i] && j-i<255 ; j++ ) ;
      if ( j-i > 4 ) {		/* run */
	hexputs ( f->f, buf+l, i-l ) ;
	hexputc ( f->f, 255 ) ; 
	hexputc ( f->f, j-i ) ; 
	hexputc ( f->f, buf[i] ^ 0xff ) ;
	l=i=j ;
      } else {
	if ( i-l >= 127 ) {	/* maximum data length */
	  hexputs ( f->f, buf+l, i-l ) ;
	  l=i ;
	} else {		/* data */
	  i++ ;
	}
      }
    }

  }
  hexputs ( f->f, buf+l, i-l ) ;

  if ( n >= 0 ) 
    memcpy ( last, buf, n ) ;

  f->pslines++ ;
}
