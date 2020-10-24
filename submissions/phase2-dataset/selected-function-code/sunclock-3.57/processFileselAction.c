void 
processFileselAction(Context, x, y, evtype)
struct Sundata * Context;
int x;
int y;
int evtype;
{
	static int pressed3 = 0;
	static int move_pos = -1;
	static int click_pos = -1;
	static int but_pos;
        char newdir[1030];
	char *s, *f, *path;

        if (evtype == MotionNotify && move_pos>=0) {
	   drawButton(Filesel, move_pos, -2);
	   move_pos = -1;
	}
        if (evtype == ButtonRelease && click_pos>=0) {
	   drawButton(Filesel, click_pos, 0);
           click_pos = -1;
	}
  
        but_pos = getButton(Filesel, x, y);

	if (evtype == ButtonPress) {
           if (click_pos>=0) drawButton(Filesel, click_pos, 0);
	   if (but_pos>=0) drawButton(Filesel, but_pos, 1);
	   click_pos = but_pos;
	   pressed3 = 1;
	   return;
	}
	if (evtype == ButtonRelease) {
	   pressed3 = 0;
	}

	if (evtype == MotionNotify) {
	   if (but_pos>=0 && but_pos!=click_pos) {
	      drawButton(Filesel, but_pos, -1);
	      move_pos = but_pos;
	   }
           if (x < FileselGeom.width - 15 ||
               y <= 2 * Context->gdata->menustrip)
	      return;
	}
	
        if (y <= Context->gdata->menustrip) {
	  if (but_pos==0 && getenv("HOME"))
	     sprintf(image_dir, "%s/", getenv("HOME")); 
	  if (but_pos==1)
	     StringReAlloc(&image_dir, share_maps_dir);
	  if (but_pos==2)
	     StringReAlloc(&image_dir, "/"); 
	  if (but_pos==3 && getcwd(NULL,1024)) {
	     sprintf(newdir, "%s/", getcwd(NULL,1024));
	     StringReAlloc(&image_dir, newdir);
	  }
	  if (but_pos>=0 && but_pos<=3) {
	     filesel_shift = 0;
	     if (dirtable) {
	        free(dirtable);
	        dirtable = NULL;
	     }
	  }
	  if (but_pos==4) {	
	      do_filesel = -1;
              processKey(Context->win, XK_w);
	      return;
	  }
	  if (but_pos==5) {	
              processKey(Context->win, XK_exclam);
	      return;
	  }
	  if (but_pos>=6) {
	     XUnmapWindow(dpy, Filesel);
	     do_filesel = 0;
	     return;
	  }
	  setupFilesel(0);
	  return;
	}
        if (y <= 2*Context->gdata->menustrip) {
	  filesel_shift = 0;
	  setupFilesel(0);
	  return;
	}

	if (x > FileselGeom.width - 15) {
	   int old_shift = filesel_shift;
	   if (y <= 2*Context->gdata->menustrip + 10) {
	      if (evtype != ButtonRelease) return;
	      if (filesel_shift == 0) return;
	      filesel_shift -= num_lines/2;
	      if (filesel_shift <0) filesel_shift = 0;
	   } else
	   if (y >= FileselGeom.height - 10) {
	      if (evtype != ButtonRelease) return;
	      if (num_table_entries-filesel_shift<num_lines) return;
	      filesel_shift += num_lines/2;
	   } else {
	      if (evtype == MotionNotify && !pressed3) {
		 return;
	      }
	      filesel_shift = ( num_table_entries *
		 ( y - 2*Context->gdata->menustrip - 10 ))
                 / (FileselGeom.height - 2*Context->gdata->menustrip - 20);
	      if (filesel_shift > num_table_entries - num_lines/2) 
                 filesel_shift = num_table_entries - num_lines/2;
	      if (filesel_shift < 0) filesel_shift = 0;
	   }
	   if (filesel_shift != old_shift)
	      setupFilesel(1);
	   return;
	}

	y = (y-2*Context->gdata->menustrip-4)/(3*Context->gdata->menustrip/4)
            +filesel_shift;
	if (y<num_table_entries) {
	   s = dirtable[y];
	   if (s==NULL || *s=='\0') return;
	   if (x > XTextWidth (Context->gdata->font[MENUFONT], s, 
                   strlen(s))+Context->gdata->charspace/4) return;
	   y = strlen(s)-1;
	   f = (char *) salloc(strlen(image_dir)+y+2);
	   strcpy(f, image_dir);
           if (s[y] == '/') {
	      int l;
	      if (!strcmp(s, "../")) {
	        l=strlen(f)-1;
		if (l==0) return;
                f[l--] = '\0';
	        while (l>=0 && f[l] != '/')
		   f[l--] = '\0';
		s = "";
	      }
              strcat(f, s);
	      l=strlen(f);
              if (f[l-1] != '/') {
                 f[l] = 'l';
                 f[++l] = '\0';
	      }
	      if (dirtable) free_dirlist(dirtable);
	      dirtable = NULL;
	      filesel_shift = 0;
	      num_table_entries=0;
	      StringReAlloc(&image_dir, f);
	      free(f);
              setupFilesel(0);
	      return;
	   } else {
	      path = (Context->wintype)? 
                    Context->map_img_file : Context->clock_img_file;
	      f = (char *)
                salloc((strlen(image_dir)+strlen(s)+2)*sizeof(char));
	      sprintf(f, "%s%s", image_dir, s);
	      if (!path || strcmp(f, path)) {
 		 if (Context->wintype)
                    StringReAlloc(&Context->map_img_file, f);
		 else
		    StringReAlloc(&Context->clock_img_file, f);
		 setupFilesel(3);
		 adjustGeom(Context, 0);
	         shutDown(Context, 0);
	         buildMap(Context, Context->wintype, 0);
	      }
	      free(f);
	   }
	}
}
