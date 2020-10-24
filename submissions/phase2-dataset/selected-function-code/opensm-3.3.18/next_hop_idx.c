static
bool next_hop_idx(struct torus *t, unsigned cdir,
		  struct t_switch *ssw, struct t_switch *dsw,
		  unsigned *pt_grp)
{
	int d;
	unsigned g;
	bool success = false;

	switch (cdir) {
	case 0:
		d = ring_dir_idx(ssw->i, dsw->i, t->x_sz);
		break;
	case 1:
		d = ring_dir_idx(ssw->j, dsw->j, t->y_sz);
		break;
	case 2:
		d = ring_dir_idx(ssw->k, dsw->k, t->z_sz);
		break;
	default:
		goto out;
	}

	cdir *= 2;
	if (d > 0)
		g = cdir + 1;
	else if (d < 0)
		g = cdir;
	else
		goto out;

	if (!ssw->ptgrp[g].port_cnt)
		goto out;

	*pt_grp = g;
	success = true;
out:
	return success;
}