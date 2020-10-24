int
post_routedelete(tid, ctx, tag)
	int tid;
	int ctx;
	int tag;
{
	int sbf;
	struct waitc *wp;
	struct pmsg *up;
	int i;

	sbf = pvm_setsbuf(pvm_mkbuf(PvmDataFoo));
	pvm_pkint(&tid, 1, 1);
	i = -1;
	pvm_pkint(&i, 1, 1);
	sbf = pvm_setsbuf(sbf);
	up = midtobuf(sbf);
	up->m_ctx = ctx;
	up->m_tag = tag;

	if (ttpcb_find(tid)) {
		wp = wait_new(WT_ROUTED);
		wp->wa_tid = pvmmytid;
		wp->wa_on = tid;
		wp->wa_mesg = up;

	} else {
		mesg_input(up);
	}
	return 0;
}
