static void opts_parse_charp(IN osm_subn_t *p_subn, IN char *p_key,
			     IN char *p_val_str, void *p_v1, void *p_v2,
			     void (*pfn)(osm_subn_t *, void *))
{
	char **p_val1 = p_v1, **p_val2 = p_v2;
	const char *current_str = *p_val1 ? *p_val1 : null_str ;

	if (p_val_str && strcmp(p_val_str, current_str)) {
		char *new;
		log_config_value(p_key, "%s", p_val_str);
		/* special case the "(null)" string */
		new = strcmp(null_str, p_val_str) ? strdup(p_val_str) : NULL;
		if (pfn)
			pfn(p_subn, new);
		if (*p_val1 && *p_val1 != *p_val2)
			free(*p_val1);
		if (*p_val2)
			free(*p_val2);
		*p_val1 = *p_val2 = new;
	}
}