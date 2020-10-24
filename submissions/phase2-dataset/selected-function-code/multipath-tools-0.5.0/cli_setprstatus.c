cli_setprstatus(void * v, char ** reply, int * len, void * data)
{
	struct multipath * mpp;
	struct vectors * vecs = (struct vectors *)data;
	char * param = get_keyparam(v, MAP);

	param = convert_dev(param, 0);
	get_path_layout(vecs->pathvec, 0);
	mpp = find_mp_by_str(vecs->mpvec, param);

	if (!mpp)
		return 1;

	if (!mpp->prflag) {
		mpp->prflag = 1;
		condlog(2, "%s: prflag set", param);
	}


	return 0;
}
