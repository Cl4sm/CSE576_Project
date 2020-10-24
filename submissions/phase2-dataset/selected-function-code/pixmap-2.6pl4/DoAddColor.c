void DoAddColor()
{
  char *cname;
  
  if (PopupDialog(input_dialog, "Color name (name or #rgb) to add:",
		  "", &cname, XtGrabExclusive) == Okay)
    {
      XColor xcolor;
      
      if ((!XParseColor(dpy, cmap, cname, &xcolor)) ||
	  (!XAllocColor(dpy, cmap, &xcolor)))
	{
	  fprintf(stderr, 
		  "Warning: color %s could not be parsed/allocated!",
		  cname);
	}
      else if (PWGetColor(pixmap_widget, xcolor.pixel) == NULL)
	{
	  PWUseColorInTable(pixmap_widget, xcolor.pixel, NULL, NULL, NULL,
			    NULL, NULL, cname);
	}
    }
}
