	struct task	*tp;
{
	int			i;
	struct task	*nodetp = 0;
	int			ptypepart;

	for (i = 0; i < partsize; i++)
		if (nodeaddr[i] == tp->t_sad.sin_addr.s_addr)
			{
			ptypepart = (0 << (ffs(tidtmask) - 1)) | TIDONNODE;
			sprintf(pvmtxt, "mpp_find looking for t%x\n", myhostpart + ptypepart + i);
			pvmlogerror(pvmtxt);
			if (nodetp = task_find( myhostpart + ptypepart + i ))
				break;	/* found match */
			}
	if (!nodetp)
		pvmlogerror( "mpp_find:  Task not found\n" );
	return nodetp;
}	/* mpp_find() */
