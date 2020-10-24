static void
logging(int fd) {
	if (Log == -1) {
		int save_nonblock = tty_nonblock(fd, 0);

		printf("\r\nLog file is: "); fflush(stdout);
		fgets(LogSpec, sizeof LogSpec, stdin);
		if (LogSpec[strlen(LogSpec) - 1] == '\n')
			LogSpec[strlen(LogSpec)-1] = '\0';
		if (LogSpec[0]) {
			Log = open(LogSpec, O_CREAT|O_APPEND|O_WRONLY, 0640);
			if (Log == -1)
				perror(LogSpec);
		}
		(void) tty_nonblock(fd, save_nonblock);
	} else {
		if (0 > close(Log))
			perror(LogSpec);
		else
			printf("\n[%s closed]\n", LogSpec);
		Log = -1;
	}
}
