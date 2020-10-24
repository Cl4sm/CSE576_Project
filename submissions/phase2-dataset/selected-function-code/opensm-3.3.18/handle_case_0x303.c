static
bool handle_case_0x303(struct torus *t, int i, int j, int k)
{
	int ip1 = canonicalize(i + 1, t->x_sz);
	int jp1 = canonicalize(j + 1, t->y_sz);
	int jp2 = canonicalize(j + 2, t->y_sz);

	if (safe_y_perpendicular(t, i, jp1, k) &&
	    install_tswitch(t, i, j, k,
			    tfind_2d_perpendicular(t->sw[ip1][jp1][k],
						   t->sw[i][jp1][k],
						   t->sw[i][jp2][k]))) {
		return true;
	}
	log_no_perp(t, 0x303, i, j, k, i, jp1, k);

	if (safe_y_perpendicular(t, ip1, jp1, k) &&
	    install_tswitch(t, ip1, j, k,
			    tfind_2d_perpendicular(t->sw[i][jp1][k],
						   t->sw[ip1][jp1][k],
						   t->sw[ip1][jp2][k]))) {
		return true;
	}
	log_no_perp(t, 0x303, i, j, k, ip1, jp1, k);
	return false;
}