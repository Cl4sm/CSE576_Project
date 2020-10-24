int main( int argc, char **argv)
{
  int err=0, doneargs=0, c=0, i ;
  int testing=0, calling=0 ;

  int nicmd[3]={0,0,0}, nlkfile=0, nverb=0 ;

  char *faxfile = FAXFILE ;

  int softaa=0, share=0, wait=0, reverse=1, ignerr=0, noretry=0, hwfc=0 ;
  int capsset=0 ;
  char *getty = "", *vcmd = "", *acmd=ANSCMD ;

  cap local = { DEFCAP } ;
  char localid  [ IDLEN + 1 ] = DEFID ;

  int maxpgerr = MAXPGERR ;

  time_t now ;
  char *header = 0, headerbuf [ MAXLINELEN ] ; 
  char *fontname = 0 ;
  faxfont font ;

  OFILE ofile ;
  int pages = 0 ;
  char *phnum="", *ansfname = DEFPAT ;
  char fnamepat [ EFAX_PATH_MAX ] ;
  
  /* print initial message to both stderr & stdout */
  argv0 = argv[0] ;
  verb[1] = "ewia" ;
  msg ( "I " Version " " Copyright ) ;
  argv0 = efaxbasename ( argv0 ) ;
  msg ( "A compiled "__DATE__ " " __TIME__ ) ;
  verb[1] = "" ;

  setlocale ( LC_ALL, "" ) ;

  while ( ! err && ! doneargs &&
	 ( c = nextopt ( argc,argv,
			"a:c:d:e:f:g:h:i:j:k:l:o:p:q:r:st:v:wx:T" ) ) != -1 ) {

    switch (c) {
    case 'a': 
      acmd = nxtoptarg ;
      break ;
    case 'c': 
      err = str2cap ( nxtoptarg, local ) ;
      capsset = 1 ;
      break ;
    case 'l': 
      if ( strlen ( nxtoptarg ) > IDLEN ) 
	msg("Wlocal ID (%s) truncated to %d characters", nxtoptarg, IDLEN ) ;
      if ( strspn ( nxtoptarg, " +0123456789" ) != strlen ( nxtoptarg ) )
	msg("Wlocal ID (%s) has non-standard characters", nxtoptarg ) ;
      sprintf ( localid, "%*.*s", IDLEN, IDLEN, nxtoptarg ) ;
      break ;
    case 'i': 
      if ( nicmd[0] < MAXICMD ) icmd[0][ nicmd[0]++ ] = nxtoptarg ;
      else err = msg ( "E2too many '-i' options"); 
      break ;
    case 'j': 
      if ( nicmd[1] < MAXICMD ) icmd[1][ nicmd[1]++ ] = nxtoptarg ;
      else err = msg ( "E2too many '-j' options"); 
      break ;
    case 'k': 
      if ( nicmd[2] < MAXICMD ) icmd[2][ nicmd[2]++ ] = nxtoptarg ;
      else err = msg ( "E2too many '-k' options"); 
      break ;
    case 'h': 
      header = nxtoptarg ; 
      break ;
    case 'f': 
      fontname = nxtoptarg ; 
      break ;
    case 'd': 
      faxfile = nxtoptarg ; 
      break ;
    case 'e': 
      vcmd = nxtoptarg ; 
      break ;
    case 'g': 
      getty = nxtoptarg ; 
      break ;
    case 'o':			/* most protocol options are globals */
      for ( ; *nxtoptarg ; nxtoptarg++ ) 
	switch ( *nxtoptarg ) {
	case '0' : c20 = 1 ; break ;
	case '1' : c1 = 1 ; break ;
	case '2' : c2 = 1 ; break ;
	case 'a' : softaa = 1 ;  break ;
	case 'e' : ignerr = 1 ;  break ;
	case 'f' : vfc = 1 ;  break ;
	case 'h' : hwfc = 1 ;  break ;
	case 'l' : lockpolldelay /= 2 ;  break ;
	case 'n' : noretry = 1 ;  break ;
	case 'r' : reverse = 0 ; break ;
	case 'x' : startchar = XON ; break ;
	case 'z' : cmdpause += T_CMD ; break ;
	 default : msg ( "Wunrecognized protocol option (%c)", *nxtoptarg ) ; 
	}
      break ;
    case 'q':
      if ( sscanf ( nxtoptarg , "%d", &maxpgerr ) != 1 || maxpgerr < 0 )
	err=msg ("E2bad quality (-q) argument (%s)", nxtoptarg ) ;
      break;
    case 'r': 
      ansfname = nxtoptarg ;
      break;
    case 's': 
      share = 1 ; 
      break;
    case 't': 
      calling=1;
      /* fall through */
    case 'p':
      if ( argv [ argc ] ) err = msg ("E2can't happen(unterminated argv)") ;
      if ( ! err ) err = newIFILE ( &ifile, argv + nxtoptind ) ;
      pages = argc - nxtoptind - ( c == 'p' ? 1 : 0 )  ;
      pages = pages < 0 ? 0 : pages ;
      phnum = nxtoptarg ;
      doneargs=1 ; 
      break;
    case 'v': 
      verb[nverb] = nxtoptarg ; 
      nverb=1;
      break ;
    case 'w': 
      wait = 1 ; 
      break ;
    case 'x': 
      if ( nlkfile < MAXLKFILE ) lkfile[ nlkfile++ ] = nxtoptarg ;
      else err = msg ( "E2too many lock files" ) ; 
      break ;
    case 'T':			/* test: begin+end session */
      testing=1;
      doneargs=1 ; 
      break ;
    default : fprintf ( stderr, Usage, argv0 ) ; err = 2 ; break ;
    }
  }

  for ( i=0 ; i<argc ; i++ ) 
    msg ( "Aargv[%d]=%s", i, argv[i]) ; 

  if ( ! nicmd[2] ) icmd[2][nicmd[2]++] = "H" ;	/* default -k command */

  readfont ( fontname, &font ) ;

  if ( ! header ) {
    char tmp [ MAXLINELEN ] ;
    now = time ( 0 ) ;
    strftime ( tmp, MAXLINELEN, "%c %%s   P. %%%%d", localtime ( &now ) ) ;
    sprintf ( header = headerbuf, tmp, localid ) ;
  }

  if ( ! err ) {
    err = begin_session ( &faxdev, faxfile, 
			 !c1 && !c20 && reverse, /* Class 2 rx bit reversal */
			 hwfc, lkfile, COMMAND, onsig ) ;
    if ( ! err ) err = setup ( &faxdev, icmd[0], ignerr ) ;
    if ( ! err ) err = modem_init ( &faxdev, local, localid, 
				    calling, calling && !pages, capsset,
				    &reverse ) ;
    if ( ! err ) err = setup ( &faxdev, icmd[1], ignerr ) ;
    if ( err == 1 ) locked = 1 ;
  }

  if ( ! err && ! testing ) {

    if ( calling ) {
      err = dial ( &faxdev, phnum, 0 ) ;
    } else {
      err = answer ( &faxdev, lkfile, wait, share, softaa, 
		    getty, vcmd, acmd ) ;
      if ( err == 1 ) locked = 1 ;
    }

    now = time(0) ;		/* do it here so use reception time */
    strftime ( fnamepat, EFAX_PATH_MAX, ansfname, localtime ( &now ) ) ;
    strncat ( fnamepat, ".%03d", EFAX_PATH_MAX - strlen ( fnamepat ) ) ;
    newOFILE ( &ofile, O_TIFF_FAX, fnamepat, 0, 0, 0, 0 ) ;
    
    if ( ! err ) {
      if ( c1 ) {
	err = c1sndrcv ( &faxdev, local, localid,
			&ofile, &ifile, pages, header, &font,
			maxpgerr, noretry, calling ) ;
      } else {
	err = c2sndrcv ( &faxdev, local, localid,
			&ofile, &ifile, pages, header, &font,
			maxpgerr, noretry, calling ) ;
      }
    }
  }

  return cleanup ( err ) ;
}
