static int handle_conditional(text *txt, char **line_p, char **pos_p,
		int *skipwhite_p, char *token,
		int neg, enum conditional_target tgt, int multi,
		const char *listdir)
{
	/* This function handles a conditional directive and returns a boolean
	 * (0 or 1) representing whether it was successfully handled or not.
	 * The conditional should already have been identified, and the type of
	 * conditional, whether it is negative, whether multiple parameters are
	 * acceptable, and the position of the first parameter should be passed
	 * in.
	*/
	char *line = *line_p;
	char *pos;
	int satisfied = 0;
	int matches;
	conditional *cond;

	if (txt->skip == NULL) {
		for (;;) {
			pos = token;
			if (*pos == '\0') break;
			while (
				(*pos >= '0' && *pos <= '9') ||
				(*pos >= 'A' && *pos <= 'Z') ||
				(*pos >= 'a' && *pos <= 'z') ||
				(*pos == '_') || (*pos == '-') ||
				(*pos == '.' && pos != token)
			) {
				pos++;
			}
			if (*pos == ' ') {
				*pos = '\0';
			} else {
				multi = 0;
			}
			if (*pos != '\0') return 1;

			matches = 0;
			if (tgt == ACTION) {
				if (txt->action == NULL) return 1;
				if (strcasecmp(token, txt->action) == 0)
						matches = 1;
			} else if (tgt == REASON) {
				if (txt->reason == NULL) return 1;
				if (strcasecmp(token, txt->reason) == 0)
						matches = 1;
			} else if (tgt == TYPE) {
				if (txt->type == NULL) return 1;
				if (strcasecmp(token, txt->type) == 0)
						matches = 1;
			} else if (tgt == CONTROL) {
				if (statctrl(listdir, token))
						matches = 1;
			}
			if ((matches && !neg) || (!matches && neg)) {
				satisfied = 1;
				break;
			}

			if (!multi) break;
			*pos = ' ';
			token = pos + 1;
		}
	} else {
		/* We consider nested conditionals as successful while skipping
		 * text so they don't register themselves as the reason for
		 * skipping, nor trigger swallowing blank lines */
		satisfied = 1;
		pos = token + 1;
		while (*pos != '\0') pos++;
		multi = 0;
	}

	cond = mymalloc(sizeof(conditional));
	cond->satisfied = satisfied;
	cond->elsepart = 0;
	cond->outer = txt->cond;
	txt->cond = cond;
	if (!satisfied) txt->skip = cond;

	if (multi) {
		*pos = ' ';
		pos++;
		while (*pos != '\0') pos++;
	}
	pos++;
	if (*skipwhite_p) {
		while (*pos == ' ' || *pos == '\t') pos++;
	}
	line = concatstr(2, line, pos);
	*pos_p = line + (*pos_p - *line_p);
	myfree(*line_p);
	*line_p = line;

	return 0;
}