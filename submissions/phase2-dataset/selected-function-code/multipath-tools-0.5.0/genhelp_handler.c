static char *
genhelp_handler (void)
{
	int i, j;
	unsigned long fp;
	struct handler * h;
	struct key * kw;
	char * reply;
	char * p;

	reply = MALLOC(INITIAL_REPLY_LEN);

	if (!reply)
		return NULL;

	p = reply;
	p += sprintf(p, VERSION_STRING);
	p += sprintf(p, "CLI commands reference:\n");

	vector_foreach_slot (handlers, h, i) {
		fp = h->fingerprint;
		vector_foreach_slot (keys, kw, j) {
			if ((kw->code & fp)) {
				fp -= kw->code;
				p += sprintf(p, " %s", kw->str);
				p += genhelp_sprint_aliases(p, keys, kw);

				if (kw->has_param)
					p += sprintf(p, " $%s", kw->str);
			}
		}
		p += sprintf(p, "\n");
	}

	return reply;
}
