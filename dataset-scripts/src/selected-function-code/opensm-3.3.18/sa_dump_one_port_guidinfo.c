static void sa_dump_one_port_guidinfo(cl_map_item_t * p_map_item, void *cxt)
{
	FILE *file = ((struct opensm_dump_context *)cxt)->file;
	osm_port_t *p_port = (osm_port_t *) p_map_item;
	uint32_t max_block;
	int block_num;

	if (!p_port->p_physp->p_guids)
		return;

	max_block = (p_port->p_physp->port_info.guid_cap + GUID_TABLE_MAX_ENTRIES - 1) /
		     GUID_TABLE_MAX_ENTRIES;

	for (block_num = 0; block_num < max_block; block_num++) {
		fprintf(file, "GUIDInfo Record:"
			" base_guid=0x%016" PRIx64 " lid=0x%04x block_num=0x%x"
			" guid0=0x%016" PRIx64 " guid1=0x%016" PRIx64
			" guid2=0x%016" PRIx64 " guid3=0x%016" PRIx64
			" guid4=0x%016" PRIx64 " guid5=0x%016" PRIx64
			" guid6=0x%016" PRIx64 " guid7=0x%016" PRIx64
			"\n\n",
			cl_ntoh64((*p_port->p_physp->p_guids)[0]),
			cl_ntoh16(osm_port_get_base_lid(p_port)), block_num,
			cl_ntoh64((*p_port->p_physp->p_guids)[block_num * GUID_TABLE_MAX_ENTRIES]),
			cl_ntoh64((*p_port->p_physp->p_guids)[block_num * GUID_TABLE_MAX_ENTRIES + 1]),
			cl_ntoh64((*p_port->p_physp->p_guids)[block_num * GUID_TABLE_MAX_ENTRIES + 2]),
			cl_ntoh64((*p_port->p_physp->p_guids)[block_num * GUID_TABLE_MAX_ENTRIES + 3]),
			cl_ntoh64((*p_port->p_physp->p_guids)[block_num * GUID_TABLE_MAX_ENTRIES + 4]),
			cl_ntoh64((*p_port->p_physp->p_guids)[block_num * GUID_TABLE_MAX_ENTRIES + 5]),
			cl_ntoh64((*p_port->p_physp->p_guids)[block_num * GUID_TABLE_MAX_ENTRIES + 6]),
			cl_ntoh64((*p_port->p_physp->p_guids)[block_num * GUID_TABLE_MAX_ENTRIES + 7]));
	}
}