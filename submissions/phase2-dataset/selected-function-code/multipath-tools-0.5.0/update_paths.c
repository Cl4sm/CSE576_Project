update_paths (struct multipath * mpp)
{
	int i, j;
	struct pathgroup * pgp;
	struct path * pp;

	if (!mpp->pg)
		return 0;

	vector_foreach_slot (mpp->pg, pgp, i) {
		if (!pgp->paths)
			continue;

		vector_foreach_slot (pgp->paths, pp, j) {
			if (!strlen(pp->dev)) {
				if (devt2devname(pp->dev, FILE_NAME_SIZE,
						 pp->dev_t)) {
					/*
					 * path is not in sysfs anymore
					 */
					pp->chkrstate = pp->state = PATH_DOWN;
					continue;
				}
				pp->mpp = mpp;
				if (pathinfo(pp, conf->hwtable, DI_ALL))
					pp->state = PATH_UNCHECKED;
				continue;
			}
			pp->mpp = mpp;
			if (pp->state == PATH_UNCHECKED ||
			    pp->state == PATH_WILD) {
				if (pathinfo(pp, conf->hwtable, DI_CHECKER))
					pp->state = PATH_UNCHECKED;
			}

			if (pp->priority == PRIO_UNDEF) {
				if (pathinfo(pp, conf->hwtable, DI_PRIO))
					pp->priority = PRIO_UNDEF;
			}
		}
	}
	return 0;
}
