static void osm_dump_node_record_to_buf(IN const ib_node_record_t * p_nr,
					OUT char * buf)
{
	if (!buf || !p_nr)
		return;
	else {
		char desc[sizeof(p_nr->node_desc.description) + 1];
		const ib_node_info_t *p_ni = &p_nr->node_info;

		memcpy(desc, p_nr->node_desc.description,
		       sizeof(p_nr->node_desc.description));
		desc[sizeof(desc) - 1] = '\0';
		sprintf(buf,
			"Node Record dump:\n"
			"\t\t\t\tRID\n"
			"\t\t\t\tLid.....................%u\n"
			"\t\t\t\tReserved................0x%X\n"
			"\t\t\t\tNodeInfo dump:\n"
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
			"\t\t\t\tvendor_id...............0x%X\n"
			"\t\t\t\tNodeDescription\n"
			"\t\t\t\t%s\n",
			cl_ntoh16(p_nr->lid), cl_ntoh16(p_nr->resv),
			p_ni->base_version, p_ni->class_version,
			ib_get_node_type_str(p_ni->node_type), p_ni->num_ports,
			cl_ntoh64(p_ni->sys_guid), cl_ntoh64(p_ni->node_guid),
			cl_ntoh64(p_ni->port_guid),
			cl_ntoh16(p_ni->partition_cap),
			cl_ntoh16(p_ni->device_id), cl_ntoh32(p_ni->revision),
			ib_node_info_get_local_port_num(p_ni),
			cl_ntoh32(ib_node_info_get_vendor_id(p_ni)), desc);
	}
}