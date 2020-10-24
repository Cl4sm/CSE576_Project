static void rijndaelIVKAT (FILE *fp, int keyLength, int blockLength)
{
	int i, ROUNDS;
	BYTE block[4*MAXBC], block2[4*MAXBC];
	BYTE keyMaterial[320];
	keyInstance keyInst;
	cipherInstance cipherInst;
	char format[10];
#ifdef TRACE_KAT_MCT
	printf ("Executing Intermediate value KAT (key %d): ", keyLength);
	fflush (stdout);
#endif /* ?TRACE_KAT_MCT */

	switch (keyLength >= blockLength ? keyLength : blockLength) {
	case 128: ROUNDS = 10; break;
	case 192: ROUNDS = 12; break;
	case 256: ROUNDS = 14; break;
	default : return; /* this cannot happen */
	}
	
	fprintf (fp,
		"\n"
		"==========\n"
		"\n"
		"KEYSIZE=%d\n",
		keyLength);
	fflush (fp);
	memset (keyMaterial, 0, sizeof (keyMaterial));
	for (i = 0; i < keyLength/8; i++) {
		sprintf (&keyMaterial[2*i], "%02X", i);
	}
	keyInst.blockLen = blockLength;
	makeKey(&keyInst, DIR_ENCRYPT, keyLength, keyMaterial);
	fprintf (fp, "KEY=%s\n", keyMaterial);
	fprintf (fp, "\nIntermediate Ciphertext Values (Encryption)\n\n");
	for (i = 0; i < blockLength/8; i++) {
		block[i] = i;
	}
	blockPrint (fp, block, blockLength, "PT");
	cipherInst.blockLen = blockLength;
	cipherInit (&cipherInst, MODE_ECB, NULL);
	for(i = 1; i < ROUNDS; i++) {
		cipherUpdateRounds (&cipherInst, &keyInst, block, blockLength/8, block2, i);
		sprintf(format,"CT%d",i);
		blockPrint (fp, block2, blockLength, format);
	}
	cipherUpdateRounds (&cipherInst, &keyInst, block, blockLength, block2, ROUNDS);
	blockPrint (fp, block2, blockLength, "CT");
	
	keyInst.blockLen = blockLength;
	makeKey(&keyInst, DIR_DECRYPT, keyLength, keyMaterial);
	fprintf (fp, "\nIntermediate Ciphertext Values (Decryption)\n\n");
	blockPrint (fp, block2, blockLength, "CT");
	cipherInst.blockLen = blockLength;
	cipherInit (&cipherInst, MODE_ECB, NULL);
	for(i = 1; i < ROUNDS; i++) {
		cipherUpdateRounds (&cipherInst, &keyInst, block2, blockLength,block, ROUNDS-i);
		sprintf(format,"PT%d",i);
		blockPrint (fp, block, blockLength, format);
	}
	cipherUpdateRounds (&cipherInst, &keyInst, block2, blockLength, block, 0);
	blockPrint (fp, block, blockLength, "PT");
	
#ifdef TRACE_KAT_MCT
	printf (" done.\n");
#endif 
}
