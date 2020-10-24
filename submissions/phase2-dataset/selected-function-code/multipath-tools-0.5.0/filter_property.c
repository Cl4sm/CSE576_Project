int
filter_property(struct config * conf, struct udev_device * udev)
{
	const char *devname = udev_device_get_sysname(udev);
	struct udev_list_entry *list_entry;
	int r;

	if (!udev)
		return 0;

	udev_list_entry_foreach(list_entry,
				udev_device_get_properties_list_entry(udev)) {
		const char *env;

		env = udev_list_entry_get_name(list_entry);
		if (!env)
			continue;

		r = _filter_property(conf, env);
		if (r) {
			log_filter(devname, NULL, NULL, NULL, env, r);
			return r;
		}
	}

	/*
	 * This is the inverse of the 'normal' matching;
	 * the environment variable _has_ to match.
	 */
	log_filter(devname, NULL, NULL, NULL, NULL,
		   MATCH_PROPERTY_BLIST_MISSING);
	return MATCH_PROPERTY_BLIST_MISSING;
}
