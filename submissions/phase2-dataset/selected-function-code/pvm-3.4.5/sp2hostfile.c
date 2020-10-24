sp2hostfile(first, count)
	int first;			/* first node in the set */
	int count;			/* number of nodes requested */
{
	int i;
	FILE *tmpfp;

	if (partsize < count) { 
		sprintf(pvmtxt, "sp2hostfile() need at least %d nodes\n", count+1);
		pvmlogperror(pvmtxt);
		return (char *)0;
	}
	(void)PVMTMPNAMFUN(nodefile);
	if (!(tmpfp = fopen(nodefile, "w"))) {
		sprintf(pvmtxt, "sp2hostfile() fopen %s", nodefile);
		pvmlogperror(pvmtxt);
		return (char *)0;
	}
	if (pvmdebmask & PDMNODE) {
		sprintf(pvmtxt, "sp2hostfile() POE host file: %s\n", nodefile);
		pvmlogerror(pvmtxt);
	}
	for (i = first; i < count + first; i++) 
		fprintf(tmpfp, "%s\n", nodelist[i]);
	fclose(tmpfp);
	return nodefile;
}
