void PWUpdateColorInTable(Widget w, Pixel pixel, char *symbol, char *sname, 
		       char *mname, char *g4name, char *gname, char *cname)
#else
void PWUpdateColorInTable(w, pixel, symbol, sname, mname, g4name, gname, cname)
     Widget w;
     Pixel pixel;
     char *symbol, *sname, *mname, *g4name, *gname, *cname;
#endif
     /* name are not used as is, instead memory is malloc'ed to fit in */
{
  PixmapWidget PW = (PixmapWidget) w;
  PWColorInfo *color = PWGetColor(w, pixel);
  
  if (color == NULL) return; /* inexistent color in Table */
  /* Update the color info in ColorTable */
  if ((symbol) && ((!color->symbol) || (strcmp(symbol, color->symbol))))
    {
      if (color->symbol) 
	XtFree(color->symbol);
      color->symbol = XtNewString(symbol);
    }
  if ((sname) && ((!color->s_name) || (strcmp(sname, color->s_name))))
    {
      if (color->s_name) 
	XtFree(color->s_name);
      color->s_name = XtNewString(sname);
    }
  if ((mname) && ((!color->m_name) || (strcmp(mname, color->m_name))))
    {
      if (color->m_name) 
	XtFree(color->m_name);
      color->m_name = XtNewString(mname);
    }
  if ((g4name) && ((!color->g4_name) || (strcmp(g4name, color->g4_name))))
    {
      if (color->g4_name) 
	XtFree(color->g4_name);
      color->g4_name = XtNewString(g4name);
    }
  if ((gname) && ((!color->g_name) || (strcmp(gname, color->g_name))))
    {
      if (color->g_name) 
	XtFree(color->g_name);
      color->g_name = XtNewString(gname);
    }
  if ((cname) && ((!color->c_name) ||
		   ((strcmp(cname, color->c_name)) &&
		    ((cname[0] != '#') || (color->c_name[0] == '#')))))
    {
      if (color->c_name) 
	XtFree(color->c_name);

      /* try to get a name from rgb_table */
      if (cname[0] == '#')
	{
	  XColor xcolor;
	  char *rgb_name;

	  xcolor.pixel = pixel;
	  XQueryColor(dpy, PW->core.colormap, &xcolor);
	  rgb_name = xpmGetRgbName(rgb_table, max_ncolors, 
				   (int) xcolor.red, 
				   (int) xcolor.green, 
				   (int) xcolor.blue);

	  if (rgb_name)
	    cname=rgb_name;
	}

      color->c_name = XtNewString(cname);
    }
  else if ((!cname) && (!color->c_name))
    {
      XColor xcolor;
      char *rgb_name;

      xcolor.pixel = pixel;
      XQueryColor(dpy, PW->core.colormap, &xcolor);
      
      if (!(rgb_name = xpmGetRgbName(rgb_table, max_ncolors,
				     (int) xcolor.red,
				     (int) xcolor.green,
				     (int) xcolor.blue)))
	{
	  color->c_name = (char *)XtMalloc(15*sizeof(char));
	  sprintf(color->c_name, "#%04X%04X%04X", 
		  xcolor.red, xcolor.green, xcolor.blue);
	} else {
	  color->c_name = XtNewString(rgb_name);
	}
    }
  
}
