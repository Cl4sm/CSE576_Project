char *get_processed_text_line(text *txt, int headers,
		const char *listaddr, const char *listdelim,
		const char *listdir)
{
	char *line;
	const char *item;
	char *pos;
	char *tmp;
	char *prev = NULL;
	int len, width, i;
	int processedlen, processedwidth;
	int wrapindentlen = -1;
	int incision, linebreak, linebreakwidth;
	int directive, inhibitbreak;
	int peeking = 0; /* for a failed conditional without an else */
	int skipwhite; /* skip whitespace after a conditional directive */
	int swallow;
	char utf8char[5] = {0, 0, 0, 0, 0};

	for (;;) {
		line = NULL;
		while (txt->src != NULL) {
			if (txt->src->upcoming != NULL) {
				if (prev != NULL) {
					/* If wrapping, we are going to swallow
					 * leading whitespace anyway, which is
					 * what the prefix will always be, so
					 * we needn't include it, nor the
					 * wrapindent; wrapindentlen is also
					 * already set from the previous
					 * iteration. */
					line = txt->src->upcoming;
					txt->src->upcoming = NULL;
					break;
				}
				/* Join the prefix, wrapindent and upcoming
				 * line. */
				len = strlen(txt->src->upcoming);
				processedlen = txt->src->processedlen;
				processedwidth = txt->src->processedwidth;
				if (txt->src->prefixwidth != 0) {
					/* prefixlen may be true for an existing
					 * prefix, not the one in txt->src, so
					 * set it afresh. */
					txt->src->prefixlen =
						strlen(txt->src->prefix);
					len += txt->src->prefixlen;
					processedlen += txt->src->prefixlen;
					processedwidth += txt->src->prefixwidth;
				}
				if (txt->wrapwidth != 0) {
					/* wrapindent is a width, but includes
					 * the prefix; the excess we make up
					 * with just spaces though, so one byte
					 * per character. */
					len += txt->wrapindent -
						txt->src->prefixwidth;
					processedlen += txt->wrapindent -
						txt->src->prefixwidth;
					processedwidth += txt->wrapindent -
						txt->src->prefixwidth;
				}
				line = mymalloc((len + 1) * sizeof(char));
				if (txt->src->prefixwidth != 0) {
					strcpy(line, txt->src->prefix);
					pos = line + txt->src->prefixlen;
				} else {
					pos = line;
				}
				if (txt->wrapwidth != 0) {
					i = txt->wrapindent -
						txt->src->prefixwidth;
					for (; i > 0; i--) *pos++ = ' ';
					wrapindentlen = pos - line;
				}
				strcpy(pos, txt->src->upcoming);
				myfree(txt->src->upcoming);
				txt->src->upcoming = NULL;
				break;
			}
			if (txt->src->limit != 0) {
				if (txt->src->fd != -1) {
					tmp = mygetline(txt->src->fd);
				} else if (txt->src->fmt != NULL) {
					item = (*txt->src->fmt->get)(
						txt->src->fmt->state);
					if (item==NULL) tmp = NULL;
					else tmp = mystrdup(item);
				} else {
					tmp = NULL;
				}
				if (txt->src->limit > 0) txt->src->limit--;
				if (tmp == NULL) {
					txt->src->upcoming = NULL;
				} else if (txt->src->transparent) {
					txt->src->upcoming = tmp;
					txt->src->processedlen = 0;
					txt->src->processedwidth = 0;
				} else {
					txt->src->upcoming =
						unistr_escaped_to_utf8(tmp);
					txt->src->processedlen = 0;
					txt->src->processedwidth = 0;
					myfree(tmp);
				}
			} else {
				txt->src->upcoming = NULL;
			}
			if (txt->src->upcoming != NULL) continue;
			close_source(txt);
		}
		if (line == NULL) {
			if (peeking) return mystrdup("");
			if (prev != NULL) return prev;
			return NULL;
		}

		if (prev != NULL) {
			/* Wrapping; join and start processing at the new bit,
			 * which is always unprocessed. */
			len = strlen(prev);
			pos = prev + len - 1;
			/* The width remains set from the previous iteration. */
			if (txt->wrapmode == WRAP_WORD) {
				while (pos >= prev + wrapindentlen &&
						(*pos == ' ' || *pos == '\t')) {
					pos--;
					len--;
					width--;
				}
			}
			pos++;
			*pos = '\0';
			pos = line;
			while (*pos == ' ' || *pos == '\t') pos++;
			if ((*pos == '\r' || *pos == '\n' || *pos == '\0') &&
					txt->skip == NULL) {
				/* Empty/white line; stop wrapping, finish
				   the last line and save the empty/white
				   line for later. */
				txt->wrapwidth = 0;
				txt->src->upcoming = line;
				txt->src->processedlen = 0;
				txt->src->processedwidth = 0;
				line = prev;
				pos = line + len;
				skipwhite = 0;
			} else {
				if (*prev == '\0') {
					tmp = mystrdup(pos);
				} else {
					if (txt->wrapmode == WRAP_WORD &&
							len > wrapindentlen) {
					    tmp = concatstr(3, prev, " ", pos);
					    len++;
					    width++;
					} else {
					    tmp = concatstr(2, prev, pos);
					}
				}
				myfree(line);
				line = tmp;
				myfree(prev);
				pos = line + len;
				skipwhite = 1;
			}
			/* We can always line-break where the input had one */
			linebreak = len;
			linebreakwidth = width;
			prev = NULL;
		} else {
			/* Not wrapping; start processing where we left off;
			 * there can't be any break opportunities in the
			 * processed part, and if it looks like there are, they
			 * must have been inhibited so aren't really. */
			pos = line + processedlen;
			len = processedlen;
			width = processedwidth;
			linebreak = 0;
			linebreakwidth = 0;
			skipwhite = 0;
		}

		if (txt->skip != NULL) {
			incision = len;
		} else {
			incision = -1;
		}
		directive = 0;
		inhibitbreak = 0;
		while (*pos != '\0') {
			if (txt->wrapwidth != 0 && width >= txt->wrapwidth &&
					!peeking && linebreak > wrapindentlen &&
					linebreak < len)
					break;
			if ((unsigned char)*pos > 0xbf && txt->skip == NULL &&
					txt->wrapmode == WRAP_CHAR &&
					!inhibitbreak) {
				linebreak = len;
				linebreakwidth = width;
			}
			if (*pos == '\r') {
				*pos = '\0';
				pos++;
				if (*pos == '\n') pos++;
				if (*pos == '\0') break;
				txt->src->upcoming = mystrdup(pos);
				txt->src->processedlen = 0;
				txt->src->processedwidth = 0;
				break;
			} else if (*pos == '\n') {
				*pos = '\0';
				pos++;
				if (*pos == '\0') break;
				txt->src->upcoming = mystrdup(pos);
				txt->src->processedlen = 0;
				txt->src->processedwidth = 0;
				break;
			} else if (*pos == ' ') {
				if (txt->skip == NULL &&
						txt->wrapmode != WRAP_USER &&
						!inhibitbreak) {
					linebreak = len + 1;
					linebreakwidth = width + 1;
				}
				inhibitbreak = 0;
			} else if (*pos == '\t') {
				/* Avoid breaking due to peeking */
				inhibitbreak = 0;
			} else if (txt->src->transparent) {
				/* Do nothing if the file is to be included
			 	 * transparently */
				if (peeking && txt->skip == NULL) break;
				inhibitbreak = 0;
			} else if (*pos == '\\' && txt->skip == NULL) {
				if (peeking) break;
				if (*(pos + 1) == '/') {
					linebreak = len;
					linebreakwidth = width;
					tmp = pos + 2;
					inhibitbreak = 0;
				} else if (*(pos + 1) == '=') {
					tmp = pos + 2;
					/* Ensure we don't wrap the next
					 * character */
					inhibitbreak = 1;
				} else {
					/* Includes space and backslash */
					tmp = pos + 1;
					/* Ensure we don't wrap a space */
					if (*(pos+1) == ' ') inhibitbreak = 1;
					else inhibitbreak = 0;
				}
				*pos = '\0';
				tmp = concatstr(2, line, tmp);
				pos = tmp + len;
				myfree(line);
				line = tmp;
				skipwhite = 0;
				continue;
			} else if (*pos == '$' && txt->skip == NULL) {
				if (peeking) break;
				substitute_one(&line, &pos, &width, listaddr,
						listdelim, listdir, txt);
				if (len != pos - line) {
					/* Cancel any break inhibition if the
					 * length changed (which will be
					 * because of $$) */
					inhibitbreak = 0;
					len = pos - line;
				}
				skipwhite = 0;
				/* The function sets up for the next character
				 * to process, so continue straight away. */
				continue;
			} else if (*pos == '%') {
				directive = 1;
				swallow = handle_directive(txt, &line, &pos,
						&width, &skipwhite,
						peeking, listdir);
				if (swallow == 1) peeking = 0;
				if (swallow == -1) break;
				if (txt->skip != NULL) {
					if (incision == -1) {
						/* We have to cut a bit out
						 * later */
						incision = pos - line;
					}
				} else {
					if (incision != -1) {
					    /* Time to cut */
					    if (pos - line != incision) {
						line[incision] = '\0';
						tmp = concatstr(2, line, pos);
						pos = tmp + incision;
						myfree(line);
						line = tmp;
					    }
					    incision = -1;
					}
				}
				if (len != pos - line) {
					/* Cancel any break inhibition if the
					 * length changed (which will be
					 * because of %% or %^% or an empty
					 * list) */
					inhibitbreak = 0;
					len = pos - line;
				}
				if (txt->wrapwidth != 0 &&
					    wrapindentlen == -1) {
					/* Started wrapping. */
					wrapindentlen = len;
				}
				/* handle_directive() sets up for the next
				 * character to process, so continue straight
				 * away. */
				continue;
			} else if (peeking && txt->skip == NULL) {
				break;
			}
			if (txt->skip == NULL) {
				len++;
				if ((unsigned char)*pos < 0x80) {
					width++;
				} else if ((unsigned char)*pos > 0xbf) {
					/* Not a UTF-8 continuation byte. */
					if (txt->zerowidth != NULL) {
					    tmp = pos;
					    utf8char[0] = *tmp++;
					    for (i = 1; i < 4; i++, tmp++) {
						if ((unsigned char)*tmp<0x80 ||
						      (unsigned char)*tmp>0xbf)
						      break;
						utf8char[i] = *tmp;
					    }
					    utf8char[i] = '\0';
					    if (strstr(txt->zerowidth, utf8char)
						    == NULL) {
						width++;
						if (txt->widthreckoning ==
						      WIDTH_WIDE)
						      width++;
					    }
					} else {
					    width++;
					    if (txt->widthreckoning ==
						    WIDTH_WIDE)
						    width++;
					}
				}
			}
			pos++;
			skipwhite = 0;
		}

		if (incision == 0) {
			/* The whole line was skipped; nothing to return yet;
			 * keep reading */
			incision = -1;
			myfree(line);
			continue;
		}

		if (incision != -1) {
			/* Time to cut */
			if (pos - line != incision) {
				line[incision] = '\0';
				tmp = mystrdup(line);
				pos = tmp + incision;
				myfree(line);
				line = tmp;
			}
			incision = -1;
		}

		if (txt->wrapwidth != 0 && !peeking) {
			if (width < txt->wrapwidth ||
					linebreak <= wrapindentlen ||
					linebreak >= len) {
				prev = line;
				continue;
			}
			if (linebreak != 0) {
				if (txt->wrapmode == WRAP_WORD &&
					line[linebreak-1] == ' ')
					line[linebreak-1] = '\0';
				if (line[linebreak] == '\0') linebreak = 0;
			}
			if (linebreak != 0) {
				if (txt->src->upcoming == NULL) {
				    tmp = mystrdup(line + linebreak);
				} else {
				    /* If something's coming up, it's because
				     * it was a new line. */
				    if (*(line + linebreak) != '\0') {
					tmp = concatstr(3, line + linebreak,
						"\n", txt->src->upcoming);
					myfree(txt->src->upcoming);
				    } else {
				    	tmp = txt->src->upcoming;
				    }
				}
				txt->src->upcoming = tmp;
				txt->src->processedlen = len - linebreak;
				txt->src->processedwidth =
						width - linebreakwidth;
			}
			line[linebreak] = '\0';
			tmp = mystrdup(line);
			myfree(line);
			line = tmp;
		} else {
			if (directive) {
				pos = line;
				while (*pos == ' ' || *pos == '\t') pos++;
				if (*pos == '\0') {
					/* Omit whitespace-only line with
					 * directives */
					myfree(line);
					continue;
				}
			}
			if (*line == '\0' && !headers && !peeking) {
				/* Non-wrapped bona fide blank line that isn't
				 * ending the headers; peek ahead to check it's
				 * not followed by an unsatisfied conditional
				 * without an else */
				peeking = 1;
				myfree(line);
				continue;
			} else if (peeking) {
				/* We found something; return preceding blank
				 * line */
				if (txt->src->upcoming == NULL) {
					txt->src->upcoming = line;
					txt->src->processedlen = len;
					txt->src->processedwidth = width;
				} else {
					tmp = txt->src->upcoming;
					txt->src->upcoming = concatstr(3,
							line, "\n",
							txt->src->upcoming);
					txt->src->processedlen = len;
					txt->src->processedwidth = width;
					myfree(line);
					myfree(tmp);
				}
				line = mystrdup("");
			}
		}

		if (txt->src->suffix != NULL) {
			tmp = concatstr(2, line, txt->src->suffix);
			myfree(line);
			return tmp;
		} else {
			return line;
		}
	}
}