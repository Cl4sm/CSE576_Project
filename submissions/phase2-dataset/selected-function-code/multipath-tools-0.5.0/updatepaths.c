static int
updatepaths (struct multipath * mpp)
{
	int i, j;
	struct pathgroup * pgp;
	struct path * pp;

	if (!mpp->pg)
		return 0;

	vector_foreach_slot (mpp->pg, pgp, i){
		if (!pgp->paths)
			continue;

		vector_foreach_slot (pgp->paths, pp, j){
			if (!strlen(pp->dev)){
				if (devt2devname(pp->dev, pp->dev_t)){
					/*
					 * path is not in sysfs anymore
					 */
					pp->state = PATH_DOWN;
					continue;
				}
				pp->mpp = mpp;
				pathinfo(pp, conf->hwtable, DI_ALL);
				continue;
			}
			pp->mpp = mpp;
			if (pp->state == PATH_UNCHECKED ||
					pp->state == PATH_WILD)
				pathinfo(pp, conf->hwtable, DI_CHECKER);

			if (pp->priority == PRIO_UNDEF)
				pathinfo(pp, conf->hwtable, DI_PRIO);
		}
	}
	return 0;
}
