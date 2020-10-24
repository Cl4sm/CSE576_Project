{
	cur_pos = get_cur();
	if (update_scr) {
		undraw_cursor();
		if(flag[REC]) rec_macro(key);
	}
	if (help_done){
		help_done = 0;
		flag[SHW] = 1;
	} else if (literal) {
		key_normal(keve->state & ControlMask ? key & 0x1f : key);
		literal = 0;
	} else {
		if (key==XK_Escape) doEscap = 1; else
		if (key & 0xFF00) key_func(key); else
		if (doEscap==2 || (keve->state & Mod1Mask)) key_binding(key);
		else {
			if (doEscap==1) key_escape(key); else
#ifdef GREEK
			if (doCtrlC) ctrlc_key(key); else
#endif
			if (doCtrlX) ctrlx_key(key); else 
			{
				if (keve->state & ControlMask) key &= 0x1f;
				if (key >= BLNK)  {
					flag[BLK] = 0;
					key_normal(key);
				}
				else key_control(key);
			}
		}
	}
	if(!doCtrlX && old_pos != line_start) {
		last_pos = old_pos;
		old_pos = line_start;
	}
	cur_pos = get_cur();

	if(update_scr) scr_update();
}
