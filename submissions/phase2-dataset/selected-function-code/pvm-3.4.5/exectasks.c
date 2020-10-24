exectasks(mp, rmp, schtid)
	struct pmsg *mp;		/* the request message */
	struct pmsg *rmp;		/* reply message blank */
	int schtid;				/* scheduler for new tasks */
{
	struct pmsg *mp2;			/* reply message hdl */
	int i;
	struct timeval now;
	struct waitc_spawn *wxp;	/* new task parameters */
	int munge_tenv = 0;
	char tmp[255];
	char *wd = 0;
	char *savewd = 0;

	wxp = TALLOC(1, struct waitc_spawn, "waix");
	BZERO((char*)wxp, sizeof(struct waitc_spawn));

	/* unpack message */

	if (upkuint(mp, &wxp->w_ptid)
	|| upkstralloc(mp, &wxp->w_file)
	|| upkint(mp, &wxp->w_flags)
	|| upkint(mp, &wxp->w_veclen)
	|| upkint(mp, &wxp->w_argc))
		goto bad;

	if (wxp->w_veclen < 1)
		goto bad;
	wxp->w_vec = TALLOC(wxp->w_veclen, int, "tids");

	wxp->w_argc += 2;
	wxp->w_argv = TALLOC(wxp->w_argc + 1, char*, "argv");
	BZERO((char*)wxp->w_argv, (wxp->w_argc + 1) * sizeof(char*));
	wxp->w_argv++;
	wxp->w_argv[0] = wxp->w_file;
	wxp->w_file = 0;
	wxp->w_argv[--wxp->w_argc] = 0;
	for (i = 1; i < wxp->w_argc; i++)
		if (upkstralloc(mp, &wxp->w_argv[i]))
			goto bad;

	if (upkuint(mp, &wxp->w_outtid)
	|| upkuint(mp, &wxp->w_outctx)
	|| upkuint(mp, &wxp->w_outtag)
	|| upkuint(mp, &wxp->w_trctid)
	|| upkuint(mp, &wxp->w_trcctx)
	|| upkuint(mp, &wxp->w_trctag)
	|| upkuint(mp, &wxp->w_nenv))
		goto bad;

	wxp->w_hosttotal = wxp->w_veclen; /* this is the task count! */

	if (pvmtracer.trctid) {
		if (!(wxp->w_trctid) && pvmtracer.trctag) {
			wxp->w_trctid = pvmtracer.trctid;
			wxp->w_trcctx = pvmtracer.trcctx;
			wxp->w_trctag = pvmtracer.trctag;
			munge_tenv++;
		}
	}

	if (pvmtracer.outtid) {
		if (!(wxp->w_outtid) && pvmtracer.outtag) {
			wxp->w_outtid = pvmtracer.outtid;
			wxp->w_outctx = pvmtracer.outctx;
			wxp->w_outtag = pvmtracer.outtag;
		}
	}

	wxp->w_env = TALLOC((wxp->w_nenv + 1), char*, "env");
	BZERO(wxp->w_env, (wxp->w_nenv + 1) * sizeof(char*));

	for (i = 0; i < wxp->w_nenv; i++)
		if (upkstralloc(mp, &wxp->w_env[i]))
			goto bad;

	if ( upkuint(mp, &wxp->w_instance) || upkuint(mp, &wxp->w_outof))
		goto bad;

	/* check for spawn working directory */

	for (i = 0; i < wxp->w_nenv; i++)
		if ( !strncmp( "PVMSPAWNWD=", wxp->w_env[i],
				strlen("PVMSPAWNWD=") ) )
			wd = STRALLOC( wxp->w_env[i] + strlen("PVMSPAWNWD=") );

	/* munge env for tracing stuff */

	if ( munge_tenv ) {
		sprintf( tmp, "PVMTMASK=%s", pvmtracer.tmask );
		pvmenvinsert( &(wxp->w_env), tmp );

		sprintf( tmp, "PVMTRCBUF=%d", pvmtracer.trcbuf );
		pvmenvinsert( &(wxp->w_env), tmp );

		sprintf( tmp, "PVMTRCOPT=%d", pvmtracer.trcopt );
		pvmenvinsert( &(wxp->w_env), tmp );

		for ( wxp->w_nenv = 0 ; wxp->w_env[ wxp->w_nenv ] ;
			(wxp->w_nenv)++ );
	}

	/* call ppi_load to get tasks running */

	wxp->w_sched = schtid;

	/* change to desired working directory (if specified) */
	if (wd) {
		savewd = (char *) getcwd( (char *) NULL, 255 );
		chdir( wd );
	}

#if defined(IMA_PGON) || defined(IMA_SP2MPI) || defined(IMA_AIX4SP2) \
		|| defined(IMA_AIX5SP2) || defined(IMA_BEOLIN)
	if (!(wxp->w_flags & PvmMppFront))
	{
		mpp_load(wxp);
	}
	else
#endif
	{
		ppi_load(wxp);
	}

	/* go back to original directory (if getcwd() was successful) */
	if (savewd)
		chdir( savewd );

for (i = 0; i < wxp->w_veclen; i++) {
		if (wxp->w_vec[i] > 0) {
			if (wxp->w_trctid > 0) {
				tev_send_newtask(
					wxp->w_trctid, wxp->w_trcctx, wxp->w_trctag,
					wxp->w_vec[i], wxp->w_ptid, wxp->w_flags,
					wxp->w_argv[0] );
			}
			if (wxp->w_outtid > 0) {
				mp2 = mesg_new(0);
				mp2->m_dst = wxp->w_outtid;
				mp2->m_ctx = wxp->w_outctx;
				mp2->m_tag = wxp->w_outtag;
				pkint(mp2, wxp->w_vec[i]);
				pkint(mp2, TO_NEW);
				pkint(mp2, wxp->w_ptid);
				sendmessage(mp2);
			}
		}
	}

	pkint(rmp, wxp->w_veclen);
	for (i = 0; i < wxp->w_veclen; i++)
		pkint(rmp, wxp->w_vec[i]);
	sendmessage(rmp);
	goto cleanup;

bad:
	pvmlogprintf("exectasks() from t%x bad msg format\n", mp->m_src);

cleanup:
	if (wxp->w_argv)
		*--wxp->w_argv = 0;
	free_wait_spawn(wxp);
	if (wd)
		PVM_FREE(wd);
	return 0;
}
