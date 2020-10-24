#else
void PWSetText( w, s ) 
     Widget	w; 
     String	s; 
#endif
{
  PixmapWidget PW = (PixmapWidget) w;

  PW->pixmap.text_string = XtNewString( s );

  if ( PW->pixmap.font_struct && PW->pixmap.text_string ) {

      int          direction, ascent, descent;
      XCharStruct  size;

      XTextExtents( PW->pixmap.font_struct, 
                    PW->pixmap.text_string, strlen(PW->pixmap.text_string), 
                    &direction, &ascent, &descent, &size );

      PW->pixmap.text_rbearing = size.rbearing;
      PW->pixmap.text_lbearing = size.lbearing;
      PW->pixmap.text_ascent   = size.ascent;
      PW->pixmap.text_descent  = size.descent;
  }
}
