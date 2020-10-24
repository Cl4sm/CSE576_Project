int
send_my_pid()
{
	struct pmsg *up;
	int sbf;
	int cc;

	sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
	pvm_pkint(&pvmmyupid, 1, 1);
	sbf = pvm_setsbuf(sbf);

	up = midtobuf(sbf);
	up->m_dst = TIDPVMD;
	up->m_tag = TM_SHMCONN;
	up->m_ctx = SYSCTX_TM;

	/* I would love a mxfer... but we have to use a custom */
	/* one-off routine! */
	/* cc = mxfer(up2, &ztv); */
	cc = shmemuptod (up);

	pvm_freebuf(sbf);
	return cc;
}
