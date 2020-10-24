void radeon_of_table(const char * file, int table)
{
	unsigned char bios[_2K];
	unsigned int reg, mask, val, op;
	int fd;
	int len;
	int count = 0;
	fd = open(file, O_RDONLY);
	if (fd < 0) {
		perror("can't open init file");
		return;
	}
	memset(bios, 0, _2K);
	len = read(fd, bios, _2K);
	close(fd);

	while (len > 0) {
		switch(table) {
		case OF_TABLE_INIT1:
			len -= 12;
			reg = OF32(count);
			mask = OF32(count + 4);
			val = OF32(count + 8);
			printf("reg: 0x%x, mask 0x%x, val 0x%x\n", reg, mask, val);
			count += 12;
			break;
		case OF_TABLE_INIT2:
			len -= 16;
			reg = OF32(count);
			mask = OF32(count + 4);
			val = OF32(count + 8);
			op = OF32(count + 12);
			printf("reg: 0x%x, mask 0x%x, val 0x%x op 0x%x\n", reg, mask, val, op);
			count += 16;
			break;
		case OF_TABLE_PLL:
			len -= 16;
			reg = OF32(count);
			mask = OF32(count + 4);
			val = OF32(count + 8);
			op = OF32(count + 12);
			printf("pll reg: 0x%x, mask 0x%x, val 0x%x, op %x\n", reg, mask, val, op);
			if (op == 5)
				printf("delay 1 ms\n");
			else if (op == 6)
				printf("delay 150 us\n");
			else if (op)
				printf("unknown op %d\n", op);
			count += 16;
			break;
		case OF_TABLE_MEMINIT:
			len -= 4;
			val = OF32(count);
			if (val >> 16 == 0x6fff)
				printf("pwup complete test\n");
			else {
				mask = val >> 24;
				val = val & 0xffff;
				printf("rmw SDRAM mask %x %x\n", val, mask);
			}
			count += 4;
			break;
		}
	}
}