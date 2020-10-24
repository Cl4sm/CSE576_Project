int receive_frame_data ( TFILE *f, uchar *buf, int n, int *len )
{
  int err=0, c, i ;

  for ( i=0 ; ( c = tgetd ( f, T3S ) ) >= 0  ; i++ )
    if ( i < n ) buf[ i ] = c ;
  
  if ( c == EOF ) {

    err = msg ( "E3timed out reading frame data" ) ;

  } else {
    
    switch ( cmd ( f, 0, TO_RTCMD ) ) {
    case OK:
    case CONNECT:
      break ;
    case ERROR:
    case NO:
      err = msg ( "W1frame error" ) ;
      break ;
    case EOF:
      err = msg ( "E3no response after frame data" ) ;
      break ;
    default:
      err = msg ( "E3wrong response after frame data" ) ;
      break ;
    }

  }

  if ( i >= n ) 
    err = msg ( "E3frame too long (%d, > %d max bytes)", i, n ) ;
  
  if ( len ) *len = i ;

  return err ;
}
