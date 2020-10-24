static enum action do_access(struct strlist *rule_strs, struct strlist *hdrs,
		const char *from, const char *listdir)
{
	int i, j;
	unsigned int match;
	char *rule_ptr;
	char errbuf[128];
	int err;
	enum action act;
	unsigned int not;
	regex_t regexp;
	char *hdr;

	for (i=0; i<rule_strs->count; i++) {

		rule_ptr = rule_strs->strs[i];

		if (strncmp(rule_ptr, "allow", 5) == 0) {
			rule_ptr += 5;
			act = ALLOW;
		} else if (strncmp(rule_ptr, "send", 4) == 0) {
			rule_ptr += 4;
			act = SEND;
		} else if (strncmp(rule_ptr, "deny", 4) == 0) {
			rule_ptr += 4;
			act = DENY;
		} else if (strncmp(rule_ptr, "moderate", 8) == 0) {
			rule_ptr += 8;
			act = MODERATE;
		} else if (strncmp(rule_ptr, "discard", 7) == 0) {
			rule_ptr += 7;
			act = DISCARD;
		} else {
			errno = 0;
			log_error(LOG_ARGS, "Unable to parse rule #%d \"%s\":"
					" Missing action keyword. Denying post from \"%s\"",
					i, rule_strs->strs[i], from);
			log_oper(listdir, OPLOGFNAME, "Unable to parse rule #%d \"%s\":"
					" Missing action keyword. Denying post from \"%s\"",
					i, rule_strs->strs[i], from);
			return DENY;
		}

		if (*rule_ptr == ' ') {
			rule_ptr++;
		} else if (*rule_ptr == '\0') {
			/* the rule is a keyword and no regexp */
			log_oper(listdir, OPLOGFNAME, "mlmmj-process: access -"
					" A mail from \"%s\" was %s by rule #%d \"%s\"",
					from, action_strs[act], i, rule_strs->strs[i]);
			return act;
		} else {
			/* we must have space or end of string */
			errno = 0;
			log_error(LOG_ARGS, "Unable to parse rule #%d \"%s\":"
					" Invalid character after action keyword."
					" Denying post from \"%s\"", i, rule_strs->strs[i], from);
			log_oper(listdir, OPLOGFNAME, "Unable to parse rule #%d \"%s\":"
					" Invalid character after action keyword."
					" Denying post from \"%s\"", i, rule_strs->strs[i], from);
			return DENY;
		}

		if (*rule_ptr == '!') {
			rule_ptr++;
			not = 1;
		} else {
			not = 0;
		}

		/* remove unanchored ".*" from beginning of regexp to stop the
		 * regexp matching to loop so long time it seems like it's
		 * hanging */
		if (strncmp(rule_ptr, "^.*", 3) == 0) {
			rule_ptr += 3;
		}
		while (strncmp(rule_ptr, ".*", 2) == 0) {
			rule_ptr += 2;
		}

		if ((err = regcomp(&regexp, rule_ptr,
				REG_EXTENDED | REG_NOSUB | REG_ICASE))) {
			regerror(err, &regexp, errbuf, sizeof(errbuf));
			regfree(&regexp);
			errno = 0;
			log_error(LOG_ARGS, "regcomp() failed for rule #%d \"%s\""
					" (message: '%s') (expression: '%s')"
					" Denying post from \"%s\"",
					i, rule_strs->strs[i], errbuf, rule_ptr, from);
			log_oper(listdir, OPLOGFNAME, "regcomp() failed for rule"
					" #%d \"%s\" (message: '%s') (expression: '%s')"
					" Denying post from \"%s\"",
					i, rule_strs->strs[i], errbuf, rule_ptr, from);
			return DENY;
		}

		match = 0;
		for (j=0; j<hdrs->count; j++) {
			if (regexec(&regexp, hdrs->strs[j], 0, NULL, 0)
					== 0) {
				match = 1;
				break;
			}
		}

		regfree(&regexp);

		if (match != not) {
			if (match) {
				hdr = mystrdup(hdrs->strs[j]);
				chomp(hdr);
				log_oper(listdir, OPLOGFNAME, "mlmmj-process: access -"
						" A mail from \"%s\" with header \"%s\" was %s by"
						" rule #%d \"%s\"", from, hdr, action_strs[act],
						i, rule_strs->strs[i]);
				myfree(hdr);
			} else {
				log_oper(listdir, OPLOGFNAME, "mlmmj-process: access -"
						" A mail from \"%s\" was %s by rule #%d \"%s\""
						" because no header matched.", from,
						action_strs[act], i, rule_strs->strs[i]);
			}
			return act;
		}

	}

	log_oper(listdir, OPLOGFNAME, "mlmmj-process: access -"
			" A mail from \"%s\" didn't match any rules, and"
			" was denied by default.", from);
	return DENY;
}