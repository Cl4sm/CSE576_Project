void 
processOptionAction(Context, x, y, button, evtype)
Sundata * Context;
int x, y, button, evtype;
{
        static int click_pos = -1;
        int i, opth, vskip, but_pos;
	KeySym key;

        opth = OptionGeom.height - 2 * Context->gdata->menustrip - 1;
	vskip = 3*Context->gdata->menustrip/8;

        if (evtype == ButtonRelease && click_pos>=0) {
	   drawButton(Option, click_pos, 0);
           click_pos = -1;
        }

	if (evtype==ButtonRelease && x>=70 && 
            x<=OptionGeom.width-15 &&
            y>=vskip && y<=Context->gdata->menustrip+vskip) {
	   text_input = OPTION_INPUT;
	   but_pos = (x-76)/XTextWidth(OptionCaller->gdata->font[MENUFONT], "_", 1);
	   i = strlen(option_entry.string);
	   if (but_pos<0) but_pos = 0;
	   if (but_pos>i) but_pos = i;
	   option_entry.caret = but_pos;
	   setupOption(0);
	   return;
	}

        if (evtype==ButtonRelease && text_input==OPTION_INPUT) {
	   text_input = NULL_INPUT;
	   setupOption(0);
	}

	if (y>opth && y<opth+Context->gdata->menustrip) {
           but_pos = getButton(Option, x, y);
	   if (but_pos>=N_OPTION)
	      option_newhint = '\033';
	   else
	   if (but_pos>=0)
	      option_newhint = OptionKey[2*but_pos];
	   if (evtype==MotionNotify) {
	      if (but_pos>=0)
                 showOptionHint(but_pos);
	   }
	   if (evtype==ButtonPress && but_pos>=0) {
	      drawButton(Option, but_pos, 1);
	      click_pos = but_pos;
	   }
	   if (evtype==ButtonRelease) {
	      if (but_pos<0) return;
	      if (but_pos<N_OPTION) {
		 key = (KeySym)tolower(option_newhint);
	         processKey(Option, key);
		 showOptionHint(but_pos);
	      } else
	         PopOption(Context);
	   }
	}
}
