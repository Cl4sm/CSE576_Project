int tdata ( TFILE *f, int t )
{
  int n, err=0 ;
  fd_set fds ;

  struct timeval timeout ;

  if ( f->fd < 0 ) msg ( "Ecan't happen (faxdata)" ) ;

  timeout.tv_sec  = t / 10 ; 
  timeout.tv_usec = ( t % 10 ) * 100000 ;

  FD_ZERO ( &fds ) ;
  FD_SET ( f->fd, &fds ) ;

  do { 
    n = select ( f->fd + 1, &fds, 0, 0, t<0 ? 0 : &timeout ) ;
    if ( n < 0 ) {
      if ( errno == EINTR ) {
	msg ( "W0  select() interrupted in tdata()" ) ;
      } else {
	err = msg ( "ES2 select() failed in tdata():" ) ;
      }
    }
  } while ( n < 0 && ! err ) ;

  return n ;
}
