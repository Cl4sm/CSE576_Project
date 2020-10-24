	       int w, int h, int lmargin,
	       short *runs, int *ppels )
{
  uchar *in, out [ MAXLINELEN * MAXFONTW / 8 + 1 ] ;
  int i, nc = 0, cw, nr, pels ;

  line = ( line * font->h + h/2 ) / h ;

  cw = font->w ;
  if ( line >= font->h ) line = font->h - 1 ;
  in = font->buf + 256/8 * cw * line ;

  copybits ( out, 0, 0 ) ;
  for ( i=0 ; txt[i] && i < MAXLINELEN ; i++ ) {
    copybits ( in, font->offset [ txt[i] ], cw ) ;
    nc++ ;
    while ( ( txt[i] == HT ) && ( nc & 7 ) ) { /* tab */
      copybits ( in, font->offset [ ' ' ], cw ) ;
      nc++ ;
    }
  }
  copybits ( 0, 0, 0 ) ;

  nr = bittorun ( out, ( nc*cw + 7 )/8, runs ) ;
  
  if ( font->w == w )
    pels = nc*cw ;
  else    
    pels = xscale ( runs, nr, ( w * 256 ) / font->w ) ;
  
  pels += xshift ( runs, nr, lmargin ) ;
  
  if ( ppels ) *ppels = pels ;

  return nr ;
}
