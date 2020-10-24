struct nodeset *
mpp_new(count, ptid)
	int count;		/* number of nodes requested */
	int ptid;		/* parent's tid */
{

	static int first = 1;
	int i;
	int last = -1;
	int pfd[2];		/* pipe to read stdout */

	long ptype = busynodes->n_ptype;

	struct nodeset *sp, *newp;

	ptype = 0;

#if defined(IMA_PGONPUMA)
	/* XXX Only run a single PUMA partition for now */
	if (appid != -1)
	{
		pvmlogerror("mpp_new() only one partition allowed, sorry\n");
		return (struct nodeset *) 0;
	}
	partsize = count;
#endif
	if (first)	/* take care of task input/output */ 
	{
		if (pipe(pfd) != -1)  	/* pfd[0] is a read, pfd[1] is a write */
		{
			dup2(pfd[1], 1);	/* send standard output write end of pipe */
			dup2(1, 2);			/* and stderr to write end of pipe */
			taskstdout = pfd[0];
			wrk_fds_add(taskstdout, 1);
		} 
		else
		{
			pvmlogperror("mpp_new() pipe");
		}

		first = 0;
	}

	if (!(newp = TALLOC(1, struct nodeset, "nsets"))) 
	{
		pvmlogerror("nodes_new() can't get memory\n");
		pvmbailout(0);
	}
	BZERO((char*)newp, sizeof(struct nodeset));
	
	newp->n_size = count;

	/* this loop is bypassed on the first mpp_new */
	for (sp = busynodes->n_link; sp != busynodes; sp = sp->n_link) 
	{
		if (sp->n_ptype == ptype) 
		{
			if (sp->n_first - last > count)
				goto done;
			else 
			{
				last = sp->n_first + sp->n_size - 1;
				if (sp->n_link == busynodes && partsize - last > count)
					goto done;
			}
		} 
		else if (sp->n_ptype > ptype) 
		{
			if (partsize - last > count)
				goto done;
			else 
			{
				ptype = sp->n_ptype;	/* go to the next layer */
				last = -1;
			}
		}
	} /* end the for loop */

	if (partsize - last <= count) 
	{
		pvmlogerror("mpp_new() not enough nodes in partition\n");
		PVM_FREE(newp);
		return (struct nodeset *) 0;
	}

done:
	if (pvmdebmask & PDMNODE) {
		sprintf(etext, "mpp_new() %d nodes %d ... ptype=%d ptid=%x\n",
			count, last+1, ptype, ptid);
		pvmlogerror(etext);
	}
	newp->n_first = last + 1;
	newp->n_ptype = ptype;
	newp->n_alive = count;
	newp->n_ptid = ptid;

	LISTPUTAFTER(sp, newp, n_link, n_rlink);

	return newp;
}
