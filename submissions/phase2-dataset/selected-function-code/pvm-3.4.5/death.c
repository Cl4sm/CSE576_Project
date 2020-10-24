APIRET death(PID pid)
{
	APIRET rc;
	HFILE hfd;
	ULONG action,plen;
	USHORT param;

	if ((rc=DosOpen((PSZ)"/dev/fastio$", (PHFILE)&hfd, (PULONG)&action,
	    (ULONG)0, FILE_SYSTEM, FILE_OPEN,
	    OPEN_SHARE_DENYNONE|OPEN_FLAGS_NOINHERIT|OPEN_ACCESS_READONLY,
	    (ULONG)0))) {
		fputs("Error opening fastio$ driver...\n",stderr);
		fputs("Please install xf86sup.sys in config.sys!\n",stderr);
		return rc;
	}

	param = pid;

	if ((rc=DosDevIOCtl(hfd, (ULONG)0x76, (ULONG)0x65,
	    (PULONG*)&param,sizeof(USHORT),&plen,
	    NULL, 0, NULL)) != 0) {
		fprintf(stderr,"fastio$ driver: kill(%i,9) failed rc=%i\n",pid,rc); 
                DosClose(hfd);
                return rc;
        }
	DosClose(hfd);
	return 0;
}
