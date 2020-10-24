{
	int i;
	BYTE block[4*MAXBC];
	BYTE keyMaterial[320];
	keyInstance keyInst;
	cipherInstance cipherInst;

#ifdef TRACE_KAT_MCT
	printf ("Executing Variable-Text KAT (key %d): ", keyLength);
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
	memset (keyMaterial, '0', keyLength/4);
	keyInst.blockLen = blockLength;
	makeKey(&keyInst, DIR_ENCRYPT, keyLength, keyMaterial);
	fprintf (fp, "KEY=%s\n", keyMaterial);
	for (i = 0; i < blockLength; i++) {
		memset (block, 0, blockLength/8);
		block[i/8] |= 1 << (7 - i%8); /* set only the i-th bit of the i-th test block */
		fprintf (fp, "\nI=%d\n", i+1);
		blockPrint (fp, block, blockLength, "PT");
		cipherInst.blockLen = blockLength;
		cipherInit (&cipherInst, MODE_ECB, NULL);
		blockEncrypt(&cipherInst, &keyInst, block, blockLength, block);
		blockPrint (fp, block, blockLength, "CT");
	}
#ifdef TRACE_KAT_MCT
	printf (" done.\n");
#endif /* ?TRACE_KAT_MCT */
}
