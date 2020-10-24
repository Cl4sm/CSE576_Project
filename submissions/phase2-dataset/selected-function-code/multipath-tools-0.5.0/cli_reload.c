cli_reload(void *v, char **reply, int *len, void *data)
{
	struct vectors * vecs = (struct vectors *)data;
	char * mapname = get_keyparam(v, MAP);
	struct multipath *mpp;
	int minor;

	mapname = convert_dev(mapname, 0);
	condlog(2, "%s: reload map (operator)", mapname);
	if (sscanf(mapname, "dm-%d", &minor) == 1)
		mpp = find_mp_by_minor(vecs->mpvec, minor);
	else
		mpp = find_mp_by_alias(vecs->mpvec, mapname);

	if (!mpp) {
		condlog(0, "%s: invalid map name. cannot reload", mapname);
		return 1;
	}

	return reload_map(vecs, mpp, 0);
}
