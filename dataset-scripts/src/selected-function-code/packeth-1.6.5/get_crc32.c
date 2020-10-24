unsigned long get_crc32(unsigned char *p, int len)
{
	register unsigned long crc;
	int i;
	if (!crc32_table_init) {
		gen_crc32_table();
		crc32_table_init = 1;
	}
	
	crc = 0xFFFFFFFF;
	for (i=0; i<len; i++) {
		crc = (crc>>8) ^ crc32_table[ (crc ^ *p++) & 0xFF ];
	}
	crc = crc^0xFFFFFFFF;
	/* big endian to little endian */
	crc = ((crc >> 24) & 0x000000FF) ^
	      ((crc >>  8) & 0x0000FF00) ^
	      ((crc <<  8) & 0x00FF0000) ^
	      ((crc << 24) & 0xFF000000);
	return crc;
}