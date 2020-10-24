int
tm_shmconn(tp, mp)
	struct task *tp;
	struct pmsg *mp;	/* remember no return message */
{
	int tid;			
	int pid;						/* real pid of task */
	int ppid;						/* probable pid */
	struct task *tp2;				/* to search for real context */
	struct pmsg *mp2;
	int cc;
	char c;
	int dummy_true = 1;				/* not so harsh "always true"? */

	if ( upkint( mp, &ppid ) ) {
		pvmlogerror(
				"tm_shmconn() bad msg format. Missing Unix pid??\n" );
		goto bail;
	}

#ifndef	SHMEM
	if ( dummy_true ) {
		pvmlogerror( "tm_shmconn() shmem conn on non shmem sys? " );
		pvmlogerror( "Bit bucket...\n" );
		goto bail;
	}
#endif
	
	/*
	*	Task should already have a correct task struct.
	*	Because TM_SHMCONN is passed after the shmem connect phase
	*	in pvmbeatask() in lpvmshmem.c
	*/

	if ( tp2 = task_findpid( ppid ) )
	{
		if ( pvmdebmask & PDMTASK ) {
			pvmlogprintf(
					"tm_shmconn() reconnect task t%x via sockets\n",
					tp2->t_tid);
		}
		/* tp->t_sched = tp2->t_sched; */
	}
	else
	{
		pvmlogerror( "tm_shmconn() conn from non shmem connd task?\n");
		goto bail;		
	}

	/* 
	 * tp2 is now the real task.... 
	 * So we now update it with the correct socket descripter and then
	 * mark the shadow one as dead.
	 *
	 */

	tp2->t_sock = tp->t_sock;

	tp->t_sock = -1;	/* tp will be freed by loclinput() */
	tp->t_rxp = 0;
	tp->t_flag |= TF_CLOSE; /* stop work() complaining - dead socket? */

	/*
	 *	kick it in the butt; it's ready to go
	 */

	/* We are happy to mark it as a shared memory connected process */
	/* i.e. please use shmem (mpp_output) not my socket to talk to me */

	/* the PRESHMCONN flag indicates that messages with MM_PRIO can be
		sent over shared memory, but no others.  tm_getopt will
		set the flag to SHMCONN
	*/
	tp2->t_flag |= TF_PRESHMCONN;

	return 0;

bail:
	/* i.e. kill it! */
	tp->t_sock = -1;	/* tp will be freed by loclinput() */
	tp->t_rxp = 0;
	tp->t_flag |= TF_CLOSE;
	return 0;
}
