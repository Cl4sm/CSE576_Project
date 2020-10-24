int cmd ( TFILE *f, char *s, int t )
{
  char buf [ CMDBUFSIZE ], *p = "" ;
  int resplen=0, pause=0 ;

  if ( t < 0 ) {
    pause = cmdpause ;
    t = -t ;
  }

  lresponse = responses ;

  retry:

  if ( s ) { 

    while ( tgets ( f, buf, CMDBUFSIZE, pause ) )
      msg ( "W- unexpected response \"%s\"", buf ) ;

    msg ( "C- command  \"%s\"", s ) ;

    if ( strlen(s) >= CMDBUFSIZE-4 ) {
      msg ( "E modem command \"%s\" too long", s ) ;
    } else {
      sprintf ( buf, "AT%s\r", s ) ;
      tput ( f, buf, strlen(buf) ) ;
    }
  }

  if ( t ) {

    msg ( "C- waiting %.1f s", ((float) t)/10 ) ;

    while ( ( p = tgets ( f, buf, CMDBUFSIZE, t ) ) ) {

      if ( ( resplen += strlen ( buf ) + 1 ) <= MAXRESPB ) {
	strcpy ( lresponse, buf ) ;
	lresponse += strlen ( buf ) + 1 ;
      }
      
      if ( ( p = strtabmatch ( (char**) prompts, buf ) ) ) {
	msg ( "C- response \"%s\"", buf ) ;
	break ;
      }
      
      if ( ! strcmp ( buf, "RING" ) ) { msleep ( 100 ) ; goto retry ; }
    }
  }

  return p ? *p : EOF ;
}
