int
ec_add(register u_int32_t o, register char *text)
{

	if (ec_last >= ec_len) {
		if (list == NULL) {
			ec_len = 512;
			list = malloc(ec_len * sizeof(*list));
		} else {
			ec_len *= 2;
			list = realloc(list, ec_len * sizeof(*list));
		}
		if (list == NULL) {
			syslog(LOG_ERR, "ec_add(): malloc: %m");
			exit(1);
		}
	}
	list[ec_last].o = o;
	list[ec_last].text = savestr(text);
	++ec_last;
	return (1);
}