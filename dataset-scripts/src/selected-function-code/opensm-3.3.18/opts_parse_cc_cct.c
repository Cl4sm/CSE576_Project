static void opts_parse_cc_cct(IN osm_subn_t *p_subn, IN char *p_key,
			      IN char *p_val_str, void *p_v1, void *p_v2,
			      void (*pfn)(osm_subn_t *, void *))
{
	osm_cct_t *p_val1 = p_v1, *p_val2 = p_v2;
	const char *current_str = p_val1->input_str ? p_val1->input_str : null_str;

	if (p_val_str && strcmp(p_val_str, current_str)) {
		osm_cct_t newcct;
		char *new;
		unsigned int len = 0;
		char *lasts;
		char *tok;
		char *ptr;

		/* special case the "(null)" string */
		new = strcmp(null_str, p_val_str) ? strdup(p_val_str) : NULL;

		if (!new) {
			log_config_value(p_key, "%s", p_val_str);
			if (pfn)
				pfn(p_subn, NULL);
			memset(p_val1->entries, '\0', sizeof(p_val1->entries));
			memset(p_val2->entries, '\0', sizeof(p_val2->entries));
			p_val1->entries_len = p_val2->entries_len = 0;
			p_val1->input_str = p_val2->input_str = NULL;
			return;
		}

		memset(&newcct, '\0', sizeof(newcct));

		tok = strtok_r(new, ",", &lasts);
		while (tok && len < OSM_CCT_ENTRY_MAX) {

			if (!(ptr = strchr(tok, ':'))) {
				log_report("invalid CCT entry\n");
				free(new);
				return;
			}
			*ptr = '\0';
			ptr++;

			newcct.entries[len].shift = strtoul(tok, NULL, 0);
			newcct.entries[len].multiplier = strtoul(ptr, NULL, 0);
			len++;
			tok = strtok_r(NULL, ",", &lasts);
		}

		free(new);

		newcct.entries_len = len;
		newcct.input_str = strdup(p_val_str);

		log_config_value(p_key, "%s", p_val_str);
		if (pfn)
			pfn(p_subn, &newcct);
		if (p_val1->input_str && p_val1->input_str != p_val2->input_str)
			free(p_val1->input_str);
		if (p_val2->input_str)
			free(p_val2->input_str);
		memcpy(p_val1->entries, newcct.entries, sizeof(newcct.entries));
		memcpy(p_val2->entries, newcct.entries, sizeof(newcct.entries));
		p_val1->entries_len = p_val2->entries_len = newcct.entries_len;
		p_val1->input_str = p_val2->input_str = newcct.input_str;
	}
}