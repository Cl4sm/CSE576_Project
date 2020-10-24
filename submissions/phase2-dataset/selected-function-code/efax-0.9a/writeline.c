void writeline ( OFILE *f, short *runs, int nr, int no )
{
  int nb = 0 ;
  uchar *p, buf [ MAXCODES ] ;

  /* if line to be output, convert to right format */

  if ( no > 0 )
    switch ( f->format ) {
    case O_PBM:
    case O_PGM:
    case O_PCL:
    case O_PS:
    case O_TIFF_RAW:
    case O_PCX:
    case O_PCX_RAW:
      nb = runtobit ( runs, nr, buf ) ;
      break ;
    case O_FAX:
    case O_TIFF_FAX:
      break ;
    }
  
  /* output `no' times. */
    
  while ( no-- > 0 ) {
    switch ( f->format ) {
    case O_PCX_RAW:
    case O_TIFF_RAW:
    case O_PBM:
      fwrite ( buf, 1, nb, f->f ) ;
      break ;
    case O_PGM:
      pgmwrite ( f, buf, nb ) ;
      break ;
    case O_TIFF_FAX:
    case O_FAX:
      p = runtocode ( &f->e, runs, nr, buf ) ;
      p = putcode ( &f->e, EOLCODE, EOLBITS, p ) ;
      nb = p - buf ;
      fwrite ( buf, 1, nb, f->f ) ;
      break ;
    case O_PCL:
      pclwrite ( f, buf, nb ) ;
      break ;
    case O_PS:
      pswrite ( f, buf, nb ) ;
      break ;
    case O_PCX:
      pcxwrite ( f, buf, nb ) ;
      break ;
    }

  /* only count lines/bytes for those formats that don't have
     headers or where we will update the headers on closing */

    switch ( f->format ) {
    case O_FAX:
    case O_TIFF_FAX:
    case O_TIFF_RAW:
    case O_PCX:
    case O_PCX_RAW:
      f->h++ ;
      f->bytes += nb ;
      break ;
    }
    
  }
}
