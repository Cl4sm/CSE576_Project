static ib_api_status_t
osmtest_validate_node_data(IN osmtest_t * const p_osmt,
			   IN node_t * const p_node,
			   IN const ib_node_record_t * const p_rec)
{
	cl_status_t status = IB_SUCCESS;

	OSM_LOG_ENTER(&p_osmt->log);

	OSM_LOG(&p_osmt->log, OSM_LOG_ERROR,
		"Checking node 0x%016" PRIx64 ", LID 0x%X\n",
		cl_ntoh64(p_rec->node_info.node_guid), cl_ntoh16(p_rec->lid));

	/*
	 * Has this record already been returned?
	 */
	if (p_node->count != 0) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0013: "
			"Already received node 0x%016" PRIx64 "\n",
			cl_ntoh64(p_node->rec.node_info.node_guid));
		status = IB_ERROR;
		goto Exit;
	}

	++p_node->count;

	/*
	 * Compare the fields the user wishes to validate.
	 */
	if ((p_node->comp.lid & p_node->rec.lid) !=
	    (p_node->comp.lid & p_rec->lid)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0014: "
			"Field mismatch node 0x%016" PRIx64 ", LID 0x%X\n"
			"\t\t\t\tExpected LID 0x%X, received 0x%X\n",
			cl_ntoh64(p_rec->node_info.node_guid),
			cl_ntoh16(p_rec->lid), p_node->rec.lid, p_rec->lid);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_node->comp.node_info.base_version &
	     p_node->rec.node_info.base_version) !=
	    (p_node->comp.node_info.base_version &
	     p_rec->node_info.base_version)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0015: "
			"Field mismatch node 0x%016" PRIx64 ", LID 0x%X\n"
			"\t\t\t\tExpected base_version 0x%X, received 0x%X\n",
			cl_ntoh64(p_rec->node_info.node_guid),
			cl_ntoh16(p_rec->lid),
			p_node->rec.node_info.base_version,
			p_rec->node_info.base_version);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_node->comp.node_info.class_version &
	     p_node->rec.node_info.class_version) !=
	    (p_node->comp.node_info.class_version &
	     p_rec->node_info.class_version)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0016: "
			"Field mismatch node 0x%016" PRIx64 ", LID 0x%X\n"
			"\t\t\t\tExpected class_version 0x%X, received 0x%X\n",
			cl_ntoh64(p_rec->node_info.node_guid),
			cl_ntoh16(p_rec->lid),
			p_node->rec.node_info.class_version,
			p_rec->node_info.class_version);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_node->comp.node_info.node_type &
	     p_node->rec.node_info.node_type) !=
	    (p_node->comp.node_info.node_type & p_rec->node_info.node_type)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0017: "
			"Field mismatch node 0x%016" PRIx64 ", LID 0x%X\n"
			"\t\t\t\tExpected node_type 0x%X, received 0x%X\n",
			cl_ntoh64(p_rec->node_info.node_guid),
			cl_ntoh16(p_rec->lid),
			p_node->rec.node_info.node_type,
			p_rec->node_info.node_type);
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_node->comp.node_info.sys_guid &
	     p_node->rec.node_info.sys_guid) !=
	    (p_node->comp.node_info.sys_guid & p_rec->node_info.sys_guid)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0018: "
			"Field mismatch node 0x%016" PRIx64 ", LID 0x%X\n"
			"\t\t\t\tExpected sys_guid 0x%016" PRIx64
			", received 0x%016" PRIx64 "\n",
			cl_ntoh64(p_rec->node_info.node_guid),
			cl_ntoh16(p_rec->lid),
			cl_ntoh64(p_node->rec.node_info.sys_guid),
			cl_ntoh64(p_rec->node_info.sys_guid));
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_node->comp.node_info.node_guid &
	     p_node->rec.node_info.node_guid) !=
	    (p_node->comp.node_info.node_guid & p_rec->node_info.node_guid)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0019: "
			"Field mismatch node 0x%016" PRIx64 ", LID 0x%X\n"
			"\t\t\t\tExpected node_guid 0x%016" PRIx64
			", received 0x%016" PRIx64 "\n",
			cl_ntoh64(p_rec->node_info.node_guid),
			cl_ntoh16(p_rec->lid),
			cl_ntoh64(p_node->rec.node_info.node_guid),
			cl_ntoh64(p_rec->node_info.node_guid));
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_node->comp.node_info.port_guid &
	     p_node->rec.node_info.port_guid) !=
	    (p_node->comp.node_info.port_guid & p_rec->node_info.port_guid)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0031: "
			"Field mismatch node 0x%016" PRIx64 ", LID 0x%X\n"
			"\t\t\t\tExpected port_guid 0x%016" PRIx64
			", received 0x%016" PRIx64 "\n",
			cl_ntoh64(p_rec->node_info.node_guid),
			cl_ntoh16(p_rec->lid),
			cl_ntoh64(p_node->rec.node_info.port_guid),
			cl_ntoh64(p_rec->node_info.port_guid));
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_node->comp.node_info.partition_cap &
	     p_node->rec.node_info.partition_cap) !=
	    (p_node->comp.node_info.partition_cap &
	     p_rec->node_info.partition_cap)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0032: "
			"Field mismatch node 0x%016" PRIx64 ", LID 0x%X\n"
			"\t\t\t\tExpected partition_cap 0x%X, received 0x%X\n",
			cl_ntoh64(p_rec->node_info.node_guid),
			cl_ntoh16(p_rec->lid),
			cl_ntoh16(p_node->rec.node_info.partition_cap),
			cl_ntoh16(p_rec->node_info.partition_cap));
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_node->comp.node_info.device_id &
	     p_node->rec.node_info.device_id) !=
	    (p_node->comp.node_info.device_id & p_rec->node_info.device_id)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0033: "
			"Field mismatch node 0x%016" PRIx64 ", LID 0x%X\n"
			"\t\t\t\tExpected device_id 0x%X, received 0x%X\n",
			cl_ntoh64(p_rec->node_info.node_guid),
			cl_ntoh16(p_rec->lid),
			cl_ntoh16(p_node->rec.node_info.device_id),
			cl_ntoh16(p_rec->node_info.device_id));
		status = IB_ERROR;
		goto Exit;
	}

	if ((p_node->comp.node_info.revision &
	     p_node->rec.node_info.revision) !=
	    (p_node->comp.node_info.revision & p_rec->node_info.revision)) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0034: "
			"Field mismatch node 0x%016" PRIx64 ", LID 0x%X\n"
			"\t\t\t\tExpected revision 0x%X, received 0x%X\n",
			cl_ntoh64(p_rec->node_info.node_guid),
			cl_ntoh16(p_rec->lid),
			cl_ntoh32(p_node->rec.node_info.revision),
			cl_ntoh32(p_rec->node_info.revision));
		status = IB_ERROR;
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}