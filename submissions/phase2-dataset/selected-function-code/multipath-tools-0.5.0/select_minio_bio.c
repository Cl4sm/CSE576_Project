int
select_minio_bio (struct multipath * mp)
{
	if (mp->mpe && mp->mpe->minio) {
		mp->minio = mp->mpe->minio;
		condlog(3, "%s: minio = %i (LUN setting)",
			mp->alias, mp->minio);
		return 0;
	}
	if (mp->hwe && mp->hwe->minio) {
		mp->minio = mp->hwe->minio;
		condlog(3, "%s: minio = %i (controller setting)",
			mp->alias, mp->minio);
		return 0;
	}
	if (conf->minio) {
		mp->minio = conf->minio;
		condlog(3, "%s: minio = %i (config file default)",
			mp->alias, mp->minio);
		return 0;
	}
	mp->minio = DEFAULT_MINIO;
	condlog(3, "%s: minio = %i (internal default)",
		mp->alias, mp->minio);
	return 0;
}
