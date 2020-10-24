static void dump_portguid_parse(char **p_last, osm_opensm_t * p_osm, FILE * out)
{
	cl_qmap_t *p_alias_port_guid_tbl;
	osm_alias_guid_t *p_alias_guid, *p_next_alias_guid;
	regexp_list_t *p_regexp, *p_head_regexp = NULL;
	FILE *output = out;

	while (1) {
		char *p_cmd = next_token(p_last);
		if (!p_cmd)
			break;

		if (strcmp(p_cmd, "file") == 0) {
			p_cmd = next_token(p_last);
			if (p_cmd) {
				output = fopen(p_cmd, "w+");
				if (output == NULL) {
					fprintf(out,
						"Could not open file %s: %s\n",
						p_cmd, strerror(errno));
					output = out;
				}
			} else
				fprintf(out, "No file name passed\n");
		} else if (!(p_regexp = malloc(sizeof(*p_regexp)))) {
			fprintf(out, "No memory\n");
			break;
		} else if (regcomp(&p_regexp->exp, p_cmd,
				   REG_NOSUB | REG_EXTENDED) != 0) {
			fprintf(out, "Cannot parse regular expression \'%s\'."
				" Skipping\n", p_cmd);
			free(p_regexp);
			continue;
		} else {
			p_regexp->next = p_head_regexp;
			p_head_regexp = p_regexp;
		}
	}

	/* Check we have at least one expression to match */
	if (p_head_regexp == NULL) {
		fprintf(out, "No valid expression provided. Aborting\n");
		goto Exit;
	}

	if (p_osm->sm.p_subn->need_update != 0) {
		fprintf(out, "Subnet is not ready yet. Try again later\n");
		goto Free_and_exit;
	}

	/* Subnet doesn't need to be updated so we can carry on */

	p_alias_port_guid_tbl = &(p_osm->sm.p_subn->alias_port_guid_tbl);
	CL_PLOCK_ACQUIRE(p_osm->sm.p_lock);

	p_next_alias_guid = (osm_alias_guid_t *) cl_qmap_head(p_alias_port_guid_tbl);
	while (p_next_alias_guid != (osm_alias_guid_t *) cl_qmap_end(p_alias_port_guid_tbl)) {

		p_alias_guid = p_next_alias_guid;
		p_next_alias_guid =
		    (osm_alias_guid_t *) cl_qmap_next(&p_next_alias_guid->map_item);

		for (p_regexp = p_head_regexp; p_regexp != NULL;
		     p_regexp = p_regexp->next)
			if (regexec(&p_regexp->exp,
				    p_alias_guid->p_base_port->p_node->print_desc,
				    0, NULL, 0) == 0) {
				fprintf(output, "0x%" PRIxLEAST64 "\n",
					cl_ntoh64(p_alias_guid->alias_guid));
				break;
			}
	}

	CL_PLOCK_RELEASE(p_osm->sm.p_lock);

Free_and_exit:
	for (; p_head_regexp; p_head_regexp = p_regexp) {
		p_regexp = p_head_regexp->next;
		regfree(&p_head_regexp->exp);
		free(p_head_regexp);
	}
Exit:
	if (output != out)
		fclose(output);
}