mpp_free(tp)
	struct task *tp;
{
	struct nodeset *sp;
	int i;
	int node;
	int ptype;
	int tid = tp->t_tid;

	if (!TIDISNODE(tid))
		return;

	node = tid & tidnmask;
	ptype = TIDTOTYPE(tid);
	tp->t_out = -1;

	for (sp = busynodes->n_link; sp != busynodes; sp = sp->n_link) 
	{
		if ((sp->n_ptype & ptypemask) == ptype && node >= sp->n_first
				&& node - sp->n_first < sp->n_size) 
		{

			if (pvmdebmask & PDMNODE) {
				sprintf(etext, "mpp_free() t%x type=%ld alive=%d\n",
					tid, sp->n_ptype, sp->n_alive);
				pvmlogerror(etext);
			}
			if (--sp->n_alive == 0) 
			{
				LISTDELETE(sp, n_link, n_rlink);
				PVM_FREE(sp);

#if defined(IMA_PGONPUMA)
				if (pvmdebmask & PDMNODE) {
					sprintf(etext, "mpp_free() freeing %d recv bufs \n", NRBUFS);
					pvmlogerror(etext);
				}
				for (i=0; i < NRBUFS; i++)
				{
					pk_free(mprecvbufs->rpkt);
					mprecvbufs++;
				}

				mprecvbufs = (MSG_INFO_PTR) NULL;

				if (pvmdebmask & PDMNODE) {
					sprintf(etext, "mpp_free() mprecvbufs is %x \n", mprecvbufs);
					pvmlogerror(etext);
				}

				for (i=0; i < partsize; i++)
				{
					if (mpdirect->ordering)
						PVM_FREE(mpdirect->ordering);
					PVM_FREE(mpdirect);
					mpdirect ++;
				}

				mpdirect = (MPP_DIRECTI_PTR) NULL;

				partsize = -1;

				if (!killing_appid)
					kill_app(appid);

				disc_app(appid);

				appid = -1;

				killing_appid = 0;
#endif

			}
			return;
		}
	}
	sprintf(etext, "mpp_free() t%x not active\n", tid);
	pvmlogerror(etext);
	return;
}
