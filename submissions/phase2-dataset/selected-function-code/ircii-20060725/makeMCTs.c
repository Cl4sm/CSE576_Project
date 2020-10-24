static void makeMCTs (const char *ecbEncryptionFile, const char *ecbDecryptionFile,
	const char *cbcEncryptionFile, const char *cbcDecryptionFile)
{
	FILE *fp;

	/* prepare ECB Encryption Monte Carlo Tests: */
	fp = fopen (ecbEncryptionFile, "w");
	fprintf (fp,
		"\n"
		"=========================\n"
		"\n"
		"FILENAME:  \"%s\"\n"
		"\n"
		"Electronic Codebook (ECB) Mode - ENCRYPTION\n"
		"Monte Carlo Test\n"
		"\n"
		"Algorithm Name: Rijndael\n"
		"Principal Submitter: %s\n",
		ecbEncryptionFile,SUBMITTER);
	fflush (fp);

	rijndaelECB_MCT (fp,
		"00000000000000000000000000000000", 128,
		"00000000000000000000000000000000", BITSPERBLOCK, DIR_ENCRYPT);

	rijndaelECB_MCT (fp,
		"000000000000000000000000000000000000000000000000", 192,
		"00000000000000000000000000000000", BITSPERBLOCK, DIR_ENCRYPT);

	rijndaelECB_MCT (fp,
		"0000000000000000000000000000000000000000000000000000000000000000", 256,
		"00000000000000000000000000000000", BITSPERBLOCK, DIR_ENCRYPT);

	fprintf (fp,
		"\n"
		"===========");
	fclose (fp);

	/* prepare ECB Decryption Monte Carlo Tests: */
	fp = fopen (ecbDecryptionFile, "w");
	fprintf (fp,
		"\n"
		"=========================\n"
		"\n"
		"FILENAME:  \"%s\"\n"
		"\n"
		"Electronic Codebook (ECB) Mode - DECRYPTION\n"
		"Monte Carlo Test\n"
		"\n"
		"Algorithm Name: Rijndael\n"
		"Principal Submitter: %s\n",
		ecbDecryptionFile,SUBMITTER);
	fflush (fp);

	rijndaelECB_MCT (fp,
		"00000000000000000000000000000000", 128,
		"00000000000000000000000000000000", BITSPERBLOCK, DIR_DECRYPT);

	rijndaelECB_MCT (fp,
		"000000000000000000000000000000000000000000000000", 192,
		"00000000000000000000000000000000", BITSPERBLOCK, DIR_DECRYPT);

	rijndaelECB_MCT (fp,
		"0000000000000000000000000000000000000000000000000000000000000000", 256,
		"00000000000000000000000000000000", BITSPERBLOCK, DIR_DECRYPT);

	fprintf (fp,
		"\n"
		"===========");
	fclose (fp);

	/* prepare CBC Encryption Monte Carlo Tests: */
	fp = fopen (cbcEncryptionFile, "w");
	fprintf (fp,
		"\n"
		"=========================\n"
		"\n"
		"FILENAME:  \"%s\"\n"
		"\n"
		"Cipher Block Chaining (CBC) Mode - ENCRYPTION\n"
		"Monte Carlo Test\n"
		"\n"
		"Algorithm Name: Rijndael\n"
		"Principal Submitter: %s\n",
		cbcEncryptionFile,SUBMITTER);
	fflush (fp);

	rijndaelCBC_MCT (fp,
		"00000000000000000000000000000000", 128,
		"00000000000000000000000000000000", "00000000000000000000000000000000", BITSPERBLOCK,
		DIR_ENCRYPT);

	rijndaelCBC_MCT (fp,
		"000000000000000000000000000000000000000000000000", 192,
		"00000000000000000000000000000000", "00000000000000000000000000000000", BITSPERBLOCK,
		DIR_ENCRYPT);

	rijndaelCBC_MCT (fp,
		"0000000000000000000000000000000000000000000000000000000000000000", 256,
		"00000000000000000000000000000000", "00000000000000000000000000000000", BITSPERBLOCK,
		DIR_ENCRYPT);

	fprintf (fp,
		"\n"
		"===========");
	fclose (fp);
 
	/* prepare CBC Decryption Monte Carlo Tests: */
	fp = fopen (cbcDecryptionFile, "w");
	fprintf (fp,
		"\n"
		"=========================\n"
		"\n"
		"FILENAME:  \"%s\"\n"
		"\n"
		"Cipher Block Chaining (CBC) Mode - DECRYPTION\n"
		"Monte Carlo Test\n"
		"\n"
		"Algorithm Name: Rijndael\n"
		"Principal Submitter: %s\n",
		cbcDecryptionFile,SUBMITTER);
	fflush (fp);

	rijndaelCBC_MCT (fp,
		"00000000000000000000000000000000", 128,
		"00000000000000000000000000000000", "00000000000000000000000000000000", BITSPERBLOCK,
		DIR_DECRYPT);

	rijndaelCBC_MCT (fp,
		"000000000000000000000000000000000000000000000000", 192,
		"00000000000000000000000000000000", "00000000000000000000000000000000", BITSPERBLOCK,
		DIR_DECRYPT);

	rijndaelCBC_MCT (fp,
		"0000000000000000000000000000000000000000000000000000000000000000", 256,
		"00000000000000000000000000000000", "00000000000000000000000000000000", BITSPERBLOCK,
		DIR_DECRYPT);

	fprintf (fp,
		"\n"
		"===========");
	fclose (fp);
	
} /* makeMCTs */
