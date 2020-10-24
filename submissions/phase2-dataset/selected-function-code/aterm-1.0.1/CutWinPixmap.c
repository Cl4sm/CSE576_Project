Pixmap
CutWinPixmap (Window win, Drawable src, int src_w, int src_h, int width, int height, GC gc, ShadingInfo * shading)
{
  unsigned int x = 0, y = 0;


  if( src == None )
  {
#ifndef TRANSPARENT
    return None ;
#else
    if( !(Options & Opt_transparent))
	return None ;
#endif
  }

  if (!GetWinPosition (win, &x, &y))
	return None;

  return CutPixmap( src, None, x, y, src_w, src_h, width, height, gc, shading );
}
