void psinit ( OFILE *f, int newfile, int page, int w, int h, int n )
{
  float ptw, pth ;

  if ( ! f ) {
    msg ( "E2 can't happen (psinit)" ) ;
    return ;
  }

  ptw = w/f->xres * 72.0 ;		   /* convert to points */
  pth = h/f->yres * 72.0 ;

  if ( newfile )
    fprintf ( f->f, PSBEGIN, 
	    (int) ptw, (int) pth,		 /* Bounding Box */
	    n ) ;				 /* buffer string length */

  fprintf ( f->f, PSPAGE, 
	  page, page,				 /* page number */
	  0.0, 0.0,				 /* shift */
	  ptw, pth,				 /* scaling */
	  w, h, 1,				 /* image size */
	  w, 0, 0, -h, 0, h ) ;			 /* CTM */
  f->pslines = 0 ;
  f->lastpageno = page ;
}
