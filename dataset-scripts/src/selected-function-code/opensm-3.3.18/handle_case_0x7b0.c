static
bool handle_case_0x7b0(struct torus *t, int i, int j, int k)
{
	int ip1 = canonicalize(i + 1, t->x_sz);
	int jp1 = canonicalize(j + 1, t->y_sz);
	int kp1 = canonicalize(k + 1, t->z_sz);

	if (install_tswitch(t, i, j, kp1,
			    tfind_face_corner(t->sw[i][j][k],
					      t->sw[i][jp1][k],
					      t->sw[i][jp1][kp1]))) {
		return true;
	}
	log_no_crnr(t, 0x7b0, i, j, k, i, j, kp1);

	if (install_tswitch(t, ip1, jp1, kp1,
			    tfind_face_corner(t->sw[i][jp1][kp1],
					      t->sw[i][jp1][k],
					      t->sw[ip1][jp1][k]))) {
		return true;
	}
	log_no_crnr(t, 0x7b0, i, j, k, ip1, jp1, kp1);
	return false;
}