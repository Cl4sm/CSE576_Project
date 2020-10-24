{
  int err=0, c, over ;
  uchar *order = f->obitorder ;
  uchar buf [ MINWRITE+1 ] ;
  int i ;

  for ( i=0 ; ! err && n > 0 ; n-- ) {
    c  = order [ *p++ ] ;
    if ( c == DLE ) buf[i++] = DLE ;
    buf[i++] = c ;
    if ( i >= MINWRITE || n == 1 ) {

      /* ``virtual'' flow control */

      if ( vfc && dcecps > 0 ) {
	over = f->bytes - ( proc_ms ( ) - f->mstart ) * dcecps / 1000 
	  - MAXDCEBUF ;
	if ( over > 0 ) msleep ( over * 1000 / dcecps ) ;
      }

      if ( tput ( f, buf, i ) < 0 )
	err = msg ( "ES2fax device write error:" ) ;

      i = 0 ;
    }
  }

  return err ;
}
