{
  int err=0 ;
  unsigned short ntag, tag, type ;
  unsigned long count, tv ;
  double ftv ;

  msg ( "F+ TIFF directory at %ld", ftell ( f->f ) ) ;

  if ( fread2 ( &ntag, f ) ) {
    err = msg ( "E2can't read TIFF tag count" ) ;
  } else {
    msg ( "F+  with %d tags", (int) ntag ) ;
  }

  for ( ; ! err && ntag > 0 ; ntag-- ) {

    err = err || fread2 ( &tag, f ) ;
    err = err || fread2 ( &type, f ) ;
    err = err || fread4 ( &count, f ) ;

    if ( type == 3 ) {		      /* left-aligned short */
      unsigned short a, b ;
      err = err || fread2 ( &a, f ) ;
      err = err || fread2 ( &b, f ) ;
      tv = a ;
    } else {			      /* long or offset to data */
      err = err || fread4 ( &tv, f ) ;
    }

    if ( type == 5 ) {		      /* float as ratio in directory data */
      long a, b, where=0 ;
      err = err || ( ( where = ftell ( f->f ) ) < 0 ) ;
      err = err || fseek ( f->f, tv, SEEK_SET ) ;
      err = err || fread4 ( &a, f ) ;
      err = err || fread4 ( &b, f ) ;
      err = err || fseek ( f->f, where, SEEK_SET ) ;
      ftv = (float) a / ( b ? b : 1 ) ;
    } else { 
      ftv = 0.0 ;
    }

    if ( err ) {
      err = msg ( "ES2can't read TIFF tag" ) ;
      continue ;
    }
    
#ifdef TIFF_DEBUG
    {
      char *tagtype[] = { "none", "byte", "ascii", "short", "long", "ratio" } ;
      msg ( "F  %3d %-5s tag %s %5ld (%3d:%s)", 
	    count,
	    type <= 5 ? tagtype[type] : "other",
	    count > 1 ? "@" : "=",
	    type == 5 ? (long) ftv : tv, tag, tagname(tag) ) ; 
    }
#endif

    switch ( tag ) {
    case 256 :			/* width */
      f->page->w = tv ;
      break ;
    case 257 :			/* height */
      f->page->h = tv ;
      break ;
    case 259 :			/* compression: 1=none, 3=G3 */
      if ( tv == 1 ) {
	f->page->format = P_RAW ;
      } else if ( tv == 3 ) {
	f->page->format = P_FAX ;
      } else {
	err = msg ( "E2can only read TIFF/G3 or TIFF/uncompressed" ) ;
      }
      break ;
    case 262 :			/* photometric interpretation */
      f->page->black_is_zero = tv ;
      break ;
    case 266 :			/* fill order */
      f->page->revbits = ( tv == 2 ? 1 : 0 ) ;
      break ;
    case 273 :			/* data offset */
      if ( count != 1 )
	err = msg ( "E2can't read multi-strip TIFF files" ) ;
      else
	f->page->offset = tv ;
      break ;
    case 282 :			/* x resolution */
      f->page->xres = ftv ;
      break ;
    case 283 :			/* y resolution */
      f->page->yres = ftv ;
      break ;
    case 292 :			/* T4 options: 1=2D, 2=uncompressed */
      if ( tv & 0x1 )
	err = msg ( "E2can't read 2D compressed TIFF-F file" ) ;
      if ( tv & 0x2 )
	err = msg ( "E2can't read uncompressed TIFF-F file" ) ;
      break ;
    case 296 :			/* units: 2=in, 3=cm */
      if ( tv == 3 ) {
	f->page->xres *= 2.54 ;
	f->page->yres *= 2.54 ;
      }
      break ;
    }

  } /* end of tag reading loop */


  if ( f->page->format == I_AUTO ) {
    msg ( "W missing TIFF compression format, set to raw" ) ;
    f->page->format = P_RAW ;
  }
  
  if ( ! err ) {

    if ( fread4 ( &(f->next), f ) ) {
      err = msg ( "E2can't read offset to next TIFF directory" ) ;
    } else {
      if ( f->next ) {
	msg ( "F , next directory at %ld.", f->next ) ;
	if ( fseek ( f->f, f->next, SEEK_SET ) )
	  err = msg ( "ES2 seek to next TIFF directory failed" ) ;
      } else {
	msg ( "F , last image." ) ;
      }
    }

  }

  if ( ! f->page->offset )
    err = msg ( "E2 missing offset to TIFF data" ) ;

  return err ;
}
