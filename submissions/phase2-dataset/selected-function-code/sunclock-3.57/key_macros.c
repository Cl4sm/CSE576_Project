void key_macros(int record)
{
	char *s=mbuf;
	int k;

	if(!record) {	/* play macro back */
		if(*s == 0) { bell(); return; };	/* ding user on empty mbuf */
		doCtrlX = 0;		/* turn off control key state from ^K */
#ifdef X11
		keve->state = 0;	/* turn off control key state from ^P */
#endif
		while(*s != 0) {
			k = 255 & *s++;
			switch(executive) {
				case MAIN:
					main_exec(k);
					break;
				case DIALOG:
					dialog(k);
					break;
				case OPTIONS:
					options(k);
					break;
			}
		}
		flag[SHW] = 1;
		return;
	}
	/* else toggle recording on/off */
	flag[REC] ^= 1;
	show_top();
	if(!flag[REC]) {	/* terminal condition */
		pmbuf[-2] = 0;	/* backup over ^K^M & terminate macro with a 0x0 */
		pmbuf = mbuf;	/* reset record pointer to start of mbuf */
	}
}
