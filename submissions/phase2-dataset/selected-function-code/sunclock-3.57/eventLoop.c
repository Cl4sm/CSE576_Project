void
eventLoop()
{
        XEvent                  ev;
        Sundata *               Context;
	Sundata *               Which;
        char                    buffer[1];
        KeySym                  keysym;

        for (;;) {
	   if (XCheckIfEvent(dpy, &ev, evpred, (XPointer)0)) {

	      /*
              fprintf(stderr, "Event %d, Window %d \n"
                   "  (Main %d, Menu %d, Sel %d, Zoom %d, Option %d)\n", 
                   ev.type, ev.xexpose.window, 
                   Seed->win, Menu, Filesel, Zoom, Option);
	      */

              switch(ev.type) {

                 case EnterNotify:
		      focus_in = 1;
		      break;
		 
                 case LeaveNotify:
		      focus_in = 0;
		      break;
		 
                 case FocusOut:
		      if (do_option && text_input == OPTION_INPUT &&
                          ev.xexpose.window == Option) {
			 text_input = NULL_INPUT;
			 setupOption(0);
		      }
		      if (do_urban && text_input >= URBAN_INPUT &&
                          ev.xexpose.window == Urban) {
			 text_input = NULL_INPUT;
			 setupUrban(0);
		      }
		      break;

                 case VisibilityNotify:
                      doExpose(ev.xexpose.window);
                      break;
		      
                 case MapNotify:
                      Context = getContext(ev.xexpose.window);
		      if (!Context) break;
		      if (Context->win!=ev.xexpose.window) break;
                      setAuxilWins(Context, DEICONIFY);
   	              Context->flags.mapped = 1;
                      updateImage(Context);
   	              Context->flags.update = 2;
		      showMapImage(Context);
		      writeStrip(Context);
		      break;

                 case UnmapNotify:
                      Context = getContext(ev.xexpose.window);
		      if (!Context) break;
		      if (Context->win!=ev.xexpose.window) break;
                      setAuxilWins(Context, ICONIFY);
   	              Context->flags.mapped = 0;
		      break;
		     
                 case Expose:
                      if (ev.xexpose.count == 0)
                      doExpose(ev.xexpose.window);
                      break;

                 case ClientMessage:
                      if (ev.xclient.message_type == wm_protocols &&
                          ev.xclient.format == 32 &&
                          ev.xclient.data.l[0] == wm_delete_window) {
                            if (ev.xexpose.window == Menu) PopMenu(MenuCaller);
                               else
                            if (ev.xexpose.window == Filesel) 
                                  PopFilesel(FileselCaller);
                               else
                            if (ev.xexpose.window == Zoom) PopZoom(ZoomCaller);
                               else
                            if (ev.xexpose.window == Option)
                                            PopOption(OptionCaller);
			       else
                            if (ev.xexpose.window == Urban)
                                            PopUrban(UrbanCaller);
			       else {
			         Context = getContext(ev.xexpose.window);
		                 if (!Context) break;
				 if (Context!=Seed || !do_dock)
                                    shutDown(Context, 1);
				 }
		      }
                      break;

                 case KeyPress:
                 case KeyRelease:
                      XLookupString((XKeyEvent *) &ev, buffer, 1, &keysym, NULL);
                      if (keysym==XK_Control_L || keysym==XK_Control_R) {
                         if (ev.type == KeyPress) control_key = 1;
                         if (ev.type == KeyRelease) control_key = 0;
                      } else
                         if (ev.type == KeyPress && keysym != XK_Mode_switch)
                            processKey(ev.xexpose.window, keysym);
                      break;

                 case ButtonPress:
		 case ButtonRelease:
                 case MotionNotify:
                      if (ev.type==ButtonPress)
                            button_pressed = ev.xbutton.button;
                      if (ev.type==ButtonRelease) button_pressed = 0;
                      processMouseEvent(ev.xexpose.window,
                            ev.xbutton.x, ev.xbutton.y,
                            ev.xbutton.button, ev.type);
                      break;

		 /* case ResizeRequest: */
		 case PropertyNotify:
		 case ConfigureNotify:
                      processResize(ev.xexpose.window);
                      break;
			
                 default:
		      break;
	      }
	   } else {
              Which = getContext(ev.xexpose.window);
              usleep(TIMESTEP);
              if (Which == NULL) Which = Seed;
	      if (Which->win==ev.xexpose.window)
	         processResize(Which->win);
              for (Context = Seed; Context; Context = Context->next)
                 if (do_sync || Context == Which || Context == RootCaller ||
                     (do_dock && Context == Seed)) {
		    if (do_sync & 2) Context->flags.update = 2;
                    doTimeout(Context);
		 }
	      do_sync &= 1;
	   }
        }
}
