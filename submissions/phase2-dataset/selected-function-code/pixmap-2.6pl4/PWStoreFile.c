int PWStoreFile(Widget w, String filename)
#else
int PWStoreFile(w, filename)
    Widget w;
    String filename;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
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

    infos.valuemask = XpmReturnColorTable;
    attribs.valuemask |= XpmReturnPixels;

    if ((status = XpmReadFileToXpmImage(filename, &xpm_image, &infos))
	== XpmSuccess)
      status = XpmCreateImageFromXpmImage(dpy, &xpm_image, &image,
					  &mask_image, &attribs);
    if (status == XpmSuccess) {
	/* Notify colors to be loaded with this pixmap */
	for (i = 0, color = xpm_image.colorTable, pixel = attribs.pixels;
	     i < xpm_image.ncolors; i++, color++, pixel++)
	{
	    char *c_name;
		    
	    switch (depth)
	      {
	      case 1:
		  c_name = color->m_color;
		  break;
	      case 4:
		  c_name = color->g4_color;
		  break;
	      case 6:
		  c_name = color->g_color;
		  break;
	      case 8:
	      default:
		  c_name = color->c_color;
		  break;
	      }
	    
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
		shift = 1;
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

        if (!mask_image) /* Xpm returns NULL when transp. is not used */
	  mask_image = CreateMaskImage(PW, image->width, image->height);
        else SetTransparentPixels(PW, image, mask_image);
	
        DestroyPixmapImage(&PW->pixmap.storage);
        DestroyMaskImage(&PW->pixmap.mask_storage);

        PW->pixmap.storage = image;
        PW->pixmap.mask_storage = mask_image;
    
        if (infos.valuemask & XpmHotspot)
	  {
	    PW->pixmap.storage_hot.x = infos.x_hotspot;
	    PW->pixmap.storage_hot.y = infos.y_hotspot;	
	  }
        else
	  {
	    PW->pixmap.storage_hot.x = PW->pixmap.storage_hot.y = NotSet;
	  }

	XpmFreeAttributes(&attribs);
	XpmFreeXpmImage(&xpm_image);
	XpmFreeXpmInfo(&infos);  
    }
    else
	XtAppWarning(XtWidgetToApplicationContext(w),
		     " read file failed.  PixmapWidget");
    
    return status;
}
