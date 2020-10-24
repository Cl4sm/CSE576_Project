void PWComments(Widget w, char **hints_cmt, char **colors_cmt, 
		char **pixels_cmt)
#else
void PWComments(w, hints_cmt, colors_cmt, pixels_cmt)
     Widget w;
     char **hints_cmt, **colors_cmt, **pixels_cmt;
#endif
{
  PixmapWidget PW = (PixmapWidget)w;
  
  if ((*hints_cmt) && (PW->pixmap.hints_cmt))
    {
      XtFree(PW->pixmap.hints_cmt);
      PW->pixmap.hints_cmt = XtNewString(*hints_cmt);
    }
  else if (*hints_cmt) PW->pixmap.hints_cmt = XtNewString(*hints_cmt);
  else *hints_cmt = XtNewString(PW->pixmap.hints_cmt);

  if ((*colors_cmt) && (PW->pixmap.colors_cmt))
    {
      XtFree(PW->pixmap.colors_cmt);
      PW->pixmap.colors_cmt = XtNewString(*colors_cmt);
    }
  else if (*colors_cmt) PW->pixmap.colors_cmt = XtNewString(*colors_cmt);
  else *colors_cmt = XtNewString(PW->pixmap.colors_cmt);

  if ((*pixels_cmt) && (PW->pixmap.pixels_cmt))
    {
      XtFree(PW->pixmap.pixels_cmt);
      PW->pixmap.pixels_cmt = XtNewString(*pixels_cmt);
    }
  else if (*pixels_cmt) PW->pixmap.pixels_cmt = XtNewString(*pixels_cmt);
  else *pixels_cmt = XtNewString(PW->pixmap.pixels_cmt);
}
