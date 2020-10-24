void
processUrbanAction(Context, x, y, button, evtype)
Sundata * Context;
int x, y, button, evtype;
{
        static int move_pos;
        static int click_pos;
        int i, j, vskip, but_pos;
	KeySym key;
        
        if (evtype == MotionNotify && move_pos>=0) {
	       drawButton(Urban, move_pos, -2);
	       move_pos = -1;
	}

        if (evtype == ButtonRelease && click_pos>=0) {
	       drawButton(Urban, click_pos, 0);
               click_pos = -1;
	} 

	vskip = 3*Context->gdata->menustrip/8;

	for (i=0; i<=4; i++)
	if (evtype==ButtonRelease && x>=urban_x[i]+1 && 
            x<=urban_x[i]+urban_w[i] &&
            y>=urban_y[i] && y<=urban_y[i]+Context->gdata->menustrip) {
	   text_input = URBAN_INPUT+i;
	   but_pos = 
             (x-urban_x[i]-6)/XTextWidth(UrbanCaller->gdata->font[MENUFONT], "_", 1);
	   j = strlen(urban_entry[i].string);
	   if (but_pos<0) but_pos = 0;
	   if (but_pos>j) but_pos = j;
	   urban_entry[i].caret = but_pos;
	   setupUrban(0);
	   return;
	}

        if (evtype==ButtonRelease) {
	   text_input = NULL_INPUT;
	   setupUrban(0);
	}

	if (y>urbanh && y<urbanh+Context->gdata->menustrip) {
           but_pos = getButton(Urban, x, y);
	   if (but_pos>=N_URBAN)
	      urban_newhint = '\033';
	   else
	   if (but_pos>=0)
	      urban_newhint = UrbanKey[2*but_pos];
	   if (evtype==MotionNotify) {
	      if (but_pos>=0 && but_pos<=N_URBAN) {
                 showUrbanHint(NULL);
		 drawButton(Urban, but_pos, -1);
                 move_pos = but_pos;
	      }
	   }
	   if (evtype==ButtonPress && but_pos>=0) {
	      drawButton(Urban, but_pos, 1);
	      click_pos = but_pos;
	   }
	   if (evtype==ButtonRelease) {
	      if (but_pos<0) return;
	      if (but_pos<N_URBAN) {
		 if (button<=2)
		    key = (KeySym)tolower(urban_newhint);
		 else
		    key = (KeySym)urban_newhint;
		 showUrbanHint(NULL);
	         processKey(Urban, key);
	      } else
	         PopUrban(Context);
	   }
	}
	if (evtype==MotionNotify && 
            (y<=urbanh || y>=urbanh+Context->gdata->menustrip)) {
	   urban_newhint = 'U';
           showUrbanHint(NULL);
	}
}
