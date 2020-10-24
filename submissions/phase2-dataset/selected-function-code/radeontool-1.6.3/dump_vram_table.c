void dump_vram_table(struct atom_context *ctx)
{
	int index = GetIndexIntoMasterTable(DATA, VRAM_UsageByFirmware);
	uint16_t data_offset;
	uint8_t crev, frev;
	uint16_t size;
	ATOM_VRAM_USAGE_BY_FIRMWARE *vram_usage;
	uint16_t fw_bytes;
	uint32_t start_offset;
	
	atom_parse_data_header(ctx, index, &size, &frev, &crev, &data_offset);

	vram_usage = (struct _ATOM_VRAM_USAGE_BY_FIRMWARE *)(ctx->bios + data_offset);
	start_offset = vram_usage->asFirmwareVramReserveInfo[0].ulStartAddrUsedByFirmware;
	fw_bytes = vram_usage->asFirmwareVramReserveInfo[0].usFirmwareUseInKb * 1024;
	printf("vram_usage 0x%08x %dkb\n", 
	       vram_usage->asFirmwareVramReserveInfo[0].ulStartAddrUsedByFirmware,
	       vram_usage->asFirmwareVramReserveInfo[0].usFirmwareUseInKb);

	if (!fb_mem)
		printf("No card to dump VRAM from\n");
	else {
		void *backing;
		int fd;

		backing = kzalloc(fw_bytes, GFP_KERNEL);

		start_offset &= 0xfffffff;
		memcpy(backing, fb_mem + start_offset, fw_bytes);

		fd = open("/tmp/vram_usage", O_RDWR | O_CREAT, 0666);

		write(fd, backing, fw_bytes);

		close(fd);

		printf("EDID offsets crt1:0x%x lcd1:0x%x dfp1:0x%x crt2:0x%x df5:0x%x\n", ATOM_CRT1_EDID_ADDR,
                       ATOM_LCD1_EDID_ADDR, ATOM_DFP1_EDID_ADDR,
                       ATOM_CRT2_EDID_ADDR, ATOM_DFP5_EDID_ADDR);
                printf("DP train 0x%x\n", ATOM_DP_TRAINING_TBL_ADDR);

	}

}