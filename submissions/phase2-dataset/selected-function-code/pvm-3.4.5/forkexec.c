int
forkexec(flags, name, argv, nenv, env, inst, hosttotal, outof, tpp)
	int flags;				/* exec options */
	char *name;				/* filename */
	char **argv;			/* arg list (argv[-1] must be there) */
	int nenv;				/* num of envars */
	char **env;				/* envars */
	int inst;				/* this processes instance */
	int hosttotal;			/* how many on this host */
	int outof;				/* how many are being spawned across machine */
	struct task **tpp;		/* return task context */
{
	int tid;				/* task tid */
	int pid;				/* task pid */
	int pfd[2];				/* pipe back from task */
	struct task *tp;		/* new task context */
	char path[MAXPATHLEN];
	struct stat sb;
	char **ep, **eplist;
	int i;
	struct pmsg *mp;		/* message to tasker */
	struct waitc *wp;
	int ac;
	int realrunstate;
	char buf[32];
#ifdef IMA_BEOSCYLD
	int node;
#endif

	static char *nullep[] = { "", 0 };
#ifndef IMA_OS2
	static int nextfakepid = 10000000;		/* XXX fix this */
#endif

	if ((tid = tid_new()) < 0) {
		pvmlogerror("forkexec() out of tids?\n");
		return PvmOutOfRes;
	}
	if ((tp = task_new(tid)) == NULL) {
		pvmlogerror("forkexec() too many tasks?\n");
		return PvmOutOfRes;
	}

	/* search for file */

	eplist = CINDEX(name, '/') ? nullep : epaths;

	for (ep = eplist; *ep; ep++) {
		(void)strcpy(path, *ep);
		if (path[0])
			(void)strcat(path, "/");
		(void)strncat(path, name, sizeof(path) - strlen(path) - 1);
#ifdef IMA_OS2
		(void)strcat(path,".exe");	/* no *.cmd !!! */
#endif
#ifdef IMA_BEOSCYLD
		/* what node are we on? */
		node = bproc_currnode();
		/* only perform the stat check when we're
		 * running on the master; since the slave
		 * nodes typically don't contain binaries,
		 * the stat call is more than likely to fail
		 * anyway; down below we perform some bproc
		 * magic to "find" the binary back on the
		 * master when we're running on a slave
		 */
		if(node == BPROC_NODE_MASTER)
#endif
		if (stat(path, &sb) == -1
				|| ((sb.st_mode & S_IFMT) != S_IFREG)
				|| !(sb.st_mode & S_IEXEC)) {
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("forkexec() stat failed <%s>\n", path);
			}
			continue;
		}

