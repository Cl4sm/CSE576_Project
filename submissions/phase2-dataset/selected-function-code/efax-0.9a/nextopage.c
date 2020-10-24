{
  int err = 0 ;
  int i, nb=0 ;
  uchar *p, codes [ ( RTCEOL * EOLBITS ) / 8 + 3 ] ;
  
  if ( f->f ) { /* terminate previous page */

    switch ( f->format ) {
    case O_PBM:
      break ;
    case O_PGM:
      break ;
    case O_FAX:
    case O_TIFF_FAX:
      for ( p = codes, i=0 ; i<RTCEOL ; i++ ) 
	p = putcode ( &f->e, EOLCODE, EOLBITS, p ) ;
      nb = putcode ( &f->e, 0, 0, p ) - codes ;
      fwrite ( codes, 1, nb, f->f ) ;
      f->bytes += nb ;
      if ( f->format == O_TIFF_FAX ) tiffinit ( f ) ;
      break ;
    case O_TIFF_RAW:
      tiffinit(f) ;		/* rewind & update TIFF header */
      break ;
    case O_PCL:
      fprintf ( f->f, PCLEND ) ;
      break ;
    case O_PS:
      fprintf ( f->f, PSPAGEEND ) ;
      if ( f->fname || page<0 ) fprintf ( f->f, PSEND, f->lastpageno ) ;
      break ;
    case O_PCX:
    case O_PCX_RAW:
      fseek ( f->f, 0, SEEK_SET ) ;
      pcxinit ( f ) ;
      break ;
    }

    if ( ferror ( f->f ) ) {
      err = msg ("ES2output error:" ) ;
    } else {
      msg ( "F+ wrote %s as %dx%d pixel %.fx%.f dpi %s page", 
  	   f->cfname, f->w, f->h, f->xres, f->yres, 
	   oformatname [f->format] ) ;
      
      switch ( f->format ) {
      case O_PS: 
  	msg ( "F  (%d lines)", f->pslines ) ;
	break ;
      case O_TIFF_RAW:
      case O_TIFF_FAX:
  	msg ( "F  (%d bytes)", f->bytes ) ;
	break ;
      default:
  	msg ( "F " ) ;
	break ;
      }

    }

  }

  if ( ! err && page >= 0 ) {	/* open new file */
    if ( f->fname ) {
      sprintf ( f->cfname, f->fname, page+1, page+1, page+1 ) ;

      if ( ! f->f )
	f->f = fopen ( f->cfname, ( f->format == O_PS ) ? "w" : "wb+" ) ;
      else
	f->f = freopen ( f->cfname, ( f->format == O_PS ) ? "w" : "wb+", f->f ) ;

      if ( ! f->f ) {
	err = msg ("ES2can't open output file %s:", f->cfname ) ;
      }
    } else {
      f->f = stdout ;
      strcpy ( f->cfname, "standard output" ) ;
    }
  }

  /* start new page */

  if ( ! err && page >= 0 ) {
    switch ( f->format ) {
    case  O_PBM:
      fprintf ( f->f, "P4 %d %d\n", f->w, f->h ) ;
      break ;
    case  O_PGM:
      fprintf ( f->f, "P5 %d %d %d\n", f->w/4, f->h/4, 255 ) ;
      break ;
    case O_FAX:
    case O_TIFF_FAX:
      if ( f->format == O_TIFF_FAX ) tiffinit ( f ) ;
      p = putcode ( &f->e, EOLCODE, EOLBITS, codes ) ;
      nb = p - codes ;
      fwrite ( codes, 1, nb, f->f ) ;
      break ;
    case O_TIFF_RAW:
      tiffinit ( f ) ;
      break ;
    case O_PCL:
      fprintf ( f->f, PCLBEGIN, (int) f->xres ) ;
      break ;
    case O_PS:
      psinit ( f, ( f->fname || page==0 ), page+1, f->w, f->h, f->w/8 ) ;
      break ;
    case O_PCX:
    case O_PCX_RAW:
      fseek ( f->f, 0, SEEK_SET ) ;
      pcxinit ( f ) ;
      break ;
    }

    if ( ferror ( f->f ) ) err = msg ("ES2output error:" ) ;
  }

  /* only count lines/bytes for those formats that don't have
     headers or where we will update the headers on closing */

  switch ( f->format ) {
  case O_FAX:
  case O_TIFF_FAX:
  case O_PCX:
  case O_PCX_RAW:
    f->h = 0 ;
    f->bytes = nb ;
    break ;
  }

  return err ;
}
