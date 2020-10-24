static void opts_parse_boolean(IN osm_subn_t *p_subn, IN char *p_key,
			       IN char *p_val_str, void *p_v1, void *p_v2,
			       void (*pfn)(osm_subn_t *, void *))
{
	boolean_t *p_val1 = p_v1, *p_val2 = p_v2;
	boolean_t val;

	if (!p_val_str)
		return;

	if (strcmp("TRUE", p_val_str))
		val = FALSE;
	else
		val = TRUE;

	if (val != *p_val1) {
		log_config_value(p_key, "%s", p_val_str);
		if (pfn)
			pfn(p_subn, &val);
		*p_val1 = *p_val2 = val;
	}
}