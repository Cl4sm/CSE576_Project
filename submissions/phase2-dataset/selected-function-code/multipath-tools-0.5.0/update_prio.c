int update_prio(struct path *pp, int refresh_all)
{
	int oldpriority;
	struct path *pp1;
	struct pathgroup * pgp;
	int i, j, changed = 0;

	if (refresh_all) {
		vector_foreach_slot (pp->mpp->pg, pgp, i) {
			vector_foreach_slot (pgp->paths, pp1, j) {
				oldpriority = pp1->priority;
				pathinfo(pp1, conf->hwtable, DI_PRIO);
				if (pp1->priority != oldpriority)
					changed = 1;
			}
		}
		return changed;
	}
	oldpriority = pp->priority;
	pathinfo(pp, conf->hwtable, DI_PRIO);

	if (pp->priority == oldpriority)
		return 0;
	return 1;
}
