select_checker(struct path *pp)
{
	struct checker * c = &pp->checker;

	if (pp->hwe && pp->hwe->checker_name) {
		checker_get(c, pp->hwe->checker_name);
		condlog(3, "%s: path checker = %s (controller setting)",
			pp->dev, checker_name(c));
		goto out;
	}
	if (conf->checker_name) {
		checker_get(c, conf->checker_name);
		condlog(3, "%s: path checker = %s (config file default)",
			pp->dev, checker_name(c));
		goto out;
	}
	checker_get(c, DEFAULT_CHECKER);
	condlog(3, "%s: path checker = %s (internal default)",
		pp->dev, checker_name(c));
out:
	if (conf->checker_timeout) {
		c->timeout = conf->checker_timeout;
		condlog(3, "%s: checker timeout = %u s (config file default)",
				pp->dev, c->timeout);
	}
	else if (sysfs_get_timeout(pp, &c->timeout) > 0)
		condlog(3, "%s: checker timeout = %u ms (sysfs setting)",
				pp->dev, c->timeout);
	else {
		c->timeout = DEF_TIMEOUT;
		condlog(3, "%s: checker timeout = %u ms (internal default)",
				pp->dev, c->timeout);
	}
	return 0;
}
