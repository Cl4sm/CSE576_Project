void info_frame(struct xmp_module_info *mi, struct xmp_frame_info *fi, struct control *ctl, int reprint)
{
	static int ord = -1, spd = -1, bpm = -1;
	char rowstr[3], numrowstr[3];
	int time;

	if (fi->virt_used > max_channels)
		max_channels = fi->virt_used;

	if (!reprint && fi->frame != 0)
		return;

	time = fi->time / 100;

	if (msg_timer > 0) {
		report("\r%-61.61s %c%c%c", msg_text,
			ctl->explore ? 'Z' : ' ',
			ctl->loop ? 'L' : ' ',
			ctl->loop > 1 ? '*' : ' ');
		msg_timer -= fi->frame_time * fi->speed / 6;
		if (msg_timer == 0)
			msg_timer--;
		else
			goto print_time;
	}

	if (msg_timer < 0) {
		reprint = 1;
		msg_timer = 0;
	}

	if (reprint || fi->pos != ord || fi->bpm != bpm || fi->speed != spd) {
	        report("\rSpeed[%02X] BPM[%02X] Pos[%02X/%02X] "
			 "Pat[%02X/%02X] Row[  /  ] Chn[  /  ]      0:00:00.0",
					fi->speed, fi->bpm,
					fi->pos, mi->mod->len - 1,
					fi->pattern, mi->mod->pat - 1);
		ord = fi->pos;
		bpm = fi->bpm;
		spd = fi->speed;
	}

	if (fi->row <= 0xff) {
		snprintf(rowstr, 3, "%02X", fi->row);
	} else {
		strcpy(rowstr, "++");
	}

	if (fi->num_rows <= 0x100) {
		snprintf(numrowstr, 3, "%02X", fi->num_rows - 1);
	} else {
		strcpy(numrowstr, "++");
	}

	report("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b"
	       "%2.2s/%2.2s] Chn[%02X/%02X] %c%c%c",
		rowstr, numrowstr, fi->virt_used, max_channels,
		ctl->explore ? 'Z' : ' ', ctl->loop ? 'L' : ' ',
		ctl->loop > 1 ? '*' : ' ');

    print_time:

	if (ctl->pause) {
		report(" - PAUSED -");
	} else {
		report("%3d:%02d:%02d.%d",
			(int)(time / (60 * 600)), (int)((time / 600) % 60),
			(int)((time / 10) % 60), (int)(time % 10));
	}

	fflush(stdout);
}