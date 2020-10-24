void PWUseColorInTable(Widget w, Pixel pixel, char *symbol, char *sname, 
		       char *mname, char *g4name, char *gname, char *cname)
#else
void PWUseColorInTable(w, pixel, symbol, sname, mname, g4name, gname, cname)
     Widget w;
     Pixel pixel;
     char *symbol, *sname, *mname, *g4name, *gname, *cname;
#endif
     /* name are not used as is, instead memory is malloc'ed to fit in */
{
  PixmapWidget PW = (PixmapWidget) w;
  Boolean is_new_color = False;
  PWColorInfo *new_color;

  if (PWGetColor(w, pixel) == NULL)  /* not yet used color 
					probably not in colorTable */
    {
      is_new_color = True;

      new_color = XtNew(PWColorInfo);
      
      new_color->symbol = 0;
      new_color->s_name = 0;
      new_color->m_name = 0;
      new_color->g4_name = 0;
      new_color->g_name = 0;
      new_color->c_name = 0;
      new_color->pixel = pixel;
      new_color->left = NULL;
      new_color->right = NULL;

      PWAddColor(w, new_color);
    }
  
  PWUpdateColorInTable(w, pixel, symbol, sname, mname, g4name, 
		       gname, cname);

  if (is_new_color && 
      (PW->pixmap.AddColorNotify != (AddColorNotifyProc) 0))
    PW->pixmap.AddColorNotify(w, pixel, new_color->c_name);
}
