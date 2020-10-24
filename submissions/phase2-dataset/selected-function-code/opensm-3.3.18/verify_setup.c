static
bool verify_setup(struct torus *t, struct fabric *f)
{
	struct coord_dirs *o;
	struct f_switch *sw;
	unsigned p, s, n = 0;
	bool success = false;
	bool all_sw_present, need_seed = true;

	if (!(t->x_sz && t->y_sz && t->z_sz)) {
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E20: missing required torus size specification!\n");
		goto out;
	}
	if (t->osm->subn.min_sw_data_vls < 2) {
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E48: Too few data VLs to support torus routing "
			"without credit loops (have switchport %d need 2)\n",
			(int)t->osm->subn.min_sw_data_vls);
		goto out;
	}
	if (t->osm->subn.min_sw_data_vls < 4)
		OSM_LOG(&t->osm->log, OSM_LOG_INFO,
			"Warning: Too few data VLs to support torus routing "
			"with a failed switch without credit loops "
			"(have switchport %d need 4)\n",
			(int)t->osm->subn.min_sw_data_vls);
	if (t->osm->subn.min_sw_data_vls < 8)
		OSM_LOG(&t->osm->log, OSM_LOG_INFO,
			"Warning: Too few data VLs to support torus routing "
			"with two QoS levels (have switchport %d need 8)\n",
			(int)t->osm->subn.min_sw_data_vls);
	if (t->osm->subn.min_data_vls < 2)
		OSM_LOG(&t->osm->log, OSM_LOG_INFO,
			"Warning: Too few data VLs to support torus routing "
			"with two QoS levels (have endport %d need 2)\n",
			(int)t->osm->subn.min_data_vls);
	/*
	 * Be sure all the switches in the torus support the port
	 * ordering that might have been configured.
	 */
	for (s = 0; s < f->switch_cnt; s++) {
		sw = f->sw[s];
		for (p = 0; p < sw->port_cnt; p++) {
			if (t->port_order[p] >= sw->port_cnt) {
				OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
					"ERR 4E21: port_order configured using "
					"port %u, but only %u ports in "
					"switch w/ GUID 0x%04"PRIx64"\n",
					t->port_order[p], sw->port_cnt - 1,
					cl_ntoh64(sw->n_id));
				goto out;
			}
		}
	}
	/*
	 * Unfortunately, there is a problem with non-unique topology for any
	 * torus dimension which has radix four.  This problem requires extra
	 * input, in the form of specifying both the positive and negative
	 * coordinate directions from a common switch, for any torus dimension
	 * with radix four (see also build_torus()).
	 *
	 * Do the checking required to ensure that the required information
	 * is present, but more than the needed information is not required.
	 *
	 * So, verify that we learned the coordinate directions correctly for
	 * the fabric.  The coordinate direction links get an invalid port
	 * set on their ends when parsed.
	 */
