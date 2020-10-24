free_wait_spawn(wxp)
	struct waitc_spawn *wxp;
{
	int i;

	if (wxp->w_file)
		PVM_FREE(wxp->w_file);
	if (wxp->w_argv) {
		for (i = 0; i < wxp->w_argc; i++)
			if (wxp->w_argv[i])
				PVM_FREE(wxp->w_argv[i]);
		PVM_FREE(wxp->w_argv);
	}
	if (wxp->w_env) {
		for (i = 0; i < wxp->w_nenv; i++)
			if (wxp->w_env[i])
				PVM_FREE(wxp->w_env[i]);
		PVM_FREE(wxp->w_env);
	}
	if (wxp->w_ht)
		ht_free(wxp->w_ht);
	if (wxp->w_vec)
		PVM_FREE(wxp->w_vec);
	PVM_FREE(wxp);
	return 0;
}
