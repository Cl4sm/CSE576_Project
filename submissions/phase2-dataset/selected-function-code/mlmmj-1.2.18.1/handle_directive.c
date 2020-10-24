static int handle_directive(text *txt, char **line_p, char **pos_p,
		int *width_p, int *skipwhite_p,
		int conditionalsonly, const char *listdir)
{
	/* This function returns 1 to swallow a preceding blank line, i.e. if
	 * we just finished processing a failed conditional without an else
	 * part, -1 if we did nothing due to only processing conditionals, and
	 * 0 otherwise. */
	char *line = *line_p;
	char *pos = *pos_p;
	char *token = pos + 1;
	char *endpos;
	char *filename;
	int limit;
	formatted *fmt;
	conditional *cond;
	int swallow;

	endpos = strchr(token, '%');
	if (endpos == NULL) {
		if (conditionalsonly) return -1;
		(*pos_p)++;
		return 0;
	}

	*pos = '\0';
	*endpos = '\0';

	if(strncmp(token, "ifaction ", 9) == 0) {
		token += 9;
		if (handle_conditional(txt, line_p, pos_p, skipwhite_p, token,
				0, ACTION, 1, listdir) == 0) return 0;
	} else if(strncmp(token, "ifreason ", 9) == 0) {
		token += 9;
		if (handle_conditional(txt, line_p, pos_p, skipwhite_p, token,
				0, REASON, 1, listdir) == 0) return 0;
	} else if(strncmp(token, "iftype ", 7) == 0) {
		token += 7;
		if (handle_conditional(txt, line_p, pos_p, skipwhite_p, token,
				0, TYPE, 1, listdir) == 0) return 0;
	} else if(strncmp(token, "ifcontrol ", 10) == 0) {
		token += 10;
		if (handle_conditional(txt, line_p, pos_p, skipwhite_p, token,
				0, CONTROL, 1, listdir) == 0) return 0;
	} else if(strncmp(token, "ifnaction ", 10) == 0) {
		token += 10;
		if (handle_conditional(txt, line_p, pos_p, skipwhite_p, token,
				1, ACTION, 0, listdir) == 0) return 0;
	} else if(strncmp(token, "ifnreason ", 10) == 0) {
		token += 10;
		if (handle_conditional(txt, line_p, pos_p, skipwhite_p, token,
				1, REASON, 0, listdir) == 0) return 0;
	} else if(strncmp(token, "ifntype ", 8) == 0) {
		token += 8;
		if (handle_conditional(txt, line_p, pos_p, skipwhite_p, token,
				1, TYPE, 0, listdir) == 0) return 0;
	} else if(strncmp(token, "ifncontrol ", 11) == 0) {
		token += 11;
		if (handle_conditional(txt, line_p, pos_p, skipwhite_p, token,
				1, CONTROL, 1, listdir) == 0) return 0;
	} else if(strcmp(token, "else") == 0) {
		if (txt->cond != NULL) {
			if (txt->skip == txt->cond) txt->skip = NULL;
			else if (txt->skip == NULL) txt->skip = txt->cond;
			txt->cond->elsepart = 1;
			endpos++;
			if (*skipwhite_p) {
				while (*endpos == ' ' || *endpos == '\t')
						endpos++;
			}
			line = concatstr(2, line, endpos);
			*pos_p = line + (*pos_p - *line_p);
			myfree(*line_p);
			*line_p = line;
			return 0;
		}
	} else if(strcmp(token, "endif") == 0) {
		if (txt->cond != NULL) {
			if (txt->skip == txt->cond) txt->skip = NULL;
			cond = txt->cond;
			swallow = (!cond->satisfied && !cond->elsepart)?1:0;
			txt->cond = cond->outer;
			myfree(cond);
			endpos++;
			if (*skipwhite_p) {
				while (*endpos == ' ' || *endpos == '\t')
						endpos++;
			}
			line = concatstr(2, line, endpos);
			*pos_p = line + (*pos_p - *line_p);
			myfree(*line_p);
			*line_p = line;
			return swallow;
		}
	}

	if (conditionalsonly) {
		*pos = '%';
		*endpos = '%';
		return -1;
	}

	if (txt->skip != NULL) {
		/* We don't process anything but conditionals if we're
		 * already skipping text in one. */
		*pos = '%';
		*endpos = '%';
		(*pos_p)++;
		(*width_p)++;
		return 0;
	}

	*skipwhite_p = 0;

	if(strcmp(token, "") == 0) {
		line = concatstr(3, line, "%", endpos + 1);
		*pos_p = line + (*pos_p - *line_p) + 1;
		(*width_p)++;
		myfree(*line_p);
		*line_p = line;
		return 0;
	} else if(strcmp(token, "^") == 0) {
		if (txt->src->prefixlen != 0) {
			line[txt->src->prefixlen] = '\0';
			line = concatstr(2, line, endpos + 1);
			*width_p = txt->src->prefixwidth;
		} else {
			line = mystrdup(endpos + 1);
			*width_p = 0;
		}
		*pos_p = line;
		myfree(*line_p);
		*line_p = line;
		return 0;
	} else if(strcmp(token, "comment") == 0 || strcmp(token, "$") == 0 ) {
		pos = endpos + 1;
		while (*pos != '\0' && *pos != '\r' && *pos != '\n') pos++;
		line = concatstr(2, line, pos);
		*pos_p = line + (*pos_p - *line_p);
		myfree(*line_p);
		*line_p = line;
		return 0;
	} else if(strncmp(token, "wrap", 4) == 0) {
		token += 4;
		limit = 0;
		if (*token == '\0') {
			limit = 76;
		} else if (*token == ' ') {
			token = numeric_token(token + 1);
			if (token != NULL) limit = atol(token);
		}
		if (limit != 0) {
			txt->wrapindent = *width_p;
			txt->wrapwidth = limit;
			line = concatstr(2, line, endpos + 1);
			*pos_p = line + (*pos_p - *line_p);
			myfree(*line_p);
			*line_p = line;
			return 0;
		}
	} else if(strcmp(token, "nowrap") == 0) {
		txt->wrapwidth = 0;
		line = concatstr(2, line, endpos + 1);
		*pos_p = line + (*pos_p - *line_p);
		myfree(*line_p);
		*line_p = line;
		return 0;
	} else if(strcmp(token, "ww") == 0 ||
			strcmp(token, "wordwrap") == 0 ||
			strcmp(token, "cw") == 0 ||
			strcmp(token, "charwrap") == 0 ||
			strcmp(token, "uw") == 0 ||
			strcmp(token, "userwrap") == 0) {
		if (*token == 'w') txt->wrapmode = WRAP_WORD;
		if (*token == 'c') txt->wrapmode = WRAP_CHAR;
		if (*token == 'u') txt->wrapmode = WRAP_USER;
		line = concatstr(2, line, endpos + 1);
		*pos_p = line + (*pos_p - *line_p);
		myfree(*line_p);
		*line_p = line;
		return 0;
	} else if(strcmp(token, "thin") == 0) {
		txt->widthreckoning = WIDTH_THIN;
		line = concatstr(2, line, endpos + 1);
		*pos_p = line + (*pos_p - *line_p);
		myfree(*line_p);
		*line_p = line;
		return 0;
	} else if(strcmp(token, "wide") == 0) {
		txt->widthreckoning = WIDTH_WIDE;
		line = concatstr(2, line, endpos + 1);
		*pos_p = line + (*pos_p - *line_p);
		myfree(*line_p);
		*line_p = line;
		return 0;
	} else if(strncmp(token, "zero ", 5) == 0) {
		token += 5;
		if (txt->zerowidth != NULL) myfree(txt->zerowidth);
		txt->zerowidth = mystrdup(token);
		line = concatstr(2, line, endpos + 1);
		*pos_p = line + (*pos_p - *line_p);
		myfree(*line_p);
		*line_p = line;
		return 0;
	} else if(strncmp(token, "control ", 8) == 0) {
		token = filename_token(token + 8);
		if (token != NULL) {
			filename = concatstr(3, listdir, "/control/", token);
			begin_new_source_file(txt,
					line_p, pos_p, width_p, filename, 0);
			myfree(filename);
			return 0;
		}
	} else if(strncmp(token, "text ", 5) == 0) {
		token = filename_token(token + 5);
		if (token != NULL) {
			filename = concatstr(3, listdir, "/text/", token);
			begin_new_source_file(txt,
					line_p, pos_p, width_p, filename, 0);
			myfree(filename);
			return 0;
		}
	} else if(strncmp(token, "originalmail", 12) == 0 &&
			txt->mailname != NULL) {
		token += 12;
		limit = 0;
		if (*token == '\0') {
			limit = -1;
		} else if (*token == ' ') {
			token = numeric_token(token + 1);
			if (token != NULL) limit = atol(token);
		} else {
			token = numeric_token(token);
			if (token != NULL) limit = atol(token);
		}
		if (limit != 0) {
			begin_new_source_file(txt, line_p, pos_p, width_p,
					txt->mailname, 1);
			if (limit == -1) txt->src->limit = -1;
			else txt->src->limit = limit - 1;
			return 0;
		}
	}
	if (token == NULL) {
		/* We have encountered a directive, but not been able to deal
		 * with it, so just advance through the string. */
		*pos = '%';
		*endpos = '%';
		(*pos_p)++;
		(*width_p)++;
		return 0;
	}

	fmt = txt->fmts;
	while (fmt != NULL) {
		if (strcmp(token, fmt->token) == 0) {
			begin_new_formatted_source(txt, line_p, pos_p, width_p,
					endpos + 1, fmt, 0);
			return 0;
		}
		fmt = fmt->next;
	}

	/* No recognised directive; just advance through the string. */
	*pos = '%';
	*endpos = '%';
	(*pos_p)++;
	(*width_p)++;
	return 0;
}