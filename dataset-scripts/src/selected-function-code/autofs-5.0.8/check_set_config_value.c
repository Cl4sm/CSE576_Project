static int check_set_config_value(const char *res, const char *name, const char *value, unsigned to_syslog)
{
	char *old_name;
	int ret;

	if (!strcasecmp(res, name)) {
		ret = setenv(name, value, 0);
		if (ret) {
			if (!to_syslog)
				fprintf(stderr,
				        "can't set config value for %s, "
					"error %d\n", name, ret);
			else
				logmsg("can't set config value for %s, "
				      "error %d", name, ret);
		}
		return 1;
	}

	old_name = alloca(strlen(name) + 9);
	strcpy(old_name, "DEFAULT_");
	strcat(old_name, name);

	if (!strcasecmp(res, old_name)) {
		ret = setenv(name, value, 0);
		if (ret) {
			if (!to_syslog)
				fprintf(stderr,
				        "can't set config value for %s, "
					"error %d\n", name, ret);
			else
				logmsg("can't set config value for %s, "
				      "error %d\n", name, ret);
		}
		return 1;
	}
	return 0;
}