void pgmwrite ( OFILE *f, uchar *buf, int n )
{
  static uchar gval [ MAXBITS * 8 / 4 ] ;
  static int init=0, lines=0 ;
  static uchar hbits [ 256 ], lbits [ 256 ] ;
  static int nybblecnt [ 16 ] = { 0,1,1,2, 1,2,2,3, 1,2,2,3, 2,3,3,4 } ;
  static uchar corr [ 17 ] = { 255, 239, 223, 207, 191, 175, 159, 143, 127, 
				111,  95,  79,  63,  47,  31,  15,   0 } ;
  int m ;
  uchar *p, *q ; 

  if ( ! init ) {	    /*  build table of bit counts in each nybble */
    short i ;
    for ( i=0 ; i<256 ; i++ ) {
	hbits [ i ] = nybblecnt [ i >> 4 & 0x0f ] ;
	lbits [ i ] = nybblecnt [ i & 0x0f ] ;
      }
    init = 1 ;
  }

  for ( m=n, p=gval, q=buf ; m-- > 0 ; q++ ) {
    *p++ += hbits [ *q ] ;
    *p++ += lbits [ *q ] ;
  }
  
  if ( ( lines++ & 0x03 ) == 0x03 ) {
    for ( p=gval, m=2*n ; m-- > 0 ; p++ ) *p = corr [ *p ] ;
    fwrite ( gval,  1, 2*n, f->f ) ;
    memset ( gval,  0, 2*n ) ;
  }
}
