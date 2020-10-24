{ 
  int err=0, c, i=0, maxrunl=0, runl=0 ;
  
  ckcmd ( f, &err, s, T2, CONNECT ) ;
  
  if ( ! err ) {

    for ( i=0 ; ( c = tgetd ( f, T3S ) ) >= 0 ; i++ )
      if ( c ) {
   	if ( runl > maxrunl ) maxrunl = runl ;
   	runl = 0 ;
      } else {
   	runl ++ ;
      }
    
    if ( c == EOF )
      err = msg ( "E3timed out during training check data" ) ;
    else
      ckcmd ( f, &err, 0, TO_RTCMD, NO ) ;
    
  }
     
  if ( runl > maxrunl ) maxrunl = runl ;
     
  if ( good ) *good = !err && maxrunl > n ;

  if ( !err ) {
    msg ( "I- received TCF - channel check (%sOK: run of %d in %d)", 
   	 maxrunl > n ? "" : "not ", maxrunl, i ) ;
  }

  return err ;
}
