	      TFILE *mf, cap local, char *localid, 
	      OFILE *outf, IFILE *inf, 
	      int pages, char *header, faxfont *font, 
	      int maxpgerr, int noretry, int calling )
{ 
  int err=0, rxpage=0, page=1, t, disbit, good, frame, last, nerr ;
  int rxdislen, ppm, try=0, pagetry=0, retry=0, remtx=0, remrx=0 ;
  int writepending=0, dp=0 ;
  cap remote = { DEFCAP }, session = { DEFCAP } ;
  char *fname=0 ;
  uchar buf [ MAXFRLEN ], *fif=buf+3 ;

  if ( ! calling ) goto RX ;

  /* Class 1 Transmitter: */

 T:  /* Transmitter Phase B - wait for DIS or DTC */

  pagetry = 0 ;
  
  frame = getfr ( mf, buf, T1 ) ;
  
  if ( frame <= 0 ) {
    err = msg ( "E3no answer from remote fax" ) ;
    goto B ;
  }
  
  if ( frame != DIS && frame != DTC ) {
    msg ( "W2 can't open page" ) ;
    goto C ;
  }

  disbit = ( frame == DIS ) ? 0x80 : 0x00 ;
  try = 0 ;

 A:				/* decide to send or receive after DIS/DTC */

  if ( frame == DIS || frame == DTC ) {
    rxdislen = dislen ( fif ) ;
    mkcap ( fif, remote, 1 ) ;
    remtx = fif[1] & 0x80 ;
    remrx = fif[1] & 0x40 ;
  }

  msg ( "N remote has %sdocument(s) to send, and can %sreceive",
       remtx ? "" : "no ", remrx ? "" : "not " ) ;

  if ( pages > 0 ) {
    if ( ! remrx ) msg ( "W remote cannot receive, trying anyways" ) ; 
    goto D ;
  } else {
    if ( ! remtx ) msg ( "W remote has nothing to send, trying anyways" )  ; 
    goto R ;
  }

 D:				/* send DCS */

  if ( rdpage ( inf, dp, &ppm, local, 0 ) ) {
    err = msg ( "E2can't open page" ) ;
    goto B ;
  }

 D_2:

  mincap ( local, remote, session ) ;

  revcpy ( (uchar*) localid, fif ) ;
  if ( ! err ) 
    err = putframe ( TSI | MORE_FR | disbit, buf, IDLEN, mf, -1 ) ;  

  mkdis ( session, fif, DCSLEN, 0, pages ) ;
  if ( ! err ) 
    err = putframe ( DCS | SUB_FR | disbit, buf, DCSLEN, mf, -1 ) ;

#ifdef USEFTS
  if ( cmd ( mf, "+FTS=" MODDLY, T3S ) != OK )
#endif
    msleep ( TMOD ) ;		/* if +FTS not supported */

  if ( ! err ) 
    err = puttrain ( mf, c1cmd[SND][TRN][session[BR]], 
		    TCFSECS*cps [ session[BR] ] ) ;
  try++ ;

  if ( ! err ) {
    cmd ( mf, "+FRS=1", T3S ) ; /* wait for TCF carrier to drop */
    frame = getfr ( mf, buf, 0 ) ;
  }

  if ( err || frame < 0 ) {
    if ( try >= 3 ) {
      goto C_timeout ;
    } else { 
      goto D_2 ;
    }
  }
  
  switch ( frame ) {

  case DIS:
  case DTC:
    if ( try >= 3 ) goto C_timeout ;
    else goto A ;

  case FTT:
    msg ( "I channel not usable at %d bps", 8*cps[session[BR]] ) ;
    remote[BR] = fallback[session[BR]] ;
    if ( remote[BR] >= 0 ) goto D_2 ;
    else { err = msg ( "E2 channel not usable at lowest speed" ) ; goto C ; }

  case CFR:
    goto I_2 ;

  default:
    err = msg ( "E3 invalid response to DCS (0x%02x)", frame ) ;
    goto C ;
  }    

 I:				/* send a page */

  if ( rdpage ( inf, dp, &ppm, local, 0 ) ) {
    err = msg ( "E2can't open page" ) ;
    goto B ;
  }

 I_2:

  ckcmd ( mf, &err, c1cmd [SND][DTA][session[BR]], TO_FT, CONNECT ) ;
  if ( !err ) {
    msleep ( 1000 ) ;
    err = send_data ( mf, inf, page, pages, local, session, header, font ) ;
  }

  pagetry++ ;

  if ( !err )
    err = end_data ( mf, session, 0, 0 ) ;
  
#ifdef USEFTS
  if ( cmd ( mf, "+FTS=" MODDLY, T3S ) != OK )
#endif
    msleep ( TMOD ) ;		/* if +FTS not supported */

				/* fix ppm if on last page of stdin */
  if ( lastpage ( inf ) ) ppm = EOP ;

  try = 0 ;
 sendppm:
  if ( !err ) err = putframe ( ppm | disbit, buf, 0, mf, -1 ) ;
  try++ ;
  
  frame = getfr ( mf, buf, 0 ) ;
  if ( frame < 0 ) {
    if ( try >= 3 ) {
      goto C_timeout ;
    } else { 
      goto sendppm ;
    }
  }

  fname = inf->page->fname ;

  switch ( noretry ? MCF : frame ) { /* common retry logic */
  case MCF:
  case RTP:
  case PIP:
    fname = inf->page->fname ;
    if ( fname ) msg ( "Isent -> %s", fname ) ;
    pagetry=0 ;
    page++ ;
    dp = 1 ;
    break ;
  case PIN:
  case RTN:
    dp = 0 ;
    retry = pagetry < NTXRETRY ;
    break ;
  default:  
    err = msg ( "E3invalid post-page response (0x%02x)", frame ) ;
    goto C ;
  }
  
  switch ( ppm ) {
    
  case MPS:
    switch ( frame ) {
    case PIN: goto E ;
    case PIP: goto E ;
    case MCF: goto I ;
    case RTP: goto D ;
    case RTN: goto D ;
    }

  case EOP:
    switch ( frame ) {
    case PIN: goto E ;
    case PIP: goto E ;
    case MCF: 
    case RTP: 
      nextipage ( inf, 1 ) ;	/* skip ahead to mark all files done */
      if ( remtx ) goto R ;	/* poll after sending */
      else goto C ;
    case RTN: 
      if ( retry ) goto D ;
      else goto C ;
    }
    
  case EOM:
    switch ( frame ) {
    case PIN: goto E ;
    case PIP: goto E ;
    case MCF: 
    case RTP: 
    case RTN: 
      cmd ( mf, "+FRS=20", T3S ) ; /* wait for ppr carrier to drop */
      if ( retry ) goto T ;
      else goto T ;
    }
    
  }  

 E:				/* ignore PIN and PIP */
  msg ( "W interrupt request ignored" ) ;
  try=0 ;
  goto A ;

  /* Class 1 Receiver */

 RX:

 R:  /* Receiver Phase B */

  if ( ! err ) err = wrpage ( outf, rxpage ) ;

  disbit=0x00 ;

  for ( t=0 ; !err && t<T1 ; t+=T2+10 ) {

    revcpy ( (uchar*) localid, fif ) ;
    if ( !err ) 
      err = putframe ( CSI | disbit | MORE_FR, buf, IDLEN, mf, -1 ) ;
    
    mkdis ( local, fif, DEFDISLEN, 1, pages ) ;
    if ( !err ) 
      err = putframe ( DIS | disbit | SUB_FR, buf, DEFDISLEN, mf, -1 ) ;

    frame = getfr ( mf, buf, 0 ) ;

    if ( frame > 0 ) {
      disbit = ( frame == DIS ) ? 0x80 : 0x00 ;
      goto F_2 ;
    }
  }
  if ( err ) goto C ;
  else goto C_timeout ;
  

 F:  /* get a command */

  last = frame ;
  frame = getfr ( mf, buf, 1 ) ;

  if ( writepending ) {		/* do postponed file close/open */
    writepending=0 ;
    err = wrpage ( outf, rxpage ) ;
    if ( err ) goto C ;
  }

  if ( frame < 0 ) {
    if ( frame == -2 ) goto getdata ; /* data carrier detected */
    if ( last == EOM ) goto R ; 
    else { err = msg ("E3 timed out waiting for command" ) ; goto B ; }
  }
  
 F_2:

  switch ( frame ) {

  case DTC:
    goto D ;

  case DIS:
    try=0 ;
    goto A ;
    
  case DCS: 
    mkcap ( fif, session, 0 ) ;
    printcap ( "session", session ) ;
    
    cmd ( mf, "+FTS=1", T3S ) ;	/* make sure DCS is over */

    gettrain ( mf, c1cmd [RCV][TRN][session[BR]], cps[session[BR]], &good ) ;

    if ( putframe ( ( good ? CFR : FTT ) | disbit, buf, 0, mf, -1 ) ||
	! good ) goto F ;

  getdata:
    
    outf->w=pagewidth[session[WD]];
    outf->h=0;
    outf->xres=204.0;
    outf->yres=vresolution[session[VR]];
    nerr = 0 ;
    
  re_getdata:

    if ( cmd ( mf, c1cmd [RCV][DTA][session[BR]], TO_FT ) != CONNECT ) 
      goto F ;			/* +FCERROR -> DCS resent */
    
    switch ( receive_data ( mf, outf, session, &nerr ) ) {
    case 0:
      good = nerr < maxpgerr ;
      msg ( "I-received -> %s", outf->cfname ) ;
      writepending=1 ;		/* ppm follows immediately, don't write yet */
      rxpage++ ;
      break ;
    case 1:
      /* no RTC, re-issue +FRM command */
      goto re_getdata ;
    default:
      good = 0 ;
      break ;
    }
    ckcmd ( mf, 0, 0, TO_RTCMD, NO ) ;
    goto F ;

    /* III: */

  case PRI_EOM:
  case PRI_MPS:
  case PRI_EOP:
    frame &=0xf7 ;		/* ignore PRocedure Interrupt bit */
  case MPS:
  case EOP:
  case EOM:
    putframe ( ( good ? MCF : RTN ) | disbit, buf, 0, mf, -1 ) ;
    if ( good && frame == MPS ) goto getdata ;
    else goto F ;
    
  case DCN:
    goto B ;
    
  default:
    err = msg ( "E3 unrecognized command" ) ;
    goto B ;

  }

 C_timeout:
  err = msg ( "E3 no command/response from remote" ) ;

 C:
  putframe ( DCN, buf, 0, mf, -1 ) ;

 B:
  ckcmd ( mf, 0, "H", TO_RESET, OK ) ;	/* hang up */

  if ( rxpage > 0 ) 
    wrpage ( outf, -1 ) ;	/* remove last file */

  return err ;
}
