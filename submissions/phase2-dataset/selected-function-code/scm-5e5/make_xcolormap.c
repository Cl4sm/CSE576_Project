     SCM sdpy;
     Colormap cmp;
{
  SCM z;
  struct xs_Colormap *xcm;
  XPointer scmptr;
  if (!XFindContext(XDISPLAY(sdpy), (XID)cmp, xtc_cmp, &scmptr))
    return (SCM)scmptr;
  DEFER_INTS;
  z = must_malloc_cell((long)sizeof(struct xs_Colormap),
		       (SCM)tc16_xcolormap,
		       s_colormap);
  xcm = COLORMAP(z);
  xcm->display = sdpy;
  xcm->dpy = DISPLAY(xcm->display)->dpy;
  xcm->cm = cmp;
  XSaveContext(XDISPLAY(sdpy), (XID)cmp, xtc_cmp, z);
  ALLOW_INTS;
  return z;
}
