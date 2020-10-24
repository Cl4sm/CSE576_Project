pk_new(len)
	int len;	/* (max) buffer size or 0 */
{
	struct pkt *pp;

	if (!(pp = TALLOC(1, struct pkt, "pkt")))
		goto oops;
/*
	BZERO((char*)pp, sizeof(struct pkt));
*/

	if (len) {		/* slave pkt */
		if (!(pp->pk_dat = pp->pk_buf = da_new(len))) {
			PVM_FREE(pp);
			goto oops;
		}
		pp->pk_max = len;
		pp->pk_len = 0;
		pp->pk_link = pp->pk_rlink = 0;

	} else {		/* master */
		pp->pk_dat = pp->pk_buf = 0;
		pp->pk_link = pp->pk_rlink = pp;
	}
	pp->pk_cpos = 0;
	pp->pk_tlink = pp->pk_trlink = 0;
	return pp;

oops:
	pvmlogerror("pk_new() can't get memory\n");
	pvmbailout(0);
	return (struct pkt*)0;
}
