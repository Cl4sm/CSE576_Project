enable_group(struct path * pp)
{
	struct pathgroup * pgp;

	/*
	 * if path is added through uev_add_path, pgindex can be unset.
	 * next update_strings() will set it, upon map reload event.
	 *
	 * we can safely return here, because upon map reload, all
	 * PG will be enabled.
	 */
	if (!pp->mpp->pg || !pp->pgindex)
		return;

	pgp = VECTOR_SLOT(pp->mpp->pg, pp->pgindex - 1);

	if (pgp->status == PGSTATE_DISABLED) {
		condlog(2, "%s: enable group #%i", pp->mpp->alias, pp->pgindex);
		dm_enablegroup(pp->mpp->alias, pp->pgindex);
	}
}
