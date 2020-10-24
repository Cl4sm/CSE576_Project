static void dump_buf(struct openconnect_info *vpninfo, char prefix, char *buf)
{
	while (*buf) {
		char *eol = buf;
		char eol_char = 0;

		while (*eol) {
			if (*eol == '\r' || *eol == '\n') {
				eol_char = *eol;
				*eol = 0;
				break;
			}
			eol++;
		}

		vpn_progress(vpninfo, PRG_DEBUG, "%c %s\n", prefix, buf);
		if (!eol_char)
			break;

		*eol = eol_char;
		buf = eol + 1;
		if (eol_char == '\r' && *buf == '\n')
			buf++;
	}
}