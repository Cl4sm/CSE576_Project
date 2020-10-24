void read_command(xmp_context handle, struct xmp_module_info *mi, struct control *ctl)
{
	int cmd;

	cmd = read_key();	
	if (cmd <= 0)
		return;

	switch (cmd) {
	case 0x1b:		/* escape */
		cmd = read_key();
		if (cmd != '[')
			goto cmd_quit;
		cmd = read_key();
		switch (cmd) {
		case 'A':
			goto cmd_next_mod;
		case 'B':
			goto cmd_prev_mod;
		case 'C':
			goto cmd_next_pos;
		case 'D':
			goto cmd_prev_pos;
		}

		break;
	case 'q':		/* quit */
	cmd_quit:
		xmp_stop_module(handle);
		ctl->pause = 0;
		ctl->skip = -2;
		break;
	case 'f':		/* jump to next order */
	cmd_next_pos:
		xmp_next_position(handle);
		ctl->pause = 0;
		break;
	case 'b':		/* jump to previous order */
	cmd_prev_pos:
		xmp_prev_position(handle);
		ctl->pause = 0;
		break;
	case 'n':		/* skip to next module */
	cmd_next_mod:
		xmp_stop_module(handle);
		ctl->pause = 0;
		ctl->skip = 1;
		break;
	case 'p':		/* skip to previous module */
	cmd_prev_mod:
		xmp_stop_module(handle);
		ctl->pause = 0;
		ctl->skip = -1;
		break;
	case 'l':
		ctl->loop++;
		ctl->loop %= 3;
		break;
	case 'Z':
		ctl->cur_seq = 1;
		break;
	case 'z':
		ctl->explore ^= 1;
		break;
	case ' ':		/* paused module */
		ctl->pause ^= 1;
		break;
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		/* toggle mute */
		xmp_channel_mute(handle, cmd - '1', 2);
		break;
	case '0':
		xmp_channel_mute(handle, 9, 2);
		break;
	case '!': {
		int i;
		for (i = 0; i < 10; i++) {
			xmp_channel_mute(handle, i, 0);
		}
		break; }
	case '?':
	case 'i':
	case 'I':
	case 'S':
	case 'm':
		ctl->display = cmd;
		break;
	case 'c':
		ctl->display = cmd;
		break;
	case '>':
		change_sequence(handle, mi, ctl, 1);
		break;
	case '<':
		change_sequence(handle, mi, ctl, -1);
		break;
	}
}