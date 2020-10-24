void keySched(UINT8 key[_MAX_KEY_COLUMNS][4])
{
        int j,rconpointer = 0;
        int r = 0;
        int t = 0;

        /* Calculate the necessary round keys */
        /* The number of calculations depends on keyBits and blockBits */
        int uKeyColumns = m_uRounds - 6;

        UINT8 tempKey[_MAX_KEY_COLUMNS][4];

        /* Copy the input key to the temporary key matrix */

        for(j = 0;j < uKeyColumns;j++)
        {
                *((UINT32*)(tempKey[j])) = *((UINT32*)(key[j]));
        }


        /* copy values into round key array */
        for(j = 0;(j < uKeyColumns) && (r <= (int)m_uRounds); )
        {
                for(;(j < uKeyColumns) && (t < 4); j++, t++)
                {
                        *((UINT32*)m_expandedKey[r][t]) = *((UINT32*)tempKey[j]);
                }


                if(t == 4)
                {
                        r++;
                        t = 0;
                }
        }

        while(r <= (int)m_uRounds)
        {
                tempKey[0][0] ^= S[tempKey[uKeyColumns-1][1]];
                tempKey[0][1] ^= S[tempKey[uKeyColumns-1][2]];
                tempKey[0][2] ^= S[tempKey[uKeyColumns-1][3]];
                tempKey[0][3] ^= S[tempKey[uKeyColumns-1][0]];
                tempKey[0][0] ^= rcon[rconpointer++];

                if (uKeyColumns != 8)
                {
                        for(j = 1; j < uKeyColumns; j++)
                        {
                                *((UINT32*)tempKey[j]) ^= *((UINT32*)tempKey[j-1]);
                        }
                } else {
                        for(j = 1; j < uKeyColumns/2; j++)
                        {
                                *((UINT32*)tempKey[j]) ^= *((UINT32*)tempKey[j-1]);
                        }
                        tempKey[uKeyColumns/2][0] ^= S[tempKey[uKeyColumns/2 - 1][0]];
                        tempKey[uKeyColumns/2][1] ^= S[tempKey[uKeyColumns/2 - 1][1]];
                        tempKey[uKeyColumns/2][2] ^= S[tempKey[uKeyColumns/2 - 1][2]];
                        tempKey[uKeyColumns/2][3] ^= S[tempKey[uKeyColumns/2 - 1][3]];
                        for(j = uKeyColumns/2 + 1; j < uKeyColumns; j++)
                        {
                                *((UINT32*)tempKey[j]) ^= *((UINT32*)tempKey[j-1]);
                        }
                }
                for(j = 0; (j < uKeyColumns) && (r <= (int)m_uRounds); )
                {
                        for(; (j < uKeyColumns) && (t < 4); j++, t++)
                        {
                                *((UINT32*)m_expandedKey[r][t]) = *((UINT32*)tempKey[j]);
                        }
                        if(t == 4)
                        {
                                r++;
                                t = 0;
                        }
                }
        }		
}