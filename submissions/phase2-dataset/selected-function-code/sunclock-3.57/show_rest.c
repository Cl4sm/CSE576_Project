{
	char *ss;
	char disp[MAXVLINE];
	int i,j,k=0,xlt=xlo;

	ss = s;
	i = 0;
	if(flag[BLK]) {
		if(cur_pos < mk)
			{ bstart = cur_pos; bend = mk; }
		else
			{ bstart = mk; bend = cur_pos; }
		if ((ss >= bstart) && (ss < bend)) highvideo();
	}
	while(*ss != EOL && i < MAXVLINE) {
		if(flag[BLK]) {
			if ((ss == bstart)) { drstr(disp+xlo,i-xlo); k += i; xlo=i=0; highvideo(); }
			if ((ss == bend)) { drstr(disp+xlo,i-xlo); k += i; xlo=i=0; lowvideo(); }
		}
		if(*ss == '\t') {
			for(j = i +(tabsize - (k+i)%tabsize); i<j; disp[i++] = ' ');
			ss++;
		}
		else disp[i++] = *ss++;
	}
	if(i) drstr(disp+xlo,i-xlo);
	if(flag[BLK] && (ss == bend)) lowvideo();
	clreol();
	xlo = xlt;
}
