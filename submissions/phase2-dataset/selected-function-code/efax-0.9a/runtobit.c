int runtobit ( short *runs, int nr, uchar *buf )
{
  static uchar zerofill [ 9 ] = { 
    0xff,  0xfe, 0xfc, 0xf8, 0xf0,  0xe0, 0xc0, 0x80, 0x00 } ;
  static uchar onefill [ 9 ] = { 
    0x00,  0x01, 0x03, 0x07, 0x0f,  0x1f, 0x3f, 0x7f, 0xff } ; 

  uchar col=0, *buf0 = buf ;
  register short len, b=8, bytes ;
  
  while ( nr-- > 0 ) {
    len = *runs++ ;
    if ( col ) *buf |= onefill  [ b ] ;		 /* right bits of cur. byte */
    else       *buf &= zerofill [ b ] ;
    if ( b > len ) {				 /* done fill */
      b -= len ;
    } else {					 /* continue to next byte */
      len -= b ; 
      buf++ ; 
      b = 8 ;
      if ( ( bytes = len>>3 ) > 0 ) {		 /* fill >1 byte */
	memset ( buf, col, bytes ) ;
	len -= bytes*8; 
	buf += bytes ;
      } 
      *buf = col ;				 /* flood the rest */
      b -= len ;
    }
    col ^= 0xff ;
  }

  return buf - buf0 + ( b < 8 ) ;
}
