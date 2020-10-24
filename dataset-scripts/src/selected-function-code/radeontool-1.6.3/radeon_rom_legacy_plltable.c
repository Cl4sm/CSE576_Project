static void radeon_rom_legacy_plltable(unsigned char *bios, int offset)
{
	int type, start, i = 0;
	start = BIOS8(offset);
	while (start != 0) {
		type = (start & 0xc0) >> 6;
		if (type == 0) {
			int reg = BIOS8(offset + i) & 0x3f;
			unsigned int val = BIOS32(offset + i + 1);
			printf("pll reg: 0x%x, val: 0x%x\n", reg, val);
			i += 5;
		} else if (type == 1) {
			int reg = BIOS8(offset + i) & 0x3f;
			int byte_offset = BIOS8(offset + i + 1);
			int and_mask = BIOS8(offset + i + 2);
			int or_mask  = BIOS8(offset + i + 3);
			printf("pll reg: 0x%x, byte offset of dword 0x%x, AND mask: 0x%x, OR mask: 0x%x\n",
			       reg, byte_offset, and_mask, or_mask);
			i += 4;
		} else if (type >= 2) {
			int cmd = BIOS8(offset + i) & 0x3f;
			switch (cmd) {
			case 1:
				printf("delay 150 us\n");
				break;
			case 2:
				printf("delay 1 ms\n");
				break;
			case 3:
				printf("wait for MC_BUSY = 0 in CLK_PWRMGT_CNTL\n");
				break;
			case 4:
				printf("wait for DLL_READY = 1 in CLK_PWRMGT_CNTL\n");
				break;
			case 5:
				printf("check and set bit 24 in CLK_PWRMGT_CNTL to 0\n");
				break;
			default:
				printf("ass\n");
				break;
			}
			i += 1;
		} else
			break;
		start = BIOS8(offset + i);
	}
}