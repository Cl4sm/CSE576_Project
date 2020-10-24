{
  char *m_name;
  char message[80];
  PWColorInfo *color = PWGetColor(pixmap_widget, current_color);
  
  sprintf(message, "Color `%s' [%d] monochrome display name:", 
	  color->c_name, current_color);
  
  if (PopupDialog(input_dialog, message, 
		  (color->m_name ? color->m_name : ""), 
		  &m_name,
		  XtGrabExclusive) == Okay)
  {
      XColor xcolor;
      
      if (!XParseColor(dpy, cmap, m_name, &xcolor))
	{
	  fprintf(stderr, "Warning: color %s could not be parsed!\n",
		  m_name);
	}
      else
	PWUpdateColorInTable(pixmap_widget, 
			     current_color,
			     color->symbol,
			     color->s_name,
			     m_name,
			     color->g4_name,
			     color->g_name,
			     color->c_name);
  }
}
