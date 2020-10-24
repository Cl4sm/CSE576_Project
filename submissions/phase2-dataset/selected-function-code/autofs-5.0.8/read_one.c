static int read_one(unsigned logopt, FILE *f, char *key, unsigned int *k_len, char *mapent, unsigned int *m_len)
{
	char *kptr, *p;
	int mapent_len, key_len;
	int ch, nch;
	LOOKUP_STATE state;
	FOUND_STATE getting, gotten;
	ESCAPES escape;

	kptr = key;
	p = NULL;
	mapent_len = key_len = 0;
	state = st_begin;
	memset(key, 0, KEY_MAX_LEN + 1);
	memset(mapent, 0, MAPENT_MAX_LEN + 1);
	getting = gotten = got_nothing;
	escape = esc_none;

	/* This is ugly.  We can't just remove \ escape sequences in the value
	   portion of an entry, because the parsing routine may need it. */

	while ((ch = getc(f)) != EOF) {
		switch (escape) {
		case esc_none:
			if (ch == '\\') {
				/* Handle continuation lines */
				if ((nch = getc(f)) == '\n')
					continue;
				ungetc(nch, f);
				escape = esc_char;
			}
			if (ch == '"')
				escape = esc_all;
			break;

		case esc_char:
			escape = esc_val;
			break;

		case esc_val:
			escape = esc_none;
			break;

		case esc_all:
			if (ch == '"')
				escape = esc_none;
			break;
		}

		switch (state) {
		case st_begin:
			if (!escape) {
				if (isspace(ch));
				else if (ch == '#')
					state = st_badent;
				else if (ch == '*') {
					state = st_star;
					*(kptr++) = ch;
					key_len++;
				} else {
					if (ch == '+')
						gotten = got_plus;
					state = st_compare;
					*(kptr++) = ch;
					key_len++;
				}
			} else if (escape == esc_all) {
				state = st_compare;
				*(kptr++) = ch;
				key_len++;
			} else if (escape == esc_char);
			else
				state = st_badent;
			break;

		case st_compare:
			if (ch == '\n') {
				state = st_begin;
				if (gotten == got_plus)
					goto got_it;
				else if (escape == esc_all) {
					warn(logopt, MODPREFIX
					    "unmatched \" in map key %s", key);
					goto next;
				} else if (escape != esc_val)
					goto got_it;
			} else if (isspace(ch) && !escape) {
				getting = got_real;
				state = st_entspc;
				if (gotten == got_plus)
					goto got_it;
			} else if (escape == esc_char);
			else {
				if (key_len == KEY_MAX_LEN) {
					state = st_badent;
					gotten = got_nothing;
					warn(logopt,
					      MODPREFIX "map key \"%s...\" "
					      "is too long.  The maximum key "
					      "length is %d", key,
					      KEY_MAX_LEN);
				} else {
					if (escape == esc_val) {
						*(kptr++) = '\\';
						key_len++;
					}
					*(kptr++) = ch;
					key_len++;
				}
			}
			break;

		case st_star:
			if (ch == '\n')
				state = st_begin;
			else if (isspace(ch) && gotten < got_star && !escape) {
				getting = got_star;
				state = st_entspc;
			} else if (escape != esc_char)
				state = st_badent;
			break;

		case st_badent:
			if (ch == '\n') {
				nch = getc(f);
				if (nch != EOF && isblank(nch)) {
					ungetc(nch, f);
					break;
				}
				ungetc(nch, f);
				state = st_begin;
				if (gotten == got_real || gotten == getting)
					goto got_it;
				warn(logopt, MODPREFIX 
				      "bad map entry \"%s...\" for key "
				      "\"%s\"", mapent, key);
				goto next;
			} else if (!isblank(ch))
				gotten = got_nothing;
			break;

		case st_entspc:
			if (ch == '\n')
				state = st_begin;
			else if (!isspace(ch) || escape) {
				if (escape) {
					if (escape == esc_char)
						break;
					if (ch <= 32) {
						getting = got_nothing;
						state = st_badent;
						break;
					}
					p = mapent;
					if (escape == esc_val) {
						*(p++) = '\\';
						mapent_len++;
					}
					*(p++) = ch;
					mapent_len++;
				} else {
					p = mapent;
					*(p++) = ch;
					mapent_len = 1;
				}
				state = st_getent;
				gotten = getting;
			}
			break;

		case st_getent:
			if (ch == '\n') {
				if (escape == esc_all) {
					state = st_begin;
					warn(logopt, MODPREFIX
					     "unmatched \" in %s for key %s",
					     mapent, key);
					goto next;
				}
				nch = getc(f);
				if (nch != EOF && isblank(nch)) {
					ungetc(nch, f);
					state = st_badent;
					break;
				}
				ungetc(nch, f);
				state = st_begin;
				if (gotten == got_real || gotten == getting)
					goto got_it;
			} else if (mapent_len < MAPENT_MAX_LEN) {
				if (p) {
					mapent_len++;
					*(p++) = ch;
				}
				nch = getc(f);
				if (nch == EOF &&
				   (gotten == got_real || gotten == getting))
				   	goto got_it;
				ungetc(nch, f);
			} else {
				warn(logopt,
				      MODPREFIX "map entry \"%s...\" for key "
				      "\"%s\" is too long.  The maximum entry"
				      " size is %d", mapent, key,
				      MAPENT_MAX_LEN);
				state = st_badent;
			}
			break;
		}
		continue;

	      got_it:
		if (gotten == got_nothing)
			goto next;

		*k_len = key_len;
		*m_len = mapent_len;

		return 1;

	      next:
		kptr = key;
		p = NULL;
		mapent_len = key_len = 0;
		memset(key, 0, KEY_MAX_LEN + 1);
		memset(mapent, 0, MAPENT_MAX_LEN + 1);
		getting = gotten = got_nothing;
		escape = esc_none;
	}

	return 0;
}