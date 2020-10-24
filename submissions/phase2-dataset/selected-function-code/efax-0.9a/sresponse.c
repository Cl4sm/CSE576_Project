{
  char *p, *r = 0 ;
  int lens, lenr ;
  
  lens = strlen ( s ) ;
  for ( p=responses ; p<lresponse ; p += strlen(p) + 1 ) {

    if ( ! strncmp ( p, s, lens ) ) {
      r = p + lens ;

      lenr = strlen ( r ) ;
      if ( strspn ( r, " \r\n" ) == lenr && r+lenr < lresponse ) r += lenr ;

      if ( ip && sscanf ( r, "%d", ip ) > 0 )
	msg ( "R read value %d from \"%s\"", *ip, r ) ;
    }
    
  }

  return r ;
}
