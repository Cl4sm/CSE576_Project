static ib_api_status_t
osmtest_validate_path_data(IN osmtest_t * const p_osmt,
			   IN path_t * const p_path,
			   IN const ib_path_rec_t * const p_rec)
{
	cl_status_t status = IB_SUCCESS;
	uint8_t lmc = 0;

	OSM_LOG_ENTER(&p_osmt->log);

	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
		"Checking path SLID 0x%X to DLID 0x%X\n",
		cl_ntoh16(p_rec->slid), cl_ntoh16(p_rec->dlid));

	status =
	    osmtest_get_local_port_lmc(p_osmt, p_osmt->local_port.lid, &lmc);
	if (status != IB_SUCCESS)
		goto Exit;

	/* HACK: Assume uniform LMC across endports in the subnet */
	/* This is the only LMC mode which OpenSM currently supports */
	/* In absence of this assumption, validation of this is much more complicated */
	if (lmc == 0) {
		/*
		 * Has this record already been returned?
		 */
		if (p_path->count != 0) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0056: "
				"Already received path SLID 0x%X to DLID 0x%X\n",
				cl_ntoh16(p_rec->slid), cl_ntoh16(p_rec->dlid));
			status = IB_ERROR;
			goto Exit;
		}
	} else {
		/* Also, this doesn't detect fewer than the correct number of paths being returned */
		if (p_path->count >= (uint32_t) (1 << (2 * lmc))) {
			OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0052: "
				"Already received path SLID 0x%X to DLID 0x%X count %d LMC %d\n",
				cl_ntoh16(p_rec->slid), cl_ntoh16(p_rec->dlid),
				p_path->count, lmc);
			status = IB_ERROR;
			goto Exit;
		}
	}

	++p_path->count;

	/*
	 * Check the fields the user wants checked.
	 */
	if ((p_path->comp.dgid.unicast.interface_id &
	     p_path->rec.dgid.unicast.interface_id) !=
	    (p_path->comp.dgid.unicast.interface_id &
	     p_rec->dgid.unicast.interface_id)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0169: "
			"DGID mismatch on path SLID 0x%X to DLID 0x%X\n"
			"\t\t\t\tExpected 0x%016" PRIx64 " 0x%016" PRIx64 "\n"
			"\t\t\t\tReceived 0x%016" PRIx64 " 0x%016" PRIx64 "\n",
			cl_ntoh16(p_path->rec.slid),
			cl_ntoh16(p_path->rec.dlid),
			cl_ntoh64(p_path->rec.dgid.unicast.prefix),
			cl_ntoh64(p_path->rec.dgid.unicast.interface_id),
			cl_ntoh64(p_rec->dgid.unicast.prefix),
			cl_ntoh64(p_rec->dgid.unicast.interface_id));
		status = IB_ERROR;
		goto Exit;
	}

	/*
	 * Check the fields the user wants checked.
	 */
	if ((p_path->comp.sgid.unicast.interface_id &
	     p_path->rec.sgid.unicast.interface_id) !=
	    (p_path->comp.sgid.unicast.interface_id &
	     p_rec->sgid.unicast.interface_id)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0057: "
			"SGID mismatch on path SLID 0x%X to DLID 0x%X\n"
			"\t\t\t\tExpected 0x%016" PRIx64 " 0x%016" PRIx64 ",\n"
			"\t\t\t\tReceived 0x%016" PRIx64 " 0x%016" PRIx64 ".\n",
			cl_ntoh16(p_path->rec.slid),
			cl_ntoh16(p_path->rec.dlid),
			cl_ntoh64(p_path->rec.sgid.unicast.prefix),
			cl_ntoh64(p_path->rec.sgid.unicast.interface_id),
			cl_ntoh64(p_rec->sgid.unicast.prefix),
			cl_ntoh64(p_rec->sgid.unicast.interface_id));
		status = IB_ERROR;
		goto Exit;
	}

	/*
	 * Compare the fields the user wishes to validate.
	 */
	if ((p_path->comp.pkey & p_path->rec.pkey) !=
	    (p_path->comp.pkey & p_rec->pkey)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0012: "
			"PKEY mismatch on path SLID 0x%X to DLID 0x%X\n"
			"\t\t\t\tExpected 0x%X, received 0x%X\n",
			cl_ntoh16(p_path->rec.slid),
			cl_ntoh16(p_path->rec.dlid),
			cl_ntoh16(p_path->rec.pkey), cl_ntoh16(p_rec->pkey));
		status = IB_ERROR;
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}