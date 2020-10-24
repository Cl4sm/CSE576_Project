void key_delword(int eol)
{
	char	*d=cur_pos;

	if(flag[BLK]) {
		block_remove_update();
		return;
	}
	if(*d == EOL) {
	        blen = 1;
	        mk = d;
	        block_put();
		key_deleol(d);
		return;
	}
	if(eol) while(*d != EOL) d++;
	else {
		while(!strchr(sdelims,*d) && *d != EOL) d++;
		while(strchr(s1delims,*d) && *d != EOL) d++;
	}
	mk = cur_pos;
	blen = d-mk;
	block_put();
	/* delete word and save to undo buf */
	file_resize(d, cur_pos);
	flag[SHW]=1;
}
