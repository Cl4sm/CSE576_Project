extern void
path_group_prio_update (struct pathgroup * pgp)
{
	int i;
	int priority = 0;
	struct path * pp;

	pgp->enabled_paths = 0;
	if (!pgp->paths) {
		pgp->priority = 0;
		return;
	}
	vector_foreach_slot (pgp->paths, pp, i) {
		if (pp->state == PATH_UP ||
		    pp->state == PATH_GHOST) {
			priority += pp->priority;
			pgp->enabled_paths++;
		}
	}
	if (pgp->enabled_paths)
		pgp->priority = priority / pgp->enabled_paths;
	else
		pgp->priority = 0;
}
