pvm_pkmesg(mid)
	int mid;
{
	int cc = 0;
	struct pmsg *mp;

	if (mid <= 0)
		cc = PvmBadParam;
	else if (mp = midtobuf(mid)) {
		if (!pvmsbuf)
			cc = PvmNoBuf;
		else if (mp == pvmsbuf)
			cc = PvmBadParam;
		else
			cc = pmsg_pack(pvmsbuf, mp);
	} else
		cc = PvmNoSuchBuf;

	return cc;
}
