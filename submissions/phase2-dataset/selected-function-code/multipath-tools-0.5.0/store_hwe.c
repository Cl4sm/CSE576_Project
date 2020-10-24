int
store_hwe (vector hwtable, struct hwentry * dhwe)
{
	struct hwentry * hwe;

	if (find_hwe_strmatch(hwtable, dhwe))
		return 0;

	if (!(hwe = alloc_hwe()))
		return 1;

	if (!dhwe->vendor || !(hwe->vendor = set_param_str(dhwe->vendor)))
		goto out;

	if (!dhwe->product || !(hwe->product = set_param_str(dhwe->product)))
		goto out;

	if (dhwe->revision && !(hwe->revision = set_param_str(dhwe->revision)))
		goto out;

	if (dhwe->uid_attribute && !(hwe->uid_attribute = set_param_str(dhwe->uid_attribute)))
		goto out;

	if (dhwe->getuid && !(hwe->getuid = set_param_str(dhwe->getuid)))
		goto out;

	if (dhwe->features && !(hwe->features = set_param_str(dhwe->features)))
		goto out;

	if (dhwe->hwhandler && !(hwe->hwhandler = set_param_str(dhwe->hwhandler)))
		goto out;

	if (dhwe->selector && !(hwe->selector = set_param_str(dhwe->selector)))
		goto out;

	if (dhwe->checker_name && !(hwe->checker_name = set_param_str(dhwe->checker_name)))
		goto out;

	if (dhwe->prio_name && !(hwe->prio_name = set_param_str(dhwe->prio_name)))
		goto out;

	if (dhwe->prio_args && !(hwe->prio_args = set_param_str(dhwe->prio_args)))
		goto out;

	if (dhwe->alias_prefix && !(hwe->alias_prefix = set_param_str(dhwe->alias_prefix)))
		goto out;

	hwe->pgpolicy = dhwe->pgpolicy;
	hwe->pgfailback = dhwe->pgfailback;
	hwe->rr_weight = dhwe->rr_weight;
	hwe->no_path_retry = dhwe->no_path_retry;
	hwe->minio = dhwe->minio;
	hwe->minio_rq = dhwe->minio_rq;
	hwe->flush_on_last_del = dhwe->flush_on_last_del;
	hwe->fast_io_fail = dhwe->fast_io_fail;
	hwe->dev_loss = dhwe->dev_loss;
	hwe->user_friendly_names = dhwe->user_friendly_names;
	hwe->retain_hwhandler = dhwe->retain_hwhandler;
	hwe->detect_prio = dhwe->detect_prio;

	if (dhwe->bl_product && !(hwe->bl_product = set_param_str(dhwe->bl_product)))
		goto out;

	if (!vector_alloc_slot(hwtable))
		goto out;

	vector_set_slot(hwtable, hwe);
	return 0;
out:
	free_hwe(hwe);
	return 1;
}
