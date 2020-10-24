extern int
select_no_path_retry(struct multipath *mp)
{
	if (mp->flush_on_last_del == FLUSH_IN_PROGRESS) {
		condlog(0, "flush_on_last_del in progress");
		mp->no_path_retry = NO_PATH_RETRY_FAIL;
		return 0;
	}
	if (mp->mpe && mp->mpe->no_path_retry != NO_PATH_RETRY_UNDEF) {
		mp->no_path_retry = mp->mpe->no_path_retry;
		condlog(3, "%s: no_path_retry = %i (multipath setting)",
			mp->alias, mp->no_path_retry);
		return 0;
	}
	if (mp->hwe && mp->hwe->no_path_retry != NO_PATH_RETRY_UNDEF) {
		mp->no_path_retry = mp->hwe->no_path_retry;
		condlog(3, "%s: no_path_retry = %i (controller setting)",
			mp->alias, mp->no_path_retry);
		return 0;
	}
	if (conf->no_path_retry != NO_PATH_RETRY_UNDEF) {
		mp->no_path_retry = conf->no_path_retry;
		condlog(3, "%s: no_path_retry = %i (config file default)",
			mp->alias, mp->no_path_retry);
		return 0;
	}
	if (mp->no_path_retry != NO_PATH_RETRY_UNDEF)
		condlog(3, "%s: no_path_retry = %i (inherited setting)",
			mp->alias, mp->no_path_retry);
	else
		condlog(3, "%s: no_path_retry = %i (internal default)",
			mp->alias, mp->no_path_retry);
	return 0;
}
