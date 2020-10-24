     SCM scmap, spxls, sargs;
{
  XColor xclr;
  Status sts;
  struct xs_Colormap *xcm;
  Bool contig = 0;
  SCM pxra, plra;
  unsigned int npixels, nplanes;
  ASRTER(NIMP(scmap) && COLORMAPP(scmap), scmap, ARG1, s_x_alloc_color_cells);
  xcm = COLORMAP(scmap);
  npixels = INUM(spxls);
  ASRTER(INUMP(spxls) && npixels > 0, spxls, ARG2, s_x_alloc_color_cells);
  pxra = make_uve(npixels, MAKINUM(32L)); /* Uniform vector of long */
  switch (ilength(sargs) + 2) {
  default: wta(sargs, (char *)WNA, s_x_alloc_color_cells);
  case 3: case 4:
    if (scm2XColor(CAR(sargs), &xclr)) {
      unsigned long rmask_return, gmask_return, bmask_return;
      sargs = CDR(sargs);
      if (NNULLP(sargs)) contig = thebool(CAR(sargs), s_x_alloc_color_cells);
      sts = XAllocColorPlanes(xcm->dpy, xcm->cm, contig,
			      VELTS(pxra), npixels,
			      xclr.red, xclr.green, xclr.blue,
			      &rmask_return, &gmask_return, &bmask_return);
      if (!sts) return BOOL_F;
      return cons2(pxra, MAKINUM(rmask_return),
		   cons2(MAKINUM(gmask_return),
			 MAKINUM(bmask_return), EOL));
    }
    nplanes = theuint(CAR(sargs), s_x_alloc_color_cells);
    sargs = CDR(sargs);
    if (NNULLP(sargs)) contig = thebool(CAR(sargs), s_x_alloc_color_cells);
    plra = make_uve(nplanes, MAKINUM(32L)); /* Uniform vector of long */
    sts = XAllocColorCells(xcm->dpy, xcm->cm, contig,
			   VELTS(plra), nplanes, VELTS(pxra), npixels);
    if (!sts) return BOOL_F;
    return cons2(pxra, plra, EOL);
  }
}
