static void radeon_dump_bios_igp_slot_config(struct atom_context *ctx, uint16_t obj)
{
	uint16_t size, data_offset;
	uint8_t frev, crev;
	int index = GetIndexIntoMasterTable(DATA, IntegratedSystemInfo);
	ATOM_INTEGRATED_SYSTEM_INFO_V2 *igp_obj = NULL;
	uint8_t obj_id, num, obj_type;

	obj_id = (obj & OBJECT_ID_MASK) >> OBJECT_ID_SHIFT;
	num = (obj & ENUM_ID_MASK) >> ENUM_ID_SHIFT;
	obj_type = (obj & OBJECT_TYPE_MASK) >> OBJECT_TYPE_SHIFT;

	if (obj_id == CONNECTOR_OBJECT_ID_PCIE_CONNECTOR) {
	    uint32_t slot_config, ct;

	    atom_parse_data_header(ctx, index, &size, &frev, &crev, &data_offset);

	    igp_obj = (ATOM_INTEGRATED_SYSTEM_INFO_V2 *)(ctx->bios + data_offset);

	    if (igp_obj) {
		if (num == 1)
		    slot_config = igp_obj->ulDDISlot1Config;
		else
		    slot_config = igp_obj->ulDDISlot2Config;

		ct = (slot_config  >> 16) & 0xff;
		printf("\tConnector: %s\n", connector_names[ct]);
		printf("\tLane Config: 0x%04x\n", slot_config & 0xffff);
	    }
	}
}