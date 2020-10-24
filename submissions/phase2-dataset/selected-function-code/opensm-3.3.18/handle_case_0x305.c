static
bool handle_case_0x305(struct torus *t, int i, int j, int k)
{
	int ip1 = canonicalize(i + 1, t->x_sz);
	int ip2 = canonicalize(i + 2, t->x_sz);
	int jp1 = canonicalize(j + 1, t->y_sz);

	if (safe_x_perpendicular(t, ip1, j, k) &&
	    install_tswitch(t, i, j, k,
			    tfind_2d_perpendicular(t->sw[ip1][jp1][k],
						   t->sw[ip1][j][k],
						   t->sw[ip2][j][k]))) {
		return true;
	}
	log_no_perp(t, 0x305, i, j, k, ip1, j, k);

	if (safe_x_perpendicular(t, ip1, jp1, k) &&
	    install_tswitch(t, i, jp1, k,
			    tfind_2d_perpendicular(t->sw[ip1][j][k],
						   t->sw[ip1][jp1][k],
						   t->sw[ip2][jp1][k]))) {
		return true;
	}
	log_no_perp(t, 0x305, i, j, k, ip1, jp1, k);
	return false;
}