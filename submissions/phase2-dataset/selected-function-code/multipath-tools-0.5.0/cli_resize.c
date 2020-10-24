int
cli_resize(void *v, char **reply, int *len, void *data)
{
	struct vectors * vecs = (struct vectors *)data;
	char * mapname = get_keyparam(v, MAP);
	struct multipath *mpp;
	int minor;
	unsigned long long size;
	struct pathgroup *pgp;
	struct path *pp;

	mapname = convert_dev(mapname, 0);
	condlog(2, "%s: resize map (operator)", mapname);
	if (sscanf(mapname, "dm-%d", &minor) == 1)
		mpp = find_mp_by_minor(vecs->mpvec, minor);
	else
		mpp = find_mp_by_alias(vecs->mpvec, mapname);

	if (!mpp) {
		condlog(0, "%s: invalid map name. cannot resize", mapname);
		return 1;
	}

	pgp = VECTOR_SLOT(mpp->pg, 0);

	if (!pgp){
		condlog(0, "%s: couldn't get path group. cannot resize",
			mapname);
		return 1;
	}
	pp = VECTOR_SLOT(pgp->paths, 0);

	if (!pp){
		condlog(0, "%s: couldn't get path. cannot resize", mapname);
		return 1;
	}
	if (!pp->udev || sysfs_get_size(pp, &size)) {
		condlog(0, "%s: couldn't get size for sysfs. cannot resize",
			mapname);
		return 1;
	}
	if (size == mpp->size) {
		condlog(0, "%s: map is still the same size (%llu)", mapname,
			mpp->size);
		return 0;
	}
	condlog(3, "%s old size is %llu, new size is %llu", mapname, mpp->size,
		size);

	if (resize_map(mpp, size, vecs) != 0)
		return 1;

	dm_lib_release();
	if (setup_multipath(vecs, mpp) != 0)
		return 1;
	sync_map_state(mpp);

	return 0;
}
