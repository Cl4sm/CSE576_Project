int osm_db_restore(IN osm_db_domain_t * p_domain)
{

	osm_log_t *p_log = p_domain->p_db->p_log;
	osm_db_domain_imp_t *p_domain_imp =
	    (osm_db_domain_imp_t *) p_domain->p_domain_imp;
	FILE *p_file;
	int status;
	char sLine[OSM_DB_MAX_LINE_LEN];
	boolean_t before_key;
	char *p_first_word, *p_rest_of_line, *p_last;
	char *p_key = NULL;
	char *p_prev_val = NULL, *p_accum_val = NULL;
	char *endptr = NULL;
	unsigned int line_num;

	OSM_LOG_ENTER(p_log);

	/* take the lock on the domain */
	cl_spinlock_acquire(&p_domain_imp->lock);

	/* open the file - read mode */
	p_file = fopen(p_domain_imp->file_name, "r");

	if (!p_file) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 6103: "
			"Failed to open the db file:%s\n",
			p_domain_imp->file_name);
		status = 1;
		goto Exit;
	}

	/* parse the file allocating new hash tables as required */
	/*
	   states:
	   before_key (0) -> in_key (1)

	   before_key: if a word on the first byte - it is the key. state=in_key
	   the rest of the line is start of the value.
	   in_key: unless the line is empty - add it (with newlines) to the value.
	   if empty: state=before_key
	 */
	status = 0;
	before_key = TRUE;
	line_num = 0;
	/* if we got to EOF in the middle of a key we add a last newline */
	while ((fgets(sLine, OSM_DB_MAX_LINE_LEN, p_file) != NULL) ||
	       ((before_key == FALSE) && strcpy(sLine, "\n"))
	    ) {
		line_num++;
		if (before_key) {
			if ((sLine[0] != ' ') && (sLine[0] != '\t')
			    && (sLine[0] != '\n')) {
				/* we got a new key */
				before_key = FALSE;

				/* handle the key */
				p_first_word =
				    strtok_r(sLine, " \t\n", &p_last);
				if (!p_first_word) {
					OSM_LOG(p_log, OSM_LOG_ERROR,
						"ERR 6104: "
						"Failed to get key from line:%u : %s (file:%s)\n",
						line_num, sLine,
						p_domain_imp->file_name);
					status = 1;
					goto EndParsing;
				}
				if (strlen(p_first_word) > OSM_DB_MAX_GUID_LEN) {
					OSM_LOG(p_log, OSM_LOG_ERROR,
						"ERR 610A: "
						"Illegal key from line:%u : %s (file:%s)\n",
						line_num, sLine,
						p_domain_imp->file_name);
					status = 1;
					goto EndParsing;
				}

				p_key = malloc(sizeof(char) *
					       (strlen(p_first_word) + 1));
				strcpy(p_key, p_first_word);

				p_rest_of_line = strtok_r(NULL, "\n", &p_last);
				if (p_rest_of_line != NULL) {
					p_accum_val = malloc(sizeof(char) *
					    (strlen(p_rest_of_line) + 1));
					strcpy(p_accum_val, p_rest_of_line);
				} else {
					p_accum_val = malloc(2);
					strcpy(p_accum_val, "\0");
				}
			} else if (sLine[0] != '\n') {
				OSM_LOG(p_log, OSM_LOG_ERROR, "ERR 6105: "
					"How did we get here? line:%u : %s (file:%s)\n",
					line_num, sLine,
					p_domain_imp->file_name);
				status = 1;
				goto EndParsing;
			}
		} /* before key */
		else {
			/* we already have a key */

			if (sLine[0] == '\n') {
				/* got an end of key */
				before_key = TRUE;

				/* make sure the key was not previously used */
				if (st_lookup(p_domain_imp->p_hash,
					      (st_data_t) p_key,
					      (void *)&p_prev_val)) {
					/* if previously used we ignore this guid */
					OSM_LOG(p_log, OSM_LOG_ERROR,
						"ERR 6106: "
						"Key:%s already exists in:%s with value:%s."
						" Removing it\n", p_key,
						p_domain_imp->file_name,
						p_prev_val);
						free(p_key);
						p_key = NULL;
						free(p_accum_val);
						p_accum_val = NULL;
						continue;
				} else {
					p_prev_val = NULL;
				}

				OSM_LOG(p_log, OSM_LOG_DEBUG,
					"Got key:%s value:%s\n", p_key,
					p_accum_val);

				/* check that the key is a number */
				if (!strtouq(p_key, &endptr, 0)
				    && *endptr != '\0') {
					OSM_LOG(p_log, OSM_LOG_ERROR,
						"ERR 610B: "
						"Key:%s is invalid\n", p_key);
						free(p_key);
						p_key = NULL;
						free(p_accum_val);
						p_accum_val = NULL;
				} else {
					/* store our key and value */
					st_insert(p_domain_imp->p_hash,
						  (st_data_t) p_key,
						  (st_data_t) p_accum_val);
				}
			} else {
				/* accumulate into the value */
				p_prev_val = p_accum_val;
				p_accum_val = malloc(strlen(p_prev_val) +
						     strlen(sLine) + 1);
				strcpy(p_accum_val, p_prev_val);
				free(p_prev_val);
				p_prev_val = NULL;
				strcat(p_accum_val, sLine);
			}
		}		/* in key */
	}			/* while lines or last line */

EndParsing:
	fclose(p_file);

Exit:
	cl_spinlock_release(&p_domain_imp->lock);
	OSM_LOG_EXIT(p_log);
	return status;
}