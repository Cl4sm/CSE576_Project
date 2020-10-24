{
	/* Encryption of one block. 
	 */
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

	/* begin with a key addition
	 */
	KeyAddition(a,rk[0],BC); 

        /* ROUNDS-1 ordinary rounds
	 */
	for(r = 1; r < ROUNDS; r++) {
		Substitution(a,S,BC);
		ShiftRow(a,0,BC);
		MixColumn(a,BC);
		KeyAddition(a,rk[r],BC);
	}
	
	/* Last round is special: there is no MixColumn
	 */
	Substitution(a,S,BC);
	ShiftRow(a,0,BC);
	KeyAddition(a,rk[ROUNDS],BC);

	return 0;
}   
