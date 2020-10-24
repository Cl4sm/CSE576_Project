select_path_group (struct multipath * mpp)
{
	int i;
	int max_priority = 0;
	int bestpg = 1;
	int max_enabled_paths = 1;
	struct pathgroup * pgp;

	if (!mpp->pg)
		return 1;

	vector_foreach_slot (mpp->pg, pgp, i) {
		if (!pgp->paths)
			continue;

		path_group_prio_update(pgp);
		if (pgp->enabled_paths) {
			if (pgp->priority > max_priority) {
				max_priority = pgp->priority;
				max_enabled_paths = pgp->enabled_paths;
				bestpg = i + 1;
			} else if (pgp->priority == max_priority) {
				if (pgp->enabled_paths > max_enabled_paths) {
					max_enabled_paths = pgp->enabled_paths;
					bestpg = i + 1;
				}
			}
		}
	}
	return bestpg;
}
