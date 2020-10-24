static ib_api_status_t
osmtest_write_node_info(IN osmtest_t * const p_osmt,
			IN FILE * fh, IN const ib_node_record_t * const p_rec)
{
	int result;
	cl_status_t status = IB_SUCCESS;
	char desc[IB_NODE_DESCRIPTION_SIZE + 1];

	OSM_LOG_ENTER(&p_osmt->log);

	memcpy(desc, p_rec->node_desc.description, IB_NODE_DESCRIPTION_SIZE);
	desc[IB_NODE_DESCRIPTION_SIZE] = '\0';

	result = fprintf(fh,
			 "DEFINE_NODE\n"
			 "lid                     0x%X\n"
			 "base_version            0x%X\n"
			 "class_version           0x%X\n"
			 "node_type               0x%X # (%s)\n"
			 "num_ports               0x%X\n"
			 "sys_guid                0x%016" PRIx64 "\n"
			 "node_guid               0x%016" PRIx64 "\n"
			 "port_guid               0x%016" PRIx64 "\n"
			 "partition_cap           0x%X\n"
			 "device_id               0x%X\n"
			 "revision                0x%X\n"
			 "# port_num              0x%X\n"
			 "# vendor_id             0x%X\n"
			 "# node_desc             %s\n"
			 "END\n\n",
			 cl_ntoh16(p_rec->lid),
			 p_rec->node_info.base_version,
			 p_rec->node_info.class_version,
			 p_rec->node_info.node_type,
			 ib_get_node_type_str(p_rec->node_info.node_type),
			 p_rec->node_info.num_ports,
			 cl_ntoh64(p_rec->node_info.sys_guid),
			 cl_ntoh64(p_rec->node_info.node_guid),
			 cl_ntoh64(p_rec->node_info.port_guid),
			 cl_ntoh16(p_rec->node_info.partition_cap),
			 cl_ntoh16(p_rec->node_info.device_id),
			 cl_ntoh32(p_rec->node_info.revision),
			 ib_node_info_get_local_port_num(&p_rec->node_info),
			 cl_ntoh32(ib_node_info_get_vendor_id
				   (&p_rec->node_info)), desc);

	if (result < 0) {
		OSM_LOG(&p_osmt->log, OSM_LOG_ERROR, "ERR 0163: "
			"Write failed\n");
		status = IB_ERROR;
		goto Exit;
	}

Exit:
	OSM_LOG_EXIT(&p_osmt->log);
	return (status);
}