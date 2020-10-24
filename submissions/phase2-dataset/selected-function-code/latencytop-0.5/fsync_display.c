int fsync_display(int duration)
{
	struct timeval start,end,now;
	int key;
	fd_set rfds;
	int curduration;

	fsync_setup_windows();
	show_title_bar();
	curduration = 3;
	if (curduration > duration)
		curduration = duration;
	parse_ftrace();
	print_global_list();
	while (1) {
		FD_ZERO(&rfds);
		FD_SET(0, &rfds);
		gettimeofday(&start, NULL);
		gettimeofday(&now, NULL);
		end.tv_sec = start.tv_sec + curduration - now.tv_sec;
		end.tv_usec = start.tv_usec - now.tv_usec;
		while (end.tv_usec < 0) {
			end.tv_sec --;
			end.tv_usec += 1000000;
		};
		curduration = duration;
		if (curduration > 5)
			curduration = 5;
		/* clear the ftrace buffer */
		write_to_file("/sys/kernel/debug/tracing/tracing_enabled", "0");
		write_to_file("/sys/kernel/debug/tracing/tracing_enabled", "1");
		key = select(1, &rfds, NULL, NULL, &end);
		parse_ftrace();
		print_global_list();

		if (key) {
			char keychar;
			keychar = fgetc(stdin);		
			if (keychar == 27) {
				keychar = fgetc(stdin);	
				if (keychar==79)
					keychar = fgetc(stdin);	
			}
			keychar = toupper(keychar);
			if (keychar == 'F') {
				fsync_cleanup_curses();
				return 1;
			}
			if (keychar == 'Q') {
				fsync_cleanup_curses();
				return 0;
			}
		}
	}

	return 1;
}
