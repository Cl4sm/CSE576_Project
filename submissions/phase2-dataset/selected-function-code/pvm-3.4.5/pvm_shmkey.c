pvm_shmkey(pid)
	int pid;
{
	static int pvmd_key = 0;

	int k;
	char *p;

	if (!pvmd_key) {
		if (p = getenv("PVMKEY"))
			pvmd_key = atoi(p);
		else
			pvmd_key = getuid();
		pvmd_key = (pvmd_key << 16) & SHMIDUIDPART;
		if (!pvmd_key)
			pvmd_key = SHMIDUIDPART;
#ifdef IMA_CSPP
		pvmd_key |= (get_scid() & 0xf) << 12;
#endif
	}

	if (pid) {
		if (!(pid &= SHMIDPIDPART))
			pid = 1;
		k = pvmd_key | pid;
		if (pvmdebmask & (PDMMEM | PDMSEM)) {
			pvmlogprintf("pvmshmkey(): shm/sem key is 0x%x\n", k);
		}

	} else {
		k = pvmd_key;

		if (pvmdebmask & (PDMMEM | PDMSEM)) {
			pvmlogprintf("pvmshmkey(): pvmd shm/sem key is 0x%x\n", k);
		}
	}

	return k;
}
