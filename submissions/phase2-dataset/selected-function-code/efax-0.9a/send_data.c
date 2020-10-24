	       cap local, cap session, char *header, faxfont *font )
{
  int done=0, err=0, noise=0, nr=0, lastnr=0, line, pixels ;
  int i, decimate, pwidth, minlen, dcecps, inheader, skip=0 ;
  uchar buf [ MAXCODES + 2*EOLBITS/8 + 1 ], *p ;
  short runs [ MAXRUNS ], lastruns [ MAXRUNS ] ;
  char headerbuf [ MAXLINELEN ] ;
  ENCODER e ;

  newENCODER ( &e ) ;

  dcecps = cps[session[BR]] ;
  minlen = ( (long)dcecps * mst[session[ST]] - 1500 + 500 ) / 1000 ;
  pwidth = pagewidth [ session [ WD ] ] ;
  decimate = local[VR] > session[VR] ;

  msg ( "T padding to %d bytes/scan line.%s", minlen+1, 
       decimate ? " reducing 196->98 lpi." : "" ) ;

  if ( vfc ) 
    msg ( "T limiting output to %d bps for %d byte modem buffer", 
	 dcecps*8, MAXDCEBUF + MINWRITE  ) ;

  if ( ckfmt ( header, 6 ) )
    msg ( "W too many %%d escapes in header format string \"%s\"", header ) ;
  else
    sprintf ( headerbuf, header, page, pages, page, pages, page, pages ) ;
  msg ("I header:[%s]", headerbuf ) ;
      
  done = err = ttymode ( mf, SEND ) ; 

  mf->start = time(0) ;
  mf->mstart = proc_ms() ;
  mf->bytes = mf->pad = mf->lines = 0 ;

  /* start T.4 data with some FILL and an EOL */

  p = buf ;
  for ( i=0 ; i<32 ; i++ ) {
    p = putcode ( &e, 0, 8, p ) ;
  }
  p = putcode ( &e, EOLCODE, EOLBITS, p ) ;

  if ( ! f || ! f->f ) 
    err = msg ( "E2can't happen(send_data)" ) ; 

  mf->lines=0 ;
  for ( line=0 ; ! done && ! err ; line++ ) {

    if ( line < HDRSPCE ) {	/* insert blank lines at the top */
      runs[0] = pwidth ;
      pixels = pwidth ;
      nr = 1 ;
    } else {
      if ( ( nr = readline ( f, runs, &pixels ) ) < 0 ) {
	done = 1 ;
	continue ;
      }
    }
				/* generate and OR in header pixels */
    if ( line >= HDRSTRT && line < HDRSTRT + HDRCHRH ) {
      int hnr ;
      short hruns [ MAXRUNS ] ;
      hnr = texttorun ( (uchar*) headerbuf, font, line-HDRSTRT, 
		       HDRCHRW, HDRCHRH, HDRSHFT,
		       hruns, 0 ) ;
      nr = runor ( runs, nr, hruns, hnr, 0, &pixels ) ;
    }
    
    inheader = line < HDRSTRT + HDRCHRH ;

    if ( decimate || ( inheader && local[VR] == 0 ) ) {
      if ( ++skip & 1 ) {	/* save the first of every 2 lines */
   	memcpy ( lastruns, runs, nr * sizeof(short) ) ;
   	lastnr = nr ;
   	continue ;		/* get next line */
      } else {			/* OR previous line into current line */
   	nr = runor ( runs, nr, lastruns, lastnr, 0, &pixels ) ;
      }
    }

    if ( nr > 0 ) {
      if ( pixels ) {
				/* make line the right width */
	if ( pixels != pwidth ) nr = xpad ( runs, nr, pwidth - pixels ) ;
				/* convert to MH coding */
	p = runtocode ( &e, runs, nr, p ) ;
				/* zero pad to minimum scan time */
	while ( p - buf < minlen ) { 
	  p = putcode ( &e, 0, 8, p ) ;
	  mf->pad ++ ;
	}
				/* add EOL */
	p = putcode ( &e, EOLCODE, EOLBITS, p ) ;
	sendbuf ( mf, buf, p - buf, dcecps ) ;
	mf->bytes += p - buf ;
	mf->lines++ ;
      } else {
	/* probably read an EOL as part of RTC */
      }
      if ( tdata ( mf, 0 ) ) noise = 1 ;
      p = buf ;
    }
  }

  for ( i=0 ; i < RTCEOL ; i++ )
    p = putcode ( &e, EOLCODE, EOLBITS, p ) ;
  p = putcode ( &e, 0, 0, p ) ;
  sendbuf ( mf, buf, p - buf, dcecps ) ;
  mf->bytes += p - buf ;
  
  if ( noise ) msg ("W- characters received while sending" ) ;

  return err ;
}
