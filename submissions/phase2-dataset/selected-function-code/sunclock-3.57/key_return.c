void key_return()
{
	char	*s = cur_pos;

	/* reset marked block on char entry */
	/* JPD: disable this behaviour !
	if(flag[BLK]) {
		key_delete();
		s = cur_pos;
	}
	else 
	*/
        if(flag[OVR] ) {
		cursor_down();
		goto_x(1);
		return;
	}
	file_resize(s, s+1);
	goto_x(1);
	*s = EOL;
	/* save newly inserted EOL to undo buf */
	u_ins(s, 1);
	ytot++;
	/* get prev line_start */
	s = line_start+1;
	cursor_down();
	goto_x(1);	/* ensure visible cursor */
	if(flag[IND]) {
		int i=0;
		/* count spaces and tabs */
		while(s[i] == BLNK || s[i] == '\t') i++;
		if(i){ /* and only if */
			file_resize(line_start+1, line_start+1+i);
			memcpy(line_start+1,s, sizeof(char) * i);
			/* save undo info */
			u_ins(line_start+1, i);
			x_offset += i;
			goto_x(i+1);
		}
	}
	flag[SHW]=1;
}