		if (taskertid) {
			mp = mesg_new(0);
			mp->m_tag = SM_STTASK;
			mp->m_dst = taskertid;
			pkint(mp, tid);
			pkint(mp, flags);
			pkstr(mp, path);
			for (ac = 1; argv[ac]; ac++) ;
			pkint(mp, ac);
			pkstr(mp, path);
			for (i = 1; i < ac; i++)
				pkstr(mp, argv[i]);
			pkint(mp, nenv + 1);
			sprintf(buf, "PVMEPID=%d", nextfakepid);
			pkstr(mp, buf);
			task_setpid(tp, nextfakepid);
			if (++nextfakepid > 20000000)
				nextfakepid = 10000000;
			for (i = 0; i < nenv; i++)
				pkstr(mp, env[i]);
			pkint(mp, inst);
			pkint(mp, hosttotal);
			pkint(mp, outof);
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("forkexec() info:: inst %d host %d outof %d\n",
							inst, hosttotal, outof);
			}
			wp = wait_new(WT_TASKSTART);
			wp->wa_tid = tid;
			wp->wa_on = taskertid;
			mp->m_wid = wp->wa_wid;
			sendmessage(mp);
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("forkexec() sent tasker t%x pid %d\n",
						tp->t_tid, tp->t_pid);
			}

		} else {
#ifdef	IMA_TITN
			if (socketpair(AF_UNIX, SOCK_STREAM, 0, pfd) == -1) {
				pvmlogperror("forkexec() socketpair");
				task_free(tp);
				return PvmOutOfRes;
			}
#else
			if (pipe(pfd) == -1) {
				pvmlogperror("forkexec() pipe");
				task_free(tp);
				return PvmOutOfRes;
			}
#endif

	/*
	* switch runstate to is-task before forking to avoid race.
	* if we're killed as a task, we don't want to clean up pvmd stuff.
	*/
			realrunstate = runstate;
			runstate = PVMDISTASK;
#if defined(IMA_CSPP) && defined(BALANCED_SPAWN)
			pid = cnx_sc_fork(CNX_INHERIT_SC, (int) __get_node_id());
#else
#ifndef IMA_OS2
			pid = fork();
#else
			pid = os2_spawn( path, argv, nenv, env,
					( flags & PvmTaskDebug ) ? debugger : 0 );
#endif
#endif
			if (pid)
				runstate = realrunstate;

			if (!pid) {

	/* close any random fds */

				dup2(pfd[1], 1);
				dup2(1, 2);
				for (i = getdtablesize(); --i > 2; )
					(void)close(i);
	/*
	* set envars
	*/
				while (nenv-- > 0) {
					pvmputenv(env[nenv]);
/*
					pvmlogprintf("forkexec() putenv(%s)\n", env[nenv]);
*/
				}
	/*
	* put expected pid in environment for libpvm in case
	* the process we exec forks before connecting back to the pvmd
	*/
				sprintf(buf, "PVMEPID=%d", getpid());
				pvmputenv(buf);
				argv[0] = path;
				if (flags & PvmTaskDebug) {
					char *p;

					argv--;
					if (p = getenv("PVM_DEBUGGER"))
						argv[0] = p;
					else
						argv[0] = debugger;
					execv(argv[0], argv);

				} else {
#ifdef IMA_BEOSCYLD
				  /* if we're running on the master */
				  if(node == BPROC_NODE_MASTER) {
				    /* simply call execv */
				    execv(path,argv);
				  }
				  /* if we're running on a slave node */
				  else {
				    /* migrate process back to the master */
				    if(bproc_move(BPROC_NODE_MASTER) != -1) {
				      /* locate the executable */
				      for(ep = eplist; *ep; ep++) {
					strcpy(path,*ep);
					if(path[0]) strcat(path,"/");
					strncat(path,name,sizeof(path)-strlen(path)-1);
					if((stat(path,&sb) == -1) ||
					   ((sb.st_mode & S_IFMT) != S_IFREG) ||
					   !(sb.st_mode & S_IEXEC)) {
					  /* try the next path */
					  continue;
					}
					else {
					  /* execmove back to the slave */
					  bproc_execmove(node,path,argv,env);
					  /* if we get this far, the call to execmove
					   * failed and we now have a ghost job back
					   * on the slave and this whacked job on the
					   * master; if we exit, both jobs are cleaned
					   * up through the magic of bproc
					   */
					  exit(1);
					}
				      }
				      /* if we fell through the loop, we didn't
				       * find the executable file; in this case
				       * we move back to the slave and return
				       */
				      if(bproc_move(node) == -1) {
					/* if the move back to the slave fails,
					 * we have a ghost job back on the slave
					 * and this whacked job on the master;
					 * if we exit, both jobs are cleaned up
					 * through the magic of bproc
					 */
					exit(1);
				      }
				      else {
					/* since the move back to the slave was
					 * successfully and we were unable to find
					 * the executable back on the master, we
					 * mimic forkexec's normal functionality
					 * of ending the for-loop
					 */
					break;
				      }
				    }
				    else if (pvmdebmask & PDMTASK) {
				      pvmlogprintf(
							"forkexec() bproc move to master failed\n");
				    }
				    /* end-if moving to master */
				  }
				  /* end-if running on a slave */
#else
					execv(path, argv);
#endif
				}
				exit(1);
			}
			if (pid == -1) {
				pvmlogperror("forkexec() fork");
				(void)close(pfd[0]);
				(void)close(pfd[1]);
				task_free(tp);
				return PvmOutOfRes;
			}
			(void)close(pfd[1]);

			task_setpid(tp, pid);
			tp->t_out = pfd[0];
			tp->t_flag |= TF_FORKD;

			wrk_fds_add(tp->t_out, 1);
			if (pvmdebmask & PDMTASK) {
				pvmlogprintf("forkexec() new task t%x pid %d pfd=%d\n",
						tp->t_tid, tp->t_pid, tp->t_out);
			}
		}

		tp->t_a_out = STRALLOC(name);
		*tpp = tp;
		return 0;
	}
	if (pvmdebmask & PDMTASK) {
		pvmlogprintf("forkexec() didn't find <%s>\n", name);
	}
	task_free(tp);
	return PvmNoFile;
}
