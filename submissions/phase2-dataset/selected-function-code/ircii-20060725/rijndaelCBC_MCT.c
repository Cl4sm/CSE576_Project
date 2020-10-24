static void rijndaelCBC_MCT (FILE *fp, const char *initKey, int keyLength,
	const char *initIV, const char *initBlock, int blockLength, BYTE direction)
{
	int i, j, r, t;
	BYTE inBlock[256/8], outBlock[256/8], binKey[256/8], cv[256/8];
	BYTE keyMaterial[320];
	BYTE iv[64+1];
	keyInstance keyInst;
	cipherInstance cipherInst;

#ifdef TRACE_KAT_MCT
	int width = 0;
	clock_t elapsed = -clock();
	printf ("Executing CBC MCT (%s, key %d): ",
		direction == DIR_ENCRYPT ? "ENCRYPT" : "DECRYPT", keyLength);
	fflush (stdout);
#endif /* ?TRACE_KAT_MCT */
	fprintf (fp,
		"\n"
		"==========\n"
		"\n"
		"KEYSIZE=%d\n", keyLength);
	fflush (fp);
	HexToBin (inBlock, initBlock, blockLength); /* this is either PT0 or CT0 */
	HexToBin (cv, initIV, blockLength);
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
		r = makeKey(&keyInst, direction, keyLength, keyMaterial);
		if (TRUE != r) {
			fprintf(stderr,"makeKey error %d\n",r);
			exit(-1);
		}
		/* do encryption/decryption: */
		blockPrint (fp, cv, blockLength, "IV");
		blockPrint (fp, inBlock, blockLength, direction == DIR_ENCRYPT ? "PT" : "CT");
		if (direction == DIR_ENCRYPT) {
			for (j = 0; j < 10000; j++) {
				for(t = 0; t < blockLength/8; t++) {
					sprintf(iv+2*t,"%02x",cv[t]);					
				}
				cipherInst.blockLen = blockLength;
				r = cipherInit (&cipherInst, MODE_CBC, iv);
				if (TRUE != r) {
					fprintf(stderr,"cipherInit error %d\n",r);
					exit(-1);
				}
				r = blockEncrypt(&cipherInst, &keyInst, inBlock, blockLength, outBlock);
				if (blockLength != r) {
					fprintf(stderr,"blockEncrypt error %d\n",r);
					exit(-1);
				}
				memcpy (inBlock, cv, blockLength/8);
				memcpy (cv, outBlock, blockLength/8);
			}
		} else {
			for (j = 0; j < 10000; j++) {
				for(t = 0; t < blockLength/8; t++) {
					sprintf(iv+2*t,"%02x",cv[t]);					
				}
				cipherInst.blockLen = blockLength;
				cipherInit (&cipherInst, MODE_CBC, iv);
				blockDecrypt(&cipherInst, &keyInst, inBlock, blockLength, outBlock);
				memcpy (cv, inBlock, blockLength/8);
				memcpy (inBlock, outBlock, blockLength/8);
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
				if (direction == DIR_ENCRYPT)
					binKey[j] ^= inBlock[j + 64/8];
				else
					binKey[j] ^= cv[j + 64/8];
			}
			for (j = 0; j < 128/8; j++) {
				binKey[j + 64/8] ^= outBlock[j];
			}
			break;
		case 256:
			for (j = 0; j < 128/8; j++) {
				if (direction == DIR_ENCRYPT)
					binKey[j] ^= inBlock[j];
				else
					binKey[j] ^= cv[j];
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
} /* rijndaelCBC_MCT */
