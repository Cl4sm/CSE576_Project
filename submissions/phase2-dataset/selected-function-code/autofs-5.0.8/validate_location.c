static int validate_location(unsigned int logopt, char *loc)
{
	char *ptr = loc;

	/* We don't know much about these */
	if (*ptr == ':')
		return 1;

	/*
	 * If a ':/' is present now it must be a host name, except
	 * for those special file systems like sshfs which use "#"
	 * and "@" in the host name part and ipv6 addresses that
	 * have ":", "[" and "]".
	 */
	if (!check_colon(ptr)) {
		char *esc;
		/*
		 * Don't forget cases where a colon is present but
		 * not followed by a "/" or, if there is no colon at
		 * all, we don't know if it is actually invalid since
		 * it may be a map name by itself, for example.
		 */
		if (!strchr(ptr, ':') ||
		    ((esc = strchr(ptr, '\\')) && *(esc + 1) == ':') ||
		    !strncmp(ptr, "file:", 5) || !strncmp(ptr, "yp:", 3) ||
		    !strncmp(ptr, "nis:", 4) || !strncmp(ptr, "nisplus:", 8) ||
		    !strncmp(ptr, "ldap:", 5) || !strncmp(ptr, "ldaps:", 6) ||
		    !strncmp(ptr, "sss:", 4) || !strncmp(ptr, "dir:", 4))
			return 1;
		error(logopt,
		      "expected colon delimeter not found in location %s",
		      loc);
		return 0;
	} else {
		while (*ptr && strncmp(ptr, ":/", 2)) {
			if (!(isalnum(*ptr) ||
			    *ptr == '-' || *ptr == '.' || *ptr == '_' ||
			    *ptr == ',' || *ptr == '(' || *ptr == ')' ||
			    *ptr == '#' || *ptr == '@' || *ptr == ':' ||
			    *ptr == '[' || *ptr == ']')) {
				error(logopt, "invalid character \"%c\" "
				      "found in location %s", *ptr, loc);
				return 0;
			}
			ptr++;
		}

		if (*ptr && !strncmp(ptr, ":/", 2))
			ptr++;
	}

	/* Must always be something following */
	if (!*ptr) {
		error(logopt, "invalid location %s", loc);
		return 0;
	}

	return 1;
}