int
mpp_load( wxp )
struct waitc_spawn *wxp;
{
int flags;              /* exec options */
char *name;             /* executable */
char **argv;            /* arg list (argv[-1] must be there) */
int count;				/* how many */
int *tids;				/* array to store new tids */
int ptid;				/* parent task ID */
int nenv;				/* length of environment */
char **envp;			/* environment strings */
int outtid;				/* output tid */
int outcod;				/* output code */
int trctid;				/* tid for trace messages */
int trccod;				/* code to use on trace messages */

    char **ep, **eplist;
	static char *nullep[] = { "", 0 };
    char path[MAXPATHLEN];
	char **cenv;

	int i, cc;
	int err = 0;
	static int first = 1;
	int j;
	int cnode;
	int cpid;
	int pvminfo[SIZEHINFO];	/* proto, myset, parent tid, frag size, NDF */
	int ptypepart;

	int *pids = 0;			/* array of OSF/1 process IDs */
	int *nodes = 0;
	int nnodes;			/* number of nodes to needed */

	msgmid_t mid;

	info_t minfo[MPPINFOSIZE];

	struct nodeset *sp;

    struct stat sb;

	struct task *tp;

	MPP_DIRECTI_PTR tdirect;

	MSGFUNC_PTR mfunc;

	/* -- initialize some variables from the struct waitc_spawn  struct -- */

	name = wxp->w_argv[0];
	argv = wxp->w_argv;
	count = wxp->w_veclen;
	tids = wxp->w_vec;
	ptid = wxp->w_ptid;
	nenv = wxp->w_nenv;
	envp = wxp->w_env; 
	

	mfunc = pvm_hostmsgfunc();

    eplist = CINDEX(name, '/') ? nullep : epaths;

