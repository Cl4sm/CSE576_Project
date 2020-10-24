{
  FILE *colorfile;
  static char *fname = ".pixmap";
  char filename[256];


  /* first try to open in local dir */
  if (!(colorfile = fopen(fname, "r")))
    { /* try in homedir */
      sprintf(filename, "%s/%s", getenv("HOME"), fname);
      if (!(colorfile = fopen(filename, "r")))
	{ /* try in X11 lib dir */
	  sprintf(filename, "%s/%s", APPDEFDIR , fname);
	  colorfile = fopen(filename, "r");
	}
    }
  
  if (colorfile) 
    { /* parse it, i.e., read name, try to alloc in cmap, add menu entry */
      char cname[512];
      int status;
      XColor xcolor;
      
      while ((status = fscanf(colorfile,"%[^\n]\n", cname))
	     && (status != EOF))
	{
	  if (!XParseColor(dpy, cmap, cname, &xcolor)) continue;
	  if (!XAllocColor(dpy, cmap, &xcolor))
	    {
	      fprintf(stderr, "Warning: color %s could not be allocated!\n",
		      cname);
	      continue;
	    }
	  else
	    {
	      PWUseColorInTable(pixmap_widget,  xcolor.pixel, NULL, NULL,
				NULL, NULL, NULL, cname);
	    }
	}
      fclose(colorfile);
    }
  else
    { /* allocate black in colormap */
      PWUseColorInTable(pixmap_widget, black,
			NULL, NULL, NULL, NULL, NULL, "black");
    }
}
