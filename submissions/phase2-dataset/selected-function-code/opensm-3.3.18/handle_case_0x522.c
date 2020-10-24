static
bool handle_case_0x522(struct torus *t, int i, int j, int k)
{
	int im1 = canonicalize(i - 1, t->x_sz);
	int ip1 = canonicalize(i + 1, t->x_sz);
	int kp1 = canonicalize(k + 1, t->z_sz);

	if (safe_x_perpendicular(t, i, j, k) &&
	    install_tswitch(t, ip1, j, k,
			    tfind_2d_perpendicular(t->sw[i][j][kp1],
						   t->sw[i][j][k],
						   t->sw[im1][j][k]))) {
		return true;
	}
	log_no_perp(t, 0x522, i, j, k, i, j, k);

	if (safe_x_perpendicular(t, i, j, kp1) &&
	    install_tswitch(t, ip1, j, kp1,
			    tfind_2d_perpendicular(t->sw[i][j][k],
						   t->sw[i][j][kp1],
						   t->sw[im1][j][kp1]))) {
		return true;
	}
	log_no_perp(t, 0x522, i, j, k, i, j, kp1);
	return false;
}