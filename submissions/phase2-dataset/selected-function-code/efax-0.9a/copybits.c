{
  uchar *f ;
  short bits ;
  static uchar *out ;
  static short x, shift ;
  static unsigned char right [ 9 ] = { 
    0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f, 0xff } ;
  
  if ( ! nb ) {				/* reset for new scan line */
    if ( in ) out = in ;		   
    else putbits ( 0, -shift ) ;	/* or flush bit buffer */
    x = 0 ;
    shift = -8 ;
  } else {
    f = in + ( from >> 3 ) ;
    bits = 8 - ( from & 7 ) ;

    if ( nb >= bits ) {
      putbits ( *f++ & right [ bits ], bits ) ;
      nb -= bits ;
    } else {
      putbits ( ( *f >> ( bits - nb ) ) & right [ bits ], nb ) ;
      nb = 0 ;
    } 

    while ( nb >= 8 ) { putbits ( *f++, 8 ) ; nb -= 8 ; }

    if ( nb > 0 ) putbits ( *f >> ( 8 - nb ), nb );
  }
}   
