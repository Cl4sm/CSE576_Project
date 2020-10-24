static
void build_master_stree_branch(struct t_switch *branch_root, int cdir)
{
	struct t_switch *sw, *n_sw, *p_sw;
	unsigned l, idx, cnt, pg, ng;

	switch (cdir) {
	case 0:
		idx = branch_root->i;
		cnt = branch_root->torus->x_sz;
		break;
	case 1:
		idx = branch_root->j;
		cnt = branch_root->torus->y_sz;
		break;
	case 2:
		idx = branch_root->k;
		cnt = branch_root->torus->z_sz;
		break;
	default:
		goto out;
	}
	/*
	 * This algorithm intends that a spanning tree branch never crosses
	 * a dateline unless the 1-D ring for which we're building the branch
	 * is interrupted by failure.  We need that guarantee to prevent
	 * multicast/unicast credit loops.
	 */
	n_sw = branch_root;		/* tip of negative cdir branch */
	ng = 2 * cdir;			/* negative cdir port group index */
	p_sw = branch_root;		/* tip of positive cdir branch */
	pg = 2 * cdir + 1;		/* positive cdir port group index */

	for (l = idx; n_sw && l >= 1; l--) {
		sw = ring_next_sw(n_sw, cdir, -1);
		if (sw && !sw_in_master_stree(sw)) {
			grow_master_stree_branch(n_sw, sw, pg, ng);
			n_sw = sw;
		} else
			n_sw = NULL;
	}
	for (l = idx; p_sw && l < (cnt - 1); l++) {
		sw = ring_next_sw(p_sw, cdir, 1);
		if (sw && !sw_in_master_stree(sw)) {
			grow_master_stree_branch(p_sw, sw, ng, pg);
			p_sw = sw;
		} else
			p_sw = NULL;
	}
	if (n_sw && p_sw)
		goto out;
	/*
	 * At least one branch couldn't grow to the dateline for this ring.
	 * That means it is acceptable to grow the branch by crossing the
	 * dateline.
	 */
	for (l = 0; l < cnt; l++) {
		if (n_sw) {
			sw = ring_next_sw(n_sw, cdir, -1);
			if (sw && !sw_in_master_stree(sw)) {
				grow_master_stree_branch(n_sw, sw, pg, ng);
				n_sw = sw;
			} else
				n_sw = NULL;
		}
		if (p_sw) {
			sw = ring_next_sw(p_sw, cdir, 1);
			if (sw && !sw_in_master_stree(sw)) {
				grow_master_stree_branch(p_sw, sw, ng, pg);
				p_sw = sw;
			} else
				p_sw = NULL;
		}
		if (!(n_sw || p_sw))
			break;
	}
out:
	return;
}