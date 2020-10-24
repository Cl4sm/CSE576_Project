int getfr ( TFILE *mf, uchar *buf, int getcmd )
{
  int err=0, frame=0, frlen, c, t ;
  char remoteid [ IDLEN + 1 ] ;
  time_t start ;
  uchar *fif=buf+3 ;
  
  start = 10*time(0) ;
  
  t = getcmd ? ( getcmd > 1 ? getcmd : T2 ) : T4 ;

 Enter:

  err = 0 ;

  if ( nframes++ ) {
    c = cmd ( mf, "+FRH=3", t ) ;
  } else {
    c = CONNECT ;		/* implied by ATA or ATD */
  }
  
  switch ( c ) {
  case EOF:			/* time out */
    tput ( mf, CAN_STR, 1 ) ;
    ckcmd ( mf, 0, 0, TO_ABRT, OK ) ;
    err = 1 ;
    break ;
  case NO:			/* S7 time out */
    err = 1 ;
    break ;
  case MODULATION:		/* data carrier (or DHS) */
    return -msg ( "W-2 wrong carrier" ) ;
    break ;
  case CONNECT:			/* frame */
    break ;
  default:			/* shouldn't happen */
    err = msg ( "E3wrong response to receive-frame command" ) ;
    break ;
  }
  
  if ( ! err ) 
    err = receive_frame_data ( mf, buf, MAXFRLEN, &frlen ) ;
  
  if ( ! err && frlen < 3 ) 
    err = msg ( "E3received short frame (%d bytes)", frlen ) ;

  logfr ( "received", frname ( buf [ 2 ] ), buf, frlen ) ;

  if ( ! err ) {

    frlen = fixframe ( buf, frlen, mf ) ;
    frame = buf [ 2 ] & 0x7f ;

    switch ( frame ) {
    case CRP:
      err = 1 ;
    case NSF:
    case NSC:
    case NSS:
      goto Enter ;
    case CIG:
    case CSI:
    case TSI:
      revcpy ( fif , (uchar*) remoteid ) ;
      msg ( "I- remote ID -> %*.*s", IDLEN, IDLEN, remoteid ) ;
      goto Enter ;
    }

  }
  
  if ( err && getcmd && ( t -= 10*time(0) - start ) > 0 ) 
    goto Enter ;

  return err ? EOF : frame ;
}