again:
	all_sw_present = true;
	o = &t->seed[n];

	if (t->x_sz == 4 && !(t->flags & X_MESH)) {
		if (o->xp_link.end[0].port >= 0) {
			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E22: Positive x" LINK_ERR_STR);
			goto out;
		}
		if (o->xm_link.end[0].port >= 0) {
			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E23: Negative x" LINK_ERR_STR);
			goto out;
		}
		if (o->xp_link.end[0].n_id != o->xm_link.end[0].n_id) {
			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E24: Positive/negative x" SEED_ERR_STR);
			goto out;
		}
	}
	if (t->y_sz == 4 && !(t->flags & Y_MESH)) {
		if (o->yp_link.end[0].port >= 0) {
			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E25: Positive y" LINK_ERR_STR);
			goto out;
		}
		if (o->ym_link.end[0].port >= 0) {
			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E26: Negative y" LINK_ERR_STR);
			goto out;
		}
		if (o->yp_link.end[0].n_id != o->ym_link.end[0].n_id) {
			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E27: Positive/negative y" SEED_ERR_STR);
			goto out;
		}
	}
	if (t->z_sz == 4 && !(t->flags & Z_MESH)) {
		if (o->zp_link.end[0].port >= 0) {
			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E28: Positive z" LINK_ERR_STR);
			goto out;
		}
		if (o->zm_link.end[0].port >= 0) {
			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E29: Negative z" LINK_ERR_STR);
			goto out;
		}
		if (o->zp_link.end[0].n_id != o->zm_link.end[0].n_id) {
			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E2A: Positive/negative z" SEED_ERR_STR);
			goto out;
		}
	}
	if (t->x_sz > 1) {
		if (o->xp_link.end[0].port >= 0 &&
		    o->xm_link.end[0].port >= 0) {
			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E2B: Positive or negative x" LINK_ERR2_STR);
			goto out;
		}
		if (o->xp_link.end[0].port < 0 &&
		    !find_f_sw(f, o->xp_link.end[0].n_id))
			all_sw_present = false;

		if (o->xp_link.end[1].port < 0 &&
		    !find_f_sw(f, o->xp_link.end[1].n_id))
			all_sw_present = false;

		if (o->xm_link.end[0].port < 0 &&
		    !find_f_sw(f, o->xm_link.end[0].n_id))
			all_sw_present = false;

		if (o->xm_link.end[1].port < 0 &&
		    !find_f_sw(f, o->xm_link.end[1].n_id))
			all_sw_present = false;
	}
	if (t->z_sz > 1) {
		if (o->zp_link.end[0].port >= 0 &&
		    o->zm_link.end[0].port >= 0) {
			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E2C: Positive or negative z" LINK_ERR2_STR);
			goto out;
		}
		if ((o->xp_link.end[0].port < 0 &&
		     o->zp_link.end[0].port < 0 &&
		     o->zp_link.end[0].n_id != o->xp_link.end[0].n_id) ||

		    (o->xp_link.end[0].port < 0 &&
		     o->zm_link.end[0].port < 0 &&
		     o->zm_link.end[0].n_id != o->xp_link.end[0].n_id) ||

		    (o->xm_link.end[0].port < 0 &&
		     o->zp_link.end[0].port < 0 &&
		     o->zp_link.end[0].n_id != o->xm_link.end[0].n_id) ||

		    (o->xm_link.end[0].port < 0 &&
		     o->zm_link.end[0].port < 0 &&
		     o->zm_link.end[0].n_id != o->xm_link.end[0].n_id)) {

			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E2D: x and z" SEED_ERR_STR);
			goto out;
		}
		if (o->zp_link.end[0].port < 0 &&
		    !find_f_sw(f, o->zp_link.end[0].n_id))
			all_sw_present = false;

		if (o->zp_link.end[1].port < 0 &&
		    !find_f_sw(f, o->zp_link.end[1].n_id))
			all_sw_present = false;

		if (o->zm_link.end[0].port < 0 &&
		    !find_f_sw(f, o->zm_link.end[0].n_id))
			all_sw_present = false;

		if (o->zm_link.end[1].port < 0 &&
		    !find_f_sw(f, o->zm_link.end[1].n_id))
			all_sw_present = false;
	}
	if (t->y_sz > 1) {
		if (o->yp_link.end[0].port >= 0 &&
		    o->ym_link.end[0].port >= 0) {
			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E2E: Positive or negative y" LINK_ERR2_STR);
			goto out;
		}
		if ((o->xp_link.end[0].port < 0 &&
		     o->yp_link.end[0].port < 0 &&
		     o->yp_link.end[0].n_id != o->xp_link.end[0].n_id) ||

		    (o->xp_link.end[0].port < 0 &&
		     o->ym_link.end[0].port < 0 &&
		     o->ym_link.end[0].n_id != o->xp_link.end[0].n_id) ||

		    (o->xm_link.end[0].port < 0 &&
		     o->yp_link.end[0].port < 0 &&
		     o->yp_link.end[0].n_id != o->xm_link.end[0].n_id) ||

		    (o->xm_link.end[0].port < 0 &&
		     o->ym_link.end[0].port < 0 &&
		     o->ym_link.end[0].n_id != o->xm_link.end[0].n_id)) {

			OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
				"ERR 4E2F: x and y" SEED_ERR_STR);
			goto out;
		}
		if (o->yp_link.end[0].port < 0 &&
		    !find_f_sw(f, o->yp_link.end[0].n_id))
			all_sw_present = false;

		if (o->yp_link.end[1].port < 0 &&
		    !find_f_sw(f, o->yp_link.end[1].n_id))
			all_sw_present = false;

		if (o->ym_link.end[0].port < 0 &&
		    !find_f_sw(f, o->ym_link.end[0].n_id))
			all_sw_present = false;

		if (o->ym_link.end[1].port < 0 &&
		    !find_f_sw(f, o->ym_link.end[1].n_id))
			all_sw_present = false;
	}
	if (all_sw_present && need_seed) {
		t->seed_idx = n;
		need_seed = false;
	}
	if (++n < t->seed_cnt)
		goto again;

	if (need_seed)
		OSM_LOG(&t->osm->log, OSM_LOG_ERROR,
			"ERR 4E30: Every configured torus seed has at "
			"least one switch missing in fabric! See "
			"torus-2QoS.conf(5) and TORUS TOPOLOGY DISCOVERY "
			"in torus-2QoS(8)\n");
	else
		success = true;
out:
	return success;
}