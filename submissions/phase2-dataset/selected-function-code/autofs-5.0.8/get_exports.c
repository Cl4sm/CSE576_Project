static char *get_exports(struct autofs_point *ap, const char *host)
{
	char buf[MAX_ERR_BUF];
	char *mapent;
	exports exp;

	debug(ap->logopt, MODPREFIX "fetchng export list for %s", host);

	exp = rpc_get_exports(host, 10, 0, RPC_CLOSE_NOLINGER);

	mapent = NULL;
	while (exp) {
		if (mapent) {
			int len = strlen(mapent) + 1;

			len += strlen(host) + 2*(strlen(exp->ex_dir) + 2) + 3;
			mapent = realloc(mapent, len);
			if (!mapent) {
				char *estr;
				estr = strerror_r(errno, buf, MAX_ERR_BUF);
				error(ap->logopt, MODPREFIX "malloc: %s", estr);
				rpc_exports_free(exp);
				return NULL;
			}
			strcat(mapent, " \"");
			strcat(mapent, exp->ex_dir);
			strcat(mapent, "\"");
		} else {
			int len = 2*(strlen(exp->ex_dir) + 2) + strlen(host) + 3;

			mapent = malloc(len);
			if (!mapent) {
				char *estr;
				estr = strerror_r(errno, buf, MAX_ERR_BUF);
				error(ap->logopt, MODPREFIX "malloc: %s", estr);
				rpc_exports_free(exp);
				return NULL;
			}
			strcpy(mapent, "\"");
			strcat(mapent, exp->ex_dir);
			strcat(mapent, "\"");
		}
		strcat(mapent, " \"");
		strcat(mapent, host);
		strcat(mapent, ":");
		strcat(mapent, exp->ex_dir);
		strcat(mapent, "\"");

		exp = exp->ex_next;
	}
	rpc_exports_free(exp);

	if (!mapent)
		error(ap->logopt, MODPREFIX "exports lookup failed for %s", host);

	return mapent;
}