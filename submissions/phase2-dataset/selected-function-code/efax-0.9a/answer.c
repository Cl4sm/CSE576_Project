	    int wait, int share, int softaa, 
	    char *getty, char *vcmd, char *acmd )
{
  int err=0, c ;
  int crate=19200, hsc=-1, i ;
  enum connectmode mode=NONE ;

  if ( ! err && share ) {
    err = ttymode ( f, COMMAND ) ;
    if ( ! err ) 
      err = unlockall ( lkfile ) ;
  }
  
  if ( ! err && wait ) {
    msg ( "Iwaiting for activity") ;
    tdata ( f, -1 ) ;
    msg ( "Iactivity detected") ;
  }
  
  if ( ! err && share ) {
    msleep ( 500 ) ;		/* let other programs lock port  */
    err = lockall ( lkfile, 1 ) ;
    if ( err )
      err = msg ( "W1can't answer: can't lock device" ) ;
    else
      err = ttymode ( f, COMMAND ) ; /* in case it was changed silently */
  }

  for ( i=0 ; ! err && mode == NONE && ( i==0 || ( i==1 && softaa ) ) ; i++ ) {

    c = cmd ( f, wait ? 0 : acmd, ( i==0 && softaa ) ? TO_DATAF : TO_A ) ;

    if ( c == DATA ) cmd ( f, c1 ? "O" : 0, TO_A ) ; /* +FAE=1 weirdness */

    mode = ansmode ( &crate, &hsc ) ;
    
    switch ( mode ) {
    case DATAMODE :
      msg ( "Idata call answered") ;
      if ( getty && *getty ) {
	char buf [ MAXGETTY ] ;
	if ( ckfmt ( getty, 6 ) ) {
	  err = msg ( "E3 too many %%d escapes in command (%s)", getty ) ;
	} else {
	  sprintf ( buf, getty, crate, crate, crate, crate, crate, crate ) ;
	  msg ( "Iexec'ing /bin/sh -c \"%s\"" , buf ) ;
	  execl ( "/bin/sh" , "sh" , "-c" , buf , (void*) 0 ) ; 
	  err = msg ( "ES2exec failed:" ) ;
	}
      } else {
	err = msg ( "E2no getty command defined for data call" ) ;
      }
      break ; 
    case FAXMODE :
      nframes = 0 ;
      msg ( "Ifax call answered") ;
      break ;
    case VOICEMODE :
      msg ( "Ivoice call answered") ;
      if ( vcmd && *vcmd ) {
	char buf [ MAXGETTY ] ;
	if ( ckfmt ( vcmd, 6 ) ) {
	} else {
	  sprintf ( buf, vcmd, f->fd, f->fd, f->fd, f->fd, f->fd, f->fd ) ;
	  msg ( "Iexec'ing /bin/sh -c \"%s\"" , buf ) ;
	  execl ( "/bin/sh" , "sh" , "-c" , buf , (void*) 0 ) ; 
	  err = msg ( "ES2exec failed:" ) ;
	}
      } else {
	err = msg ( "E2no voice command defined for voice call" ) ;
      }
      break ; 
    case NONE:
      if ( i==0 && softaa && hsc < 0 && getty && *getty ) {
	int j ;			/* switch to fax for 2nd try */
	for ( j=0 ; j<3 ; j++ ) 
	  if ( cmd ( f, c1 ? "+FCLASS=1" : 
		    ( c20 ? "+FCLASS=2.0" : "+FCLASS=2" ), -TO_RESET ) 
	      == OK ) break ; 
	wait = 0 ;
	acmd = ANSCMD ;
      } else {
	err = msg ( "E3unable to answer call") ;
      }
      break ;
    default:
      err = msg ( "E3can't happen(answer)" ) ;
      break ;
    }
    
  }

  return err  ;
}
