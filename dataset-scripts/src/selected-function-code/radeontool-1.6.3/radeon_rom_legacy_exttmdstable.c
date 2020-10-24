static void radeon_rom_legacy_exttmdstable(unsigned char *bios, int hdr)
{
	int offset, rev;
	int nr_blocks;
	int table_start;
	unsigned short index;
	unsigned short id;
	offset = BIOS16(hdr + 0x58);
	if (offset == 0) {
		printf("No External TMDS table found\n");
		return;
	}

	rev = BIOS8(offset);

	printf("Found External TMDS Table rev %d\n", rev);

	if (rev == 2)
	    printf("single link\n");
	else if (rev == 3)
	    printf("dual link\n");

	printf("table size %08x\n", BIOS16(offset+1));


	nr_blocks = BIOS8(offset+3);

	printf("no blocks %08x\n", nr_blocks);

	table_start = offset+4;
	printf("max freq %d, slave i2c %02x i2c %d conn id %d, flags %d\n",
	       BIOS16(table_start), BIOS8(table_start+2), BIOS8(table_start+3),
	       BIOS8(table_start+4), BIOS8(table_start+5));

	index = table_start+6;
	printf("next short is %04X\n", BIOS16(index)>>13);

	id = BIOS16(index);
	printf("id is %04X %d\n", id, id & 0x1fff);

	while (id != 0xffff) {
	       switch(id >> 13) {
	       case 0:
		       printf("mm write\n");
		       index += 6;
		       break;
	       case 2:
		       printf("mm mask write\n");
		       index += 10;
		       break;
	       case 4:
		       printf("delay microsec \n");
		       index += 4;
		       break;
	       case 5:
		       printf("pll mask write \n");
		       index += 10;
		       break;
	       case 6:
		       printf("i2c write %02X\n", BIOS8(index+2));
		       index += 3;
		       break;
	       default:
		       printf("unknown id %d\n", id>>13);
		       return;
	       };

	       id = BIOS16(index);
	       printf("id is %04X %d\n", id, id & 0x1fff);

	}
}