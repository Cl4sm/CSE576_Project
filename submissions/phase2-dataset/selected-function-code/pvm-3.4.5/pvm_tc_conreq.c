static int
pvm_tc_conreq(mid)
	int mid;
{
	int src;					/* sender of request */
	int sbf = 0;				/* reply message mid */
	int ttpro;					/* protocol revision */
	int ackd;					/* allow connection (0) */
	int i;
	int ictx;

	pvm_bufinfo(mid, (int *)0, (int *)0, &src);

	pvmlogprintf("pvm_tc_conreq() TCP conn request from t%x!\n", src);

	sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
	ttpro = TDPROTOCOL;
	ackd = 1;
	pvm_pkint(&ttpro, 1, 1);
	pvm_pkint(&ackd, 1, 1);
	pvm_pkstr("");
i = pvmrescode;
pvmrescode = 1;
	ictx = pvm_setcontext(SYSCTX_TC);
	pvm_send(src, TC_CONACK);
	pvm_setcontext(ictx);
pvmrescode = i;
	pvm_freebuf(pvm_setsbuf(sbf));

	pvm_freebuf(mid);
	return 0;
}
