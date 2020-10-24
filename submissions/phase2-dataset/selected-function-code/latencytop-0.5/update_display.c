static int update_display(int duration, char *filterchar)
{
	struct timeval start,end,now;
	int key;
	int repaint = 1;
	fd_set rfds;

	gettimeofday(&start, NULL);
	setup_windows();
	show_title_bar();
	print_global_list();
	while (!done_yet(duration, &start)) {
		if (repaint) {
			display_process_list(pid_with_max, *filterchar);
			print_process(pid_with_max);
		}
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		gettimeofday(&now, NULL);
		end.tv_sec = start.tv_sec + duration - now.tv_sec;
		end.tv_usec = start.tv_usec - now.tv_usec;
		while (end.tv_usec < 0) {
			end.tv_sec --;
			end.tv_usec += 1000000;
		};
		key = select(1, &rfds, NULL, NULL, &end);
		repaint = 1;

		if (key) {
			char keychar;
			keychar = fgetc(stdin);		
			if (keychar == 27) {
				keychar = fgetc(stdin);	
				if (keychar==79)
					keychar = fgetc(stdin);	
			}
			keychar = toupper(keychar);
			if (keychar == 'Z' || keychar == 'A' || keychar == 'D')
				pid_with_max = one_pid_back(pid_with_max, *filterchar);
			if (keychar == 'X' || keychar == 'B' || keychar == 'C') 
				pid_with_max = one_pid_forward(pid_with_max, *filterchar);
			if (keychar == 'Q') 
				return 0;
			if (keychar == 'R') {
				cursor_e = NULL;
				return 1;
			}
			if (keychar == 'S') {
				keychar = fgetc(stdin);
				if (keychar == 27) {
					keychar = fgetc(stdin);	
					if (keychar==79)
						keychar = fgetc(stdin);	
				}
				keychar = toupper (keychar);
				if (keychar >= 'A' && keychar <= 'Z') 
					*filterchar = keychar;
				else if (keychar == '0')
					*filterchar = '\0';
			}
			if (keychar == 'F') {
				endwin();
				if (!fsync_display(duration))
					return 0;
				setup_windows();
				show_title_bar();
			}
			if (keychar < 32)
				repaint = 0;
		}
	}
	cursor_e = NULL;
	return 1;
}
