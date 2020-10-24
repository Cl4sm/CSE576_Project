void
ttpcb_delete(pcbp)
	struct ttpcb *pcbp;
{
	struct pmsg *up;

	if (pcbp->tt_link) {
		LISTDELETE(pcbp, tt_link, tt_rlink);
	}
	if (pcbp->tt_fd != -1) {
		pvm_fd_delete(pcbp->tt_fd, 3);
		(void)close(pcbp->tt_fd);
	}
	if (up = pcbp->tt_rxfrag) {
		while (up->m_link != up)
			umbuf_free(up->m_link);
		pmsg_unref(up);
	}
	if (pcbp->tt_rxf)
		fr_unref(pcbp->tt_rxf);
#ifndef NOUNIXDOM
	if (pcbp->tt_spath)
		(void)unlink(pcbp->tt_spath);
#endif

	PVM_FREE(pcbp);
}
