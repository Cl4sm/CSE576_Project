unsigned int defaults_get_logging(void)
{
	char *res;
	unsigned int logging = DEFAULT_LOGGING;

	res = get_env_string(ENV_NAME_LOGGING);
	if (!res)
		return logging;

	if (!strcasecmp(res, "none"))
		logging = DEFAULT_LOGGING;
	else {
		if (!strcasecmp(res, "verbose"))
			logging |= LOGOPT_VERBOSE;

		if (!strcasecmp(res, "debug"))
			logging |= LOGOPT_DEBUG;
	}

	free(res);

	return logging;
}