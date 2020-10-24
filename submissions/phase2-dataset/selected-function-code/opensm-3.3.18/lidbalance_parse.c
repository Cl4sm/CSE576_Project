static void lidbalance_parse(char **p_last, osm_opensm_t * p_osm, FILE * out)
{
	char *p_cmd;
	uint64_t guid = 0;
	osm_switch_t *p_sw;

	p_cmd = next_token(p_last);
	if (p_cmd) {
		char *p_end;

		guid = strtoull(p_cmd, &p_end, 0);
		if (!guid || *p_end != '\0') {
			fprintf(out, "Invalid switchguid specified\n");
			help_lidbalance(out, 1);
			return;
		}
	}

	cl_plock_acquire(&p_osm->lock);
	if (guid) {
		p_sw = osm_get_switch_by_guid(&p_osm->subn, cl_hton64(guid));
		if (!p_sw) {
			fprintf(out, "switchguid not found\n");
			goto lock_exit;
		}
		lidbalance_check(p_osm, p_sw, out);
	} else {
		cl_qmap_t *p_sw_guid_tbl = &p_osm->subn.sw_guid_tbl;
		for (p_sw = (osm_switch_t *) cl_qmap_head(p_sw_guid_tbl);
		     p_sw != (osm_switch_t *) cl_qmap_end(p_sw_guid_tbl);
		     p_sw = (osm_switch_t *) cl_qmap_next(&p_sw->map_item))
			lidbalance_check(p_osm, p_sw, out);
	}

lock_exit:
	cl_plock_release(&p_osm->lock);
	return;
}