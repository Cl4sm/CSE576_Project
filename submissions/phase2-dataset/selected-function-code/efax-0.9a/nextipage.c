{
  int err=0 ;

  int ( *reset [NPFORMATS] ) ( IFILE * ) = {
    raw_reset, fax_reset, pbm_reset, text_reset, pcx_reset
  }, (*pf)(IFILE*) ;

  /* close current file if any and set to NULL */

  if ( f->f ) {
    fclose ( f->f ) ;
    f->f = 0 ;
  }

  /*  if requested, point to next page and check if done */

  if ( dp ) {
    f->page++ ;
  }
  
  if ( f->page > f->lastpage ) {
    err = 1 ;
  }

  /* open the file and seek to start of image data */

  if ( ! err ) {
    f->f = fopen ( f->page->fname, (f->page->format == P_TEXT) ? "r" : "rb" ) ;
    if ( ! f->f )
      err = msg ( "ES2can't open %s:", f->page->fname ) ;
  }

  if ( ! err && fseek ( f->f, f->page->offset, SEEK_SET ) )
    err = msg ( "ES2 seek failed" ) ;

  /* default initializations */

  f->lines = f->page->h ;

  /* coding-specific initializations for this page */
  
  if ( ! err ) {
    pf = reset[f->page->format] ;
    if ( pf )
      err = (*pf)(f) ;
  }

  return err ;
}
