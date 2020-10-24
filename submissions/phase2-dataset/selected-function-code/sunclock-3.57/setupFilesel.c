setupFilesel(mode)
int mode;
{
	int i, j, b, d, p, w, h, ht, skip;
	char *s, *sp;

        if (!do_filesel) return;

        XSetWindowColormap(dpy, Filesel, FileselCaller->gdata->cmap);
        XSetWindowBackground(dpy, Filesel, 
           FileselCaller->gdata->pixel[MENUBGCOLOR]);

	d = FileselCaller->gdata->charspace/3;
        h = FileselCaller->gdata->menustrip;

	if (mode < 0) {
          BasicSettings(FileselCaller);
          XClearArea(dpy, Filesel,  0, 0, 
             FileselGeom.width, FileselGeom.height, False);
	  for (i=0; i<=6; i++)
	     drawButton(Filesel, i, 0);
	}
	
        BasicSettings(FileselCaller);

        if (mode <= 0) {
            XClearArea(dpy, Filesel,  0, FileselCaller->gdata->menustrip+1,
                FileselGeom.width - 15, FileselGeom.height, False);
            XClearArea(dpy, Filesel,  
                FileselGeom.width - 15, FileselCaller->gdata->menustrip+1,
                13, FileselCaller->gdata->menustrip, False);
            XSetForeground(dpy,  FileselCaller->gdata->wingc,
                FileselCaller->gdata->pixel[MENUFGCOLOR]);
            XDrawImageString(dpy, Filesel, FileselCaller->gdata->wingc,
                d, FileselCaller->gdata->font[MENUFONT]->max_bounds.ascent + 
                   FileselCaller->gdata->menustrip+4, 
                image_dir, strlen(image_dir));

            h = 2*FileselCaller->gdata->menustrip;
            XDrawLine(dpy, Filesel, FileselCaller->gdata->wingc, 
                0, h, FileselGeom.width, h);

            XDrawLine(dpy, Filesel, FileselCaller->gdata->wingc, 
                FileselGeom.width - 15, 2*FileselCaller->gdata->menustrip, 
                FileselGeom.width - 15, FileselGeom.height);
            /* Drawing small triangular icons */
            w = FileselGeom.width - 7;
            h = 2 * FileselCaller->gdata->menustrip + 1;
            for (i=0; i<=10; i++)
	       XDrawLine(dpy, Filesel, FileselCaller->gdata->wingc, 
                     w-i/2, h+i, w+i/2, h+i);
            h = FileselGeom.height-1;
            for (i=0; i<=10; i++)
               XDrawLine(dpy, Filesel, FileselCaller->gdata->wingc, 
                     w-i/2, h-i, w+i/2, h-i);
	}

	if (mode<=1)
           XClearArea(dpy, Filesel,  0, 2*FileselCaller->gdata->menustrip+1,
               FileselGeom.width - 15, FileselGeom.height, False);

	if (!dirtable) 
	   dirtable = get_dir_list(image_dir, &num_table_entries);
	if (dirtable)
           qsort(dirtable, num_table_entries, sizeof(char *), dup_strcmp);
	else {
	   char error[] = "Directory inexistent or inaccessible !!!";
	   XSetForeground(dpy, FileselCaller->gdata->wingc, 
                               FileselCaller->gdata->pixel[IMAGECOLOR]);
           XDrawImageString(dpy, Filesel, 
                     FileselCaller->gdata->wingc, d, 
                     3*FileselCaller->gdata->menustrip,
		     error, strlen(error));
	   return;
	}

	skip = (3*FileselCaller->gdata->menustrip)/4;
	num_lines = (FileselGeom.height-2*FileselCaller->gdata->menustrip)/skip;
        /* drawing the thumb */
        XSetForeground(dpy, FileselCaller->gdata->wingc,
           FileselCaller->gdata->pixel[ZOOMBGCOLOR]);
        XFillRectangle(dpy, Filesel, FileselCaller->gdata->wingc, 
           FileselGeom.width - 14, 2*FileselCaller->gdata->menustrip+12,
           14, FileselGeom.height-2*FileselCaller->gdata->menustrip-23);

        w = FileselGeom.width - 12;
        p = FileselGeom.height - 2 * FileselCaller->gdata->menustrip - 28;
        ht = p * (num_lines+1) / (num_table_entries+1);
        if (ht<20) ht = 20;
        if (ht>p/2) ht = p/2;
        p = p - ht;
        h = 2 * FileselCaller->gdata->menustrip + 14;
        if (num_table_entries>2)
           h += (filesel_shift * p)/(num_table_entries-2);
	drawBox(Filesel, FileselCaller->gdata->wingc, 
                FileselCaller->gdata->pixel, w, h, w+9, h+ht, 0);

        for (i=0; i<num_table_entries-filesel_shift; i++) 
	if (i<num_lines) {
	  s = dirtable[i+filesel_shift];
	  b = (s[strlen(s)-1]=='/');
          if (b==0) {
	    if (strstr(s,".gif") || strstr(s,".jpg") ||
		strstr(s,".png") || strstr(s,".xpm") ||
		strstr(s,".vmf"))
	       b=2;
	  }
	  j = FileselCaller->gdata->font[MENUFONT]->max_bounds.ascent + 
              2 * FileselCaller->gdata->menustrip + i*skip + 3;
	  sp = (FileselCaller->wintype)? 
	    FileselCaller->map_img_file : FileselCaller->clock_img_file;
	  if (strstr(sp,s)) {
	     if (mode<=3)
                XClearArea(dpy, Filesel, 2, 
	   /* FileselCaller->gdata->font[MENUFONT]->max_bounds.ascent+ */
                      2 * FileselCaller->gdata->menustrip + 1, 3, 
                   FileselGeom.height, False);
	     if (mode==3) {
  	        XSetForeground(dpy, FileselCaller->gdata->wingc, 
                               FileselCaller->gdata->pixel[CHANGECOLOR]);
                XDrawRectangle(dpy, Filesel, FileselCaller->gdata->wingc,
		  d/4, j-FileselCaller->gdata->font[MENUFONT]->max_bounds.ascent/2, 3,4);
	     } else {
  	        XSetForeground(dpy, FileselCaller->gdata->wingc, 
                               FileselCaller->gdata->pixel[CHOICECOLOR]);
                XFillRectangle(dpy, Filesel, FileselCaller->gdata->wingc,
                  d/4, j-FileselCaller->gdata->font[MENUFONT]->max_bounds.ascent/2, 3,4);
	     }
	  }
	  if (mode<=1) {
  	     XSetForeground(dpy, FileselCaller->gdata->wingc,
                (b==0)? FileselCaller->gdata->pixel[MENUFGCOLOR] :
                        FileselCaller->gdata->pixel[DIRCOLOR+b-1]);
             XDrawImageString(dpy, Filesel, FileselCaller->gdata->wingc, 
                d, j, s, strlen(s));
	  }
	}
}
