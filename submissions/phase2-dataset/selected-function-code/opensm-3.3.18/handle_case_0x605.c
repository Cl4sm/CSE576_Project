static
bool handle_case_0x605(struct torus *t, int i, int j, int k)
{
	int jp1 = canonicalize(j + 1, t->y_sz);
	int kp1 = canonicalize(k + 1, t->z_sz);
	int kp2 = canonicalize(k + 2, t->z_sz);

	if (safe_z_perpendicular(t, i, j, kp1) &&
	    install_tswitch(t, i, j, k,
			    tfind_2d_perpendicular(t->sw[i][jp1][kp1],
						   t->sw[i][j][kp1],
						   t->sw[i][j][kp2]))) {
		return true;
	}
	log_no_perp(t, 0x605, i, j, k, i, j, kp1);

	if (safe_z_perpendicular(t, i, jp1, kp1) &&
	    install_tswitch(t, i, jp1, k,
			    tfind_2d_perpendicular(t->sw[i][j][kp1],
						   t->sw[i][jp1][kp1],
						   t->sw[i][jp1][kp2]))) {
		return true;
	}
	log_no_perp(t, 0x605, i, j, k, i, jp1, kp1);
	return false;
}