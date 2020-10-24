int
cli_getprstatus (void * v, char ** reply, int * len, void * data)
{
	struct multipath * mpp;
	struct vectors * vecs = (struct vectors *)data;
	char * param = get_keyparam(v, MAP);

	param = convert_dev(param, 0);
	get_path_layout(vecs->pathvec, 0);
	mpp = find_mp_by_str(vecs->mpvec, param);

	if (!mpp)
		return 1;

	condlog(3, "%s: prflag = %u", param, (unsigned int)mpp->prflag);

	*reply =(char *)malloc(2);
	*len = 2;
	memset(*reply,0,2);


	sprintf(*reply,"%d",mpp->prflag);
	(*reply)[1]='\0';


	condlog(3, "%s: reply = %s", param, *reply);

	return 0;
}
