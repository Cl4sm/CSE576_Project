void
ttpcb_dead(pcbp)
	struct ttpcb *pcbp;
{
	struct pmsg *up;
	int sbf;

	pcbp->tt_state = TTDEAD;
	if (pcbp->tt_fd != -1) {
		pvm_fd_delete(pcbp->tt_fd, 3);
		(void)close(pcbp->tt_fd);

	/* notify routing socket closed */

	/*
	XXX problems with the route notify messages:
	XXX they can be delayed / cause deadlock because gotem isn't incremented
	*/
		check_routedelete(pcbp);
		pcbp->tt_fd = -1;
	}
#ifndef NOUNIXDOM
	if (pcbp->tt_spath) {
		(void)unlink(pcbp->tt_spath);
		pcbp->tt_spath = 0;
	}
#endif
	if (pcbp->tt_rxf) {
		fr_unref(pcbp->tt_rxf);
		pcbp->tt_rxf = 0;
	}
	if (up = pcbp->tt_rxfrag) {
		while (up->m_link != up)
			umbuf_free(up->m_link);
	}
}
