fill_directstruct( dstruct , nsbufs, rtid, rpid, tagbase, seq , appid)
MPP_DIRECTI_PTR dstruct;
int nsbufs;
int rtid;
int rpid;
int tagbase;
int seq;
int appid;
{
	/* Initialize the variables. Set unknown values to -1 */

	dstruct -> rtid = rtid;
	dstruct -> rpid = rpid;
	dstruct -> tagbase = tagbase;
	dstruct -> nbufs = nsbufs;
	dstruct -> sseq = seq;
	dstruct -> appid = appid;

	return PvmOk;
}
