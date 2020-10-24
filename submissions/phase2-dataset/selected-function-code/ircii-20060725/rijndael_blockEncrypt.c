int rijndael_blockEncrypt(rijndael_cipherInstance *cipher,
	rijndael_keyInstance *key, BYTE *input, int inputLen, BYTE *outBuffer)
{
	int i, j, t, numBlocks;
	word8 block[4][MAXBC];

	
        /* check parameter consistency: */
        if (key == NULL ||
                key->direction != DIR_ENCRYPT ||
                (key->keyLen != 128 && key->keyLen != 192 && key->keyLen != 256)) {
                return BAD_KEY_MAT;
        }
        if (cipher == NULL ||
                (cipher->mode != MODE_ECB && cipher->mode != MODE_CBC && cipher->mode != MODE_CFB1) ||
                (cipher->blockLen != 128 && cipher->blockLen != 192 && cipher->blockLen != 256)) {
                return BAD_CIPHER_STATE;
        }


	numBlocks = inputLen/cipher->blockLen;
	
	switch (cipher->mode) {
	case MODE_ECB: 
		for (i = 0; i < numBlocks; i++) {
			for (j = 0; j < cipher->blockLen/32; j++) {
				for(t = 0; t < 4; t++)
				/* parse input stream into rectangular array */
					block[t][j] = input[cipher->blockLen/8*i+4*j+t] & 0xFF;
			}
			rijndaelEncrypt (block, key->keyLen, cipher->blockLen, key->keySched);
			for (j = 0; j < cipher->blockLen/32; j++) {
				/* parse rectangular array into output ciphertext bytes */
				for(t = 0; t < 4; t++)
					outBuffer[cipher->blockLen/8*i+4*j+t] = (BYTE) block[t][j];
			}
		}
		break;
		
	case MODE_CBC:
		for (j = 0; j < cipher->blockLen/32; j++) {
			for(t = 0; t < 4; t++)
			/* parse initial value into rectangular array */
				block[t][j] = cipher->IV[t+4*j] & 0xFF;
		}
		for (i = 0; i < numBlocks; i++) {
			for (j = 0; j < cipher->blockLen/32; j++) {
				for(t = 0; t < 4; t++)
				/* parse input stream into rectangular array and exor with 
				   IV or the previous ciphertext */
					block[t][j] ^= input[cipher->blockLen/8*i+4*j+t] & 0xFF;
			}
			rijndaelEncrypt (block, key->keyLen, cipher->blockLen, key->keySched);
			for (j = 0; j < cipher->blockLen/32; j++) {
				/* parse rectangular array into output ciphertext bytes */
				for(t = 0; t < 4; t++)
					outBuffer[cipher->blockLen/8*i+4*j+t] = (BYTE) block[t][j];
			}
		}
		break;
	
	default: return BAD_CIPHER_STATE;
	}
	
	return numBlocks*cipher->blockLen;
}
