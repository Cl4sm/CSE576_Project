static int parse_ca_cong_common(char *p_val_str, uint8_t *sl, unsigned int *val_offset) {
	char *new, *lasts, *sl_str, *val_str;
	uint8_t sltmp;

	new = strcmp(null_str, p_val_str) ? strdup(p_val_str) : NULL;
	if (!new)
		return -1;

	sl_str = strtok_r(new, " \t", &lasts);
	val_str = strtok_r(NULL, " \t", &lasts);

	if (!val_str) {
		log_report("value must be specified in addition to SL\n");
		free(new);
		return -1;
	}

	sltmp = strtoul(sl_str, NULL, 0);
	if (sltmp >= IB_CA_CONG_ENTRY_DATA_SIZE) {
		log_report("invalid SL specified\n");
		free(new);
		return -1;
	}

	*sl = sltmp;
	*val_offset = (unsigned int)(val_str - new);

	free(new);
	return 0;
}