int tgetd ( TFILE *f, int t )
{ 
  int c ;

  if ( ( c = tgetc(f,t) ) < 0 )
    c = EOF ;
  else
    if ( c != DLE )
      c = f->ibitorder[c] ;
    else {			/* escape sequence */
      c = tgetc(f,t) ;
      if ( c == ETX )
	c = -2 ;
      else
	if ( c == DLE || c == SUB )
	  c = f->ibitorder [ DLE ] ;
	else
	  c = msg ( "W0invalid escape sequence (DLE-%s) in data", cname(c) ) ;
    }
  
  return c ;
}
