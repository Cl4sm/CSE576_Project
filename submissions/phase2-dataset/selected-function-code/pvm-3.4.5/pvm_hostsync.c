int
pvm_hostsync(host, clk, delta)
	int host;				/* pvmd tid of host */
	struct timeval *clk;	/* current time on host */
	struct timeval *delta;	/* time relative to local clock */
{
	int cc;
	int sbf, rbf;
	struct timeval myta, mytb, remt;
	int i[2];

	if (!(cc = BEATASK)) {
		sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
		rbf = pvm_setrbuf(0);
		pvm_pkint(&host, 1, 1);

		gettimeofday(&myta, (struct timezone *) NULL);
		if ((cc = msendrecv(TIDPVMD, TM_HOSTSYNC,SYSCTX_TM)) > 0) {
			gettimeofday(&mytb, (struct timezone *) NULL);

			pvm_upkint(&cc, 1, 1);
			if (cc >= 0) {
				cc = 0;
				pvm_upkint(i, 2, 1);
				remt.tv_sec = i[0];
				remt.tv_usec = i[1];

				if (clk)
					*clk = remt;

				if (delta) {
					TVDIVN(&myta, &myta, 2);
					TVDIVN(&mytb, &mytb, 2);
					TVXADDY(&myta, &myta, &mytb);
					TVXSUBY(&myta, &myta, &remt);
					*delta = myta;
				}
			}
			pvm_freebuf(pvm_setrbuf(rbf));
		} else
			pvm_setrbuf(rbf);
		pvm_freebuf(pvm_setsbuf(sbf));
	}

	if (cc < 0)
		lpvmerr("pvm_host_sync", cc);
	return cc;
}
