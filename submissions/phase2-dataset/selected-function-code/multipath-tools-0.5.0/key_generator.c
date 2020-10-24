char *
key_generator (const char * str, int state)
{
	static int index, len, has_param;
	static unsigned long rlfp;	
	struct key * kw;
	int i;
	struct handler *h;
	vector v = NULL;

	if (!state) {
		index = 0;
		has_param = 0;
		rlfp = 0;
		len = strlen(str);
		int r = get_cmdvec(rl_line_buffer, &v);
		/*
		 * If a word completion is in progess, we don't want
		 * to take an exact keyword match in the fingerprint.
		 * For ex "show map[tab]" would validate "map" and discard
		 * "maps" as a valid candidate.
		 */
		if (v && len)
			vector_del_slot(v, VECTOR_SIZE(v) - 1);
		/*
		 * Clean up the mess if we dropped the last slot of a 1-slot
		 * vector
		 */
		if (v && !VECTOR_SIZE(v)) {
			vector_free(v);
			v = NULL;
		}
		/*
		 * If last keyword takes a param, don't even try to guess
		 */
		if (r == E_NOPARM) {
			has_param = 1;
			return (strdup("(value)"));
		}
		/*
		 * Compute a command fingerprint to find out possible completions.
		 * Once done, the vector is useless. Free it.
		 */
		if (v) {
			rlfp = fingerprint(v);
			free_keys(v);
		}
	}
	/*
	 * No more completions for parameter placeholder.
	 * Brave souls might try to add parameter completion by walking paths and
	 * multipaths vectors.
	 */
	if (has_param)
		return ((char *)NULL);
	/*
	 * Loop through keywords for completion candidates
	 */
	vector_foreach_slot_after (keys, kw, index) {
		if (!strncmp(kw->str, str, len)) {
			/*
			 * Discard keywords already in the command line
			 */
			if (key_match_fingerprint(kw, rlfp)) {
				struct key * curkw = find_key(str);
				if (!curkw || (curkw != kw))
					continue;
			}
			/*
			 * Discard keywords making syntax errors.
			 *
			 * nfp is the candidate fingerprint we try to
			 * validate against all known command fingerprints.
			 */
			unsigned long nfp = rlfp | kw->code;
			vector_foreach_slot(handlers, h, i) {
				if (!rlfp || ((h->fingerprint & nfp) == nfp)) {
					/*
					 * At least one full command is
					 * possible with this keyword :
					 * Consider it validated
					 */
					index++;
					return (strdup(kw->str));
				}
			}
		}
	}
	/*
	 * No more candidates
	 */
	return ((char *)NULL);
}
