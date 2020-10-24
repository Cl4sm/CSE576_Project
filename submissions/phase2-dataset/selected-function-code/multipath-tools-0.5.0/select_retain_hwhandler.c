extern int
select_retain_hwhandler (struct multipath * mp)
{
	unsigned int minv_dm_retain[3] = {1, 5, 0};

	if (!VERSION_GE(conf->version, minv_dm_retain)) {
		mp->retain_hwhandler = RETAIN_HWHANDLER_OFF;
		condlog(3, "%s: retain_attached_hw_handler disabled (requires kernel version >= 1.5.0)", mp->alias);
		return 0;
	}

	if (mp->hwe && mp->hwe->retain_hwhandler) {
		mp->retain_hwhandler = mp->hwe->retain_hwhandler;
		condlog(3, "%s: retain_attached_hw_handler = %d (controller default)", mp->alias, mp->retain_hwhandler);
		return 0;
	}
	if (conf->retain_hwhandler) {
		mp->retain_hwhandler = conf->retain_hwhandler;
		condlog(3, "%s: retain_attached_hw_handler = %d (config file default)", mp->alias, mp->retain_hwhandler);
		return 0;
	}
	mp->retain_hwhandler = 0;
	condlog(3, "%s: retain_attached_hw_handler = %d (compiled in default)", mp->alias, mp->retain_hwhandler);
	return 0;
}
