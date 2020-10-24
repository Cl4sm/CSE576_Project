void
sort_pathgroups (struct multipath *mp) {
	int i, j;
	struct pathgroup * pgp1, * pgp2;

	if (!mp->pg)
		return;

	vector_foreach_slot(mp->pg, pgp1, i) {
		path_group_prio_update(pgp1);
		for (j = i - 1; j >= 0; j--) {
			pgp2 = VECTOR_SLOT(mp->pg, j);
			if (!pgp2)
				continue;
			if (pgp2->priority > pgp1->priority ||
			    (pgp2->priority == pgp1->priority &&
			     pgp2->enabled_paths >= pgp1->enabled_paths)) {
				vector_move_up(mp->pg, i, j + 1);
				break;
			}
		}
		if (j < 0 && i != 0)
		vector_move_up(mp->pg, i, 0);
	}
}
