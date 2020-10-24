{
  int err=0, hsc=-1 ;
  char c, dsbuf [ 128 ], *p ;

  sprintf ( dsbuf, nowait ? "D%.126s;" : "D%.127s" , s ) ;
  msg ( "Idialing %s", dsbuf+1 ) ;

  c = cmd ( f, dsbuf, TO_A ) ;

  if ( ( p = sresponse ( "+FCSI:", 0 ) ) != 0 ||
       ( p =  sresponse ( "+FCI:", 0 ) ) != 0 ) {
    msg ( "I- remote ID -> %s", p ) ;
  }

  if ( nowait && c == OK ) {
    msg ( "Icalled" ) ;
    nframes = 1 ;
  } else if ( c1 && c == CONNECT ) {
    msg ( "Iconnected" ) ; 
    nframes = 0 ;
  } else if ( !c1 && c == OK ) {
    msg ( "Iconnected" ) ; 
  } else if ( c ==  BUSY ) {
    err = msg ( "W1number is busy" ) ; 
  } else {
    err = msg ( "E2dial command failed" ) ;
  }

  gethsc ( &hsc, err ? 0 : &err ) ;

  return err ;
}
