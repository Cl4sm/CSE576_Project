{
  char *c_name;
  char message[80];
  PWColorInfo *color = PWGetColor(pixmap_widget, current_color);

  sprintf(message, "Color `%s' [%d] color display name:", 
	  color->c_name, current_color);
/*  RetryColorCname: */
  if ((PopupDialog(input_dialog, message, 
		   (color->c_name ? color->c_name : ""), 
		   &c_name,
		   XtGrabExclusive) == Okay) && (c_name))
  {
      XColor xcolor;
      
/*      if (strcasecmp(c_name, NoColorName) == 0)
	{
	  if (PopupDialog(error_dialog, "Can't rename color to None",
			  NULL, NULL, XtGrabExclusive) == Retry)
	    goto RetryColorCname;
	}
      else*/ if ((strcasecmp(c_name, NoColorName) != 0) &&
		 (!XParseColor(dpy, cmap, c_name, &xcolor)))
	{
	  fprintf(stderr, "Warning: color %s could not be parsed!\n",
		  c_name);
	}
      else
	{
	  FixColorMenuLabel(color->c_name, c_name);
	  PWUpdateColorInTable(pixmap_widget, 
			       current_color,
			       color->symbol,
			       color->s_name,
			       color->m_name,
			       color->g4_name,
			       color->g_name,
			       c_name);
	}
    }
}
