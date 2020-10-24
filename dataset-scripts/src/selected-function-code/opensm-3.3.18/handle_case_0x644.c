static
bool handle_case_0x644(struct torus *t, int i, int j, int k)
{
	int jm1 = canonicalize(j - 1, t->y_sz);
	int jp1 = canonicalize(j + 1, t->y_sz);
	int kp1 = canonicalize(k + 1, t->z_sz);

	if (safe_y_perpendicular(t, i, j, k) &&
	    install_tswitch(t, i, jp1, k,
			    tfind_2d_perpendicular(t->sw[i][j][kp1],
						   t->sw[i][j][k],
						   t->sw[i][jm1][k]))) {
		return true;
	}
	log_no_perp(t, 0x644, i, j, k, i, j, k);

	if (safe_y_perpendicular(t, i, j, kp1) &&
	    install_tswitch(t, i, jp1, kp1,
			    tfind_2d_perpendicular(t->sw[i][j][k],
						   t->sw[i][j][kp1],
						   t->sw[i][jm1][kp1]))) {
		return true;
	}
	log_no_perp(t, 0x644, i, j, k, i, j, kp1);
	return false;
}