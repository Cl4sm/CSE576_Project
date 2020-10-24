static void dump_mcast_routes(cl_map_item_t * item, FILE * file, void *cxt)
{
	osm_switch_t *p_sw = (osm_switch_t *) item;
	osm_mcast_tbl_t *p_tbl;
	int16_t mlid_ho = 0;
	int16_t mlid_start_ho;
	uint8_t position = 0;
	int16_t block_num = 0;
	boolean_t first_mlid;
	boolean_t first_port;
	const osm_node_t *p_node;
	uint16_t i, j;
	uint16_t mask_entry;
	char sw_hdr[256];
	char mlid_hdr[32];

	p_node = p_sw->p_node;

	p_tbl = osm_switch_get_mcast_tbl_ptr(p_sw);

	sprintf(sw_hdr, "\nSwitch 0x%016" PRIx64 "\nLID    : Out Port(s)\n",
		cl_ntoh64(osm_node_get_node_guid(p_node)));
	first_mlid = TRUE;
	while (block_num <= p_tbl->max_block_in_use) {
		mlid_start_ho = (uint16_t) (block_num * IB_MCAST_BLOCK_SIZE);
		for (i = 0; i < IB_MCAST_BLOCK_SIZE; i++) {
			mlid_ho = mlid_start_ho + i;
			position = 0;
			first_port = TRUE;
			sprintf(mlid_hdr, "0x%04X :",
				mlid_ho + IB_LID_MCAST_START_HO);
			while (position <= p_tbl->max_position) {
				mask_entry =
				    cl_ntoh16((*p_tbl->
					       p_mask_tbl)[mlid_ho][position]);
				if (mask_entry == 0) {
					position++;
					continue;
				}
				for (j = 0; j < 16; j++) {
					if ((1 << j) & mask_entry) {
						if (first_mlid) {
							fprintf(file, "%s",
								sw_hdr);
							first_mlid = FALSE;
						}
						if (first_port) {
							fprintf(file, "%s",
								mlid_hdr);
							first_port = FALSE;
						}
						fprintf(file, " 0x%03X ",
							j + (position * 16));
					}
				}
				position++;
			}
			if (first_port == FALSE)
				fprintf(file, "\n");
		}
		block_num++;
	}
}