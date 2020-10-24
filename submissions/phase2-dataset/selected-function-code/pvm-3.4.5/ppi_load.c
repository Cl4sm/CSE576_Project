int
ppi_load(wxp)
	struct waitc_spawn *wxp;
{
	int i;
	struct task *tp;
	int err = 0;

#ifdef WIN32
	/* even being left untouched the filename gets a further */
	/* extension to %SystemRoot% which causes to return PvmNoFile */
	char filename[64];
	strcpy(filename,wxp->w_argv[0]);
#endif

	for (i = 0; i < wxp->w_veclen; i++) {
		if (err) {
			wxp->w_vec[i] = err;

		} else {
#ifdef WIN32
			strcpy(wxp->w_argv[0],filename);
#endif		
			if ( err = forkexec(wxp->w_flags, wxp->w_argv[0], 
					wxp->w_argv, wxp->w_nenv, wxp->w_env, 
					(wxp->w_instance+i), wxp->w_hosttotal, wxp->w_outof, &tp)) 
		/* inst+i as each process is started inst is really an offset */
			{
				wxp->w_vec[i] = err;

			} else {
				tp->t_ptid = wxp->w_ptid;
				tp->t_outtid = wxp->w_outtid;
				tp->t_outctx = wxp->w_outctx;
				tp->t_outtag = wxp->w_outtag;
				tp->t_trctid = wxp->w_trctid;
				tp->t_trcctx = wxp->w_trcctx;
				tp->t_trctag = wxp->w_trctag;
				tp->t_sched = wxp->w_sched;
				wxp->w_vec[i] = tp->t_tid;
			}
		}
	}
	return 0;
}
