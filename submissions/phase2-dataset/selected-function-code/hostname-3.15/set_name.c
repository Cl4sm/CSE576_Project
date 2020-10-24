void
set_name(enum type_t type, char *name)
{
	int i;

	switch (type) {
	case DEFAULT:
		/* Whitespaces are invalid characters in a hostname. */
		/* Thus remove trailing and leading whitespaces. */
		while (isspace(*name)) { name++; }
		for (i = strlen(name) - 1; i >= 0 && isspace(name[i]); i--);       
		name[i+1] = '\0';

		/* Now check for a valid name. */
		if (!check_name(name))
			errx(1, "the specified hostname is invalid");

		if (sethostname(name, strlen(name))) {
			if (errno == EPERM)
				errx(1, "you must be root to change the host name");
			else if (errno == EINVAL)
				errx(1, "name too long");
		}
		break;

	case NIS:
	case NIS_DEF:
		if (setdomainname(name, strlen(name))) {
			if (errno == EPERM)
				errx(1, "you must be root to change the domain name");
			else if (errno == EINVAL)
				errx(1, "name too long");
		}
		break;

	default:
		/*
		 * Only the host name and the domain name can be set using this
		 * command.
		 */
		usage(stderr);
	}
}