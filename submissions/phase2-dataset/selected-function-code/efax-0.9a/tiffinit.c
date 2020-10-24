int tiffinit ( OFILE *f )
{
  int err=0, compr=1 ;
  long tdoff, doff ;

  fseek ( f->f, 0, SEEK_SET ) ;

  /* 0 ==> (start of TIFF file) */

  /* write magic, TIFF version and offset to directory */

  fwrite2 ( bigendian ? 0x4d4d : 0x4949, f ) ;
  fwrite2 ( 42, f ) ;
  fwrite4 ( 8, f ) ;

  /* 8 ==> directory */

  fwrite2 ( NTAGS, f ) ;

  /* figure out offsets within file and compression code */

  tdoff = 8 + 2 + NTAGS*12 + 4 ;     /* offset to directory data */
  doff = tdoff + NRATIO*8 ;	     /* offset to image data */

  switch ( f->format ) {
  case O_TIFF_RAW: compr = 1 ; break ;
  case O_TIFF_FAX: compr = 3 ; break ;
  default: err = msg ( "E2can't happen(tiffinit)" ) ; break ;
  }

  /* write directory tags, 12 bytes each */

  wtag( f, 1, 256, 4, 1, f->w ) ;     /* width long */
  wtag( f, 1, 257, 4, 1, f->h ) ;     /* length long */
  wtag( f, 0, 258, 3, 1, 1 ) ;	      /* bits/sample short */

  wtag( f, 0, 259, 3, 1, compr ) ;    /* compresssion(g3=3) short */
  wtag( f, 0, 262, 3, 1, 0 ) ;	      /* photometric(0-min=white) short */
  wtag( f, 0, 266, 3, 1, 1 ) ;	      /* fill order(msb2lsb=1) short */
  wtag( f, 1, 273, 4, 1, doff ) ;     /* strip offsets long */

  wtag( f, 0, 274, 3, 1, 1 ) ;	      /* orientation(1=normal) short */
  wtag( f, 0, 277, 3, 1, 1 ) ;	      /* samples/pixel short */
  wtag( f, 1, 278, 4, 1, f->h ) ;     /* rows/strip long */
  wtag( f, 1, 279, 4, 1, f->bytes ) ; /* strip byte counts long */

  wtag( f, 1, 282, 5, 1, tdoff+0 ) ;  /* xresolution ratio */
  wtag( f, 1, 283, 5, 1, tdoff+8 ) ;  /* yresolution ratio */
  wtag( f, 0, 284, 3, 1, 1 ) ;	      /* storage(1=single plane) short */
  wtag( f, 1, 292, 4, 1, 0 ) ;	      /* g3options long */

  wtag( f, 0, 296, 3, 1, 2 ) ;	      /* resolution units(2=in,3=cm) short */
  wtag( f, 0, 327, 3, 1, 0 ) ;	      /* clean fax(0=clean) short */
  
  fwrite4 ( 0, f ) ;		      /* offset to next dir (no more) */

  /* ==> tdoff (tag data offset), write ratios for floats here */

  fwrite4 ( f->xres+0.5, f ) ;
  fwrite4 ( 1, f ) ;
  fwrite4 ( f->yres+0.5, f ) ;
  fwrite4 ( 1, f ) ;

  /* ==> doff (strip data offset), image data goes here */

  return err ;
}
