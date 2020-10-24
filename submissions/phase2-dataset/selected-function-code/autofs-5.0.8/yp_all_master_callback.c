int yp_all_master_callback(int status, char *ypkey, int ypkeylen,
		    char *val, int vallen, char *ypcb_data)
{
	struct callback_master_data *cbdata =
			(struct callback_master_data *) ypcb_data;
	unsigned int timeout = cbdata->timeout;
	unsigned int logging = cbdata->logging;
	unsigned int logopt = cbdata->logopt;
	time_t age = cbdata->age;
	char *buffer;
	unsigned int len;

	if (status != YP_TRUE)
		return status;

	/* Ignore zero length and single non-printable char keys */
	if (ypkeylen == 0 || (ypkeylen == 1 && !isprint(*ypkey))) {
		warn(logopt, MODPREFIX
		     "ignoring invalid map entry, zero length or "
		     "single character non-printable key");
		return 0;
	}

	/*
	 * Ignore keys beginning with '+' as plus map
	 * inclusion is only valid in file maps.
	 */
	if (*ypkey == '+')
		return 0;

	*(ypkey + ypkeylen) = '\0';
	*(val + vallen) = '\0';

	len = ypkeylen + 1 + vallen + 2;

	buffer = alloca(len);
	if (!buffer) {
		error(logopt, MODPREFIX "could not malloc parse buffer");
		return 0;
	}
	memset(buffer, 0, len);

	strcat(buffer, ypkey);
	strcat(buffer, " ");
	strcat(buffer, val);

	master_parse_entry(buffer, timeout, logging, age);

	return 0;
}