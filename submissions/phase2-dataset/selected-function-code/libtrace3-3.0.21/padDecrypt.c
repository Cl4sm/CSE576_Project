int padDecrypt(const UINT8 *input, int inputOctets, UINT8 *outBuffer)
{
        int i, numBlocks, padLen;
        UINT32 block[4];
        UINT32 iv[4];
        UINT8 *blockb = (UINT8 *)block;

        if(m_state != Valid)return RIJNDAEL_NOT_INITIALIZED;
        if(m_direction != Decrypt)return RIJNDAEL_BAD_DIRECTION;

        if(input == 0 || inputOctets <= 0)return 0;

        if((inputOctets % 16) != 0)return RIJNDAEL_CORRUPTED_DATA;

        numBlocks = inputOctets/16;

        switch(m_mode){
                case ECB:
                        for (i = numBlocks - 1; i > 0; i--)
                        {
                                r_decrypt(input, outBuffer);
                                input += 16;
                                outBuffer += 16;
                        }

                        r_decrypt(input, blockb);
                        padLen = blockb[15];
                        if (padLen >= 16)return RIJNDAEL_CORRUPTED_DATA;
                        for(i = 16 - padLen; i < 16; i++)
                        {
                                if(blockb[i] != padLen)return RIJNDAEL_CORRUPTED_DATA;
                        }
                        memcpy(outBuffer, blockb, 16 - padLen);
                        break;
                case CBC:
                        memcpy(iv, m_initVector, 16);
                        /* all blocks but last */
                        for (i = numBlocks - 1; i > 0; i--)
                        {
                                r_decrypt(input, blockb);
                                block[0] ^= iv[0];
                                (block)[1] ^= iv[1];
                                (block)[2] ^= iv[2];
                                (block)[3] ^= iv[3];
                                memcpy(iv, input, 16);
                                memcpy(outBuffer, block, 16);
                                input += 16;
                                outBuffer += 16;
                        }
                        /* last block */
                        r_decrypt(input, blockb);
                        (block)[0] ^= iv[0];
                        (block)[1] ^= iv[1];
                        (block)[2] ^= iv[2];
                        (block)[3] ^= iv[3];
                        padLen = blockb[15];
                        if(padLen <= 0 || padLen > 16)return RIJNDAEL_CORRUPTED_DATA;
                        for(i = 16 - padLen; i < 16; i++)
                        {
                                if(blockb[i] != padLen)return RIJNDAEL_CORRUPTED_DATA;
                        }
                        memcpy(outBuffer, blockb, 16 - padLen);
                        break;

                default:
                        return -1;
                        break;
        }

        return 16*numBlocks - padLen;
}