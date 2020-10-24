int modemsync ( TFILE *f )
{
  int err=0, method=0 ;

  for ( method=0 ; ! err ; method++ ) {
    switch ( method ) {
    case 0 : 
      break ;
    case 1 :
      ttymode ( f, VOICECOMMAND ) ;
      break ;
    case 2 : 
      msg ("Isync: dropping DTR") ;
      ttymode ( f, COMMAND ) ; msleep ( 200 ) ;
      ttymode ( f, DROPDTR ) ; msleep ( 200 ) ;
      ttymode ( f, COMMAND ) ; 
      break ;
    case 3 :
      msg ("Isync: sending escapes") ;
      ttymode ( f, VOICECOMMAND ) ;
      tput ( f, CAN_STR, 1 ) ;
      tput ( f, DLE_ETX, 2 ) ; 
      msleep ( 100 ) ;
      ttymode ( f, COMMAND ) ;
      tput ( f, CAN_STR, 1 ) ;
      tput ( f, DLE_ETX, 2 ) ; 
      msleep ( 1500 ) ;
      tput ( f, "+++", 3 ) ; 
      break ;
    case 4 :
      err = msg ("E4sync: modem not responding") ;
      continue ;
    }
    while ( method && cmd ( f, 0, 20 ) != EOF ) ;
    if ( cmd ( f, "Q0V1", -20 ) == OK ) break ;
  }
  return err ;
} 
