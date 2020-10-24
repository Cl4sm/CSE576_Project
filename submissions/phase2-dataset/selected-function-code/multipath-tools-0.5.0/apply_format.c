apply_format (char * string, char * cmd, struct path * pp)
{
	char * pos;
	char * dst;
	char * p;
	char * q;
	int len;
	int myfree;

	if (!string)
		return 1;

	if (!cmd)
		return 1;

	dst = cmd;
	p = dst;
	pos = strchr(string, '%');
	myfree = CALLOUT_MAX_SIZE;

	if (!pos) {
		strcpy(dst, string);
		return 0;
	}

	len = (int) (pos - string) + 1;
	myfree -= len;

	if (myfree < 2)
		return 1;

	snprintf(p, len, "%s", string);
	p += len - 1;
	pos++;

	switch (*pos) {
	case 'n':
		len = strlen(pp->dev) + 1;
		myfree -= len;

		if (myfree < 2)
			return 1;

		snprintf(p, len, "%s", pp->dev);
		for (q = p; q < p + len; q++) {
			if (q && *q == '!')
				*q = '/';
		}
		p += len - 1;
		break;
	case 'd':
		len = strlen(pp->dev_t) + 1;
		myfree -= len;

		if (myfree < 2)
			return 1;

		snprintf(p, len, "%s", pp->dev_t);
		p += len - 1;
		break;
	default:
		break;
	}
	pos++;

	if (!*pos)
		return 0;

	len = strlen(pos) + 1;
	myfree -= len;

	if (myfree < 2)
		return 1;

	snprintf(p, len, "%s", pos);
	condlog(3, "reformated callout = %s", dst);
	return 0;
}
