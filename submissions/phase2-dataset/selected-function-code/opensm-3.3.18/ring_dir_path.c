static
int ring_dir_path(struct torus *t, unsigned cdir,
		  struct t_switch *ssw, struct t_switch *dsw)
{
	int d = 0;
	struct t_switch *sw;

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
		break;
	}
	if (!d)
		goto out;

	sw = ssw;
	while (sw) {
		sw = ring_next_sw(sw, cdir, d);
		if (sw == dsw)
			goto out;
	}
	d *= -1;
	sw = ssw;
	while (sw) {
		sw = ring_next_sw(sw, cdir, d);
		if (sw == dsw)
			goto out;
	}
	d = 0;
out:
	return d;
}