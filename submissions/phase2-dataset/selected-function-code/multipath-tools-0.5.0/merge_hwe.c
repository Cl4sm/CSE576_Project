static int
merge_hwe (struct hwentry * dst, struct hwentry * src)
{
	merge_str(vendor);
	merge_str(product);
	merge_str(revision);
	merge_str(getuid);
	merge_str(uid_attribute);
	merge_str(features);
	merge_str(hwhandler);
	merge_str(selector);
	merge_str(checker_name);
	merge_str(prio_name);
	merge_str(prio_args);
	merge_str(alias_prefix);
	merge_str(bl_product);
	merge_num(pgpolicy);
	merge_num(pgfailback);
	merge_num(rr_weight);
	merge_num(no_path_retry);
	merge_num(minio);
	merge_num(minio_rq);
	merge_num(flush_on_last_del);
	merge_num(fast_io_fail);
	merge_num(dev_loss);
	merge_num(user_friendly_names);
	merge_num(retain_hwhandler);
	merge_num(detect_prio);

	/*
	 * Make sure features is consistent with
	 * no_path_retry
	 */
	if (dst->no_path_retry == NO_PATH_RETRY_FAIL)
		remove_feature(&dst->features, "queue_if_no_path");
	else if (dst->no_path_retry != NO_PATH_RETRY_UNDEF)
		add_feature(&dst->features, "queue_if_no_path");

	return 0;
}
