adopt_paths (vector pathvec, struct multipath * mpp, int get_info)
{
	int i;
	struct path * pp;

	if (!mpp)
		return 0;

	if (update_mpp_paths(mpp, pathvec))
		return 1;

	vector_foreach_slot (pathvec, pp, i) {
		if (!strncmp(mpp->wwid, pp->wwid, WWID_SIZE)) {
			condlog(3, "%s: ownership set to %s",
				pp->dev, mpp->alias);
			pp->mpp = mpp;

			if (!mpp->paths && !(mpp->paths = vector_alloc()))
				return 1;

			if (!find_path_by_dev(mpp->paths, pp->dev) &&
			    store_path(mpp->paths, pp))
					return 1;
			if (get_info && pathinfo(pp, conf->hwtable,
						 DI_PRIO | DI_CHECKER))
				return 1;
		}
	}
	return 0;
}
