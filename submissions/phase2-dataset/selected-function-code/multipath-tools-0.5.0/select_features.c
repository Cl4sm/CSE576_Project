select_features (struct multipath * mp)
{
	struct mpentry * mpe;
	char *origin;

	if ((mpe = find_mpe(mp->wwid)) && mpe->features) {
		mp->features = STRDUP(mpe->features);
		origin = "LUN setting";
	} else if (mp->hwe && mp->hwe->features) {
		mp->features = STRDUP(mp->hwe->features);
		origin = "controller setting";
	} else if (conf->features) {
		mp->features = STRDUP(conf->features);
		origin = "config file default";
	} else {
		mp->features = set_default(DEFAULT_FEATURES);
		origin = "internal default";
	}
	condlog(3, "%s: features = %s (%s)",
		mp->alias, mp->features, origin);
	if (strstr(mp->features, "queue_if_no_path")) {
		if (mp->no_path_retry == NO_PATH_RETRY_UNDEF)
			mp->no_path_retry = NO_PATH_RETRY_QUEUE;
		else if (mp->no_path_retry == NO_PATH_RETRY_FAIL) {
			condlog(1, "%s: config error, overriding 'no_path_retry' value",
				mp->alias);
			mp->no_path_retry = NO_PATH_RETRY_QUEUE;
		}
	}
	return 0;
}
