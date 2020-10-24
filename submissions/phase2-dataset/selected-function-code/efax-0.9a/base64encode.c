{
  int err=0, c ;
  uchar n=0, m=0, bits=0 ;

  static uchar chartab[65] = 
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ" 
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/" ;

  while ( ( c = fgetc ( stdin ) ) >= 0 ) {
    switch ( n ) {
    case 0:
      putc ( chartab [ c >> 2 ], stdout ) ;
      bits = c & 0x3 ;
      n = 1 ;
      break ;
    case 1:
      putc ( chartab [ (bits << 4) | ( c >> 4 ) ], stdout ) ;
      bits = c & 0xf ;
      n = 2 ;
      break ;
    case 2:
      putc ( chartab [ (bits << 2) | ( c >> 6 ) ], stdout ) ;
      putc ( chartab [ c & 0x3f ], stdout ) ;
      n = 0 ;
      if ( ++m >= 18 ) {
	putc ( '\n', stdout ) ;
	m = 0 ;
      }
      break ;
    }

  }

  switch ( n ) {
  case 0:
    break ;
  case 1:
    putc ( chartab [ (bits << 4) | ( 0 >> 4 ) ], stdout ) ;
    putc ( '=', stdout ) ;
    putc ( '=', stdout ) ;
    break ;
  case 2 :
    putc ( chartab [ (bits << 2) | ( 0 >> 6 ) ], stdout ) ;
    putc ( '=', stdout ) ;
    break ;
  }

  putc ( '\n', stdout ) ;

  return err ;
}
