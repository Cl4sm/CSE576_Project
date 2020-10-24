int
shm_wrt_pkts(tp)
struct task * tp;
{
struct pkt *pp, *pp2, *pptmp;
struct pkt tmplist;

	
	if (tp -> t_txq)
	{
		pptmp = &tmplist;
		pptmp -> pk_link = pptmp->pk_rlink = pptmp;

		/* Take packets off of the t_txq */
		pp = tp -> t_txq -> pk_link;
		while (pp != tp -> t_txq)
		{
			pp2 = pp;
			pp = pp -> pk_link;
			LISTDELETE(pp2, pk_link, pk_rlink);
			LISTPUTBEFORE(pptmp, pp2, pk_link, pk_rlink);
		}

		pp = pptmp -> pk_link;

		while (pp != pptmp)
		{
			pp2 = pp;
			pp = pp -> pk_link;
			LISTDELETE(pp2, pk_link, pk_rlink);
			if (tp -> t_flag & TF_SHMCONN)
			{
				if ( pvmdebmask & PDMPACKET)
					pvmlogprintf("shm_wrt_pkts for %x \n", tp -> t_tid);
				mpp_output(tp, pp2);
			}
			else
				break;
		}	
	}
		
	return 0;
}
