static
bool handle_case_0x530(struct torus *t, int i, int j, int k)
{
	int ip1 = canonicalize(i + 1, t->x_sz);
	int km1 = canonicalize(k - 1, t->z_sz);
	int kp1 = canonicalize(k + 1, t->z_sz);

	if (safe_z_perpendicular(t, i, j, k) &&
	    install_tswitch(t, i, j, kp1,
			    tfind_2d_perpendicular(t->sw[ip1][j][k],
						   t->sw[i][j][k],
						   t->sw[i][j][km1]))) {
		return true;
	}
	log_no_perp(t, 0x530, i, j, k, i, j, k);

	if (safe_z_perpendicular(t, ip1, j, k) &&
	      install_tswitch(t, ip1, j, kp1,
			      tfind_2d_perpendicular(t->sw[i][j][k],
						     t->sw[ip1][j][k],
						     t->sw[ip1][j][km1]))) {
		return true;
	}
	log_no_perp(t, 0x530, i, j, k, ip1, j, k);
	return false;
}