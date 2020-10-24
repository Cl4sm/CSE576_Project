int rijndaelDecrypt (word8 a[4][MAXBC], int keyBits, int blockBits, word8 rk[MAXROUNDS+1][4][MAXBC])
{
	int r, BC, ROUNDS;
	
	switch (blockBits) {
	case 128: BC = 4; break;
	case 192: BC = 6; break;
	case 256: BC = 8; break;
	default : return (-2);
	}

	switch (keyBits >= blockBits ? keyBits : blockBits) {
	case 128: ROUNDS = 10; break;
	case 192: ROUNDS = 12; break;
	case 256: ROUNDS = 14; break;
	default : return (-3); /* this cannot happen */
	}

	/* To decrypt: apply the inverse operations of the encrypt routine,
	 *             in opposite order
	 * 
	 * (KeyAddition is an involution: it 's equal to its inverse)
	 * (the inverse of Substitution with table S is Substitution with the inverse table of S)
	 * (the inverse of Shiftrow is Shiftrow over a suitable distance)
	 */

        /* First the special round:
	 *   without InvMixColumn
	 *   with extra KeyAddition
	 */
	KeyAddition(a,rk[ROUNDS],BC);
	Substitution(a,Si,BC);
	ShiftRow(a,1,BC);              
	
	/* ROUNDS-1 ordinary rounds
	 */
	for(r = ROUNDS-1; r > 0; r--) {
		KeyAddition(a,rk[r],BC);
		InvMixColumn(a,BC);      
		Substitution(a,Si,BC);
		ShiftRow(a,1,BC);                
	}
	
	/* End with the extra key addition
	 */
	
	KeyAddition(a,rk[0],BC);    

	return 0;
}
