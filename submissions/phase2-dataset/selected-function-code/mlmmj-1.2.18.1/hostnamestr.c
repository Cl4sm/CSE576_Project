char *hostnamestr()
{
	struct hostent *hostlookup;
	char *hostname = NULL;
	size_t len = 512;

	for (;;) {
		len *= 2;
		myfree(hostname);

		hostname = mymalloc(len);
		hostname[len-1] = '\0';

		/* gethostname() is allowed to:
		 * a) return -1 and undefined in hostname
		 * b) return 0 and an unterminated string in hostname
		 * c) return 0 and a NUL-terminated string in hostname
		 *
		 * We keep expanding the buffer until the hostname is
		 * NUL-terminated (and pray that it is not truncated)
		 * or an error occurs.
		 */
		if (gethostname(hostname, len - 1)) {
			if (errno == ENAMETOOLONG) {
				continue;
			}
			myfree(hostname);
			return mystrdup("localhost");
		}
		
		if (hostname[len-1] == '\0') {
			break;
		}
	}

	if (strchr(hostname, '.')) {
		/* hostname is FQDN */
		return hostname;
	}

	if ((hostlookup = gethostbyname(hostname))) {
		myfree(hostname);
		return mystrdup(hostlookup->h_name);
	}

	return hostname;
}