void radeon_dump_bios_object_table(struct atom_context *ctx)
{
	uint16_t size, data_offset;
	uint8_t frev, crev;
	int index = GetIndexIntoMasterTable(DATA, Object_Header);
	ATOM_CONNECTOR_OBJECT_TABLE *con_obj;
	ATOM_DISPLAY_OBJECT_PATH_TABLE *path_obj;
	ATOM_ENCODER_OBJECT_TABLE *enc_obj;
	ATOM_OBJECT_TABLE *router_obj;
	ATOM_INTEGRATED_SYSTEM_INFO_V2 *igp_obj = NULL;
	ATOM_OBJECT_HEADER *obj_header;
	int i, j, k, path_size;

	atom_parse_data_header(ctx, index, &size, &frev, &crev, &data_offset);

	if (crev < 2)
		return;

	obj_header = (ATOM_OBJECT_HEADER *)(ctx->bios + data_offset);

	printf("\nSupported Devices: ");
	radeon_print_supported_devices(obj_header->usDeviceSupport);
	printf("\n");

	path_obj = (ATOM_DISPLAY_OBJECT_PATH_TABLE *)(ctx->bios + data_offset + obj_header->usDisplayPathTableOffset);
	con_obj = (ATOM_CONNECTOR_OBJECT_TABLE *)(ctx->bios + data_offset + obj_header->usConnectorObjectTableOffset);
	enc_obj = (ATOM_ENCODER_OBJECT_TABLE *)(ctx->bios + data_offset + obj_header->usEncoderObjectTableOffset);
	router_obj = (ATOM_OBJECT_TABLE *)(ctx->bios + data_offset + obj_header->usRouterObjectTableOffset);

	path_size = 0;
	for (i = 0; i < path_obj->ucNumOfDispPath; i++) {
	    uint8_t *addr = (uint8_t *)path_obj->asDispPath;
	    ATOM_DISPLAY_OBJECT_PATH *path;
	    addr += path_size;
	    path = (ATOM_DISPLAY_OBJECT_PATH *)addr;

	    if (obj_header->usDeviceSupport & path->usDeviceTag) {
		printf("\nDevice: ");
		radeon_print_supported_devices(path->usDeviceTag);
		printf("\n");
		// connector object
		printf("==== Connector Info ====\n");
		radeon_print_object(path->usConnObjectId);
		for (j = 0; j < con_obj->ucNumberOfObjects; j++) {
		    ATOM_COMMON_RECORD_HEADER *record;
		    int record_base;
		    uint16_t con_obj_id = le16_to_cpu(con_obj->asObjects[j].usObjectID);

		    if (path->usConnObjectId == con_obj_id) {
			record = (ATOM_COMMON_RECORD_HEADER *)
			    (ctx->bios + data_offset + le16_to_cpu(con_obj->asObjects[j].usRecordOffset));
			record_base = le16_to_cpu(con_obj->asObjects[j].usRecordOffset);

			while (record->ucRecordType > 0 &&
			       record->ucRecordType <= ATOM_MAX_OBJECT_RECORD_NUMBER) {
			    //printf("record type %d\n", record->ucRecordType);

			    switch(record->ucRecordType) {
			    case ATOM_I2C_RECORD_TYPE:
				{
				    ATOM_I2C_RECORD *i2c_record = (ATOM_I2C_RECORD *)record;
				    ATOM_I2C_ID_CONFIG_ACCESS *i2c_config =
					(ATOM_I2C_ID_CONFIG_ACCESS *)&i2c_record->sucI2cId;
				    printf("  ATOM_I2C_RECORD_TYPE\n");
				    printf("    i2c mux:%d engine:%d hw_cap:%d\n",
					   i2c_record->sucI2cId.bfI2C_LineMux,
					   i2c_record->sucI2cId.bfHW_EngineID,
					   i2c_record->sucI2cId.bfHW_Capable);
				    radeon_lookup_gpio(ctx, i2c_config->ucAccess);
				}
				break;
			    case ATOM_HPD_INT_RECORD_TYPE:
				{
				    ATOM_HPD_INT_RECORD *hdp = (ATOM_HPD_INT_RECORD *)record;
				    printf("  ATOM_HPD_INT_RECORD_TYPE\n");
				    printf("    gpioid: %d, plugged pin state: 0x%x\n",
					   hdp->ucHPDIntGPIOID, hdp->ucPluggged_PinState);
				}
				break;
			    case ATOM_CONNECTOR_DEVICE_TAG_RECORD_TYPE:
				{
				    ATOM_CONNECTOR_DEVICE_TAG_RECORD *dev_tag_rec =
					(ATOM_CONNECTOR_DEVICE_TAG_RECORD *)record;
				    printf("  ATOM_CONNECTOR_DEVICE_TAG_RECORD_TYPE\n");
				    printf("    Devices: ");
				    for (k = 0; k < dev_tag_rec->ucNumberOfDevice; k++) {
					ATOM_CONNECTOR_DEVICE_TAG *dev_tag =
					    &dev_tag_rec->asDeviceTag[k];
					radeon_print_supported_devices(dev_tag->usDeviceID);
				    }
				    printk("\n");
				}
				break;
			    case ATOM_CONNECTOR_CVTV_SHARE_DIN_RECORD_TYPE:
				{
				    ATOM_CONNECTOR_CVTV_SHARE_DIN_RECORD *cvtv =
					(ATOM_CONNECTOR_CVTV_SHARE_DIN_RECORD *)record;
				    printf("  ATOM_CONNECTOR_CVTV_SHARE_DIN_RECORD_TYPE\n");
				    printf("    gpioid: %d, tv active state: %d\n",
					   cvtv->ucGPIOID, cvtv->ucTVActiveState);
				}
				break;
			    case ATOM_CONNECTOR_PCIE_SUBCONNECTOR_RECORD_TYPE:
				{
				    ATOM_CONNECTOR_PCIE_SUBCONNECTOR_RECORD *pcie =
					(ATOM_CONNECTOR_PCIE_SUBCONNECTOR_RECORD *)record;
				    printf("  ATOM_CONNECTOR_PCIE_SUBCONNECTOR_RECORD_TYPE\n");
				    printf("    %s\n", connector_names[pcie->ucSubConnectorType]);
				}
				break;
			    case ATOM_OBJECT_GPIO_CNTL_RECORD_TYPE:
				printf("  ATOM_OBJECT_GPIO_CNTL_RECORD_TYPE\n");
				break;
			    case ATOM_CONNECTOR_CF_RECORD_TYPE:
				printf("  ATOM_CONNECTOR_CF_RECORD_TYPE\n");
				break;

			    }
			    record = (ATOM_COMMON_RECORD_HEADER *)((char *)record + record->ucRecordSize);
			}
			break;
		    }
		}
		radeon_dump_bios_igp_slot_config(ctx, path->usConnObjectId);
		printf("==== GPU Info ====\n");
		radeon_print_object(path->usGPUObjectId);
		printf("==== Encoder info ====\n");
		for (k = 0; k < ((path->usSize - 8) / 2); k++) {
		    uint8_t obj_type = (path->usGraphicObjIds[k] & OBJECT_TYPE_MASK) >> OBJECT_TYPE_SHIFT;

		    if (obj_type == GRAPH_OBJECT_TYPE_ENCODER) {
			radeon_print_object(path->usGraphicObjIds[k]);
			for (j = 0; j < enc_obj->ucNumberOfObjects; j++) {
			    ATOM_COMMON_RECORD_HEADER *record;
			    int record_base;
			    uint16_t enc_obj_id = le16_to_cpu(enc_obj->asObjects[j].usObjectID);

			    if (path->usGraphicObjIds[k] == enc_obj_id) {
				record = (ATOM_COMMON_RECORD_HEADER *)
				    (ctx->bios + data_offset + le16_to_cpu(enc_obj->asObjects[j].usRecordOffset));
				record_base = le16_to_cpu(enc_obj->asObjects[j].usRecordOffset);

				while (record->ucRecordType > 0 &&
				       record->ucRecordType <= ATOM_MAX_OBJECT_RECORD_NUMBER) {
				    //printf("record type %d\n", record->ucRecordType);

				    switch(record->ucRecordType) {
				    case ATOM_ENCODER_FPGA_CONTROL_RECORD_TYPE:
					printf("  ATOM_ENCODER_FPGA_CONTROL_RECORD_TYPE\n");
					break;
				    case ATOM_ENCODER_DVO_CF_RECORD_TYPE:
					printf("  ATOM_ENCODER_DVO_CF_RECORD_TYPE\n");
					break;
				    case ATOM_OBJECT_GPIO_CNTL_RECORD_TYPE:
					printf("  ATOM_OBJECT_GPIO_CNTL_RECORD_TYPE\n");
					break;
				    }
				    record = (ATOM_COMMON_RECORD_HEADER *)((char *)record + record->ucRecordSize);
				}
				break;
			    }
			}
		    }
		}

		printf("==== Router Info ====\n");
		for (k = 0; k < ((path->usSize - 8) / 2); k++) {
		    uint8_t obj_type = (path->usGraphicObjIds[k] & OBJECT_TYPE_MASK) >> OBJECT_TYPE_SHIFT;

		    if (obj_type == GRAPH_OBJECT_TYPE_ROUTER) {
			radeon_print_object(path->usGraphicObjIds[k]);
			for (j = 0; j < router_obj->ucNumberOfObjects; j++) {
			    ATOM_COMMON_RECORD_HEADER *record;
			    int record_base;
			    uint16_t router_obj_id = le16_to_cpu(router_obj->asObjects[j].usObjectID);

			    if (path->usGraphicObjIds[k] == router_obj_id) {
				record = (ATOM_COMMON_RECORD_HEADER *)
				    (ctx->bios + data_offset + le16_to_cpu(router_obj->asObjects[j].usRecordOffset));
				record_base = le16_to_cpu(router_obj->asObjects[j].usRecordOffset);

				while (record->ucRecordType > 0 &&
				       record->ucRecordType <= ATOM_MAX_OBJECT_RECORD_NUMBER) {
				    //printf("record type %d\n", record->ucRecordType);

				    switch(record->ucRecordType) {
				    case ATOM_I2C_RECORD_TYPE:
					{
					    ATOM_I2C_RECORD *i2c_record = (ATOM_I2C_RECORD *)record;
					    printf("  ATOM_I2C_RECORD_TYPE\n");
					    printf("    i2c mux:%d engine:%d hw_cap:%d\n",
						   i2c_record->sucI2cId.bfI2C_LineMux,
						   i2c_record->sucI2cId.bfHW_EngineID,
						   i2c_record->sucI2cId.bfHW_Capable);
					    radeon_lookup_gpio(ctx, i2c_record->sucI2cId.bfI2C_LineMux);
					}
					break;
				    case ATOM_ROUTER_DDC_PATH_SELECT_RECORD_TYPE:
					{
					    ATOM_ROUTER_DDC_PATH_SELECT_RECORD *ddc_path =
						(ATOM_ROUTER_DDC_PATH_SELECT_RECORD *)record;
					    printf("  ATOM_ROUTER_DDC_PATH_SELECT_RECORD_TYPE\n");
					    printf("    mux type: %d, mux cntl pin: %d, mux state: %d\n",
						   ddc_path->ucMuxType,
						   ddc_path->ucMuxControlPin,
						   ddc_path->ucMuxState[0]);
					}
					break;
				    case ATOM_ROUTER_DATA_CLOCK_PATH_SELECT_RECORD_TYPE:
					{
					    ATOM_ROUTER_DATA_CLOCK_PATH_SELECT_RECORD *clk_data_path =
						(ATOM_ROUTER_DATA_CLOCK_PATH_SELECT_RECORD *)record;
					    printf("  ATOM_ROUTER_DATA_CLOCK_PATH_SELECT_RECORD_TYPE\n");
					    printf("    mux type: %d, mux cntl pin: %d, mux state: %d\n",
						   clk_data_path->ucMuxType,
						   clk_data_path->ucMuxControlPin,
						   clk_data_path->ucMuxState[0]);
					}
					break;
				    case ATOM_OBJECT_GPIO_CNTL_RECORD_TYPE:
					printf("  ATOM_OBJECT_GPIO_CNTL_RECORD_TYPE\n");
					break;
				    }
				    record = (ATOM_COMMON_RECORD_HEADER *)((char *)record + record->ucRecordSize);
				}
				break;
			    }
			}
		    }
		}
	    }
	    path_size += path->usSize;
	}
}