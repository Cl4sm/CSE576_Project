ib_api_status_t osm_log_init_v2(IN osm_log_t * p_log, IN boolean_t flush,
				IN uint8_t log_flags, IN const char *log_file,
				IN unsigned long max_size,
				IN boolean_t accum_log_file)
{
	p_log->level = log_flags | OSM_LOG_SYS;
	p_log->flush = flush;
	p_log->count = 0;
	p_log->max_size = max_size << 20; /* convert size in MB to bytes */
	p_log->accum_log_file = accum_log_file;
	p_log->log_file_name = (char *)log_file;
	memset(p_log->per_mod_log_tbl, 0, sizeof(p_log->per_mod_log_tbl));

	openlog("OpenSM", LOG_CONS | LOG_PID, LOG_USER);

	if (log_file == NULL || !strcmp(log_file, "-") ||
	    !strcmp(log_file, "stdout"))
		p_log->out_port = stdout;
	else if (!strcmp(log_file, "stderr"))
		p_log->out_port = stderr;
	else if (open_out_port(p_log))
		return IB_ERROR;

	if (cl_spinlock_init(&p_log->lock) == CL_SUCCESS)
		return IB_SUCCESS;
	else
		return IB_ERROR;
}