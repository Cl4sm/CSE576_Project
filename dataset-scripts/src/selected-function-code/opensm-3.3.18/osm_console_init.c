int osm_console_init(osm_subn_opt_t * opt, osm_console_t * p_oct, osm_log_t * p_log)
{
	p_oct->socket = -1;
	strncpy(p_oct->client_type, opt->console, sizeof(p_oct->client_type));

	/* set up the file descriptors for the console */
	if (strcmp(opt->console, OSM_LOCAL_CONSOLE) == 0) {
		p_oct->in = stdin;
		p_oct->out = stdout;
		p_oct->in_fd = fileno(stdin);
		p_oct->out_fd = fileno(stdout);

		osm_console_prompt(p_oct->out);
#ifdef ENABLE_OSM_CONSOLE_LOOPBACK
	} else if (strcmp(opt->console, OSM_LOOPBACK_CONSOLE) == 0
#ifdef ENABLE_OSM_CONSOLE_SOCKET
		   || strcmp(opt->console, OSM_REMOTE_CONSOLE) == 0
#endif
		   ) {
		struct sockaddr_in sin;
		int optval = 1;

		if ((p_oct->socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
			OSM_LOG(p_log, OSM_LOG_ERROR,
				"ERR 4B01: Failed to open console socket: %s\n",
				strerror(errno));
			return -1;
		}

		if (setsockopt(p_oct->socket, SOL_SOCKET, SO_REUSEADDR,
			       &optval, sizeof(optval))) {
			OSM_LOG(p_log, OSM_LOG_ERROR,
		                "ERR 4B06: Failed to set socket option: %s\n",
		                strerror(errno));
		        return -1;
		}

		sin.sin_family = AF_INET;
		sin.sin_port = htons(opt->console_port);
#ifdef ENABLE_OSM_CONSOLE_SOCKET
		if (strcmp(opt->console, OSM_REMOTE_CONSOLE) == 0)
			sin.sin_addr.s_addr = htonl(INADDR_ANY);
		else
#endif
			sin.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
		if (bind(p_oct->socket, &sin, sizeof(sin)) < 0) {
			OSM_LOG(p_log, OSM_LOG_ERROR,
				"ERR 4B02: Failed to bind console socket: %s\n",
				strerror(errno));
			return -1;
		}
		if (listen(p_oct->socket, 1) < 0) {
			OSM_LOG(p_log, OSM_LOG_ERROR,
				"ERR 4B03: Failed to listen on console socket: %s\n",
				strerror(errno));
			return -1;
		}

		signal(SIGPIPE, SIG_IGN);	/* protect ourselves from closed pipes */
		p_oct->in = NULL;
		p_oct->out = NULL;
		p_oct->in_fd = -1;
		p_oct->out_fd = -1;
		OSM_LOG(p_log, OSM_LOG_INFO,
			"Console listening on port %d\n", opt->console_port);
#endif
	}

	return 0;
}