locl_spawn( file, argv )
char *file;
char **argv;
{
	struct pmsg *spawnmesg;

	char **ep;

	int i;
	int n;

	spawnmesg = mesg_new(0);
	spawnmesg->m_tag = DM_EXEC;
	spawnmesg->m_dst = TIDPVMD;

	pkint(spawnmesg, PvmParentNotSet);
	pkstr(spawnmesg, file);
	pkint(spawnmesg, PvmTaskDefault | PvmMppFront);
	pkint(spawnmesg, 1); /* number of tasks per host */

	if (argv)
		for (n = 0; argv[n]; n++);
	else
		n = 0;
	pkint(spawnmesg, n);
	for ( i=0 ; i < n ; i++ )
		pkstr(spawnmesg, argv[i]);

	pkint(spawnmesg, 0); /* outtid */
	pkint(spawnmesg, 0); /* outctx */
	pkint(spawnmesg, 0); /* outtag */
	pkint(spawnmesg, 0); /* trctid */
	pkint(spawnmesg, 0); /* trcctx */
	pkint(spawnmesg, 0); /* trctag */

	pkint(spawnmesg, 0); /* inherit pvmd env vars as is */

	pkint(spawnmesg, 0); /* start proc location */
	pkint(spawnmesg, 1); /* total tasks to spawn */

	sendmessage(spawnmesg);
}
