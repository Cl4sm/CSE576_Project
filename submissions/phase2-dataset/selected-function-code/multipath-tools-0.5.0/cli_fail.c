int
cli_fail(void * v, char ** reply, int * len, void * data)
{
	struct vectors * vecs = (struct vectors *)data;
	char * param = get_keyparam(v, PATH);
	struct path * pp;
	int r;

	param = convert_dev(param, 1);
	pp = find_path_by_dev(vecs->pathvec, param);

	if (!pp)
		 pp = find_path_by_devt(vecs->pathvec, param);

	if (!pp || !pp->mpp || !pp->mpp->alias)
		return 1;

	condlog(2, "%s: fail path %s (operator)",
		pp->mpp->alias, pp->dev_t);

	r = dm_fail_path(pp->mpp->alias, pp->dev_t);
	/*
	 * Suspend path checking to avoid auto-reinstating the path
	 */
	if (!r)
		checker_disable(&pp->checker);
	return r;
}
