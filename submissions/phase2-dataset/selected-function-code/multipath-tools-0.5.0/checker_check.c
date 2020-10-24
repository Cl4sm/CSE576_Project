int checker_check (struct checker * c)
{
	int r;

	if (!c)
		return PATH_WILD;

	c->message[0] = '\0';
	if (c->disable) {
		MSG(c, "checker disabled");
		return PATH_UNCHECKED;
	}
	if (c->fd <= 0) {
		MSG(c, "no usable fd");
		return PATH_WILD;
	}
	r = c->check(c);

	return r;
}
