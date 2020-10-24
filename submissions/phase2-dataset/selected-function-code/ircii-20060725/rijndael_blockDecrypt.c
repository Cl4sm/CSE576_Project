int rijndael_blockDecrypt(rijndael_cipherInstance *cipher,
	rijndael_keyInstance *key, BYTE *input, int inputLen, BYTE *outBuffer)
{
	int i, j, t, numBlocks;
	word8 block[4][MAXBC];

	if (cipher == NULL ||
		key == NULL ||
		key->direction == DIR_ENCRYPT ||
		cipher->blockLen != key->blockLen) {
		return BAD_CIPHER_STATE;
	}

        /* check parameter consistency: */
        if (key == NULL ||
                key->direction != DIR_DECRYPT ||
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
			rijndaelDecrypt (block, key->keyLen, cipher->blockLen, key->keySched);
			for (j = 0; j < cipher->blockLen/32; j++) {
				/* parse rectangular array into output ciphertext bytes */
				for(t = 0; t < 4; t++)
					outBuffer[cipher->blockLen/8*i+4*j+t] = (BYTE) block[t][j];
			}
		}
		break;
		
	case MODE_CBC:
		/* first block */
		for (j = 0; j < cipher->blockLen/32; j++) {
			for(t = 0; t < 4; t++)
			/* parse input stream into rectangular array */
				block[t][j] = input[4*j+t] & 0xFF;
		}
		rijndaelDecrypt (block, key->keyLen, cipher->blockLen, key->keySched);
		
		for (j = 0; j < cipher->blockLen/32; j++) {
			/* exor the IV and parse rectangular array into output ciphertext bytes */
			for(t = 0; t < 4; t++)
				outBuffer[4*j+t] = (BYTE) (block[t][j] ^ cipher->IV[t+4*j]);
		}
		
		/* next blocks */
		for (i = 1; i < numBlocks; i++) {
			for (j = 0; j < cipher->blockLen/32; j++) {
				for(t = 0; t < 4; t++)
				/* parse input stream into rectangular array */
					block[t][j] = input[cipher->blockLen/8*i+4*j+t] & 0xFF;
			}
			rijndaelDecrypt (block, key->keyLen, cipher->blockLen, key->keySched);
			
			for (j = 0; j < cipher->blockLen/32; j++) {
				/* exor previous ciphertext block and parse rectangular array 
				       into output ciphertext bytes */
				for(t = 0; t < 4; t++)
					outBuffer[cipher->blockLen/8*i+4*j+t] = (BYTE) (block[t][j] ^ 
						input[cipher->blockLen/8*i+4*j+t-4*cipher->blockLen/32]);
			}
		}
		break;
	
	default: return BAD_CIPHER_STATE;
	}
	
	return numBlocks*cipher->blockLen;
}
