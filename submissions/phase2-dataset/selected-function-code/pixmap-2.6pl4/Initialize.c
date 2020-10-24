    PixmapWidget request, new;
    ArgList argv;
    Cardinal argc;
{
    XGCValues  values;
    XtGCMask   mask;

    dpy = XtDisplay(new);
    screen = DefaultScreen(dpy);
    depth = DefaultDepth(dpy,screen);

    if (max_ncolors == -1)
      max_ncolors = xpmReadRgbNames(rgb_fname, rgb_table);

    /* allocate max colors + 1 colorTable entries because 0 is transparent */
    if (depth <= MAX_DEPTH) {
      new->pixmap.colorTable = (PWColorInfo **) XtCalloc((1<<depth) + 1, 
							 sizeof(PWColorInfo*));
    } else {
      new->pixmap.colorTable = XtNew(PWColorInfo *);
      *(new->pixmap.colorTable) = NULL;
    }
    
    new->pixmap.notify = NULL;
    new->pixmap.cardinal = 0;
    new->pixmap.current = 0;
    new->pixmap.fold = False;
    new->pixmap.changed = False;
    new->pixmap.zooming = False;
    new->pixmap.selection.own = False;
    new->pixmap.selection.limbo = False;
    new->pixmap.clear_pixel = WhitePixelOfColormap(dpy, new->core.colormap);
    new->pixmap.hints_cmt = NULL;
    new->pixmap.colors_cmt = NULL;
    new->pixmap.pixels_cmt = NULL;
    new->pixmap.filename = XtNewString(new->pixmap.filename);
    new->pixmap.status = NULL;

    new->pixmap.request_stack = (PWRequestStack *)
	XtMalloc(sizeof(PWRequestStack));
    new->pixmap.request_stack[0].request = NULL;
    new->pixmap.request_stack[0].call_data = NULL;
    new->pixmap.request_stack[0].trap = False;

    new->core.width = new->pixmap.width * new->pixmap.squareW + 
	2 * new->pixmap.distance;
    new->core.height = new->pixmap.height * new->pixmap.squareH + 
	2 * new->pixmap.distance;

    new->pixmap.extensions = NULL;
    new->pixmap.nextensions = 0;
    new->pixmap.buffer_extensions = NULL;
    new->pixmap.buffer_nextensions = 0;

    new->pixmap.text_string = NULL;
    new->pixmap.font_struct = NULL;
/*
    PWSetFont(new, "-*-fixed-medium-*-*-*-13-*-*-*-*-70-*-*" );
*/
  
    new->pixmap.hot.x = new->pixmap.hot.y = NotSet;
    new->pixmap.buffer_hot.x = new->pixmap.buffer_hot.y = NotSet;
    
    new->pixmap.mark.from_x = new->pixmap.mark.from_y = NotSet;
    new->pixmap.mark.to_x = new->pixmap.mark.to_y = NotSet;
    new->pixmap.buffer_mark.from_x = new->pixmap.buffer_mark.from_y = NotSet;
    new->pixmap.buffer_mark.to_x = new->pixmap.buffer_mark.to_y = NotSet;

    values.foreground = new->pixmap.foreground_pixel;
    values.background = new->core.background_pixel;
    values.foreground ^= values.background;
    values.function = GXcopy;
    values.plane_mask = AllPlanes;
    mask = GCForeground | GCBackground | GCFunction | GCPlaneMask;
    new->pixmap.drawing_gc = XCreateGC(dpy, RootWindow(dpy,screen), 
				       mask, &values);

    values.foreground = new->pixmap.highlight_pixel;
    values.background = new->core.background_pixel;
    values.foreground ^= values.background;
    values.function = GXxor;
    values.plane_mask = AllPlanes;
    mask = GCForeground | GCBackground | GCFunction | GCPlaneMask;
    new->pixmap.highlighting_gc = XCreateGC(dpy, RootWindow(dpy, screen),
					    mask, &values);

    values.foreground = new->pixmap.framing_pixel;
    values.background = new->core.background_pixel;
    values.foreground ^= values.background;
    values.function = GXxor;
    values.plane_mask = AllPlanes;
    mask = GCForeground | GCBackground | GCFunction | GCPlaneMask;
    new->pixmap.framing_gc = XCreateGC(dpy, RootWindow(dpy, screen),
				       mask, &values);

    values.foreground = new->pixmap.transparent_pixel;
    values.background = new->pixmap.clear_pixel;
    values.function = GXcopy;
    mask = GCForeground | GCBackground | GCFunction;
    if (new->pixmap.stipple != XtUnspecifiedPixmap)
      {
	values.stipple = new->pixmap.stipple;
	mask |= GCStipple | GCFillStyle;
      }
    values.fill_style = (new->pixmap.stippled ? 
			 FillOpaqueStippled : FillSolid);
    new->pixmap.transparent_gc = XCreateGC(dpy, RootWindow(dpy, screen),
					   mask, &values);
    
    new->pixmap.storage = NULL;
    new-> pixmap.mask_storage = NULL;
    
    new->pixmap.image = CreatePixmapImage(new, 
					  new->pixmap.width,
					  new->pixmap.height);
    new->pixmap.mask_image = CreateMaskImage(new,
					     new->pixmap.width,
					     new->pixmap.height);
    
    new->pixmap.buffer = CreatePixmapImage(new, 
					   new->pixmap.width,
					   new->pixmap.height);
    new->pixmap.mask_buffer = CreateMaskImage(new, 
					      new->pixmap.width,
					      new->pixmap.height);
    
    /* add transparent pixel in ColorTable */
    PWUseColorInTable((Widget)new, TRANSPARENT(dpy, screen), NULL,
		      NULL, NULL, NULL, NULL, NoColorName);
    
    /* add clear pixel in ColorTable */
    PWUseColorInTable((Widget)new, new->pixmap.clear_pixel, NULL,
		      NULL, NULL, NULL, NULL, "white");
    
    /* Read file */
    readFile(new, new->pixmap.filename);

    InternalResize(new);
    Resize(new);
}
