extern int
select_hwhandler (struct multipath * mp)
{
	if (mp->hwe && mp->hwe->hwhandler) {
		mp->hwhandler = mp->hwe->hwhandler;
		condlog(3, "%s: hwhandler = %s (controller setting)",
			mp->alias, mp->hwhandler);
		return 0;
	}
	if (conf->hwhandler) {
		mp->hwhandler = conf->hwhandler;
		condlog(3, "%s: hwhandler = %s (config file default)",
			mp->alias, mp->hwhandler);
		return 0;
	}
	mp->hwhandler = set_default(DEFAULT_HWHANDLER);
	condlog(3, "%s: hwhandler = %s (internal default)",
		mp->alias, mp->hwhandler);
	return 0;
}
