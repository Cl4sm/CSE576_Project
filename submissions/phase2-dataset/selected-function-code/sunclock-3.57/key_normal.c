void key_normal(int key)
{
	char *s=line_start+x_offset;
	int xtmp;

	/* reset marked block on char entry */
	if(flag[BLK]) { key_delete(); s = cur_pos; }

	if(flag[OVR] && *s != EOL) {
		/* save old overwrite char to undo buf */
		u_del(s, 1);
		putch(*s = key);
		flag[CHG] = 1;
	}
	else {
		file_resize(s, s+1);
		*s = key;
		flag[SHW] =1 ;
	}
	cursor_right();
	/* save new overwrite/insert char to undo buf */
	u_ins(s, 1);

	if(!flag[FIL] || xtru < screen_width) return;

	xtmp = block_fill();	/* cursor_down */
}
