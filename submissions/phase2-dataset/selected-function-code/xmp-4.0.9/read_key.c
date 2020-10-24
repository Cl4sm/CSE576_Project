static int read_key(void)
{
	char key;
	int ret;

#if defined HAVE_TERMIOS_H && !defined WIN32
#ifdef __CYGWIN__
	if (stdin_ready_for_reading())
#endif
		ret = read(0, &key, 1);
#elif defined WIN32
	if (kbhit()) {
		key = getch();
		ret = 1;
	}
#elif defined __AMIGA__
	/* Amiga CLI */
	{
		BPTR in = Input();
		if (WaitForChar(in, 1)) {
			Read(in, &key, 1);
			ret = 1;
		}
	}
#else
	ret = 0;
#endif

	if (ret <= 0) {
		return -1;
	}

	return key;
}