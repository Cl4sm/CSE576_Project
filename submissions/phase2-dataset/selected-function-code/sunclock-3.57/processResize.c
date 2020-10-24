void 
processResize(win)
Window win;
{
           int i, x, y, w, h, num = 0;
           struct Sundata * Context = NULL;
           struct Geometry * Geom = NULL;

           if (win == Menu) return;

           if (win == Filesel) {
	      if (!do_filesel) return;
              Geom = &FileselGeom;
              num = 3;
           }
           if (win == Zoom) {
	      if (!do_zoom) return;
              Geom = &ZoomGeom;
              num = 4;
           }
           if (win == Option) {
	      if (!do_option) return;
              Geom = &OptionGeom;
              num = 5;
           }
           if (win == Urban) {
	      if (!do_urban) return;
              Geom = &UrbanGeom;
              num = 6;
           }

           if (num) {
              if (getPlacement(win, &x, &y, &w, &h)) return;
              if (w==Geom->width && h==Geom->height) return;
              if (w<Geom->w_mini) w = Geom->w_mini;
              if (h<Geom->h_mini) h = Geom->h_mini;
              Geom->width = w;
              Geom->height = h;
              if (verbose)
                 fprintf(stderr, "Resizing %s to %d %d\n", 
                    widget_type[num], w, h);
              XSelectInput(dpy, win, 0);
              setSizeHints(NULL, num);
              setProtocols(NULL, num);
              if (num==3)
                 setupFilesel(-1);
              if (num==4) {
                 if (zoompix) {
                    XFreePixmap(dpy, zoompix);
                    zoompix = 0;
                 }
                 setupZoom(-1);
              }
              if (num==5) {
   	         w = ((OptionGeom.width-86) / 
                   XTextWidth(OptionCaller->gdata->font[MENUFONT], "_", 1))-2;
		 resetStringLength(w, &option_entry);
                 setupOption(-1);
              }
              if (num==6) {
		 text_input = NULL_INPUT;
		 setupUrban(-2);
                 for (i=0; i<=4; i++) {
	            w = (urban_w[i]/ 
                      XTextWidth(UrbanCaller->gdata->font[MENUFONT],"_",1))-2;
	            resetStringLength(w, &urban_entry[i]);
		    urban_entry[i].string[urban_entry[i].maxlength] = '\0';
		 }
		 setupUrban(-1);
	      }
              return;
           }

           Context = getContext(win);
           if(!Context) return;

           if (Context==Seed && !Context->wintype && do_dock) return;

           if (getPlacement(win, &x, &y, &w, &h)) return;
           h -= Context->hstrip;
           if (w==Context->geom.width && h==Context->geom.height) return;
           Context->prevgeom = Context->geom;
           if (w<Context->geom.w_mini) w = Context->geom.w_mini;
           if (h<Context->geom.h_mini) h = Context->geom.h_mini;
	   Context->flags.update=2;
	   showMapImage(Context);
	   clearStrip(Context);
	   writeStrip(Context);
	   XFlush(dpy);
           Context->geom.width = w;
           Context->geom.height = h;
           if (Context->wintype) {
              MapGeom.width = w;
              MapGeom.height = h;
           } else {
              ClockGeom.width = w;
              ClockGeom.height = h;
           }
           adjustGeom(Context, 0);
	   warningNew(Context);
           shutDown(Context, 0);
           setZoomAspect(Context, 3);
           buildMap(Context, Context->wintype, 2);
           XFlush(dpy);
           usleep(2*TIMESTEP);
}
