{
	int i, j;
	unsigned int s;
	BYTE block[4*MAXBC], block2[4*MAXBC];
	BYTE keyMaterial[320];
	keyInstance keyInst;
	cipherInstance cipherInst;

#ifdef TRACE_KAT_MCT
	printf ("Executing Tables KAT (key %d): ", keyLength);
	fflush (stdout);
#endif /* ?TRACE_KAT_MCT */
	fprintf (fp,
		"\n"
		"==========\n"
		"\n"
		"KEYSIZE=%d\n"
		"\n", keyLength);
	fflush (fp);

	memset (keyMaterial, 0, sizeof (keyMaterial));
	
	for(i = 0; i < 64; i++) {
		fprintf (fp, "\nI=%d\n", i+1);
		for(j = 0; j < keyLength/4; j++) {
			fscanf(in,"%c",&keyMaterial[j]);
		}
		keyInst.blockLen = blockLength;
		makeKey(&keyInst, DIR_ENCRYPT, keyLength, keyMaterial);
		
		fprintf (fp, "KEY=%s\n", keyMaterial);
		
		for(j = 0; j < blockLength/8; j++) {
			fscanf(in,"%02x",&s);
			block[j] = s;
		}
		fscanf(in,"%c",&s);
		fscanf(in,"%c",&s);
		blockPrint (fp, block, blockLength, "PT");
		cipherInst.blockLen = blockLength;
		cipherInit (&cipherInst, MODE_ECB, NULL);
		blockEncrypt(&cipherInst, &keyInst, block, blockLength, block2);
		blockPrint (fp, block2, blockLength, "CT");
	}
	for(i = 64; i < 128; i++) {
		fprintf (fp, "\nI=%d\n", i+1);
		for(j = 0; j < keyLength/4; j++) {
			fscanf(in,"%c",&keyMaterial[j]);
		}
		keyInst.blockLen = blockLength;
		makeKey(&keyInst, DIR_DECRYPT, keyLength, keyMaterial);
		
		fprintf (fp, "KEY=%s\n", keyMaterial);
		
		for(j = 0; j < blockLength/8; j++) {
			fscanf(in,"%02x",&s);
			block[j] = s;
		}
		fscanf(in,"%c",&s);
		fscanf(in,"%c",&s);
		cipherInst.blockLen = blockLength;
		cipherInit (&cipherInst, MODE_ECB, NULL);
		blockDecrypt(&cipherInst, &keyInst, block, blockLength, block2);
		blockPrint (fp, block2, blockLength, "PT");
		blockPrint (fp, block, blockLength, "CT");
	}

#ifdef TRACE_KAT_MCT
	printf (" done.\n");
#endif /* ?TRACE_KAT_MCT */
}
