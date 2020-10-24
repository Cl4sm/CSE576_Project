int ttlocked ( char *fname, int log )
{
  int err=0, ipid ;
  FILE *f ;
  pid_t pid = 0 ;
  char buf [ EFAX_PATH_MAX ] = "" ;

  if ( fname && *fname == BINLKFLAG ) fname++ ;

  if ( fname && ( f = fopen ( fname , "r" ) ) ) {

    if ( fread ( buf, sizeof(char), EFAX_PATH_MAX-1, f )  == sizeof(pid_t) || 
	sscanf ( buf, "%d" , &ipid ) != 1 ) {
      pid = * (pid_t *) buf ;
      if ( log ) msg ("X+ read binary pid %d from %s", (int) pid, fname ) ;
    } else {
      char *p ;
      pid = (pid_t) ipid ;
      if ( log ) {
	msg ( "X+ read HDB pid %d [",  (int) pid ) ;
	for ( p=buf ; *p ; p++ ) msg ( "X+ %s", cname ( *p ) ) ;
	msg ( "X+ ] from %s", fname ) ;
      }
    }

    if ( kill ( pid, 0 ) && errno == ESRCH ) {
      if ( log ) msg ("X  - stale" ) ;
      if ( remove ( fname ) ) 
	err = msg ( "ES2can't remove stale lock %s from pid %d:", 
		   fname, pid ) ;
      else 
	err = msg ( "I0removed stale lock %s from pid %d", fname, pid ) ;
    } else { 
      if ( pid != getpid() ) {
	err = 1 ;
	if ( log ) msg ( "X1  (not our pid)" ) ;
      } else { 
	err = 3 ; 
	if ( log ) msg ( "X3  (our pid)" ) ;
      }
    }
    fclose ( f ) ;
  }
  return err ;
}
