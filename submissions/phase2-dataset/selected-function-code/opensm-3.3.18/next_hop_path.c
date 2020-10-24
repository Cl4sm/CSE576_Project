static
bool next_hop_path(struct torus *t, unsigned cdir,
		   struct t_switch *ssw, struct t_switch *dsw,
		   unsigned *pt_grp)
{
	struct t_switch *tsw = NULL;
	bool success = false;
	int d;

	/*
	 * If the path from ssw to dsw turns, this is the switch where the
	 * turn happens.
	 */
	switch (cdir) {
	case 0:
		tsw = t->sw[dsw->i][ssw->j][ssw->k];
		break;
	case 1:
		tsw = t->sw[ssw->i][dsw->j][ssw->k];
		break;
	case 2:
		tsw = t->sw[ssw->i][ssw->j][dsw->k];
		break;
	default:
		goto out;
	}
	if (tsw) {
		d = ring_dir_path(t, cdir, ssw, tsw);
		cdir *= 2;
		if (d > 0)
			*pt_grp = cdir + 1;
		else if (d < 0)
			*pt_grp = cdir;
		else
			goto out;
		success = true;
	}
out:
	return success;
}