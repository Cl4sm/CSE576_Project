static int
get_uid (struct path * pp)
{
	char *c;
	const char *origin;

	if (!pp->uid_attribute && !pp->getuid)
		select_getuid(pp);

	if (!pp->udev) {
		condlog(1, "%s: no udev information", pp->dev);
		return 1;
	}

	memset(pp->wwid, 0, WWID_SIZE);
	if (pp->getuid) {
		char buff[CALLOUT_MAX_SIZE];

		/* Use 'getuid' callout, deprecated */
		condlog(1, "%s: using deprecated getuid callout", pp->dev);
		if (apply_format(pp->getuid, &buff[0], pp)) {
			condlog(0, "error formatting uid callout command");
			memset(pp->wwid, 0, WWID_SIZE);
		} else if (execute_program(buff, pp->wwid, WWID_SIZE)) {
			condlog(3, "error calling out %s", buff);
			memset(pp->wwid, 0, WWID_SIZE);
		}
		origin = "callout";
	} else {
		const char *value;

		value = udev_device_get_property_value(pp->udev,
						       pp->uid_attribute);
		if ((!value || strlen(value) == 0) && conf->dry_run == 2)
			value = getenv(pp->uid_attribute);
		if (value && strlen(value)) {
			size_t len = WWID_SIZE;

			if (strlen(value) + 1 > WWID_SIZE) {
				condlog(0, "%s: wwid overflow", pp->dev);
			} else {
				len = strlen(value);
			}
			strncpy(pp->wwid, value, len);
		} else {
			condlog(3, "%s: no %s attribute", pp->dev,
				pp->uid_attribute);
		}
		origin = "udev";
	}
	/* Strip any trailing blanks */
	c = strchr(pp->wwid, '\0');
	c--;
	while (c && c >= pp->wwid && *c == ' ') {
		*c = '\0';
		c--;
	}
	condlog(3, "%s: uid = %s (%s)", pp->dev,
		*pp->wwid == '\0' ? "<empty>" : pp->wwid, origin);
	return 0;
}
