static int parse_server_string(unsigned logopt, const char *url, struct lookup_context *ctxt)
{
	char buf[MAX_ERR_BUF], *tmp = NULL, proto[9];
	const char *ptr, *name;
	int l, al_len;

	memset(proto, 0, 9);
	ptr = url;

	debug(logopt, MODPREFIX
	      "Attempting to parse LDAP information from string \"%s\".", ptr);

	ctxt->port = LDAP_PORT;
	if (!strncmp(ptr, "ldap:", 5) || !strncmp(ptr, "ldaps:", 6)) {
		if (*(ptr + 4) == 's') {
			ctxt->port = LDAPS_PORT;
			memcpy(proto, ptr, 6);
			strcat(proto, "//");
			ptr += 6;
		} else {
			memcpy(proto, ptr, 5);
			strcat(proto, "//");
			ptr += 5;
		}
	}

	if (!strncmp(ptr, "//", 2)) {
		const char *s = ptr + 2;
		const char *q = NULL;

		/* Isolate the server(s). */
		if ((q = strchr(s, '/'))) {
			l = q - s;
			if (*proto) {
				al_len = l + strlen(proto) + 2;
				tmp = malloc(al_len);
			} else {
				al_len = l + 1;
				tmp = malloc(al_len);
			}
			if (!tmp) {
				char *estr;
				estr = strerror_r(errno, buf, sizeof(buf));
				logerr(MODPREFIX "malloc: %s", estr);
				return 0;
			}
			ctxt->server = tmp;
			memset(ctxt->server, 0, al_len);
			if (*proto) {
				strcpy(ctxt->server, proto);
				memcpy(ctxt->server + strlen(proto), s, l);
				strcat(ctxt->server, "/");
			} else
				memcpy(ctxt->server, s, l);
			ptr = q + 1;
		} else {
			crit(logopt,
			     MODPREFIX "invalid LDAP map syntax %s", ptr);
			return 0;
/* TODO: why did I put this here, the parser shouldn't let this by
			l = strlen(ptr);
			tmp = malloc(l + 1);
			if (!tmp) {
				char *estr;
				estr = strerror_r(errno, buf, sizeof(buf));
				crit(logopt, MODPREFIX "malloc: %s", estr);
				return 0;
			}
			ctxt->server = tmp;
			memset(ctxt->server, 0, l + 1);
			memcpy(ctxt->server, s, l);
*/
		}
	} else if (strchr(ptr, ':') != NULL || *ptr == '[') {
		const char *q = NULL;

		/* Isolate the server. Include the port spec */
		if (*ptr != '[') {
			q = strchr(ptr, ':');
			if (!q) {
				crit(logopt, MODPREFIX
				     "LDAP server name not found in %s", ptr);
				return 0;
			}
		} else {
			q = ++ptr;
			while (*q == ':' || isxdigit(*q))
				q++;
			if (*q != ']') {
				crit(logopt, MODPREFIX
				     "invalid LDAP map syntax %s", ptr);
				return 0;
			}
			q++;
			if (*q == ':')
				q++;
		}

		if (isdigit(*q))
			while (isdigit(*q))
				q++;

		if (*q != ':') {
			crit(logopt,
			     MODPREFIX "invalid LDAP map syntax %s", ptr);
			return 0;
		}

		l = q - ptr;
		if (*proto) {
			al_len = l + strlen(proto) + 2;
			tmp = malloc(al_len);
		} else {
			al_len = l + 1;
			tmp = malloc(al_len);
		}
		/* Isolate the server's name. */
		if (!tmp) {
			char *estr;
			estr = strerror_r(errno, buf, sizeof(buf));
			logerr(MODPREFIX "malloc: %s", estr);
			return 0;
		}
		ctxt->server = tmp;
		memset(ctxt->server, 0, al_len);
		if (*proto) {
			strcpy(ctxt->server, proto);
			memcpy(ctxt->server + strlen(proto), ptr, l);
			strcat(ctxt->server, "/");
		} else
			memcpy(ctxt->server, ptr, l);
		ptr += l + 1;
	}

	/* TODO: why did I do this - how can the map name "and" base dn be missing? */
	if (!ptr)
		goto done;

	/*
	 * For nss support we can have a map name with no
	 * type or dn info. If present a base dn must have
	 * at least an "=" and a "," to be at all functional.
	 * If a dn is given it must be fully specified or
	 * the later LDAP calls will fail.
	 */
	l = strlen(ptr);
	if ((name = strchr(ptr, '='))) {
		char *base;

		/*
		 * An '=' with no ',' means a mapname has been given so just
		 * grab it alone to keep it independent of schema otherwize
		 * we expect a full dn.
		 */
		if (!strchr(ptr, ',')) {
			char *map = strdup(name + 1);
			if (map)
				ctxt->mapname = map;
			else {
				char *estr;
				estr = strerror_r(errno, buf, sizeof(buf));
				logerr(MODPREFIX "strdup: %s", estr);
				if (ctxt->server)
					free(ctxt->server);
				return 0;
			}
			
		} else {
			base = malloc(l + 1);
			if (!base) {
				char *estr;
				estr = strerror_r(errno, buf, sizeof(buf));
				logerr(MODPREFIX "malloc: %s", estr);
				if (ctxt->server)
					free(ctxt->server);
				return 0;
			}
			ctxt->base = base;
			memset(ctxt->base, 0, l + 1);
			memcpy(ctxt->base, ptr, l);
		}
	} else {
		char *map = malloc(l + 1);
		if (!map) {
			char *estr;
			estr = strerror_r(errno, buf, sizeof(buf));
			logerr(MODPREFIX "malloc: %s", estr);
			if (ctxt->server)
				free(ctxt->server);
			return 0;
		}
		ctxt->mapname = map;
		memset(ctxt->mapname, 0, l + 1);
		memcpy(map, ptr, l);
	}

	if (!ctxt->server && *proto) {
		if (!strncmp(proto, "ldaps", 5)) {
			info(logopt, MODPREFIX
			     "server must be given to force ldaps, connection "
			     "will use LDAP client configured protocol");
		}
	}
done:
	if (ctxt->mapname)
		debug(logopt, MODPREFIX "mapname %s", ctxt->mapname);
	else
		debug(logopt, MODPREFIX "server \"%s\", base dn \"%s\"",
			ctxt->server ? ctxt->server : "(default)",
			ctxt->base);

	return 1;
}