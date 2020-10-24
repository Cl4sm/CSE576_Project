cli_init (void) {
	if (load_keys())
		return 1;

	if (alloc_handlers())
		return 1;

	add_handler(LIST+PATHS, NULL);
	add_handler(LIST+PATHS+FMT, NULL);
	add_handler(LIST+STATUS, NULL);
	add_handler(LIST+DAEMON, NULL);
	add_handler(LIST+MAPS, NULL);
	add_handler(LIST+MAPS+STATUS, NULL);
	add_handler(LIST+MAPS+STATS, NULL);
	add_handler(LIST+MAPS+FMT, NULL);
	add_handler(LIST+MAPS+TOPOLOGY, NULL);
	add_handler(LIST+TOPOLOGY, NULL);
	add_handler(LIST+MAP+TOPOLOGY, NULL);
	add_handler(LIST+CONFIG, NULL);
	add_handler(LIST+BLACKLIST, NULL);
	add_handler(LIST+DEVICES, NULL);
	add_handler(LIST+WILDCARDS, NULL);
	add_handler(ADD+PATH, NULL);
	add_handler(DEL+PATH, NULL);
	add_handler(ADD+MAP, NULL);
	add_handler(DEL+MAP, NULL);
	add_handler(SWITCH+MAP+GROUP, NULL);
	add_handler(RECONFIGURE, NULL);
	add_handler(SUSPEND+MAP, NULL);
	add_handler(RESUME+MAP, NULL);
	add_handler(RESIZE+MAP, NULL);
	add_handler(RESET+MAP, NULL);
	add_handler(RELOAD+MAP, NULL);
	add_handler(DISABLEQ+MAP, NULL);
	add_handler(RESTOREQ+MAP, NULL);
	add_handler(DISABLEQ+MAPS, NULL);
	add_handler(RESTOREQ+MAPS, NULL);
	add_handler(REINSTATE+PATH, NULL);
	add_handler(FAIL+PATH, NULL);
	add_handler(QUIT, NULL);
	add_handler(SHUTDOWN, NULL);
	add_handler(GETPRSTATUS+MAP, NULL);
	add_handler(SETPRSTATUS+MAP, NULL);
	add_handler(UNSETPRSTATUS+MAP, NULL);
	add_handler(FORCEQ+DAEMON, NULL);
	add_handler(RESTOREQ+DAEMON, NULL);

	return 0;
}
