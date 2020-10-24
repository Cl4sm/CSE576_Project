{
	int i, j, r;
	BYTE block[4*MAXBC];
	BYTE keyMaterial[320];
	BYTE byteVal = (BYTE)'8';
	keyInstance keyInst;
	cipherInstance cipherInst;

#ifdef TRACE_KAT_MCT
	printf ("Executing Variable-Key KAT (key %d): ", keyLength);
	fflush (stdout);
#endif /* ?TRACE_KAT_MCT */
	fprintf (fp,
		"\n"
		"==========\n"
		"\n"
		"KEYSIZE=%d\n"
		"\n", keyLength);
	fflush (fp);
	memset (block, 0, blockLength/8);
	blockPrint (fp, block, blockLength, "PT");
	memset (keyMaterial, 0, sizeof (keyMaterial));
	memset (keyMaterial, '0', keyLength/4);
	for (i = 0; i < keyLength; i++) {
		keyMaterial[i/4] = byteVal; /* set only the i-th bit of the i-th test key */
		keyInst.blockLen = blockLength;
		r = makeKey(&keyInst, DIR_ENCRYPT, keyLength, keyMaterial);
		if (TRUE != r) {
			fprintf(stderr,"makeKey error %d\n",r);
			exit(-1);
		}
		fprintf (fp, "\nI=%d\n", i+1);
		fprintf (fp, "KEY=%s\n", keyMaterial);
		memset (block, 0, blockLength/8);
		cipherInst.blockLen = blockLength;
		r = cipherInit (&cipherInst, MODE_ECB, NULL);
		if (TRUE != r) {
			fprintf(stderr,"cipherInit error %d\n",r);
			exit(-1);
		}
		r = blockEncrypt(&cipherInst, &keyInst, block, blockLength, block);
		if (blockLength != r) {
			fprintf(stderr,"blockEncrypt error %d\n",r);
			exit(-1);
		}
		blockPrint (fp, block, blockLength, "CT");
		/* now check decryption: */
		keyInst.blockLen = blockLength;
		makeKey(&keyInst, DIR_DECRYPT, keyLength, keyMaterial);
		blockDecrypt(&cipherInst, &keyInst, block, blockLength, block);
		for (j = 0; j < blockLength/8; j++) {
			assert (block[j] == 0);
		}
		/* undo changes for the next iteration: */
		keyMaterial[i/4] = (BYTE)'0';
		byteVal =
			(byteVal == '8') ?	'4' :
			(byteVal == '4') ?	'2' :
			(byteVal == '2') ?	'1' :
		/*	(byteVal == '1') */	'8';
	}
	assert (byteVal == (BYTE)'8');
	
#ifdef TRACE_KAT_MCT
	printf (" done.\n");
#endif /* ?TRACE_KAT_MCT */
} /* rijndaelVKKAT */
