int putframe ( int type, uchar *buf, int len, TFILE *f, int t )
{
  int err=0 ;

  buf [ 0 ] = 0xff ;
  buf [ 1 ] = type & MORE_FR ? 0xc0 : 0xc8 ;
  buf [ 2 ] = type & 0xff ;

  if ( nframes++ && ! ( type & SUB_FR ) )
    ckcmd ( f, &err, "+FTH=3" , TO_FT, CONNECT ) ;
  
  if ( ! err ) {

    if ( ! buf[len+2] ) {
      msg ( "Wlast byte of frame is NULL" ) ;
    }

    /* ttymode ( f, SEND ) ; */
    sendbuf ( f, buf, len+3, 0 ) ;
    tput ( f, DLE_ETX, 2 ) ; 
    /* ttymode ( f, COMMAND ) ; */

    logfr ( "sent", frname ( buf [ 2 ] ), buf, len+3 ) ;

    ckcmd ( f, &err, 0, TO_DRAIN_H, ( type & MORE_FR ) ? CONNECT : OK ) ;
  }

  return err ;
}
