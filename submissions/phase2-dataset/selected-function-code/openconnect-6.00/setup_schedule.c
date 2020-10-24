static void setup_schedule (const unsigned char *key_56, DES_KS ks)
{
	unsigned char key[8];
	int i, c, bit;

	for (i = 0; i < 8; i++) {
		key[i] = HIKEYBITS (key_56, i * 7);
		/* Mask in the low bits only if they're used. It doesn't
		 * matter if we get an unwanted bit 0; it's going to be
		 * overwritten with parity anyway. */
		if (i && i < 7)
			key[i] |= LOKEYBITS(key_56, i * 7);

		/* Fix parity */
		for (c = bit = 0; bit < 8; bit++)
			if (key[i] & (1 << bit))
				c++;
		if (!(c & 1))
			key[i] ^= 0x01;
	}

	deskey (ks, key, 0);
}