static
bool handle_case_0x511(struct torus *t, int i, int j, int k)
{
	int ip1 = canonicalize(i + 1, t->x_sz);
	int ip2 = canonicalize(i + 2, t->x_sz);
	int kp1 = canonicalize(k + 1, t->z_sz);

	if (safe_x_perpendicular(t, ip1, j, k) &&
	    install_tswitch(t, i, j, k,
			    tfind_2d_perpendicular(t->sw[ip1][j][kp1],
						   t->sw[ip1][j][k],
						   t->sw[ip2][j][k]))) {
		return true;
	}
	log_no_perp(t, 0x511, i, j, k, ip1, j, k);

	if (safe_x_perpendicular(t, ip1, j, kp1) &&
	    install_tswitch(t, i, j, kp1,
			    tfind_2d_perpendicular(t->sw[ip1][j][k],
						   t->sw[ip1][j][kp1],
						   t->sw[ip2][j][kp1]))) {
		return true;
	}
	log_no_perp(t, 0x511, i, j, k, ip1, j, kp1);
	return false;
}