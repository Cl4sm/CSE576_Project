setProtocols(Context, num)
struct Sundata * Context;
int				num;
{
	Window			win = 0;
	long                    mask;

	mask = FocusChangeMask | VisibilityChangeMask | ExposureMask | 
               StructureNotifyMask | 
               ButtonPressMask | ButtonReleaseMask | KeyPressMask;

	if (num>=1)
	       mask |= PointerMotionMask;

        /* use StructureNotifyMask rather than  ResizeRedirectMask 
           to avoid bug in some window mangers... as enlightenment !
           All events would be:
	   for (i=0; i<=24; i++) mask |= 1L<<i;
         */

        switch(num) {

	   case 0:
	   case 1:
                if (!Context) return;
	        mask |= EnterWindowMask | LeaveWindowMask;
		win = Context->win;
		break;

	   case 2:
		win = Menu;
		break;

	   case 3:
	        win = Filesel;
		break;

	   case 4:
	        win = Zoom;
		break;

	   case 5:
	        win = Option;
		mask |= KeyReleaseMask;
		break;

	   case 6:
	        win = Urban;
		mask |= KeyReleaseMask;
		break;

 	   default:
	        break;
	}

	if (!win) return;

       	XSelectInput(dpy, win, mask);
	XSetWMProtocols(dpy, win, &wm_delete_window, 1);
}
