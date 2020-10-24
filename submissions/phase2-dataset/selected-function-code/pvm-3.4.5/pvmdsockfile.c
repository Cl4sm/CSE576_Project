char *
pvmdsockfile()
{
	static char buf[255];

	char hna[128];
	char *pvmtmp;
	char *p;

#ifdef	IMA_CSPP
	int scid = get_scid();	/* default (system) subcomplex ID is 1 */
#endif

	pvmtmp = pvmgettmp();

#ifdef	SHAREDTMP

	if (gethostname(hna, sizeof(hna)-1) == -1) {
		pvmlogerror("pvmdsockfile() can't gethostname()\n");
		return (char*)0;
	}
	if (p = CINDEX(hna, '.'))
		*p = 0;

#ifdef	IMA_CSPP
	if (scid > 1)
		(void)sprintf(buf, TDSOCKNAME_CSPP,
				pvmtmp, pvm_useruid, scid, hna);
	else
#endif
		(void)sprintf(buf, TDSOCKNAME, pvmtmp, pvm_useruid, hna);

#else

#ifdef	IMA_CSPP
	if (scid > 1)
		(void)sprintf(buf, TDSOCKNAME_CSPP, pvmtmp, pvm_useruid, scid);
	else
#endif
#ifdef WIN32
		sprintf( buf, "%s\\pvmd.%s", pvmtmp, username );
#else
		(void)sprintf(buf, TDSOCKNAME, pvmtmp, pvm_useruid);
#endif

#endif

	/* Append a Virtual Machine ID, If Set */
	if ( p = getenv("PVM_VMID") ) {
		strcat( buf, "." );
		strcat( buf, p );
	}

	return buf;
}
