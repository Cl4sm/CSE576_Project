{
  int err=0, i, n, fformat=0 ;
  char **p ;
  uchar buf[128] ;
  int ( *fun ) ( IFILE * ) ;
  
  int ( *first [NIFORMATS] ) ( IFILE * ) = {
    auto_first, pbm_first, fax_first, text_first, tiff_first, 
    dfax_first, pcx_first, raw_first, dcx_first
  } ;

  int ( *next [NIFORMATS] ) ( IFILE * ) = {
    auto_next, pbm_next, fax_next, text_next, tiff_next, 
    dfax_next, pcx_next, raw_next, dcx_next
  } ;

  f->page = f->pages ;

  /* get info for all pages in all files */

  for ( p=fnames ; ! err && *p ; p++ ) {

    if ( ! ( f->f = fopen ( *p, "rb" ) ) )
      err = msg ( "ES2 can't open %s:", *p ) ;
    
    if ( ! err ) {
      n = fread ( buf, 1, 128, f->f ) ;
      if ( ferror ( f->f ) )
	err = msg ( "ES2 can't open %s:", *p ) ;
    }
    
    if ( ! err ) {
      fformat = getformat ( buf, n ) ;
      if ( ! fformat )
	err = msg ( "E2 can't get format of %s", *p ) ;
    }

    if ( ! err && fseek ( f->f, 0, SEEK_SET ) )
      err = msg ( "ES2 can't rewind %s:", *p ) ;

    /* get format information for all pages in this file */

    for ( i=0 ; ! err ; i++ ) {

      page_init ( f->page, *p ) ;

      if ( ( fun = i ? next[fformat] : first[fformat] ) )
	err = (*fun)(f) ;

      if ( ! err ) {

	page_report ( f->page, fformat, f->page - f->pages + 1 ) ;

	f->page++ ;
	
	if ( f->page >= f->pages + MAXPAGE )
	  err = msg ( "E2 too many pages (max is %d)", MAXPAGE ) ;
      }

      if ( ! f->next ) break ;
    }

    if ( f->f ) {
      fclose ( f->f ) ;
      f->f = 0 ;
    }

  }

  f->lastpage = f->page - 1 ;

  f->page = f->pages ;
  
  if ( ! normalbits[1] ) initbittab() ;	/* bit-reverse table initialization */

  return err ;
}
