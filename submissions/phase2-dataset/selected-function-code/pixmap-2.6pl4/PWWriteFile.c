#else
int PWWriteFile(w, filename)
    Widget w;
    String filename;
#endif
{
    PixmapWidget PW = (PixmapWidget) w;
    XImage *image, *mask_image;
    XPoint hot;
    int status;
    XpmImage *xpm_image;
    XpmInfo *infos;
    
    if (PW->pixmap.zooming) {
	image = CreatePixmapImage(PW,
				  (Dimension) PW->pixmap.zoom.image->width,
				  (Dimension) PW->pixmap.zoom.image->height);
	CopyImageData(PW->pixmap.zoom.image, image, 0, 0, 
		      (Dimension) PW->pixmap.zoom.image->width,
		      (Dimension) PW->pixmap.zoom.image->height, 0, 0);
	CopyImageData(PW->pixmap.image, image, 
		      0, 0, 
		      PW->pixmap.image->width - 1,
		      PW->pixmap.image->height - 1,
		      PW->pixmap.zoom.at_x, PW->pixmap.zoom.at_y);

	mask_image = CreateMaskImage(PW,
				     (Dimension)PW->pixmap.zoom.image->width,
				     (Dimension)PW->pixmap.zoom.image->height);
	CopyImageData(PW->pixmap.zoom.mask_image, mask_image, 0, 0, 
		      (Dimension) PW->pixmap.zoom.mask_image->width,
		      (Dimension) PW->pixmap.zoom.mask_image->height, 0, 0);
	CopyImageData(PW->pixmap.mask_image, mask_image, 
		      0, 0, 
		      PW->pixmap.mask_image->width - 1,
		      PW->pixmap.mask_image->height - 1,
		      PW->pixmap.zoom.at_x, PW->pixmap.zoom.at_y);

	if (QuerySet(PW->pixmap.hot.x, PW->pixmap.hot.y))
	  {
	    hot.x = PW->pixmap.hot.x + PW->pixmap.zoom.at_x;
	    hot.y = PW->pixmap.hot.y + PW->pixmap.zoom.at_y;
	  }
	else hot = PW->pixmap.zoom.hot;
    }
    else {
	image = PW->pixmap.image;
	mask_image = PW->pixmap.mask_image;
	hot = PW->pixmap.hot;
    }
    
    if (filename) 
      {
	XtFree(PW->pixmap.filename);
	PW->pixmap.filename = XtNewString(filename);
      }

    if (_PWDEBUG)
	fprintf(stderr, "Saving filename: %s\n", filename);

    buildXpmImageAndAttributes(PW, image, mask_image,
			       &xpm_image, &infos);
    if (QuerySet(hot.x, hot.y))
      {
	infos->valuemask |= XpmHotspot;
	infos->x_hotspot = hot.x;
	infos->y_hotspot = hot.y;
      }
    
    /* need to account for zooming for ports ?? */
    infos->valuemask |= XpmExtensions;	/* save extensions */
    CopyExtensions( &infos->extensions, &infos->nextensions,
                    PW->pixmap.extensions, PW->pixmap.nextensions );
    
    status = XpmWriteFileFromXpmImage(PW->pixmap.filename, xpm_image,
				      infos);

    XpmFreeXpmImage(xpm_image);
    XtFree((char *)xpm_image);
    XpmFreeXpmInfo(infos);
    XtFree((char *)infos);
    
    if (PW->pixmap.zooming) {
	DestroyPixmapImage(&image);
	DestroyMaskImage(&mask_image);
    }
        
    if (status == XpmSuccess)
      PW->pixmap.changed = False;
    
    return status;
}
