static void subn_validate_neighbor(osm_subn_t *p_subn)
{
	cl_qlist_t entries;
	osm_db_neighbor_elem_t *p_item;
	boolean_t valid_entry;
	uint64_t guid;
	uint8_t port;

	OSM_LOG_ENTER(&(p_subn->p_osm->log));
	cl_qlist_init(&entries);

	if (osm_db_neighbor_guids(p_subn->p_neighbor, &entries)) {
		OSM_LOG(&(p_subn->p_osm->log), OSM_LOG_ERROR, "ERR 7512: "
			"could not get neighbor entry list\n");
		goto Exit;
	}

	while ((p_item =
		(osm_db_neighbor_elem_t *) cl_qlist_remove_head(&entries))
	       != (osm_db_neighbor_elem_t *) cl_qlist_end(&entries)) {
		valid_entry = TRUE;

		OSM_LOG(&(p_subn->p_osm->log), OSM_LOG_DEBUG,
			"Validating neighbor for guid:0x%016" PRIx64
			", port %d\n",
			p_item->guid, p_item->portnum);
		if (p_item->guid == 0) {
			OSM_LOG(&(p_subn->p_osm->log), OSM_LOG_ERROR,
				"ERR 7513: found invalid zero guid\n");
			valid_entry = FALSE;
		} else if (p_item->portnum == 0) {
			OSM_LOG(&(p_subn->p_osm->log), OSM_LOG_ERROR,
				"ERR 7514: found invalid zero port for "
				"guid: 0x%016" PRIx64 "\n",
				p_item->guid);
			valid_entry = FALSE;
		} else if (osm_db_neighbor_get(p_subn->p_neighbor,
					       p_item->guid, p_item->portnum,
					       &guid, &port)) {
			OSM_LOG(&(p_subn->p_osm->log), OSM_LOG_ERROR,
				"ERR 7515: could not find neighbor for "
				"guid: 0x%016" PRIx64 ", port %d\n",
				p_item->guid, p_item->portnum);
			valid_entry = FALSE;
		} else if (guid == 0) {
			OSM_LOG(&(p_subn->p_osm->log), OSM_LOG_ERROR,
				"ERR 7516: found invalid neighbor "
				"zero guid for guid: 0x%016" PRIx64
				", port %d\n",
				p_item->guid, p_item->portnum);
			valid_entry = FALSE;
		} else if (port == 0) {
			OSM_LOG(&(p_subn->p_osm->log), OSM_LOG_ERROR,
				"ERR 7517: found invalid neighbor "
				"zero port for guid: 0x%016" PRIx64
				", port %d\n",
				p_item->guid, p_item->portnum);
			valid_entry = FALSE;
		} else if (osm_db_neighbor_get(p_subn->p_neighbor,
					       guid, port, &guid, &port) ||
			   guid != p_item->guid || port != p_item->portnum) {
			OSM_LOG(&(p_subn->p_osm->log), OSM_LOG_ERROR,
				"ERR 7518: neighbor does not point "
				"back at us (guid: 0x%016" PRIx64
				", port %d)\n",
				p_item->guid, p_item->portnum);
			valid_entry = FALSE;
		}

		if (valid_entry == FALSE) {
			if (osm_db_neighbor_delete(p_subn->p_neighbor,
						   p_item->guid,
						   p_item->portnum))
				OSM_LOG(&(p_subn->p_osm->log), OSM_LOG_ERROR,
					"ERR 7519: failed to delete entry for "
					"guid:0x%016" PRIx64 " port:%u\n",
					p_item->guid, p_item->portnum);
		}
		free(p_item);
	}

Exit:
	OSM_LOG_EXIT(&(p_subn->p_osm->log));
}