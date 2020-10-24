	word8 rk[MAXROUNDS+1][4][MAXBC], int rounds)
/* Decrypt only a certain number of rounds.
 * Only used in the Intermediate Value Known Answer Test.
 * Operations rearranged such that the intermediate values
 * of decryption correspond with the intermediate values
 * of encryption.
 */
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


	/* make number of rounds sane */
	if (rounds > ROUNDS) rounds = ROUNDS;

        /* First the special round:
	 *   without InvMixColumn
	 *   with extra KeyAddition
	 */
	KeyAddition(a,rk[ROUNDS],BC);
	Substitution(a,Si,BC);
	ShiftRow(a,1,BC);              
	
	/* ROUNDS-1 ordinary rounds
	 */
	for(r = ROUNDS-1; r > rounds; r--) {
		KeyAddition(a,rk[r],BC);
		InvMixColumn(a,BC);      
		Substitution(a,Si,BC);
		ShiftRow(a,1,BC);                
	}
	
	if (rounds == 0) {
		/* End with the extra key addition
		 */	
		KeyAddition(a,rk[0],BC);
	}    

	return 0;
}
