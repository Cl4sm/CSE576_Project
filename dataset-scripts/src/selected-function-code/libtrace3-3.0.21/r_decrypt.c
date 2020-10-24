void r_decrypt(const UINT8 a[16], UINT8 b[16])
{
        int r;
	UINT32 temp[4];
	UINT32 *a32 = (UINT32 *)a;
	UINT32 *b32 = (UINT32 *)b;
	UINT32 *mkey;
	UINT8 *tempb;
	
	mkey = (UINT32 *)m_expandedKey[m_uRounds][0];
	temp[0] = a32[0] ^ *mkey;
	mkey = (UINT32 *)m_expandedKey[m_uRounds][1];
	temp[1] = a32[1] ^ *mkey;
	mkey = (UINT32 *)m_expandedKey[m_uRounds][2];
	temp[2] = a32[2] ^ *mkey;
	mkey = (UINT32 *)m_expandedKey[m_uRounds][3];
	temp[3] = a32[3] ^ *mkey;

	tempb = (UINT8 *)temp;
	b32[0] = decrypt_b_from_T(tempb[0], tempb[13], tempb[10], tempb[7]);
	b32[1] = decrypt_b_from_T(tempb[4], tempb[1], tempb[14], tempb[11]);
	b32[2] = decrypt_b_from_T(tempb[8], tempb[5], tempb[2], tempb[15]);
	b32[3] = decrypt_b_from_T(tempb[12], tempb[9], tempb[6], tempb[3]);

        for(r = m_uRounds-1; r > 1; r--)
        {
		mkey = (UINT32 *)m_expandedKey[r][0];
                temp[0] = b32[0] ^ *mkey;
		mkey = (UINT32 *)m_expandedKey[r][1];
                temp[1] = b32[1] ^ *mkey;
		mkey = (UINT32 *)m_expandedKey[r][2];
                temp[2] = b32[2] ^ *mkey;
		mkey = (UINT32 *)m_expandedKey[r][3];
                temp[3] = b32[3] ^ *mkey;
                
		tempb = (UINT8 *)temp;
		b32[0] = decrypt_b_from_T(tempb[0], tempb[13], tempb[10], tempb[7]);
		b32[1] = decrypt_b_from_T(tempb[4], tempb[1], tempb[14], tempb[11]);
		b32[2] = decrypt_b_from_T(tempb[8], tempb[5], tempb[2], tempb[15]);
		b32[3] = decrypt_b_from_T(tempb[12], tempb[9], tempb[6], tempb[3]);
		
        }
	
	mkey = (UINT32 *)m_expandedKey[1][0];
	temp[0] = b32[0] ^ *mkey;
	mkey = (UINT32 *)m_expandedKey[1][1];
	temp[1] = b32[1] ^ *mkey;
	mkey = (UINT32 *)m_expandedKey[1][2];
	temp[2] = b32[2] ^ *mkey;
	mkey = (UINT32 *)m_expandedKey[1][3];
	temp[3] = b32[3] ^ *mkey;

	tempb = (UINT8 *)temp;
        b[ 0] = S5[tempb[0]];
        b[ 1] = S5[tempb[13]];
        b[ 2] = S5[tempb[10]];
        b[ 3] = S5[tempb[7]];
        b[ 4] = S5[tempb[4]];
        b[ 5] = S5[tempb[1]];
        b[ 6] = S5[tempb[14]];
        b[ 7] = S5[tempb[11]];
        b[ 8] = S5[tempb[8]];
        b[ 9] = S5[tempb[5]];
        b[10] = S5[tempb[2]];
        b[11] = S5[tempb[15]];
        b[12] = S5[tempb[12]];
        b[13] = S5[tempb[9]];
        b[14] = S5[tempb[6]];
        b[15] = S5[tempb[3]];
	
	mkey = (UINT32 *)m_expandedKey[0][0];
	b32[0] ^= *mkey;
	mkey = (UINT32 *)m_expandedKey[0][1];
	b32[1] ^= *mkey;
	mkey = (UINT32 *)m_expandedKey[0][2];
	b32[2] ^= *mkey;
	mkey = (UINT32 *)m_expandedKey[0][3];
	b32[3] ^= *mkey;

}