int rijndaelEncryptRound (word8 a[4][MAXBC], int keyBits, int blockBits, 
		word8 rk[MAXROUNDS+1][4][MAXBC], int rounds)
/* Encrypt only a certain number of rounds.
 * Only used in the Intermediate Value Known Answer Test.
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

	/* begin with a key addition
	 */
	KeyAddition(a,rk[0],BC);
        
	/* at most ROUNDS-1 ordinary rounds
	 */
	for(r = 1; (r <= rounds) && (r < ROUNDS); r++) {
		Substitution(a,S,BC);
		ShiftRow(a,0,BC);
		MixColumn(a,BC);
		KeyAddition(a,rk[r],BC);
	}
	
	/* if necessary, do the last, special, round: 
	 */
	if (rounds == ROUNDS) {
		Substitution(a,S,BC);
		ShiftRow(a,0,BC);
		KeyAddition(a,rk[ROUNDS],BC);
	}

	return 0;
}   
