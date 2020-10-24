free_hwe (struct hwentry * hwe)
{
	if (!hwe)
		return;

	if (hwe->vendor)
		FREE(hwe->vendor);

	if (hwe->product)
		FREE(hwe->product);

	if (hwe->revision)
		FREE(hwe->revision);

	if (hwe->getuid)
		FREE(hwe->getuid);

	if (hwe->uid_attribute)
		FREE(hwe->uid_attribute);

	if (hwe->features)
		FREE(hwe->features);

	if (hwe->hwhandler)
		FREE(hwe->hwhandler);

	if (hwe->selector)
		FREE(hwe->selector);

	if (hwe->checker_name)
		FREE(hwe->checker_name);

	if (hwe->prio_name)
		FREE(hwe->prio_name);

	if (hwe->prio_args)
		FREE(hwe->prio_args);

	if (hwe->alias_prefix)
		FREE(hwe->alias_prefix);

	if (hwe->bl_product)
		FREE(hwe->bl_product);

	FREE(hwe);
}
