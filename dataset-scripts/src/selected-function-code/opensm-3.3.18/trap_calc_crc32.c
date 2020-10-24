static uint32_t trap_calc_crc32(void *buffer, uint32_t count)
{
	uint32_t temp1, temp2;
	uint32_t crc = -1L;
	unsigned char *p = (unsigned char *)buffer;
	/* precalculated table for faster crc calculation */
	static uint32_t crc_table[256];
	static boolean_t first = TRUE;
	int i, j;

	/* if we need to initialize the lookup table */
	if (first) {
		/* calc the CRC table */
		for (i = 0; i <= 255; i++) {
			crc = i;
			for (j = 8; j > 0; j--)
				if (crc & 1)
					crc = (crc >> 1) ^ CRC32_POLYNOMIAL;
				else
					crc >>= 1;
			crc_table[i] = crc;
		}
		first = FALSE;
	}

	crc = -1L;
	/* do the calculation */
	while (count-- != 0) {
		temp1 = (crc >> 8) & 0x00FFFFFFL;
		temp2 = crc_table[((int)crc ^ *p++) & 0xFF];
		crc = temp1 ^ temp2;
	}
	return crc;
}