static void osm_dump_node_info_to_buf(IN const ib_node_info_t * p_ni,
				      OUT char * buf)
{
	if (!buf || !p_ni)
		return;
	else {
		sprintf(buf,
			"NodeInfo dump:\n"
			"\t\t\t\tbase_version............0x%X\n"
			"\t\t\t\tclass_version...........0x%X\n"
			"\t\t\t\tnode_type...............%s\n"
			"\t\t\t\tnum_ports...............%u\n"
			"\t\t\t\tsys_guid................0x%016" PRIx64 "\n"
			"\t\t\t\tnode_guid...............0x%016" PRIx64 "\n"
			"\t\t\t\tport_guid...............0x%016" PRIx64 "\n"
			"\t\t\t\tpartition_cap...........0x%X\n"
			"\t\t\t\tdevice_id...............0x%X\n"
			"\t\t\t\trevision................0x%X\n"
			"\t\t\t\tport_num................%u\n"
			"\t\t\t\tvendor_id...............0x%X\n",
			p_ni->base_version, p_ni->class_version,
			ib_get_node_type_str(p_ni->node_type), p_ni->num_ports,
			cl_ntoh64(p_ni->sys_guid), cl_ntoh64(p_ni->node_guid),
			cl_ntoh64(p_ni->port_guid),
			cl_ntoh16(p_ni->partition_cap),
			cl_ntoh16(p_ni->device_id), cl_ntoh32(p_ni->revision),
			ib_node_info_get_local_port_num(p_ni),
			cl_ntoh32(ib_node_info_get_vendor_id(p_ni)));
	}
}