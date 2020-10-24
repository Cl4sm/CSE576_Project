static
unsigned sl2vl_entry(struct torus *t, struct t_switch *sw,
		     int input_pt, int output_pt, unsigned sl)
{
	unsigned id, od, vl, data_vls;

	if (sw && sw->port[input_pt])
		id = sw->port[input_pt]->pgrp->port_grp / 2;
	else
		id = TORUS_MAX_DIM;

	if (sw && sw->port[output_pt])
		od = sw->port[output_pt]->pgrp->port_grp / 2;
	else
		od = TORUS_MAX_DIM;

	if (sw)
		data_vls = t->osm->subn.min_sw_data_vls;
	else
		data_vls = t->osm->subn.min_data_vls;

	vl = 0;
	if (sw && od != TORUS_MAX_DIM) {
		if (data_vls >= 2)
			vl |= vl_set_loop_vl(sl_get_use_loop_vl(sl, od));
		if (data_vls >= 4)
			vl |= vl_set_turn_vl(id, od);
		if (data_vls >= 8)
			vl |= vl_set_qos_vl(sl_get_qos(sl));
	} else {
		if (data_vls >= 2)
			vl |= vl_set_ca_qos_vl(sl_get_qos(sl));
	}
	return vl;
}