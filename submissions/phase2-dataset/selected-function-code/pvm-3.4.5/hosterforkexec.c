static int
hosterforkexec(tid,name, tpp)
	int tid;			/* tid given by hosterexectask */
	char *name;			/* filename */

	struct task **tpp;		/* return task context */
{
	int pid=-1;			/* task pid */
	char *argv[2];
	struct task *tp;		/* new task context */
	char *expected_pid;
	char buf[32];
	char *myenv[100];
	char **p, **q;
	struct stat sb;	
	extern int *ptr_nfp;		/* XXX fix this */

	if (stat(name, &sb) == -1) 
		return PvmNoFile;

	tp = task_new(tid);		

	p = myenv;
	q = environ;
	while (*q) {
		*p++ = *q++;   	
	}
			/* copy all the environment for 
				 	socket stuff and more */
	expected_pid=malloc(20 * sizeof(char));
	sprintf(expected_pid, "PVMEPID=%d", *ptr_nfp);
		
	*p++ = expected_pid;
	*p=0;
	pvmputenv(expected_pid);
	argv[0]=name;
	argv[1]=0;

	pid = spawnve(P_NOWAIT,name,argv,myenv); 
	if (pid == -1) {
			pvmlogperror("forkexec_hoster() _spawnve");
			/* task_free(&tp); */
			pvmbailout(0);
			return PvmOutOfRes;
	}
		
	task_setpid(tp,*ptr_nfp);
	*ptr_nfp=*ptr_nfp + 1;

	tp->t_flag |= TF_FORKD;

	tp->t_a_out = STRALLOC(name);
	 
	*tpp = tp;
	return 0;

}		
