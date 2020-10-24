		    char **lkfile, ttymodes mode, void (*onsig) (int) )
{
  int i, err=0, busy=0, minbusy=0 ;

  do {
    err = lockall ( lkfile, busy >= minbusy ) ;
    if ( ! err ) err = ttyopen ( f, fname, reverse, hwfc ) ;
    if ( err == 1 ) { 
      if ( busy++ >= minbusy ) {
	msg ( "W %s locked or busy. waiting.", fname ) ;
	minbusy = minbusy ? minbusy*2 : 1 ;
      }
      msleep ( lockpolldelay ) ;
    }
  } while ( err == 1 ) ;
  
  if ( ! err ) msg ( "Iopened %s", fname ) ;

  if ( ! err ) err = ttymode ( f, mode ) ;

  if ( ! err ) {
    rd_init ( ) ;
    f->rd_state = RD_BEGIN ;
  }
  
  for ( i=0 ; ! err && catch [ i ] ; i++ ) 
    if ( signal ( catch [ i ], onsig ) == SIG_ERR ) 
      err = msg ( "ES2can't set signal %d handler:", catch [ i ] ) ;
  
  if ( !err ) err = modemsync ( f ) ;

  return err ;
}
