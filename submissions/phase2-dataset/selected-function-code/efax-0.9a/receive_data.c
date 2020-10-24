int receive_data ( TFILE *mf, OFILE *f, cap session, int *nerr )
{
  int err=0, line, lines, nr, len ;
  int pwidth = pagewidth [ session [ WD ] ] ;
  short runs [ MAXRUNS ] ;
  DECODER d ;

  if ( ! f || ! f->f ) {
    msg ( "E2 can't happen (writeline)" ) ;
  } 
  
  newDECODER ( &d ) ;

  lines=0 ; 
  for ( line=0 ; ( nr = readfaxruns ( mf, &d, runs, &len ) ) >= 0 ; line++ ) {
    if ( nr > 0 && len > 0 && line ) { /* skip first line+EOL and RTC */
      if ( len != pwidth ) { 
	(*nerr)++ ;
	if ( *nerr <= MAXERRPRT ) msg ("R-+ (%d:%d)", line, len ) ;
	nr = xpad ( runs, nr, pwidth - len ) ;
      } 
      writeline ( f, runs, nr, 1 ) ;
      lines++ ;
    }
    if ( ferror ( f->f ) ) {
      err = msg ("ES2file write:") ;
      tput ( mf, CAN_STR, 1 ) ;
      msg ("Wdata reception CANcelled") ;
    } 
  }
  
  if ( *nerr ) {
    if ( *nerr > MAXERRPRT ) msg ("R-+ ....." ) ;
    msg ("R-  : reception errors" ) ;
    msg ("W- %d reception errors", *nerr ) ;
  }

  if ( nr == EOF ) { 
    while ( tgetd ( mf, TO_CHAR ) >= 0 ) ; /* got RTC, wait for DLE-ETX */
  } else {
    err = 1 ;			/* DLE-ETX without RTC - should try again */
  }

  msg ( "I- received %d lines, %d errors", lines, *nerr ) ;

  return err ;
}
