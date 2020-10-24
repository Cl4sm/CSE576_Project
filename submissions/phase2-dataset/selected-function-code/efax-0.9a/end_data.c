{
  int err=0, c ;
  uchar *p ;
  long dt, draintime ;

  if ( ! ppm ) p = DLE_ETX ;
  else if ( ppm == MPS ) p = "\020," ; 
  else if ( ppm == EOM ) p = "\020;" ; 
  else if ( ppm == EOP ) p = "\020." ; 
  else {
    p = "" ;
    err = msg ( "E2 can't happen (end_data)" ) ;
  }

  tput ( mf, p, 2 ) ;

  dt = time(0) - mf->start ;
				/* time to drain buffers + 100% + 4s */
  draintime = ( 2 * ( mf->bytes / cps[ session[BR] ] + 1 - dt ) + 4 ) * 10 ;
  draintime = draintime < TO_DRAIN_D ? TO_DRAIN_D : draintime ;

  c = ckcmd ( mf, 0, 0, (int) draintime, OK ) ;

  if ( good ) *good = ( c == OK ) ? 1 : 0 ;

  dt = time(0) - mf->start ;

  msg ( "Isent %d+%d lines, %d+%d bytes, %d s  %d bps" , 
       HDRSPCE, mf->lines-HDRSPCE, 
       mf->bytes-mf->pad, mf->pad, (int) dt, (mf->bytes*8)/dt ) ;

  if ( mf->bytes / (dt+1) > cps[session[BR]] )
    msg ( "E flow control did not work" ) ;

  if ( ! err ) err = ttymode ( mf, COMMAND ) ;

  return err ;
}
