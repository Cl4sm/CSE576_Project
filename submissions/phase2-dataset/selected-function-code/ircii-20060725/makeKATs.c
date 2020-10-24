static void makeKATs (const char *vkFile, const char *vtFile, const char *tblFile,
                      const char *ivFile)
{
	FILE *fp, *fp2;

	/* prepare Variable Key Known Answer Tests: */
	fp = fopen (vkFile, "w");
	fprintf (fp,
		"\n"
		"=========================\n"
		"\n"
		"FILENAME:  \"%s\"\n"
		"\n"
		"Electronic Codebook (ECB) Mode\n"
		"Variable Key Known Answer Tests\n"
		"\n"
		"Algorithm Name: Rijndael\n"
		"Principal Submitter: %s\n",
		vkFile,SUBMITTER);
	fflush (fp);

	rijndaelVKKAT (fp, 128, BITSPERBLOCK); /* test for 128-bit key */
	rijndaelVKKAT (fp, 192, BITSPERBLOCK); /* test for 192-bit key */
	rijndaelVKKAT (fp, 256, BITSPERBLOCK); /* test for 256-bit key */

	fprintf (fp,
		"\n"
		"==========");
	fclose (fp);

	/* prepare Variable Text Known Answer Tests: */
	fp = fopen (vtFile, "w");
	fprintf (fp,
		"\n"
		"=========================\n"
		"\n"
		"FILENAME:  \"%s\"\n"
		"\n"
		"Electronic Codebook (ECB) Mode\n"
		"Variable Text Known Answer Tests\n"
		"\n"
		"Algorithm Name: Rijndael\n"
		"Principal Submitter: %s\n",
		vtFile,SUBMITTER);
	fflush (fp);

	rijndaelVTKAT (fp, 128, BITSPERBLOCK);
	rijndaelVTKAT (fp, 192, BITSPERBLOCK);
	rijndaelVTKAT (fp, 256, BITSPERBLOCK);

	fprintf (fp,
		"\n"
		"==========");
	fclose (fp);

	/* prepare Tables Known Answer Tests: */
	fp = fopen (tblFile, "w");
	fprintf (fp,
	        "/* Description of what tables are tested:\n"
		"   The provided implementations each use a different set of tables\n"
		"    - Java implementation: uses no tables\n"
		"    - reference C implementation: uses Logtable, Alogtable, S, Si, rcon\n"
		"    - fast C implementation: uses Logtable, Alogtable,  rcon\n"
		"        and additionally, T1, T2, T3, T4, T5, T6, T7, T8\n"
		"        and (for the inverse key schedule only) U1, U2, U3, U4.\n"
		"   All these tables are tested.\n" 
		"\n"
		"=========================\n"
		"\n"
		"FILENAME:  \"%s\"\n"
		"\n"
		"Electronic Codebook (ECB) Mode\n"
		"Tables Known Answer Tests\n"
		"\n"
		"Algorithm Name: Rijndael\n"
		"Principal Submitter: %s\n",
		tblFile,SUBMITTER);
	fflush (fp);


	if (NULL != (fp2 = fopen("table.128","r"))) {
		rijndaelTKAT (fp, 128, 128,fp2);
		fclose(fp2);
	} else {
		printf("Table Known Answer test expects file table.128\n");
		fclose(fp);
		exit (EXIT_FAILURE);		
	}
	if (NULL != (fp2 = fopen("table.192","r"))) {
		rijndaelTKAT (fp, 128, 192,fp2);
		fclose(fp2);
	} else {
		printf("Table Known Answer test expects file table.192\n");
		fclose(fp);
		exit (EXIT_FAILURE);		
	}
	if (NULL != (fp2 = fopen("table.256","r"))) {
		rijndaelTKAT (fp, 128, 256,fp2);
		fclose(fp2);
	} else {
		printf("Table Known Answer test expects file table.192\n");
		fclose(fp);
		exit (EXIT_FAILURE);		
	}
	
	fprintf (fp,
		"\n"
		"==========");
	fclose (fp);

	/* prepare Intermediate Values Known Answer Tests: */
	fp = fopen (ivFile, "w");
	fprintf (fp,
		"\n"
		"=========================\n"
		"\n"
		"FILENAME:  \"%s\"\n"
		"\n"
		"Electronic Codebook (ECB) Mode\n"
		"Intermediate Value Known Answer Tests\n"
		"\n"
		"Algorithm Name: Rijndael\n"
		"Principal Submitter: %s\n",
		ivFile,SUBMITTER);
	fflush (fp);

	rijndaelIVKAT (fp, 128, BITSPERBLOCK);
	rijndaelIVKAT (fp, 192, BITSPERBLOCK);
	rijndaelIVKAT (fp, 256, BITSPERBLOCK);

	fprintf (fp,
		"\n"
		"==========");
	fclose (fp);

}
