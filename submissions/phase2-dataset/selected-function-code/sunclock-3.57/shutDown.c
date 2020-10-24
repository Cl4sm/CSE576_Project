void
shutDown(Context, all)
struct Sundata * Context;
int all;
{
        struct Sundata * ParentContext, *NextContext;

	if (all<0)
	   Context = Seed;

      repeat:
	fflush(stderr);

        NextContext = Context->next;
        ParentContext = parentContext(Context);

	if (runlevel!=IMAGERECYCLE) {
  	   if (Context->xim) {
              XDestroyImage(Context->xim); 
              Context->xim = NULL;
	   }
	   if (Context->ximdata) {
	      free(Context->ximdata);
	      Context->ximdata = NULL;
	   }
	   while (Context->label) {
	      struct TextLabel *ptr;
	      ptr = (Context->label)->next;
              free(Context->label->text);
              free(Context->label);
	      Context->label = ptr;
	   }
	   if (Context->mappix) {
              XFreePixmap(dpy, Context->mappix);
	      Context->mappix = 0;
 	   }
           if (Context->daypixel) {
	      free(Context->daypixel);
	      Context->daypixel = NULL;
           }
           if (Context->nightpixel) {
	      free(Context->nightpixel);
	      Context->nightpixel = NULL;
           }
           if (Context->vmfpixels) {
	      free(Context->vmfpixels);
              Context->vmfpixels = NULL;
	   }
           if (Context->tr1) {
              free(Context->tr1);
              Context->tr1 = NULL;
           }
           if (Context->tr2) {
              free(Context->tr2);
              Context->tr2 = NULL;
           }
           if (Context->daywave) {
              free(Context->daywave);
              Context->daywave = NULL;
	   }
	}
        destroyGCs(Context);

	Context->flags.hours_shown = 0;

        if (all) {
	   last_time = 0;

           if (Context->win) {
	      setAuxilWins(Context, DESTROY);
	      if (Context == RootCaller) {
                 RootCaller = NULL;
		 do_root = 0;
	      }
	      XDestroyWindow(dpy, Context->win);
  	      Context->win = 0;
	   }
           if (Context->clock_img_file) {
              free(Context->clock_img_file);
	      Context->clock_img_file = NULL;
	   }
           if (Context->map_img_file) {
              free(Context->map_img_file);
	      Context->map_img_file = NULL;
	   }
	
           free(Context->spotsizes);
           free(Context->sizelimits);
  
	   if (all<0) {
	      free(Context);
	      if (NextContext) {
	         Context = NextContext;
	         goto repeat;
	      }
	      else {
	        endup:
                 if (zoompix) XFreePixmap(dpy, zoompix);
                 if (textpix) XFreePixmap(dpy, textpix);
                 if (rootpix) XFreePixmap(dpy, rootpix);
                 XCloseDisplay(dpy);
         	 if (dirtable) free(dirtable);
         	 exit(0);
 	      }
 	   }
	   if (ParentContext)
	      ParentContext->next = Context->next;
	   else
              Seed = Context->next;
	   free(Context);
           if (Seed == NULL) goto endup;
	}
}
