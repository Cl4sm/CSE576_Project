buildMap(Context, wintype, build)
struct Sundata * Context;
int wintype, build;
{
   Window win;
   int old_w, old_h, old_s, resize;

   if (build < 2) 
      resize = 0;
   else {
      resize = 1;
      build = 0;
   }

   if (build) {
      struct Sundata * NewContext;
      NewContext = (struct Sundata *)salloc(sizeof(struct Sundata));
      NewContext->next = NULL;
      if (Context) {
         if (Context->next) {
            NewContext->next = Context->next;
            Context->next = NewContext;
         } else
            Context->next = NewContext;
      } else
         Seed = NewContext;
      Context = NewContext;
      Context->wintype = wintype;
      if (do_menu<0) {
         do_menu = 1;
         MenuCaller = Context;
      }
      if (do_filesel<0) {
         do_filesel = 1;
         FileselCaller = Context;
      }
      if (do_zoom<0) {
         do_zoom = 1;
         ZoomCaller = Context;
      }
      if (do_option<0) {
         do_option = 1;
         OptionCaller = Context;
      }
   }

   makeContext(Context, build);

   win = Context->win;
   if (win)
      XSelectInput(dpy, Context->win, 0);

   if (createImage(Context)) {
     if (Seed->next) {
         shutDown(Context, 0);
         Context = Seed;
         return;
     } else
         shutDown(Context, -1);
   }
   checkGeom(Context, 0);

   if (win) {
      old_s = Context->hstrip;
      Context->hstrip = (wintype)? 
          Context->gdata->mapstrip : Context->gdata->clockstrip;
      setClassHints(Context->win, wintype);
      setSizeHints(Context, wintype);
      getPlacement(Context->win, &Context->geom.x, &Context->geom.y, 
                   &old_w, &old_h);
      old_h -= Context->hstrip;
      if (resize || Context->hstrip != old_s ||
          Context->geom.width!=old_w || Context->geom.height!=old_h) {
	 XMapRaised(dpy, Context->win);
	 Context->flags.mapped = 1;
	 XFlush(dpy);
         usleep(TIMESTEP);
         setAuxilWins(Context, REMAP);
      } else
         setAuxilWins(Context, RESET);
      if (runlevel!=IMAGERECYCLE || color_depth<=8)
         createWorkImage(Context);
      setProtocols(Context, Context->wintype);
   } else {
      createWorkImage(Context);
      if (screen_saver) {
         Context->flags.mapped = 0;
         do_root = 2;
	 Context->hstrip = Context->gdata->mapstrip;
	 checkLocation(Context, CityInit);
	 drawImageToRootWindow(Context, 0);
	 return;
      }
      Context->win = newWindow(Context, &Context->geom, wintype);
      setSizeHints(Context, wintype);
      XMapWindow(dpy, Context->win);
      Context->flags.mapped = 1;
      XFlush(dpy);
      usleep(TIMESTEP);
      setAuxilWins(Context, REATTRIB);
      setProtocols(Context, wintype);
      Context->prevgeom.width = 0;
   }

   checkLocation(Context, CityInit);
   if (Context->flags.colorlevel == MONOCHROME) drawAll(Context);
   clearStrip(Context);
   if (Context->gdata->cmap!=cmap0)
      XSetWindowColormap(dpy, Context->win, Context->gdata->cmap);
   runlevel = RUNNING;
   option_changes = 0;
   Context->flags.update = 4;
   updateImage(Context);
   showMapImage(Context);
   do_sync |= 2;
}
