select_selector (struct multipath * mp)
{
	if (mp->mpe && mp->mpe->selector) {
		mp->selector = mp->mpe->selector;
		condlog(3, "%s: selector = %s (LUN setting)",
			mp->alias, mp->selector);
		return 0;
	}
	if (mp->hwe && mp->hwe->selector) {
		mp->selector = mp->hwe->selector;
		condlog(3, "%s: selector = %s (controller setting)",
			mp->alias, mp->selector);
		return 0;
	}
	if (conf->selector) {
		mp->selector = conf->selector;
		condlog(3, "%s: selector = %s (config file default)",
			mp->alias, mp->selector);
		return 0;
	}
	mp->selector = set_default(DEFAULT_SELECTOR);
	condlog(3, "%s: selector = %s (internal default)",
		mp->alias, mp->selector);
	return 0;
}
