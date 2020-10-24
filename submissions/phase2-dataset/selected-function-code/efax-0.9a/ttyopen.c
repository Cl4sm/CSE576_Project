{
  int flags, err=0 ;

  tinit ( f, open ( fname, O_RDWR | O_NDELAY | O_NOCTTY ), reverse, hwfc ) ;

  if ( f->fd < 0 ) {
    if ( errno == EBUSY ) {
      err = 1 ; 
    } else {
      err = msg ( "ES2can't open serial port %s:", fname ) ;
    }
  }

  if ( ! err ) {
    if ( ( flags = fcntl( f->fd, F_GETFL, 0 ) ) < 0 ||
	fcntl( f->fd, F_SETFL, ( flags & ~O_NDELAY ) ) < 0 )
      err = msg ( "ES2fax device fcntl failed %s:", fname ) ;
  }

#ifdef TIOCSSOFTCAR
  { 
    int arg = 1 ;
    if ( ! err )
      if ( ioctl ( f->fd, TIOCSSOFTCAR, &arg ) )
	msg ("WS unable to set software carrier:" ) ;
  }
#endif

  return err ;
}
