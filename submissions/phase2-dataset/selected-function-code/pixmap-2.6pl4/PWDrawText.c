#else
void PWDrawText(w, at_x, at_y, value)
     Widget	w;
     Position	at_x, at_y;	/* text origin */
     int        value;
#endif
{
  PixmapWidget PW = (PixmapWidget) w;

  Pixmap     pixmap;
  XImage     *image;
  GC         gc;
  XGCValues  gcv;
  Position   x, y, i, j;
  Dimension  width, height;

  if ( ! PW->pixmap.text_string || 
       ! PW->pixmap.font_struct ||
       strlen(PW->pixmap.text_string) == 0 )
      return;

  /* 
     To draw text: 
     put text in tmp pixmap of depth 1 then get image 
     foreach pixel in image, if set draw into pixmapwidget 
  */

  /* text extent, add 1 pixel pad because fonts vary is how ascent is defined */
  width  = PW->pixmap.text_rbearing - PW->pixmap.text_lbearing + 2;
  height = PW->pixmap.text_ascent + PW->pixmap.text_descent + 2;

  /* create white pixmap size of text extent with depth one */
  pixmap = XCreatePixmap( dpy, XtWindow(PW), width, height, 1 );
  gcv.font       = PW->pixmap.font_struct->fid;
  gcv.function   = GXcopy;
  gcv.foreground = WhitePixel( dpy, screen );
  gcv.background = WhitePixel( dpy, screen );
  gc = XCreateGC( dpy, pixmap, 
                  GCFunction | GCFont | GCForeground | GCBackground, 
                  &gcv );
  XFillRectangle( dpy, pixmap, gc, 0, 0, width, height ); /* clear */
  
  /* draw text in pixmap */
  x = (PW->pixmap.text_lbearing < 0) ? - PW->pixmap.text_lbearing : 0;
  y = PW->pixmap.text_ascent /* - 1 */; /* include pad */
  XSetForeground( dpy, gc, BlackPixel(dpy, screen) );
  XDrawString( dpy, pixmap, gc, x, y,
               PW->pixmap.text_string, strlen(PW->pixmap.text_string) );

  /* get image of text */
  image = XGetImage(dpy, pixmap, 0, 0, width, height, -1/*allplanes*/,ZPixmap);

  /* translate (at_x,at_y) from text origin to upper left of text extent */
  x = at_x;
  y = at_y;
  x += PW->pixmap.text_lbearing;
  y -= PW->pixmap.text_ascent - 1;

  /* foreach set pixel in image, draw into pixmapwidget */
  for (i = 0; i < (Position)width; i++) {
      for (j = 0; j < (Position)height; j++) {

          if (!QueryInPixmap(PW, x + i, y + j)) continue;

          if ( XGetPixel(image, i, j) == BlackPixel(dpy, screen) ) {
              PWDrawPoint( (Widget) PW, x + i, y + j, value );
          }
      }
  }
  
  /* clean up */
  XFreePixmap( dpy, pixmap );
  XFreeGC( dpy, gc );
  XDestroyImage( image );
}
