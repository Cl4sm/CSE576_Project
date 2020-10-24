pvmsetlog(how)
	int how;
{
	char buf[255];
	char hna[128];
	char *pvmtmp;
	char *p;

#ifdef	IMA_CSPP
	int scid = get_scid();	/* default (system) subcomplex ID is 1) */
					/* differentiate logfile when not on system s/c */
#endif

	if (2 & how & ~log_how) {
		pvmtmp = pvmgettmp();
#ifdef	SHAREDTMP
		if (gethostname(hna, sizeof(hna)-1) == -1) {
			pvmlogerror("pvmsetlog() can't gethostname()\n");
			return 0;
		}
		if (p = CINDEX(hna, '.'))
			*p = 0;
#ifdef	IMA_CSPP
		if (scid > 1)
			(void)sprintf(buf, PVMDLOGFILE_CSPP,
					pvmtmp, pvm_useruid, scid, hna);
		else
#endif
			(void)sprintf(buf, PVMDLOGFILE, pvmtmp, pvm_useruid, hna);

#else	/*SHAREDTMP*/
#ifdef	IMA_CSPP
		if (scid > 1)
			(void)sprintf(buf, PVMDLOGFILE_CSPP,
					pvmtmp, pvm_useruid, scid);
		else
#endif
#ifndef WIN32
		(void)sprintf(buf, PVMDLOGFILE, pvmtmp, pvm_useruid);
#else
		(void)sprintf(buf, "%s\\pvml.%s", pvmtmp, username);	
#endif		
#endif	/*SHAREDTMP*/

		/* Append a Virtual Machine ID, If Set */
		if ( p = getenv("PVM_VMID") ) {
			strcat( buf, "." );
			strcat( buf, p );
		}

		if ((log_fd = open(buf, O_WRONLY|O_TRUNC|O_CREAT|O_APPEND, 0600)) != -1)
			log_ff = fdopen(log_fd, "a");
		else
			how &= ~2;

		if (p = getenv("PVMDLOGMAX"))
			pvmdlogmax = atoi(p);

		pvmdlogmax = pvmdlogmax;  /* for picky SGI compiler warning */
	}

	if (2 & log_how & ~how) {
		(void)fclose(log_ff);
		log_ff = 0;
		log_fd = -1;
	}

	log_how = how;
	return 0;
}
