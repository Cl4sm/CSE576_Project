dm_no_partitions(int major, int minor)
{
	char params[PARAMS_SIZE], *ptr;
	int i, num_features;

	if (dm_get_map(major, minor, params))
		return 0;

	ptr = params;
	num_features = strtoul(params, &ptr, 10);
	if ((ptr == params) || num_features == 0) {
		/* No features found, return success */
		return 0;
	}
	for (i = 0; (i < num_features); i++) {
		if (!ptr || ptr > params + strlen(params))
			break;
		/* Skip whitespaces */
		while(ptr && *ptr == ' ') ptr++;
		if (!strncmp(ptr, FEATURE_NO_PART, strlen(FEATURE_NO_PART)))
			return 1;
		ptr = strchr(ptr, ' ');
	}
	return 0;
}
