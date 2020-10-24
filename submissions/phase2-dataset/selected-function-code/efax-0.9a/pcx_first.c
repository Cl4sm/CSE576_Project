{
  int err=0, xmin, xmax, ymin, ymax, nc, nb ;
  long start ;

  start = ftell ( f->f ) ;

  fseek ( f->f, start+4, SEEK_SET ) ;
  xmin = fgeti ( f ) ;
  ymin = fgeti ( f ) ;
  xmax = fgeti ( f ) ;
  ymax = fgeti ( f ) ;

  f->page->w = xmax - xmin + 1 ;
  f->page->h = ymax - ymin + 1 ;

  f->page->xres = fgeti ( f ) ;
  f->page->yres = fgeti ( f ) ;

  fseek ( f->f, start+0x41, SEEK_SET ) ;
  nc = fgetc ( f->f ) ;
  nb = fgeti ( f ) ;

  if ( nc != 1 ) 
    msg ( "W mono PCX file has %d colour planes", nc ) ;

  if ( nb != ( f->page->w + 15 ) / 16 * 2 )
    msg ( "W PCX file has %d bytes per scan line for %d pels",
	  nb, f->page->w ) ;

  f->page->offset = start + 128 ;
  f->page->format = P_PCX ;

  return err ;
}
