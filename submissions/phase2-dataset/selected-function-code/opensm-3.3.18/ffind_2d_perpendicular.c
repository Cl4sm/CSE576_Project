struct f_switch *ffind_2d_perpendicular(struct f_switch *fsw0,
					struct f_switch *fsw1,
					struct f_switch *fsw2)
{
	int p1;
	struct link *l;
	struct endpoint *far_end;
	struct f_switch *fsw3 = NULL;

	if (!(fsw0 && fsw1 && fsw2))
		goto out;

	/*
	 * Look at all the ports on the switch, fsw1,  that is the base of
	 * the perpendicular.
	 */
	for (p1 = 0; p1 < fsw1->port_cnt; p1++) {
		/*
		 * Ignore everything except switch links that haven't
		 * been installed into the torus.
		 */
		if (!(fsw1->port[p1] && fsw1->port[p1]->sw &&
		      fsw1->port[p1]->type == PASSTHRU))
			continue;

		l = fsw1->port[p1]->link;

		if (l->end[0].n_id == fsw1->n_id)
			far_end = &l->end[1];
		else
			far_end = &l->end[0];
		/*
		 * Ignore CAs
		 */
		if (!(far_end->type == PASSTHRU && far_end->sw))
			continue;

		fsw3 = far_end->sw;
		if (fsw3->n_id == fsw2->n_id)	/* wrong perpendicular */
			continue;

		if (ffind_face_corner(fsw0, fsw1, fsw3))
			goto out;
	}
	fsw3 = NULL;
out:
	return fsw3;
}