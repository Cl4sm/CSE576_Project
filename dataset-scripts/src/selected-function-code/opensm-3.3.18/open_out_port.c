static int open_out_port(IN osm_log_t * p_log)
{
	struct stat st;

	if (p_log->accum_log_file)
		p_log->out_port = fopen(p_log->log_file_name, "a+");
	else
		p_log->out_port = fopen(p_log->log_file_name, "w+");

	if (!p_log->out_port) {
		syslog(LOG_CRIT, "Cannot open file \'%s\' for %s: %s\n",
		       p_log->log_file_name,
		       p_log->accum_log_file ? "appending" : "writing",
		       strerror(errno));
		fprintf(stderr, "Cannot open file \'%s\': %s\n",
			p_log->log_file_name, strerror(errno));
		return -1;
	}

	if (fstat(fileno(p_log->out_port), &st) == 0)
		p_log->count = st.st_size;

	syslog(LOG_NOTICE, "%s log file opened\n", p_log->log_file_name);

	if (p_log->daemon) {
		dup2(fileno(p_log->out_port), 0);
		dup2(fileno(p_log->out_port), 1);
		dup2(fileno(p_log->out_port), 2);
	}

	return 0;
}