unsigned int defaults_read_config(unsigned int to_syslog)
{
	FILE *f;
	char buf[MAX_LINE_LEN];
	char *res;

	f = open_fopen_r(DEFAULTS_CONFIG_FILE);
	if (!f)
		return 0;

	while ((res = fgets(buf, MAX_LINE_LEN, f))) {
		char *key, *value;

		if (!parse_line(res, &key, &value))
			continue;

		if (check_set_config_value(key, ENV_NAME_MASTER_MAP, value, to_syslog) ||
		    check_set_config_value(key, ENV_NAME_TIMEOUT, value, to_syslog) ||
		    check_set_config_value(key, ENV_NAME_NEGATIVE_TIMEOUT, value, to_syslog) ||
		    check_set_config_value(key, ENV_NAME_BROWSE_MODE, value, to_syslog) ||
		    check_set_config_value(key, ENV_NAME_LOGGING, value, to_syslog) ||
		    check_set_config_value(key, ENV_NAME_FORCE_STD_PROG_MAP_ENV, value, to_syslog) ||
		    check_set_config_value(key, ENV_LDAP_TIMEOUT, value, to_syslog) ||
		    check_set_config_value(key, ENV_LDAP_NETWORK_TIMEOUT, value, to_syslog) ||
		    check_set_config_value(key, ENV_NAME_MAP_OBJ_CLASS, value, to_syslog) ||
		    check_set_config_value(key, ENV_NAME_ENTRY_OBJ_CLASS, value, to_syslog) ||
		    check_set_config_value(key, ENV_NAME_MAP_ATTR, value, to_syslog) ||
		    check_set_config_value(key, ENV_NAME_ENTRY_ATTR, value, to_syslog) ||
		    check_set_config_value(key, ENV_NAME_VALUE_ATTR, value, to_syslog) ||
		    check_set_config_value(key, ENV_APPEND_OPTIONS, value, to_syslog) ||
		    check_set_config_value(key, ENV_MOUNT_WAIT, value, to_syslog) ||
		    check_set_config_value(key, ENV_UMOUNT_WAIT, value, to_syslog) ||
		    check_set_config_value(key, ENV_AUTH_CONF_FILE, value, to_syslog) ||
		    check_set_config_value(key, ENV_MAP_HASH_TABLE_SIZE, value, to_syslog) ||
		    check_set_config_value(key, ENV_MOUNT_NFS_DEFAULT_PROTOCOL, value, to_syslog))
			;
	}

	if (!feof(f) || ferror(f)) {
		if (!to_syslog) {
			fprintf(stderr,
				"fgets returned error %d while reading %s\n",
				ferror(f), DEFAULTS_CONFIG_FILE);
		} else {
			logmsg("fgets returned error %d while reading %s",
			      ferror(f), DEFAULTS_CONFIG_FILE);
		}
		fclose(f);
		return 0;
	}

	fclose(f);
	return 1;
}