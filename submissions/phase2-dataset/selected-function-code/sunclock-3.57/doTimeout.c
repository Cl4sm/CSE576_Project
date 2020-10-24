void
doTimeout(Context)
struct Sundata * Context;
{
        if (!Context) return;

        if (QLength(dpy) && Context->flags.update <= 2)
                return;         /* ensure events processed first */

        if (Context->flags.update)
           Context->count = 0;
        else
           Context->count = (Context->count+1) % TIMECOUNT;

        if (Context->count==0) {
           updateImage(Context);
           showMapImage(Context);
           writeStrip(Context);
           if (Context->flags.colorlevel==MONOCHROME) pulseMarks(Context);
           if (do_root == 2 && Context == RootCaller)
              drawImageToRootWindow(Context, 0);
	   XFlush(dpy);
	   if (Context->flags.animate) {
	      if (abs(Context->time-Context->animtime) >= 
                  Context->flags.animperiod) {
		 Context->animtime = Context->time;
	         Context->jump += progress_value[Context->flags.progress];
                 Context->flags.update = 4;
	      }
	   }
        }
}
