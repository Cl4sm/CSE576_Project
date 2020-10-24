int readfont ( char *fname, faxfont *font )
{
  int err=0, i, j, n=0, nr, nb, fontok=0, pels ;
  char *fnames [2] = { 0, 0 } ;
  short runs [ MAXRUNS ] ;
  IFILE f;

  if ( fname && *fname ) {

    fnames[0] = fname ;
    
    newIFILE ( &f, fnames ) ;
    
    if ( nextipage ( &f, 0 ) ) {
      err = msg ( "E2 can't open font file %s", fnames[0] ) ;
    }
    
    nb = 0 ;
    while ( ! err && ( nr = readline ( &f, runs, &pels ) ) >= 0 ) {
      if ( nb+pels/8 < MAXFONTBUF ) {
	nb += runtobit ( runs, nr, font->buf+nb ) ;
      } else {
	err = msg ("E2font file %s too large (max %d bytes)", 
		   fnames[0], MAXFONTBUF ) ;
      }
    }
    
    if ( ! err && nb != f.page->w * f.page->h / 8 )
      err = msg ( "E2 read %d bytes of font data for %dx%d bitmap",
		 nb, f.page->w, f.page->h ) ;
    
    if ( ! err && ( f.page->w / 256 > MAXFONTW || f.page->h > MAXFONTH ) ) {
      err = msg ( "E2font size (%dx%d) too large", f.page->w, f.page->h ) ;
    }
    
    if ( err ) {
      font->w = font->h = 0 ;
    } else {
      font->w = f.page->w / 256 ;
      font->h = f.page->h ;
      for ( i=0 ; i<256 ; i++ ) font->offset[i] = i*font->w ;
      msg ("Iread %dx%d font %s (%d bytes)", font->w, font->h, fname, nb ) ;
      fontok = 1 ;
    }

    if ( f.f ) {
      fclose ( f.f ) ;
      f.f = 0 ;
    }
  }    

  if ( ! fontok ) {	           /* use built-in font */

    font->w = STDFONTW ;
    font->h = STDFONTH ;

    for ( i=j=0 ; j<STDFONTBUF ; i++ )	   /* expand bit map */
      if ( stdfont [ i ] == 0 )
	for ( n = stdfont [ ++i ] ; n > 0 ; n-- ) 
	  font->buf [ j++ ] = 0 ;
      else
	font->buf [ j++ ] = stdfont [ i ] ;

    if ( i != 1980 ) err = msg ( "E2can't happen(readfont)" ) ;

    for ( i=0 ; i<256 ; i++ ) font->offset[i] = i*font->w ;
  }

  return err ;
}
