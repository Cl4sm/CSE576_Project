{
  int i, n, nc=0, err=0 ;
  char c ;
  static char *unitstr[] = { " 0in", " 1cm", " 2mm", " 3pt", 0 } ;
  static float unitval[] = { 1.0, 2.54, 25.4, 72.0, 1.0 } ;

  if ( ! arg ) 
    err = msg ( "E2 missing argument" ) ;

  if ( !x || !y )
    err = msg ( "E2 can't happen (getxy)" ) ;

  if ( ! err ) {
    n = sscanf ( arg , "%f%c%f%n", x, &c, y, &nc ) ;
    switch ( n ) {
    case 0 : err = msg ( "E2bad X value in (%s)", arg ) ; break ;
    case 2 : err = msg ( "E2bad Y value in (%s)", arg ) ; break ;
    }      
  }

  if ( ! err ) {
    if ( dim ) {
      if ( n != 3 ) {
	err = msg ( "Emissing Y dimension in (%s)", arg ) ;
      } else {
	while ( arg [ nc ] && isspace ( arg [ nc ] ) ) nc++ ;
	if ( arg [ nc ] ) {
	  if ( ( i = lookup ( unitstr, arg+nc ) ) >= 0 ) {
	    *x /= unitval [ i ] ;
	    *y /= unitval [ i ] ;
	  } else {
	    err = msg ( "E2bad units: `%s'", arg+nc ) ;
	  }
	}
      }
    } else {
      if ( n == 1 ) *y = *x ;
    }
  }
  
  if ( ! err )
    msg ( "Aconverted (%s) into %f x %f", arg, *x, *y ) ;

  return err ;
}
