void dialog(int key)
{
	int skey;

#ifdef X11
	undraw_cursor();
	gotoxy(diastart+col,yl1);
	skey = key & 0xff;
	if(keve->state & ControlMask) skey &= 0x1f;
	/* special controls for search&replace Y/N/Esc/! */
	if (dialogCB == do_sar) {
		switch(tolower(skey)) {
		case 27:  search_mode = 2; break;
		case 'n': search_mode = 3; break;
		case 10:
		case 13:
		case 'y': search_mode = 4; break;
		case '!': search_mode = 5; break;
		default:  search_mode = 6; break;
		}
		do_sar();
		first = key;
		return;
	}
	if(key == XK_End) {
		while((diabuf[col]!=0) && (col<dblen)) putch(diabuf[col++]);
	} else {
		switch(skey){
			case 8: {
				if(col < 0) col = strlen(diabuf);
				if(col) { col--; diabuf[col] = '\0'; outxy.X--; putch(' '); outxy.X--;}
				break;
			}
			case  3:
			case 13:
			case 27: {
				executive = MAIN;
				diabuf[col] = 0;
				if(*diabuf && skey == 13)
					dialogCB();
				flag[SHW] = 1;
				scr_update();
				show_top();
				break;
			}
			default: {
				if(col < 0 || !first) {
					col = 0;
					gotoxy(diastart+col,yl1);
					clreol();
				}
				if (col < dblen) {
					diabuf[col++] = key;
					diabuf[col] = '\0';
					putch(key);
				} else
					bell();
			}
		}
	}
	first = key;
	draw_cursor();
#else
	skey = key & 0xff;

	gotoxy(diastart+col,yl1);

	/* special control for search&replace Y/N/Esc/! */
	if (dialogCB == do_sar) {
		switch(tolower(skey)) {
		case 27:  search_mode = 2; break;
		case 'n': search_mode = 3; break;
		case 10:
		case 13:
		case 'y': search_mode = 4; break;
		case '!': search_mode = 5; break;
		default:  search_mode = 6; break;
		}
		do_sar();
		first = key;
		return;
	}
	if(key == 0x0c) {	/* ^L */
		while(diabuf[col] != 0) putch(diabuf[col++]);
	} else {
		switch(skey){
			case 8: {
				if(col < 0) col = strlen(diabuf);
				if(col) { col--; gotoxy(--outxy.X,outxy.Y); putch(' '); gotoxy(outxy.X,outxy.Y);}
				break;
			}
			case  3:
			case 10:
			case 13:
			case 27: {
				executive = MAIN;
				diabuf[col] = 0;
				if(*diabuf && (skey == 10 || skey == 13))
					dialogCB();
				flag[SHW] = 1;
				scr_update();
				show_top();
				break;
			}
			default: {
				if(col < 0 || !first) {
					col = 0;
					gotoxy(diastart+col,yl1);
					clreol();
				}
				diabuf[col++] = key;
				putch(key);
			}
		}
	}
	first = key;
#endif
}
