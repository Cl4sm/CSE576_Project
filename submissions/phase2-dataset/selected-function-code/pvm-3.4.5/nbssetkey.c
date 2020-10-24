nbssetkey(key)
char *key;
{
	register i, j, k;
	int t;

	/*
	 * First, generate C and D by permuting
	 * the key.  The low order bit of each
	 * 8-bit char is not used, so C and D are only 28
	 * bits apiece.
	 */
	for (i=0; i<28; i++) {
		C[i] = key[PC1_C[i]-1];
		D[i] = key[PC1_D[i]-1];
	}
	/*
	 * To generate Ki, rotate C and D according
	 * to schedule and pick up a permutation
	 * using PC2.
	 */
	for (i=0; i<16; i++) {
		/*
		 * rotate.
		 */
		for (k=0; k<shifts[i]; k++) {
			t = C[0];
			for (j=0; j<28-1; j++)
				C[j] = C[j+1];
			C[27] = t;
			t = D[0];
			for (j=0; j<28-1; j++)
				D[j] = D[j+1];
			D[27] = t;
		}
		/*
		 * get Ki. Note C and D are concatenated.
		 */
		for (j=0; j<24; j++) {
			KS[i][j] = C[PC2_C[j]-1];
			KS[i][j+24] = D[PC2_D[j]-28-1];
		}
	}
}
