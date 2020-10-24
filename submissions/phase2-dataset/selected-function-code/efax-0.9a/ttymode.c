{
  int err=0, i ;         
  static struct termios t, oldt, *pt ;
  static int saved=0 ;

  if ( ! saved ) {
    if ( tcgetattr ( f->fd, &oldt ) ) 
      err = msg ( "ES2tcgetattr on fd=%d failed:", f->fd ) ;
    else
      saved=1 ;
  }

  t.c_iflag = IGNBRK | IGNPAR ;
  t.c_oflag = 0 ;
  t.c_cflag = CS8 | CREAD | CLOCAL ;
  t.c_lflag = 0 ;
  
  for ( i=0 ; i<NCCS ; i++ ) t.c_cc[i] = 0 ;

  t.c_cc[VMIN]  = 1 ; 
  t.c_cc[VTIME] = 0 ; 
  t.c_cc[VSTOP] = XOFF;
  t.c_cc[VSTART] = XON;

  pt = &t ;

  switch ( mode ) {
  case VOICESEND :
    t.c_iflag |= IXON ;
    t.c_cflag |= f->hwfc ? CRTSCTS : 0 ;
  case VOICECOMMAND :                                         
    cfsetospeed ( pt, B38400 ) ; 
    cfsetispeed ( pt, B38400 ) ; 
    break ;
  case SEND : 
    t.c_iflag |= IXON ;
    t.c_cflag |= f->hwfc ? CRTSCTS : 0 ;
  case COMMAND :                                         
    cfsetospeed ( pt, B19200 ) ; 
    cfsetispeed ( pt, B19200 ) ; 
    break ;
  case DROPDTR : 
    cfsetospeed ( pt, B0 ) ;                
    break ;
  case ORIGINAL :
    if ( saved ) pt = &oldt ;
    break ;
  default : 
    err = msg ("E2can't happen(ttymode)") ; 
    break ;
  }
  
  if ( ! err && tcsetattr ( f->fd, TCSADRAIN, pt ) )
    err = msg ( "ES2tcsetattr on fd=%d failed:", f->fd ) ;

  if ( ! err && checktermio ( pt, f ) ) 
    msg ( "Wterminal mode not set properly" ) ;
  
  tcflow ( f->fd, TCOON ) ;	/* in case XON got lost */

  return err ;
}
