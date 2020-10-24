{
	int m, rep;

        if (ycur<0) { y=0 ; scroll_down(); ycur = 0; }
        if (ycur>ytot) ycur = ytot;

        m = (ycur * vtot) / (ytot? ytot : 1);
	rep = abs(ytru-ycur)/screen_height;

	if(m<0) scroll_down();					/* if cursor at top of screen */
	else if(m>vtot+ftheight) scroll_up();	/* if cursor at bottom of screen */
	else if(m<pos) {
	     for (m=0; m<rep; m++)
	         cursor_pageup();		/* if cursor above thumb */
	}
	else if(m>pos+vcur) {
	     for (m=0; m<rep; m++)
	         cursor_pagedown();	/* if cursor below thumb */
	}

	goto_y(ycur);  /* cursor on thumb so track it */

	scr_update(); /* does show_vbar */
}
