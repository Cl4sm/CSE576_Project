int getformat ( uchar *p, int n )
{
  int format = 0 ;

  /* figure out file type if not already set */

  if ( ! format && n < 2 ) {
    format = I_TEXT ;
    msg ( "W only read %d byte(s) from input file, assuming text",  n ) ;
  } 

  if ( ! format && ! p[0] && ! ( p[1] & 0xe0 ) ) {
    format = I_FAX ;
  } 

  if ( ! format && ! strncmp ( p, "P4", 2 ) ) {
    format = I_PBM ;
  }

  if ( ! format && n >= 128 && p[0] == 0x0a && 
       strchr ("\02\03\05", p[1] ) && p[2] <= 1 ) {
    if ( p[65] != 1 ) {
      msg ( "E can't read colour PCX" ) ;
    } else {
      format = p[2] ? I_PCX : I_PCX ;
    }
  }

  if ( ! format && ! strncmp ( p, "%!", 2 ) ) {
    msg ( "W Postscript input file will be treated as text" ) ;
  }

  if ( ! format && ( p[0] == 'M' || p[1] == 'I' ) && ( p[1] == p[0] ) ) {
    format = I_TIFF ;
  }
  
  if ( ! format &&
       ( p[0] == 0x3a && p[1] == 0xde && 
	 p[2] == 0x68 && p[3] == 0xb1) ) {
    format = I_DCX ;
  }
  
  if ( ! format &&  n ) { /* "90% printable" heuristic */
    int i, nprint = 0 ;
    for ( i=0 ; i<n ; i++ ) {
      if ( isspace ( p[i] ) || isprint ( p[i] ) ) {
	nprint++ ;
      }
    }
    if ( ( nprint / (float) n ) > 0.90 ) {
      format = I_TEXT ;
    }
  }

  return format ;
}
