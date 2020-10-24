{
  int i, m, err=0 ;
  uchar buf [ MINWRITE ] = { 0 } ;

#ifdef ADDTCFRTC
  ENCODER e ;
  uchar *p ;
#endif

  ckcmd ( f, &err, s, TO_FT, CONNECT ) ;
  
  if ( ! err ) {
    ttymode ( f, SEND ) ;

    /* send n bytes of zeros */

    for ( i=0 ; i < n ; i += m ) {
      m = n-i < MINWRITE ? n-i : MINWRITE ;
      sendbuf ( f, buf, m, 0 ) ;
    }

#ifdef ADDTCFRTC
    /* append RTC in case modem is looking for it */

    newENCODER ( &e ) ;

    p = buf ;
    for ( i=0 ; i < RTCEOL ; i++ )
      p = putcode ( &e, EOLCODE, EOLBITS, p ) ;
    p = putcode ( &e, 0, 0, p ) ;
    sendbuf ( f, buf, p - buf, 0 ) ;
#endif

    tput ( f, DLE_ETX, 2 ) ; 


    ckcmd ( f, &err, 0, TO_DRAIN_D, OK ) ;
    msg ( "I- sent TCF - channel check of %d bytes", n ) ;

    ttymode ( f, COMMAND ) ;
  }
  
  return err ;
}