	/* -- look for the executable name in the executable path (ep) -- */
    for (ep = eplist; *ep; ep++) 
	{
		strcpy(path, *ep);	 /* search for file */

		if (path[0])
			(void)strcat(path, "/");

		strncat(path, name, sizeof(path) - strlen(path) - 1);

        if (stat(path, &sb) == -1
                || ((sb.st_mode & S_IFMT) != S_IFREG)
                || !(sb.st_mode & S_IEXEC)) 
		{
            if (pvmdebmask & PDMTASK) 
			{
                sprintf(etext, "mpp_load() stat failed <%s>\n", path);
                pvmlogerror(etext);
            }
			continue;	/* try next path element */
        }

		/* Here we have found an executable -- try to start it */

		if ( !(pids = TALLOC(count, int, "pids")) 
				||  !(nodes = TALLOC(count, int, "nodes"))) 
		{
			err = PvmNoMem;
			goto done;
		}

		if (!(sp = mpp_new(count, ptid)))  
		{
			err = PvmOutOfRes;
			goto done;
		}

		for (j = 0; j < count; j++)
			nodes[j] = sp->n_first + j;

		/* copy the pvmd's environment, augment with what is passed to us */
		cenv = pvmcopyenv(environ);
		while (nenv > 0)
			pvmenvinsert(&cenv, envp[--nenv]);


		/* Do the load, return how many were actually started */

		err = nx_loadve((long *) nodes, (long) count, (long) sp->n_ptype, (long *)pids, path, argv, cenv);

		pvmfreeenv(cenv); /* free the copied environment */

		if (err < count)  /* loadve yacked ?? */
		{
			sprintf(etext, "mpp_load() loaded only %d <%s>\n", err, path);
			pvmlogerror(etext);
			err = PvmDSysErr;
			goto done;
		}

		if (first)		/* first time through, do some initialization */ 
		{
			if ( _setptype(PVMDPTYPE) < 0)
        		pvmlogperror("mpp_load() setptype to PVMDPTYPE");
			first = 0;

			/* Set up the prealloced buffers      */
			mpdirect = new_vdirectstruct( partsize, NRBUFS, NSBUFS );	

			mprecvbufs = init_recv_list(NRBUFS, PMTDBASE, 
					MAXFRAGSIZE, 0, MPPANY, mfunc);

		}


		/* configure the tasks
		/* alright, send some important information to the tasks that we
			just started */

		ptypepart = ((sp->n_ptype & ptypemask) << (ffs(tidtmask) - 1)) 
					| TIDONNODE;
		pvminfo[0] = TDPROTOCOL;
		pvminfo[1] = myhostpart + ptypepart;
		pvminfo[2] = ptid;
		pvminfo[3] = MAXFRAGSIZE;
		pvminfo[4] = myndf;
		sprintf(etext, "partsize is %d\n",partsize); 
		pvmlogerror(etext);
		pvminfo[5] = partsize;
		pvminfo[6] = wxp->w_outtid;
		pvminfo[7] = wxp->w_outtag;
		pvminfo[8] = wxp->w_outctx;
		pvminfo[9] = wxp->w_trctid;
		pvminfo[10] = wxp->w_trctag;
		pvminfo[11] = wxp->w_trcctx;

		if (pvmdebmask & PDMTASK) 
		{
			sprintf(etext, "mpp_load() %d type=%ld ptid=%x pid%ld... t%x...\n",
					count, sp->n_ptype, ptid, pids[0], 
					myhostpart + ptypepart + sp->n_first);

			pvmlogerror(etext);
		}

		pvmlogerror("Starting configuration Message send Loop\n");
		for (i = 0; i < count; i ++ )
		{
			cnode =  nodes[i];
			if ( (*mfunc->imsgsend)(appid, PMTCONF,  (char *) pvminfo, 
					sizeof(pvminfo), cnode, PVMDPTYPE, &mid) < 0) 
			{
				pvmlogperror("mpp_load() configuration message");
				err = PvmDSysErr;
				goto done;
			}
			while (! ((*mfunc->msgdone)(appid, &mid, minfo)));
		}
		pvmlogerror("Finished Message send Loop\n");


		/* create new task structs */

		for (j = 0; j < count; j++) 
		{
			if (pids[j] > 0) 
			{
				if ((tp = task_new(myhostpart + ptypepart
						+ sp->n_first + j)) == NULL) {
					err = PvmOutOfRes;
					goto done;
				}
				task_setpid(tp, pids[j]);
				tp->t_a_out = STRALLOC(name);
				tp->t_ptid = ptid;
				tp->t_flag |= TF_CONN;		/* no need for the authorization */
				tp->t_out = taskstdout;
				tp->t_outtid = wxp->w_outtid;
				pvmlogprintf("setting output context of %x to %x\n",
								tp->t_tid, tp->t_outtid);
				tp->t_outctx = wxp->w_outctx;
				tp->t_outtag = wxp->w_outtag;
				tp->t_trctid = wxp->w_trctid;
				tp->t_trcctx = wxp->w_trcctx;
				tp->t_trctag = wxp->w_trctag;
				tids[j] = tp->t_tid;

			/* initialize the sequence numbers for this task */
				fill_directstruct (mpdirect + nodes[j], NSBUFS, tp->t_tid, 
						pids[j], PMTDBASE, 0, appid);   
				init_chunkostruct( (mpdirect + nodes[j])->ordering, NRBUFS);

			} else
				tids[j] = PvmDSysErr;
		}

		if (pids)
			PVM_FREE(pids);

		if (nodes)
			PVM_FREE(nodes);

		return 0;
	}


	/* here, only if there is an error in the way things were spawned */
	if (pvmdebmask & PDMTASK) 
	{
		sprintf(etext, "mpp_load() didn't find <%s>\n", name);
		pvmlogerror(etext);
	}

	err = PvmNoFile;

done:
	for (j = 0; j < count; j++)
		tids[j] = err;

	if (pids)
		PVM_FREE(pids);

	return err;
}
