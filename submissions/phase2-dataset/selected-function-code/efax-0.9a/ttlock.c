{
  int err=0, dirlen, bin=0 ;    
  FILE *f=0 ;    
  pid_t pid = getpid ( ) ;
  char *p , buf [ EFAX_PATH_MAX ] = "" ;

  if ( fname && *fname == BINLKFLAG ) {
    fname++ ; 
    bin = 1 ;
  }

  err = ttlocked ( fname, log ) ;

  if ( ! err ) {
    dirlen = ( p = strrchr( fname , '/' ) ) ? p-fname+1 : strlen ( fname ) ;
    sprintf ( buf , "%.*sTMP..%05d" , dirlen , fname , (int) pid ) ;
    if ( ! ( f = fopen( buf, "w" ) ) ) 
      err = msg ( "ES2can't open pre-lock file %s:", buf ) ;
  }

  if ( ! err && f ) {
    if ( bin ) {
      if ( fwrite ( &pid, sizeof(pid_t), 1, f ) < 1 ) 
	err = msg ( "ES2can't write pre-lock file %s:", buf ) ;
    } else {
      if ( fprintf ( f, "%10d\n", (int) pid ) < 0 )
	err = msg ( "ES2can't write pre-lock file %s:", buf ) ;
    }
  }

  if ( ! err && f ) {
    if ( rename ( buf , fname ) == 0 ) {
      chmod ( fname , 0444 ) ;
      msg ( "Xcreated %s lock file %s", bin ? "binary" : "text", fname ) ; 
    } else {
      err = ttlocked ( fname, log ) ;
      if ( ! err )
	err = msg ( "ES2can't rename lock file %s to %s:", buf, fname ) ;
    }
  }

  if ( f ) { 
    fclose ( f ) ; 
    if ( err ) remove ( buf ) ; 
  }

  return err ;
}
