{ 
  static int init=0 ;
  static FILE *logfile [ NLOG ] ;
  static char msgbuf [ NLOG ] [ MAXMSGBUF ] ;
  static time_t logtime [ NLOG ] = { 0, 0 } ;
  static int atcol1 [ NLOG ] = { 1, 1 } ;
  
  int err=0, i, flags=0 ;
  char *p ;
  
  va_list ap ;
  va_start ( ap, fmt ) ;

  if ( ! init ) {
    logfile[0] = stderr ;
    logfile[1] = stdout ;
    for ( i=0 ; i<NLOG ; i++ )
      setvbuf ( logfile[i], msgbuf[i], _IOFBF, MAXMSGBUF ) ;
    cname ( 0 ) ;
    init = 1 ;
  }
  
  for ( i=0 ; i<NLOG ; i++ ) {

    for ( p=fmt ; *p ; p++ ) {
      switch ( *p ) {
      case ' ': p++ ; goto print ;
      case 'E': flags |= E ; break ;
      case 'W': flags |= W ; break ;
      case 'S': flags |= S ; break ;
      case '+': flags |= NOLF ; break ;
      case '-': flags |= NOFLSH ; break ;
      default: 
	if ( isdigit ( *p ) ) {
	  err = *p - '0' ; 
	} else if ( ! isupper ( *p ) ) {
	  goto print ;
	}
      }
    }
      
    print:

    if ( strchr ( verb[i], tolower ( *fmt ) ) ) {
      
      if ( atcol1[i] ) {
	fprintf ( logfile[i], "%s: ", argv0 ) ;
	logtime[i] = tstamp ( logtime[i], logfile[i] ) ; 
	fputs ( ( flags & E ) ? " Error: " : 
		( flags & W ) ? " Warning: " : 
		" ",
		logfile[i] ) ;
      }
      vfprintf( logfile[i], p, ap ) ;
      if ( flags & S ) fprintf ( logfile[i], " %s", strerror ( errno ) ) ;
      if ( ! ( flags & NOLF ) ) fputs ( "\n", logfile[i] ) ;
      atcol1[i] = flags & NOLF ? 0 : 1 ;
      if ( ! ( flags & NOFLSH ) ) fflush ( logfile[i] ) ;
      
    }
    
  }
  
  va_end ( ap ) ;
  
  return err ;
}
