{
	char path[FILE_NAME_SIZE];
	char *arg;
	char *temp, *regex, *prio;
	char split_char[] = " \t";
	int priority = DEFAULT_PRIORITY, path_found = 0;
	regex_t pathe;

	/* Return default priority if there is no argument */
	if (!prio_args)
		return priority;

	arg = temp = STRDUP(prio_args);

	regex = get_next_string(&temp, split_char);

	/* Return default priority if the argument is not parseable */
	if (!regex)
		return priority;

	if (!strcmp(regex, HBTL)) {
		sprintf(path, "%d:%d:%d:%d", pp->sg_id.host_no,
			pp->sg_id.channel, pp->sg_id.scsi_id, pp->sg_id.lun);
	} else if (!strcmp(regex, DEV_NAME)) {
		strcpy(path, pp->dev);
	} else {
		condlog(0, "%s: %s - Invalid arguments", pp->dev,
			pp->prio.name);
		return priority;
	}

	while (!path_found) {
		if (!temp)
			break;
		if (!(regex = get_next_string(&temp, split_char)))
			break;
		if (!(prio = get_next_string(&temp, split_char)))
			break;

		if (!regcomp(&pathe, regex, REG_EXTENDED|REG_NOSUB)) {
			if (!regexec(&pathe, path, 0, NULL, 0)) {
				path_found = 1;
				priority = atoi(prio);
			}
			regfree(&pathe);
		}
	}

	FREE(arg);
	return priority;
}
