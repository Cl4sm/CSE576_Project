void r_encrypt(const UINT8 a[16], UINT8 b[16])
{
        int r;
	UINT32 temp[4];
	UINT32 *a32 = (UINT32 *)a;
	UINT32 *b32 = (UINT32 *)b;
	UINT32 *mkey;
	UINT8 *tempb;

	mkey = (UINT32 *)m_expandedKey[0][0];
	temp[0] = a32[0] ^ *mkey;
	mkey = (UINT32 *)m_expandedKey[0][1];
	temp[1] = a32[1] ^ *mkey;
	mkey = (UINT32 *)m_expandedKey[0][2];
	temp[2] = a32[2] ^ *mkey;
	mkey = (UINT32 *)m_expandedKey[0][3];
	temp[3] = a32[3] ^ *mkey;

	tempb = (UINT8 *)temp;
	b32[0] = encrypt_b_from_T(tempb[0], tempb[5], tempb[10], tempb[15]);
	b32[1] = encrypt_b_from_T(tempb[4], tempb[9], tempb[14], tempb[3]);
	b32[2] = encrypt_b_from_T(tempb[8], tempb[13], tempb[2], tempb[7]);
	b32[3] = encrypt_b_from_T(tempb[12], tempb[1], tempb[6], tempb[11]);
	
        for(r = 1; r < (int)m_uRounds-1; r++)
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
		b32[0] = encrypt_b_from_T(tempb[0], tempb[5], tempb[10], tempb[15]);
		b32[1] = encrypt_b_from_T(tempb[4], tempb[9], tempb[14], tempb[3]);
		b32[2] = encrypt_b_from_T(tempb[8], tempb[13], tempb[2], tempb[7]);
		b32[3] = encrypt_b_from_T(tempb[12], tempb[1], tempb[6], tempb[11]);
        }
	mkey = (UINT32 *)m_expandedKey[m_uRounds - 1][0];
	temp[0] = b32[0] ^ *mkey;
	mkey = (UINT32 *)m_expandedKey[m_uRounds - 1][1];
	temp[1] = b32[1] ^ *mkey;
	mkey = (UINT32 *)m_expandedKey[m_uRounds - 1][2];
	temp[2] = b32[2] ^ *mkey;
	mkey = (UINT32 *)m_expandedKey[m_uRounds - 1][3];
	temp[3] = b32[3] ^ *mkey;
        
	tempb = (UINT8 *)temp;
        b[ 0] = T1[tempb[0]][1];
        b[ 1] = T1[tempb[5]][1];
        b[ 2] = T1[tempb[10]][1];
        b[ 3] = T1[tempb[15]][1];
        b[ 4] = T1[tempb[4]][1];
        b[ 5] = T1[tempb[9]][1];
        b[ 6] = T1[tempb[14]][1];
        b[ 7] = T1[tempb[3]][1];
        b[ 8] = T1[tempb[8]][1];
        b[ 9] = T1[tempb[13]][1];
        b[10] = T1[tempb[2]][1];
        b[11] = T1[tempb[7]][1];
        b[12] = T1[tempb[12]][1];
        b[13] = T1[tempb[1]][1];
        b[14] = T1[tempb[6]][1];
        b[15] = T1[tempb[11]][1];
	
	mkey = (UINT32 *)m_expandedKey[m_uRounds][0];
	b32[0] ^= *mkey;
	mkey = (UINT32 *)m_expandedKey[m_uRounds][1];
	b32[1] ^= *mkey;
	mkey = (UINT32 *)m_expandedKey[m_uRounds][2];
	b32[2] ^= *mkey;
	mkey = (UINT32 *)m_expandedKey[m_uRounds][3];
	b32[3] ^= *mkey;


}