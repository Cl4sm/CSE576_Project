static void rijndaelECB_MCT (FILE *fp, const char *initKey, int keyLength,
	const char *initBlock, int blockLength, BYTE direction)
{
	int i, j;
	BYTE inBlock[4*MAXBC], outBlock[4*MAXBC], binKey[4*MAXKC];
	BYTE keyMaterial[320];
	keyInstance keyInst;
	cipherInstance cipherInst;

#ifdef TRACE_KAT_MCT
	int width = 0;
	clock_t elapsed = -clock();
	printf ("Executing ECB MCT (%s, key %d): ",
		direction == DIR_ENCRYPT ? "ENCRYPT" : "DECRYPT", keyLength);
	fflush (stdout);
#endif /* ?TRACE_KAT_MCT */
	fprintf (fp,
		"\n"
		"=========================\n"
		"\n"
		"KEYSIZE=%d\n", keyLength);
	fflush (fp);
	HexToBin (outBlock, initBlock, blockLength);
	HexToBin (binKey, initKey, keyLength);
	for (i = 0; i < 400; i++) {
#ifdef TRACE_KAT_MCT                 
        while (width-- > 0) putchar ('\b'); width = printf ("%d", i); fflush (stdout);    
#endif /* ?TRACE_KAT_MCT */
		fprintf (fp, "\nI=%d\n", i);
		/* prepare key: */
		for (j = 0; j < keyLength/8; j++) {
			sprintf (&keyMaterial[2*j], "%02X", binKey[j]);
		}
		keyMaterial[keyLength/4] = 0;
		fprintf (fp, "KEY=%s\n", keyMaterial);
		keyInst.blockLen = blockLength;
		makeKey(&keyInst, direction, keyLength, keyMaterial);
		/* do encryption/decryption: */
		blockPrint (fp, outBlock, blockLength, direction == DIR_ENCRYPT ? "PT" : "CT");
		cipherInst.blockLen = blockLength;
		cipherInit (&cipherInst, MODE_ECB, NULL);
		if (direction == DIR_ENCRYPT) {
			for (j = 0; j < 10000; j++) {
				memcpy (inBlock, outBlock, blockLength/8);
				blockEncrypt(&cipherInst, &keyInst, inBlock, blockLength, outBlock);
			}
		} else {
			for (j = 0; j < 10000; j++) {
				memcpy (inBlock, outBlock, blockLength/8);
				blockDecrypt(&cipherInst, &keyInst, inBlock, blockLength, outBlock);
			}
		}
		blockPrint (fp, outBlock, blockLength, direction == DIR_ENCRYPT ? "CT" : "PT");
		/* prepare new key: */
		switch (keyLength) {
		case 128:
			for (j = 0; j < 128/8; j++) {
				binKey[j] ^= outBlock[j];
			}
			break;
		case 192:
			for (j = 0; j < 64/8; j++) {
				binKey[j] ^= inBlock[j + 64/8];
			}
			for (j = 0; j < 128/8; j++) {
				binKey[j + 64/8] ^= outBlock[j];
			}
			break;
		case 256:
			for (j = 0; j < 128/8; j++) {
				binKey[j] ^= inBlock[j];
			}
			for (j = 0; j < 128/8; j++) {
				binKey[j + 128/8] ^= outBlock[j];
			}
			break;
		}
	}
#ifdef TRACE_KAT_MCT
	elapsed += clock();
	printf (" done (%.1f s).\n", (float)elapsed/CLOCKS_PER_SEC);
#endif /* ?TRACE_KAT_MCT */
} /* rijndaelECB_MCT */
