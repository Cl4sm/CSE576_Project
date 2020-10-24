static
bool handle_case_0x503(struct torus *t, int i, int j, int k)
{
	int ip1 = canonicalize(i + 1, t->x_sz);
	int kp1 = canonicalize(k + 1, t->z_sz);
	int kp2 = canonicalize(k + 2, t->z_sz);

	if (safe_z_perpendicular(t, i, j, kp1) &&
	    install_tswitch(t, i, j, k,
			    tfind_2d_perpendicular(t->sw[ip1][j][kp1],
						   t->sw[i][j][kp1],
						   t->sw[i][j][kp2]))) {
		return true;
	}
	log_no_perp(t, 0x503, i, j, k, i, j, kp1);

	if (safe_z_perpendicular(t, ip1, j, kp1) &&
	    install_tswitch(t, ip1, j, k,
			    tfind_2d_perpendicular(t->sw[i][j][kp1],
						   t->sw[ip1][j][kp1],
						   t->sw[ip1][j][kp2]))) {
		return true;
	}
	log_no_perp(t, 0x503, i, j, k, ip1, j, kp1);
	return false;
}