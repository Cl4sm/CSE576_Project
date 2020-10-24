tobuf_init()
{
	int i;
	struct pvmminfo minfo;
	int ictx;

	if (!tobuflist) {
		tobuflist = TALLOC(1, struct tobuf, "obuf");
		BZERO((char*)tobuflist, sizeof(struct tobuf));
		tobuflist->o_link = tobuflist->o_rlink = tobuflist;

		i = pvm_setopt(PvmResvTids, 1);

		BZERO(&minfo, sizeof(minfo));
		minfo.src = -1;
		minfo.ctx = SYSCTX_TC;
		minfo.tag = TC_OUTPUT;
		pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvmclaimo);
		minfo.tag = TC_OUTPUTX;
		pvm_addmhf(minfo.src, minfo.tag, minfo.ctx, pvmflusho);

		ictx = pvm_setcontext(SYSCTX_TC);
		pvm_notify(PvmTaskExit, TC_OUTPUTX, 1, &pvmmytid);
		pvm_setcontext(ictx);

		pvm_setopt(PvmResvTids, i);
	}
	return 0;
}
