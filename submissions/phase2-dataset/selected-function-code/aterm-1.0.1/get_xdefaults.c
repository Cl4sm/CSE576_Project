get_xdefaults(FILE * stream, const char *name)
{
    unsigned int    len;
    char           *str, buffer[256];

    if (stream == NULL)
	return;
    len = strlen(name);
    while ((str = fgets(buffer, sizeof(buffer), stream)) != NULL) {
	unsigned int    entry, n;

	while (*str && isspace(*str))
	    str++;		/* leading whitespace */

	if ((str[len] != '*' && str[len] != '.') ||
	    (len && strncmp(str, name, len)))
	    continue;
	str += (len + 1);	/* skip `name*' or `name.' */

# ifdef KEYSYM_RESOURCE
	if (!parse_keysym(str, NULL))
# endif				/* KEYSYM_RESOURCE */
	    for (entry = 0; entry < optList_size(); entry++) {
		const char     *const kw = optList[entry].kw;

		if (kw == NULL)
		    continue;
		n = strlen(kw);
		if (str[n] == ':' && Str_match(str, kw)) {
		/* skip `keyword:' */
		    str += (n + 1);
		    str = Str_skip_space(str);
		    str = Str_trim(str);
		    n = (str ? strlen(str) : 0);
		    if (n && *(optList[entry].dp) == NULL) {
		    /* not already set */
			int		s;
			char           *p = MALLOC((n + 1) * sizeof(char));

			STRCPY(p, str);
			*(optList[entry].dp) = p;
			if (optList_isBool(entry)) {
			    s = my_strcasecmp(p, "TRUE") == 0
				|| my_strcasecmp(p, "YES") == 0
				|| my_strcasecmp(p, "ON") == 0
				|| my_strcasecmp(p, "1") == 0;
			    if (optList_isReverse(entry))
				s = !s;
			    if (s)
				Options |= (optList[entry].flag);
			    else {
				if (my_strcasecmp(str, "FALSE"))
				    print_error("Cannot parse value \"%s\" from resource \"%s\" as boolean",
						str, kw);
				Options &= ~(optList[entry].flag);
			    }
			}
		    }
		    break;
		}
	    }
    }
    rewind(stream);
}
