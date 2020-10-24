void
free_config (struct config * conf)
{
	if (!conf)
		return;

	if (conf->dev)
		FREE(conf->dev);

	if (conf->multipath_dir)
		FREE(conf->multipath_dir);

	if (conf->selector)
		FREE(conf->selector);

	if (conf->uid_attribute)
		FREE(conf->uid_attribute);

	if (conf->getuid)
		FREE(conf->getuid);

	if (conf->features)
		FREE(conf->features);

	if (conf->hwhandler)
		FREE(conf->hwhandler);

	if (conf->bindings_file)
		FREE(conf->bindings_file);

	if (conf->wwids_file)
		FREE(conf->wwids_file);
	if (conf->prio_name)
		FREE(conf->prio_name);

	if (conf->alias_prefix)
		FREE(conf->alias_prefix);

	if (conf->prio_args)
		FREE(conf->prio_args);

	if (conf->checker_name)
		FREE(conf->checker_name);
	if (conf->reservation_key)
		FREE(conf->reservation_key);

	free_blacklist(conf->blist_devnode);
	free_blacklist(conf->blist_wwid);
	free_blacklist(conf->blist_property);
	free_blacklist_device(conf->blist_device);

	free_blacklist(conf->elist_devnode);
	free_blacklist(conf->elist_wwid);
	free_blacklist(conf->elist_property);
	free_blacklist_device(conf->elist_device);

	free_mptable(conf->mptable);
	free_hwtable(conf->hwtable);
	free_keywords(conf->keywords);
	FREE(conf);
}
