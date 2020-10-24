select_getuid (struct path * pp)
{
	if (pp->hwe && pp->hwe->uid_attribute) {
		pp->uid_attribute = pp->hwe->uid_attribute;
		condlog(3, "%s: uid_attribute = %s (controller setting)",
			pp->dev, pp->uid_attribute);
		return 0;
	}
	if (pp->hwe && pp->hwe->getuid) {
		pp->getuid = pp->hwe->getuid;
		condlog(3, "%s: getuid = %s (deprecated) (controller setting)",
			pp->dev, pp->getuid);
		return 0;
	}
	if (conf->uid_attribute) {
		pp->uid_attribute = conf->uid_attribute;
		condlog(3, "%s: uid_attribute = %s (config file default)",
			pp->dev, pp->uid_attribute);
		return 0;
	}
	if (conf->getuid) {
		pp->getuid = conf->getuid;
		condlog(3, "%s: getuid = %s (deprecated) (config file default)",
			pp->dev, pp->getuid);
		return 0;
	}
	pp->uid_attribute = STRDUP(DEFAULT_UID_ATTRIBUTE);
	condlog(3, "%s: uid_attribute = %s (internal default)",
		pp->dev, pp->uid_attribute);
	return 0;
}
