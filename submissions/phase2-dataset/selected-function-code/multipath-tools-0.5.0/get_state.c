int
get_state (struct path * pp, int daemon)
{
	struct checker * c = &pp->checker;
	int state;

	condlog(3, "%s: get_state", pp->dev);

	if (!checker_selected(c)) {
		if (daemon) {
			if (pathinfo(pp, conf->hwtable, DI_SYSFS) != 0) {
				condlog(3, "%s: couldn't get sysfs pathinfo",
					pp->dev);
				return PATH_UNCHECKED;
			}
		}
		select_checker(pp);
		if (!checker_selected(c)) {
			condlog(3, "%s: No checker selected", pp->dev);
			return PATH_UNCHECKED;
		}
		checker_set_fd(c, pp->fd);
		if (checker_init(c, pp->mpp?&pp->mpp->mpcontext:NULL)) {
			memset(c, 0x0, sizeof(struct checker));
			condlog(3, "%s: checker init failed", pp->dev);
			return PATH_UNCHECKED;
		}
	}
	checker_clear_message(c);
	if (daemon)
		checker_set_async(c);
	if (!conf->checker_timeout &&
	    sysfs_get_timeout(pp, &(c->timeout)) <= 0)
		c->timeout = DEF_TIMEOUT;
	state = checker_check(c);
	condlog(3, "%s: state = %s", pp->dev, checker_state_name(state));
	if (state != PATH_UP && state != PATH_GHOST &&
	    strlen(checker_message(c)))
		condlog(3, "%s: checker msg is \"%s\"",
			pp->dev, checker_message(c));
	return state;
}
