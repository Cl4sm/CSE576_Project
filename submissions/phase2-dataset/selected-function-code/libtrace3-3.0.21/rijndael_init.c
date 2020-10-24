int rijndael_init(Mode mode,Direction dir,const UINT8 * key,KeyLength keyLen,UINT8 * initVector)
{
        int i;
        UINT32 uKeyLenInBytes;
        UINT8 keyMatrix[_MAX_KEY_COLUMNS][4];
        UINT32 j; 

        /* Not initialized yet */
        m_state = Invalid;

        /* Check the mode */
        if((mode != CBC) && (mode != ECB) && (mode != CFB1))return RIJNDAEL_UNSUPPORTED_MODE;
        m_mode = mode;

        /* And the direction */
        if((dir != Encrypt) && (dir != Decrypt))return RIJNDAEL_UNSUPPORTED_DIRECTION;
        m_direction = dir;

        /* Allow to set an init vector */
        if(initVector)
        {
                /* specified init vector */
                for(i = 0;i < MAX_IV_SIZE;i++)
                {
                        m_initVector[i] = initVector[i];
                }
        } else {
                /* zero init vector */
                for(i = 0;i < MAX_IV_SIZE;i++)
                {
                        m_initVector[i] = 0;
                }
        }


        /* And check the key length */
        switch(keyLen)
        {
                case Key16Bytes:
                        uKeyLenInBytes = 16;
                        m_uRounds = 10;
                        break;
                case Key24Bytes:
                        uKeyLenInBytes = 24;
                        m_uRounds = 12;
                        break;
                case Key32Bytes:
                        uKeyLenInBytes = 32;
                        m_uRounds = 14;
                        break;
                default:
                        return RIJNDAEL_UNSUPPORTED_KEY_LENGTH;
                        break;
        }
        /* The number of rounds is calculated as
        // m_uRounds = (m_uKeyLenInBits / 32) + 6;
	*/

        if(!key)return RIJNDAEL_BAD_KEY;


        for(j = 0; j < uKeyLenInBytes; j++)
                keyMatrix[j >> 2][j & 3] = key[j]; 

        keySched(keyMatrix);

        if(m_direction == Decrypt) keyEncToDec();

        m_state = Valid;

        return RIJNDAEL_SUCCESS;
}