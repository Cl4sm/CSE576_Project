     Widget current, request, new;
{
  PixmapWidget p_old = (PixmapWidget) current;
  PixmapWidget p_new = (PixmapWidget) new;
  PixmapWidget p_req = (PixmapWidget) request;
  
  if ((p_old->pixmap.cursor != p_req->pixmap.cursor) && XtIsRealized(new))
    XDefineCursor(XtDisplay(new), XtWindow(new), p_new->pixmap.cursor);
  if (p_old->pixmap.squareW != p_req->pixmap.squareW)
    {
	if ((p_old->pixmap.proportional == True) &&
	    (p_new->pixmap.squareH != p_new->pixmap.squareW))
	    p_new->pixmap.squareH = p_new->pixmap.squareW;
	InternalResize(p_new);
    }
  
  if (p_old->pixmap.squareH != p_req->pixmap.squareH)
    {
	if ((p_old->pixmap.proportional == True) &&
	    (p_new->pixmap.squareH != p_new->pixmap.squareW))
	    p_new->pixmap.squareW = p_new->pixmap.squareH;
	InternalResize(p_new);
    }
  
    return FALSE;
}
