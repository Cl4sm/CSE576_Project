{
	int i;
	char *s;

	pvmlogerror("pidtid_dump()\n");
	for (i = 0; i < maxpidtid; i++) {
		switch (pidtids[i].pt_stat) {
		case ST_EXIT:
			s = 0;
			break;

		case ST_NOTREADY:
			s = "NOTREADY";
			break;

		case ST_SHMEM:
			s = "SHMEM";
			break;

		case ST_SOCKET:
			s = "SOCKET";
			break;

		case ST_FINISH:
			s = "FINISH";
			break;

		default:
			s = "UNKNOWN";
			break;
		}

		if (s) {
			sprintf(pvmtxt, "%4d pid %d tid %x ptid %x stat %s key 0x%x",
					i,
					pidtids[i].pt_pid,
					pidtids[i].pt_tid,
					pidtids[i].pt_ptid,
					s,
					pidtids[i].pt_key);
#ifdef IMA_CSPP
			sprintf(pvmtxt + strlen(pvmtxt), " node %d\n", pidtids[i].pt_node);
#else
			strcat(pvmtxt, "\n");
#endif
			pvmlogerror(pvmtxt);
		}
	}
	return 0;
}
