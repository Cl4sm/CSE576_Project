int
select_minio_rq (struct multipath * mp)
{
	if (mp->mpe && mp->mpe->minio_rq) {
		mp->minio = mp->mpe->minio_rq;
		condlog(3, "%s: minio = %i rq (LUN setting)",
			mp->alias, mp->minio);
		return 0;
	}
	if (mp->hwe && mp->hwe->minio_rq) {
		mp->minio = mp->hwe->minio_rq;
		condlog(3, "%s: minio = %i rq (controller setting)",
			mp->alias, mp->minio);
		return 0;
	}
	if (conf->minio) {
		mp->minio = conf->minio_rq;
		condlog(3, "%s: minio = %i rq (config file default)",
			mp->alias, mp->minio);
		return 0;
	}
	mp->minio = DEFAULT_MINIO_RQ;
	condlog(3, "%s: minio = %i rq (internal default)",
		mp->alias, mp->minio);
	return 0;
}
