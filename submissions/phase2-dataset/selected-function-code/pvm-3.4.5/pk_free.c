void
pk_free(pp)
	struct pkt *pp;
{
	struct pkt *pp2, *pp3;

	if (pp->pk_buf) {		/* slave pkt */
		if (pp->pk_tlink) {
			LISTDELETE(pp, pk_tlink, pk_trlink);
		}
		da_unref(pp->pk_buf);

	} else {				/* master pkt */

	/* free all pkts in chain */
		for (pp2 = pp->pk_link; pp2 != pp; pp2 = pp3) {
			pp3 = pp2->pk_link;
			LISTDELETE(pp2, pk_link, pk_rlink);
			pk_free(pp2);
		}
	}
	PVM_FREE(pp);
}
