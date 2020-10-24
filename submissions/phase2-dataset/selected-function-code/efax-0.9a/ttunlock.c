int ttunlock ( char *fname )
{
  int err = 0 ;

  if ( fname && *fname == BINLKFLAG ) fname++ ;

  switch ( ttlocked ( fname, 1 ) ) {
  case 0: break ;
  case 1: err = msg ( "E1won't remove lock %s (not ours)" , fname ) ; break ;
  case 2: err = 2 ; break ; 
  case 3:
    if ( remove ( fname ) ) {
      err = msg ( "ES2can't remove lock %s:", fname ) ;
    } else { 
      err = msg ( "X0removed lock file %s", fname ) ;
    }
    break ;
  default:
    err = msg ( "E2can't happen (ttunlock)" ) ;
    break ;
  }
  return err ;
}
