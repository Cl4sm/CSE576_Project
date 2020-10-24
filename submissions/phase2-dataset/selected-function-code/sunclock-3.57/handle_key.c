{
		char chstr[256];
		int x=outxy.X, y=outxy.Y, n=0;

		/* display keyboard shift/control/alt status */
		highvideo();
		gotoxy(0,yl1);
		for(n=5;n;chstr[n--]=' ');
		if(state & ShiftMask) chstr[n++] = 'S';
		if(state & Mod1Mask) chstr[n++] = 'A';
		if(skey & 0xff00) chstr[n++] = 'F';
		if(state & ControlMask) chstr[n++] = '^';
		chstr[n] = skey & 0xff;
		chstr[5] = '\0';
		cputs(chstr);
		chstr[0] = '\0';

#ifdef DEBUG
		/* display raw key event data */
		sprintf(chstr,"k=%4x,s=%2x.",skey,state);
#endif /* DEBUG */
		gotoxy(screen_width - 12,0);
		clreol();
		cputs(chstr);
		gotoxy(x,y);
		lowvideo();

		if((skey >= 0xffe1) && (skey <= 0xffee)) return;
		if(skey == NoSymbol) return;

		switch(executive) {
		case MAIN:
				main_exec(skey);
				break;
		case DIALOG:
				dialog(skey);
				break;
		case OPTIONS:
				options(skey);
				break;
		}
}
