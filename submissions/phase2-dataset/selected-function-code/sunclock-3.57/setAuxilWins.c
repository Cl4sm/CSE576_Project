void
setAuxilWins(Context, mode)
Sundata * Context;
int mode;
{
#define NUMWIDGETS 5


int * bool_state[NUMWIDGETS] = { 
  &do_menu, &do_filesel, &do_zoom, &do_option, &do_urban };

Window * window[NUMWIDGETS] = { &Menu, &Filesel, &Zoom, &Option, &Urban };

struct Sundata ** caller[NUMWIDGETS] = { 
  &MenuCaller, &FileselCaller, &ZoomCaller, &OptionCaller, &UrbanCaller };

char * char_newhint[NUMWIDGETS] = {
   (char *)&menu_newhint, NULL, 
   &zoom_newhint, &option_newhint, &urban_newhint };

setupCB setup_proc[NUMWIDGETS] = {
   &setupMenu, &setupFilesel, &setupZoom, &setupOption, &setupUrban };

popCB pop_proc[NUMWIDGETS] = {
   &PopMenu, &PopFilesel, &PopZoom, &PopOption, &PopUrban };

int i, announce=1;
  
      if (option_changes && mode==RESET) mode = REMAP;

      for (i=0; i<NUMWIDGETS; i++) if (*bool_state[i]) {
	 if (announce && verbose && mode!=RESET) {
	   fprintf(stderr, 
              "Resetting auxiliary widgets in mode %d...\n", mode);
	   announce = 0;
	 }
	 
	 switch(mode) {

         case RESET:
	    if (Context == *caller[i]) {
  	       *bool_state[i] = 1;
	       *caller[i] = Context;
	       setup_proc[i](-1);
	    }
	    break;

         case REMAP:
	    if (Context != *caller[i]) break;
	 case REATTRIB:
	    *bool_state[i] = 0;
	    if (*window[i]) 
               XUnmapWindow(dpy, *window[i]);
	    if (char_newhint[i])
	       *char_newhint[i] = ' ';
	    pop_proc[i](Context);
	    break;

         case DESTROY:
	    if (Context == *caller[i]) {
	       if (*window[i]) {
		  XDestroyWindow(dpy, *window[i]);
                  *window[i] = 0;
	       }
	       *bool_state[i] = 0;
	       *caller[i] = NULL;
	    }
	    break;

         case ICONIFY:
	    if (Context == *caller[i])
	       XIconifyWindow(dpy, *window[i], scr);
	    break;

         case DEICONIFY:
	    if (Context == *caller[i]) {
	       XMapWindow(dpy, *window[i]);
	       setup_proc[i](-1);
	    }
	    break;

	 }
      }

      XFlush(dpy);
      if (mode == REMAP) usleep(2*TIMESTEP);
}
