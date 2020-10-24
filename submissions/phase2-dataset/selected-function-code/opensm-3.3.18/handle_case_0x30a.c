static
bool handle_case_0x30a(struct torus *t, int i, int j, int k)
{
	int im1 = canonicalize(i - 1, t->x_sz);
	int ip1 = canonicalize(i + 1, t->x_sz);
	int jp1 = canonicalize(j + 1, t->y_sz);

	if (safe_x_perpendicular(t, i, j, k) &&
	    install_tswitch(t, ip1, j, k,
			    tfind_2d_perpendicular(t->sw[i][jp1][k],
						   t->sw[i][j][k],
						   t->sw[im1][j][k]))) {
		return true;
	}
	log_no_perp(t, 0x30a, i, j, k, i, j, k);

	if (safe_x_perpendicular(t, i, jp1, k) &&
	    install_tswitch(t, ip1, jp1, k,
			    tfind_2d_perpendicular(t->sw[i][j][k],
						   t->sw[i][jp1][k],
						   t->sw[im1][jp1][k]))) {
		return true;
	}
	log_no_perp(t, 0x30a, i, j, k, i, jp1, k);
	return false;
}