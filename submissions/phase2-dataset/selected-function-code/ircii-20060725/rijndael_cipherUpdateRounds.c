int rijndael_cipherUpdateRounds(rijndael_cipherInstance *cipher,
	rijndael_keyInstance *key, BYTE *input, int inputLen, BYTE *outBuffer, int rounds)
{
	int j, t;
	word8 block[4][MAXBC];

	if (cipher == NULL ||
		key == NULL ||
		cipher->blockLen != key->blockLen) {
		return BAD_CIPHER_STATE;
	}

	for (j = 0; j < cipher->blockLen/32; j++) {
		for(t = 0; t < 4; t++)
			/* parse input stream into rectangular array */
			block[t][j] = input[4*j+t] & 0xFF;
	}
	switch (key->direction) {
	case DIR_ENCRYPT:
		rijndaelEncryptRound (block, key->keyLen, cipher->blockLen, 
				key->keySched, rounds);
	break;
		
	case DIR_DECRYPT:
		rijndaelDecryptRound (block, key->keyLen, cipher->blockLen, 
				key->keySched, rounds);
	break;
		
	default: return BAD_KEY_DIR;
	} 
	for (j = 0; j < cipher->blockLen/32; j++) {
		/* parse rectangular array into output ciphertext bytes */
		for(t = 0; t < 4; t++)
			outBuffer[4*j+t] = (BYTE) block[t][j];
	}
	
	return TRUE;
}
