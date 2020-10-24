void DoSymbolicName()
{
  char *s_name;
  char message[80];
  PWColorInfo *color = PWGetColor(pixmap_widget, current_color);
  
  sprintf(message, "Color `%s' [%d] symbolic name:", 
	  color->c_name, current_color);
  
  if (PopupDialog(input_dialog, message, 
		  (color->s_name ? color->s_name : ""), 
		  &s_name,
		  XtGrabExclusive) == Okay)
    PWUpdateColorInTable(pixmap_widget, 
			 current_color,
			 color->symbol,
			 s_name,
			 color->m_name,
			 color->g4_name,
			 color->g_name,
			 color->c_name);
}
