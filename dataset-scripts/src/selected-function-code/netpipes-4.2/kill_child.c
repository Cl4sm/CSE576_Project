void kill_child(childpid)
     int	childpid;
{
    if (verbose)
	fprintf(stderr, "%s: child time limit exceeded; killing %s.\n",
		progname, childprogname);

    kill (childpid, SIGTERM);
    if (!deadchild)
	sleep(2);
    kill (childpid, SIGHUP);
    if (!deadchild)
	sleep(2);
    kill (childpid, SIGKILL);

    {
      int i=0;
      while (!deadchild) {
	int rval;
	sleep(1);
	rval = kill(childpid, SIGKILL);
	if (rval != 0 && errno == ESRCH)
	  break;
	if (i==5 && verbose) {
	  fprintf(stderr, "%s: child %s refuses to die [%d]. "
			"I refuse to stop killing it.\n",
			progname, childprogname, rval);
	}
	i++;
      }
    }
    /* it better be dead now */
}