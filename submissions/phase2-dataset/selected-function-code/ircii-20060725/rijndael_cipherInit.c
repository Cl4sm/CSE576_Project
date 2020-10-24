int rijndael_cipherInit(rijndael_cipherInstance *cipher, BYTE mode, char *IV)
{
	int i, j, t;
	
	if ((mode == MODE_ECB) || (mode == MODE_CBC) || (mode == MODE_CFB1)) {
		cipher->mode = mode;
	} else {
		return BAD_CIPHER_MODE;
	}
	
	if (IV != NULL) {
 		for(i = 0; i < cipher->blockLen/8; i++) {		
			t = IV[2*i];
			if ((t >= '0') && (t <= '9')) j = (t - '0') << 4;
			else if ((t >= 'a') && (t <= 'f')) j = (t - 'a' + 10) << 4; 
			else if ((t >= 'A') && (t <= 'F')) j = (t - 'A' + 10) << 4; 
			else return BAD_CIPHER_INSTANCE;
		
			t = IV[2*i+1];
			if ((t >= '0') && (t <= '9')) j ^= (t - '0');
			else if ((t >= 'a') && (t <= 'f')) j ^= (t - 'a' + 10); 
			else if ((t >= 'A') && (t <= 'F')) j ^= (t - 'A' + 10); 
			else return BAD_CIPHER_INSTANCE;
			
			cipher->IV[i] = (BYTE) j;
		} 
	}

	return TRUE;
}
