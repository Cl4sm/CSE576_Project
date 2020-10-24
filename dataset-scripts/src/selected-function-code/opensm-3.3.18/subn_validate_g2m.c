static void subn_validate_g2m(osm_subn_t *p_subn)
{
	cl_qlist_t guids;
	osm_db_guid_elem_t *p_item;
	uint64_t mkey;
	boolean_t valid_entry;

	OSM_LOG_ENTER(&(p_subn->p_osm->log));
	cl_qlist_init(&guids);

	if (osm_db_guid2mkey_guids(p_subn->p_g2m, &guids)) {
		OSM_LOG(&(p_subn->p_osm->log), OSM_LOG_ERROR, "ERR 7506: "
			"could not get mkey guid list\n");
		goto Exit;
	}

	while ((p_item = (osm_db_guid_elem_t *) cl_qlist_remove_head(&guids))
	       != (osm_db_guid_elem_t *) cl_qlist_end(&guids)) {
		valid_entry = TRUE;

		if (p_item->guid == 0) {
			OSM_LOG(&(p_subn->p_osm->log), OSM_LOG_ERROR,
				"ERR 7507: found invalid zero guid");
			valid_entry = FALSE;
		} else if (osm_db_guid2mkey_get(p_subn->p_g2m, p_item->guid,
						&mkey)) {
			OSM_LOG(&(p_subn->p_osm->log), OSM_LOG_ERROR,
				"ERR 7508: could not get mkey for guid:0x%016"
				PRIx64 "\n", p_item->guid);
			valid_entry = FALSE;
		}

		if (valid_entry == FALSE) {
			if (osm_db_guid2mkey_delete(p_subn->p_g2m,
						    p_item->guid))
				OSM_LOG(&(p_subn->p_osm->log), OSM_LOG_ERROR,
					"ERR 7509: failed to delete entry for "
					"guid:0x%016" PRIx64 "\n",
					p_item->guid);
		}
		free(p_item);
	}

Exit:
	OSM_LOG_EXIT(&(p_subn->p_osm->log));
}