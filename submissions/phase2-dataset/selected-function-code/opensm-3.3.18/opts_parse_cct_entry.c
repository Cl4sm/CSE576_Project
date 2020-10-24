static void opts_parse_cct_entry(IN osm_subn_t *p_subn, IN char *p_key,
				 IN char *p_val_str, void *p_v1, void *p_v2,
				 void (*pfn)(osm_subn_t *, void *))
{
	osm_cct_entry_t *p_cct1 = p_v1, *p_cct2 = p_v2;
	osm_cct_entry_t cct;
	char buf[512] = { 0 };
	char *ptr;

	strncpy(buf, p_val_str, 511);

	if (!(ptr = strchr(buf, ':'))) {
		log_report("invalid CCT entry\n");
		return;
	}

	*ptr = '\0';
	ptr++;

	cct.shift = strtoul(buf, NULL, 0);
	cct.multiplier = strtoul(ptr, NULL, 0);

	if (cct.shift != p_cct1->shift
	    || cct.multiplier != p_cct1->multiplier) {
		log_config_value(p_key, "%s", p_val_str);
		if (pfn)
			pfn(p_subn, &cct);
		p_cct1->shift = p_cct2->shift = cct.shift;
		p_cct1->multiplier = p_cct2->multiplier = cct.multiplier;
	}
}