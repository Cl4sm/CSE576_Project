osm_prtn_t *osm_prtn_make_new(osm_log_t * p_log, osm_subn_t * p_subn,
			      const char *name, uint16_t pkey)
{
	osm_prtn_t *p = NULL, *p_check;

	pkey &= cl_hton16((uint16_t) ~ 0x8000);
	if (!pkey) {
		if (name && (p = osm_prtn_find_by_name(p_subn, name)))
			return p;
		if (!(pkey = generate_pkey(p_subn)))
			return NULL;
	}

	p = osm_prtn_new(name, pkey);
	if (!p) {
		OSM_LOG(p_log, OSM_LOG_ERROR, "Unable to create"
			" partition \'%s\' (0x%04x)\n", name, cl_ntoh16(pkey));
		return NULL;
	}

	p_check = (osm_prtn_t *) cl_qmap_insert(&p_subn->prtn_pkey_tbl,
						p->pkey, &p->map_item);
	if (p != p_check) {
		OSM_LOG(p_log, OSM_LOG_VERBOSE, "Duplicated partition"
			" definition: \'%s\' (0x%04x) prev name \'%s\'"
			".  Will use it\n",
			name, cl_ntoh16(pkey), p_check->name);
		osm_prtn_delete(p_subn, &p);
		p = p_check;
	}

	return p;
}