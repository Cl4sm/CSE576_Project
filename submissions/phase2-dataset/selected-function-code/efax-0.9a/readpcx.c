{
  int err=0, n, c ;

  while ( !err && len > 0 ) {
    if ( ( c = fgetc ( f->f ) ) < 0 ) {
      err = msg ( "ES2 PCX read failed" ) ;
    } else {
      if ( ( c & 0xc0 ) == 0xc0 ) {	/* a run count */
   	n = c & 0x3f ;
   	c = fgetc ( f->f ) ;
   	if ( ( c = fgetc ( f->f ) ) < 0 ) {
   	  err = msg ( "ES2 PCX read failed" ) ;
   	} else {
   	  memset ( p, c, n <= len ? n : len ) ;
   	  p += n ;
   	  len -= n ;
   	}
      } else {			/* bits 0 to 7 are image data */
	*p++ = c ;
	len-- ;
      }
    }
  }

  if ( len != 0 )
    msg ( "W PCX run-length coding error" ) ;

  return err ;
}
