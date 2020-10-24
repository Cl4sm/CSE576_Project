     Widget w;
     Pixel pixel;
     char *name;
{
  Widget wi;
  int max_colors = 1<<DisplayPlanes(dpy,screen);
  
  if ((pixel != TRANSPARENT(dpy, screen)) && (pixel >= max_colors))
    printf(
    "Warning: Color pixel %d out of bounds for display (pixel range [0->%d]\n",
	   pixel, max_colors-1);
  else
    {
      int *id = (int *)XtMalloc(sizeof(int));
      Arg wargs[1];

      *id = pixel + Color_offset;

      /* Two kinds of colors: not transparent and transparent */
      if (pixel != TRANSPARENT(dpy, screen))
      {
#ifndef USE_ATHENA
	  XtSetArg(wargs[0], XmNbackground, pixel);
#else /* USE_ATHENA */
	  XtSetArg(wargs[0], XtNbackground, pixel);
#endif /* USE_ATHENA */
	  
#ifdef DEBUG
	  printf("Adding color %d %s to menu\n", pixel, name);
#endif /* DEBUG */
	  
#ifndef USE_ATHENA
	  wi = (Widget)XmCreatePushButton(color_pane_widget, name,
					  wargs, 1);
	  XtAddCallback(wi, XmNactivateCallback, TheCallback, id);
	  XtManageChild(wi);
#else /* USE_ATHENA */
	  wi = XtCreateManagedWidget(name, commandWidgetClass, 
				     color_pane_widget, wargs, 1);
	  XtAddCallback(wi, XtNcallback, TheCallback, id);
#endif /* USE_ATHENA */
      }
      else if (pixel == TRANSPARENT(dpy, screen))
      {
#ifndef USE_ATHENA
	  XtSetArg(wargs[0], XmNbackground,
		   PWGetTransparentPixel(w));
	  wi = (Widget)XmCreatePushButton(color_pane_widget, name,
					  wargs, 1);
	  XtAddCallback(wi, XmNactivateCallback, TheCallback, id);
	  XtManageChild(wi);
#else /* USE_ATHENA */
	  XtSetArg(wargs[0], XtNbackground,
		   PWGetTransparentPixel(w));
	  wi = XtCreateManagedWidget(name, commandWidgetClass, 
				     color_pane_widget, wargs, 1);
	  XtAddCallback(wi, XtNcallback, TheCallback, id);
#endif /* USE_ATHENA */
      }
  }
}
