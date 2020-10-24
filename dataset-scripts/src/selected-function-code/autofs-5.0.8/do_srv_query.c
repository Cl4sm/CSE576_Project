static int do_srv_query(unsigned int logopt, char *name, u_char **packet)
{
	int len = PACKETSZ;
	unsigned int last_len = len;
	char ebuf[MAX_ERR_BUF];
	u_char *buf;

	while (1) {
		buf = malloc(last_len);
		if (!buf) {
			char *estr = strerror_r(errno, ebuf, MAX_ERR_BUF);
			error(logopt, "malloc: %s", estr);
			return -1;
		}

		len = res_query(name, C_IN, T_SRV, buf, last_len);
		if (len < 0) {
			char *estr = strerror_r(errno, ebuf, MAX_ERR_BUF);
			error(logopt, "Failed to resolve %s (%s)", name, estr);
			free(buf);
			return -1;
		}

		if (len == last_len) {
			/* These shouldn't too large, bump by PACKETSZ only */
			last_len += PACKETSZ;
			free(buf);
			continue;
		}

		break;
	}

	*packet = buf;

	return len;
}