static
bool build_link(struct fabric *f,
		guid_t sw_guid0, int sw_port0, guid_t sw_guid1, int sw_port1)
{
	struct link *l;
	struct f_switch *sw0, *sw1;
	bool success = false;

	/*
	 * The link may already exist.
	 */
	l = find_f_link(f, sw_guid0, sw_port0, sw_guid1, sw_port1);
	if (l) {
		success = true;
		goto out;
	}
	/*
	 * The switches must already exist.
	 */
	sw0 = find_f_sw(f, sw_guid0);
	if (!sw0) {
		OSM_LOG(&f->osm->log, OSM_LOG_ERROR,
			"ERR 4E0A: missing switch w/GUID 0x%04"PRIx64"\n",
			cl_ntoh64(sw_guid0));
		goto out;
	}
	sw1 = find_f_sw(f, sw_guid1);
	if (!sw1) {
		OSM_LOG(&f->osm->log, OSM_LOG_ERROR,
			"ERR 4E0B: missing switch w/GUID 0x%04"PRIx64"\n",
			cl_ntoh64(sw_guid1));
		goto out;
	}
	l = alloc_flink(f);
	if (!l)
		goto out;

	l->end[0].type = PASSTHRU;
	l->end[0].port = sw_port0;
	l->end[0].n_id = sw_guid0;
	l->end[0].sw = sw0;
	l->end[0].link = l;

	sw0->port[sw_port0] = &l->end[0];

	l->end[1].type = PASSTHRU;
	l->end[1].port = sw_port1;
	l->end[1].n_id = sw_guid1;
	l->end[1].sw = sw1;
	l->end[1].link = l;

	sw1->port[sw_port1] = &l->end[1];

	success = true;
out:
	return success;
}