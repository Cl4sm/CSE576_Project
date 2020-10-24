static int readFile(PixmapWidget PW, String filename)
#else
static int readFile(PW, filename)
    PixmapWidget PW;
    String filename;
#endif
{
    int status, i, shift = 0;
    XImage *image, *mask_image;
    XpmAttributes attribs;
    Pixel *pixel;
    XpmInfo infos;
    XpmImage xpm_image;
    XpmColor *color;

    InitializeXpmInfo(&infos);
    InitializeXpmImage(&xpm_image);
    InitializeXpmAttributes(PW, &attribs);
    infos.valuemask = XpmReturnComments | XpmReturnColorTable
      | XpmReturnExtensions;
    attribs.valuemask |= XpmReturnPixels;

    status = XpmReadFileToXpmImage(filename, &xpm_image, &infos);
    if (status == XpmSuccess)
      status = XpmCreateImageFromXpmImage(dpy, &xpm_image, &image,
					  &mask_image, &attribs);
    if (status == XpmSuccess) {
	/* Notify colors to be loaded with this pixmap */
	for (i = 0, color = xpm_image.colorTable, pixel = attribs.pixels;
	     i < xpm_image.ncolors; i++, color++, pixel++)
	  {
	      /* look for a defined color value */
	      char *c_name;
	      char **defaults = (char **)color;
	      int default_index, def_index;

	      switch (depth)
		  {
		  case 1:
		      default_index = 2;
		      break;
		  case 4:
		      default_index = 3;
		      break;
		  case 6:
		      default_index = 4;
		      break;
		  case 8:
		  default:
		      default_index = 5;
		      break;
		  }
	      def_index = default_index;
	      while (def_index > 1 && defaults[def_index] == NULL)
		  --def_index;
	      if (def_index < 2) {/* nothing towards mono, so try
				   * towards color */
		  def_index = default_index + 1;
		  while (def_index <= 5 && defaults[def_index] == NULL)
		      ++def_index;
	      }
	      if (def_index > 5) {
		  fprintf(stderr, "color #%d undefined, use black.\n", i + 1);
		  c_name = "black";
	      } else
		  c_name = defaults[def_index];

	      if (_PWDEBUG) {
		  printf("Pixel %d\n", *(pixel-shift));
		  printf("Color name %s\n", c_name);
		  printf("colorTable[%d][str] %s\n", i, color->string);
		  printf("colorTable[%d][s] %s\n", i, color->symbolic);
		  printf("colorTable[%d][m] %s\n", i, color->m_color);
		  printf("colorTable[%d][g4] %s\n", i, color->g4_color);
		  printf("colorTable[%d][g] %s\n", i, color->g_color);
		  printf("colorTable[%d][c] %s\n", i, color->c_color);
	      }
	      
	      if (strcasecmp(c_name, "none") == 0) 
	      {
		  PWUpdateColorInTable((Widget)PW, 
				       TRANSPARENT(dpy, screen), 
				       color->string, 
				       color->symbolic,
				       color->m_color,
				       color->g4_color,
				       color->g_color,
				       color->c_color);
		shift++;
	      }
	      else 
	      {
		  PWUseColorInTable((Widget)PW, 
				    *(pixel-shift),
				    color->string, 
				    color->symbolic,
				    color->m_color,
				    color->g4_color,
				    color->g_color,
				    color->c_color);
	      }
	  }	      

	if (PW->pixmap.hints_cmt) XtFree(PW->pixmap.hints_cmt);
	if (infos.hints_cmt) 
	  PW->pixmap.hints_cmt = XtNewString(infos.hints_cmt);
	else
	  PW->pixmap.hints_cmt = NULL;

	if (PW->pixmap.colors_cmt) XtFree(PW->pixmap.colors_cmt);
	if (infos.colors_cmt) 
	  PW->pixmap.colors_cmt = XtNewString(infos.colors_cmt);
	else
	  PW->pixmap.colors_cmt = NULL;

	if (PW->pixmap.pixels_cmt) XtFree(PW->pixmap.pixels_cmt);
	if (infos.pixels_cmt) 
	  PW->pixmap.pixels_cmt = XtNewString(infos.pixels_cmt);
	else
	  PW->pixmap.pixels_cmt = NULL;

        if (!mask_image) /* Xpm returns NULL when transp. is not used */
	  mask_image = CreateMaskImage(PW, image->width, image->height);
        else SetTransparentPixels(PW, image, mask_image);
      
	DestroyPixmapImage(&PW->pixmap.buffer);
	DestroyMaskImage(&PW->pixmap.mask_buffer);

	PW->pixmap.buffer = PW->pixmap.image;
        PW->pixmap.mask_buffer = PW->pixmap.mask_image;

	PW->pixmap.image = image;
        PW->pixmap.mask_image = mask_image;
	PW->pixmap.width = xpm_image.width;
	PW->pixmap.height = xpm_image.height;
      
        if (infos.valuemask & XpmHotspot)
	  {
	    PW->pixmap.hot.x = infos.x_hotspot;
	    PW->pixmap.hot.y = infos.y_hotspot;	
	  }
        else
	  {
	    PW->pixmap.hot.x = PW->pixmap.hot.y = NotSet;
	  }

        CopyExtensions( &PW->pixmap.extensions, &PW->pixmap.nextensions,
                        infos.extensions, infos.nextensions );
        if (PW->pixmap.extensionNotify)
            PW->pixmap.extensionNotify ((Widget) PW);

	PW->pixmap.changed = False;
	PW->pixmap.zooming = False;
	XpmFreeAttributes(&attribs);
	XpmFreeXpmImage(&xpm_image);
	XpmFreeXpmInfo(&infos);
    }
    return (status);
}
