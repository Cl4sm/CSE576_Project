	      TFILE *mf, cap local, char *localid, 
	      OFILE *outf, IFILE *inf, 
	      int pages, char *header, faxfont *font, 
	      int maxpgerr, int noretry, int calling )
{
  int err=0, done=0, page, pagetry, nerr, c, dp=0 ;
  int ppm=0, good, hsc, changed ;
  int remtx=0 ;
  char *fname=0 ;
  cap session = { 0,0,0,0, 0,0,0,0 } ;
  char buf [ CMDBUFSIZE ] ;

  hsc=-1 ;			/* will be set >= 0 on hangup */

  if ( sresponse ( "+FPO", 0 ) ) {
    remtx = 1 ;
    msg ( "N remote has document(s) to send." ) ;
  }

  if ( calling ) {
    if ( pages ) goto send ;
    else goto poll ;
  } else {
    if ( pages ) goto pollserver ;
    else goto receive ;
  }

  /* Class 2 Send */

 pollserver:

  /* with +FLP[L]=1 the modem should accept +FDT. */

 send:
  
  page=1 ;
  
  pagetry=0 ;
  while ( ! err && ! done ) {

    err = rdpage ( inf, dp, &ppm, local, &changed ) ;

    if ( ! err && changed ) {
      sprintf ( buf, c20 ? "+FIS=%d,%d,%d,%d" : "+FDIS=%d,%d,%d,%d", 
	       local[0], local[1], local[2], local[3] ) ;
      ckcmd ( mf, 0, buf, TO_FT, OK ) ;
      if ( gethsc ( &hsc, &err ) ) {
	continue ;
      }
    }
    
    ckcmd ( mf, &err, "+FDT", -TO_C2B, CONNECT ) ;
    if ( err || gethsc ( &hsc, &err ) ) { 
      done=1 ; 
      continue ; 
    }

    getc2dcs ( session ) ; 

    if ( ! c20 ) getstartc ( mf ) ;

    send_data ( mf, inf, page, pages, local, session, header, font ) ;
    pagetry++ ;

    if ( c20 ) {
      end_data ( mf, session, ppm, &good ) ;
    } else {
      end_data ( mf, session, 0, 0 ) ;

      gethsc ( &hsc, &err ) ;

      if ( ! err && hsc < 0 ) {
	ckcmd ( mf, &err, ppm == EOP ? "+FET=2" : 
	       ppm == EOM ? "+FET=1" : "+FET=0" , TO_C2PP, OK ) ;
      }

      gethsc ( &hsc, &err ) ;

      if ( ! err && hsc < 0 ) {
	if ( sresponse ( "+FPTS:", &good ) ) {
	  good &= 1 ;		/* odd values mean received OK */
	} else {			/* no +FPTS and +FHNG probably NG */
	  good = gethsc ( 0, 0 ) ? 0 :  
	    msg ( "W1no +FPTS response, assumed received" ) ;
	}
      }

    }
    
    if ( noretry ) good = 1;
    
    if ( good ) {
      fname = inf->page->fname ;
      if ( fname ) msg ( "Isent -> %s", fname ) ;
      pagetry=0 ;
      page++ ;
      dp = 1 ;
      if ( ppm == EOP ) {
	nextipage ( inf, 1 ) ;	/* skip ahead to mark all files done */
	done = 1 ;
      }
    } else {
      dp = 0 ;
      if ( pagetry >= NTXRETRY )
	err = msg ( "E2too many page send retries" ) ;
    }

    if ( gethsc ( &hsc, &err ) )  done=1 ;

    if ( good && lastpage ( inf ) ) {
      done = 1 ;
    }
  }

  goto done ;

  /* Class 2 Receive */

 poll:

  /* with +FSP[L]=1 and +FPO[LL]: the modem should now accept +FDR. */

 receive:

  getc2dcs ( session ) ;	/* get ATA responses */

  done=0 ;
  for ( page=0 ; ! err && ! done ; page++ ) {

    if ( ! ( err = wrpage ( outf, page ) ) ) {
      c = cmd ( mf, "+FDR", -TO_C2R ) ;

      switch ( c ) {

      case CONNECT:
   	getc2dcs ( session ) ; 

   	outf->w=pagewidth[session[WD]];
   	outf->h=0;
	outf->xres=204.0;
	outf->yres=vresolution[session[VR]];
	nerr = 0 ;
	
	tput ( mf, &startchar, 1 ) ;

	if ( receive_data ( mf, outf, session, &nerr ) == 0 ) {
	  good = nerr < maxpgerr ;
	  msg ( "I-received -> %s", outf->cfname ) ;
	} else { 
	  good = 0 ;
	}
	
	ckcmd ( mf, &err, 0, TO_C2EOR, OK ) ;

	if ( err || gethsc ( &hsc, &err ) )  { 
	  wrpage ( outf, page+1 ) ;
	  wrpage ( outf, -1 ) ;
	  done=1 ; 
	  continue ; 
	}
	
	if ( ! good ) {
	  msg ( "Wreception errors" ) ;
	  ckcmd ( mf, 0, c20 ? "+FPS=2" : "+FPTS=2",  T3S, OK ) ;
	  if ( gethsc ( &hsc, &err ) ) continue ;
	}
	break ;

      case OK:
	wrpage ( outf, -1 ) ;	/* no more pages */
	done=1 ;
	if ( gethsc ( &hsc, &err ) ) continue ;
	break ;

      default:
	wrpage ( outf, -1 ) ;	/* oops */
	err = msg ( "E3receive (+FDR) command failed") ;
	break ;
      }
    }
  } 

  
 done:
  if ( hsc < 0 ) ckcmd ( mf, 0, c20 ? "+FKS" : "+FK", TO_RESET, OK ) ;

  return err ;
}
