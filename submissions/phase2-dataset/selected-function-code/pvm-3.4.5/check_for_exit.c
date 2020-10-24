void
check_for_exit(src)
	int src;
{
	extern struct peer *peers;
	struct peer *pp;

	for (pp = peers->p_link; pp != peers; pp = pp->p_link)
		if (pp->p_tid == src) {
			if (pp->p_exited) {
				int i;
				int detach = 1;
				struct pmsg *up;

				for (i = 1; i < pvmmidhsiz; i++) {
					if ((up = pvmmidh[i].m_umb) && (up->m_src == src)) {
						detach = 0;
						break;
					}
				}

				if (detach) {
					peer_detach(pp);
				}
			}
			break;
		}
}
