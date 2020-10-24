static
bool handle_case_0x30c(struct torus *t, int i, int j, int k)
{
	int ip1 = canonicalize(i + 1, t->x_sz);
	int jm1 = canonicalize(j - 1, t->y_sz);
	int jp1 = canonicalize(j + 1, t->y_sz);

	if (safe_y_perpendicular(t, i, j, k) &&
	    install_tswitch(t, i, jp1, k,
			    tfind_2d_perpendicular(t->sw[ip1][j][k],
						   t->sw[i][j][k],
						   t->sw[i][jm1][k]))) {
		return true;
	}
	log_no_perp(t, 0x30c, i, j, k, i, j, k);

	if (safe_y_perpendicular(t, ip1, j, k) &&
	    install_tswitch(t, ip1, jp1, k,
			    tfind_2d_perpendicular(t->sw[i][j][k],
						   t->sw[ip1][j][k],
						   t->sw[ip1][jm1][k]))) {
		return true;
	}
	log_no_perp(t, 0x30c, i, j, k, ip1, j, k);
	return false;
}