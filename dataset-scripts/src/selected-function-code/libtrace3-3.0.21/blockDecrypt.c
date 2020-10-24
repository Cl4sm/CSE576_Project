int blockDecrypt(const UINT8 *input, int inputLen, UINT8 *outBuffer)
{
        int i, k, numBlocks;
        UINT32 block[4], iv[4], *miv, *input32;
        UINT8 *ivb;

        miv = (UINT32 *)m_initVector;
        input32 = (UINT32 *)input;


        if(m_state != Valid)return RIJNDAEL_NOT_INITIALIZED;
        if((m_mode != CFB1) && (m_direction == Encrypt))return RIJNDAEL_BAD_DIRECTION;

        if (input == 0 || inputLen <= 0)return 0;

        numBlocks = inputLen/128;

        switch(m_mode)
        {
                case ECB:
                        for (i = numBlocks; i > 0; i--)
                        {
                                r_decrypt(input,outBuffer);
                                input += 16;
                                input32 += 4;
                                outBuffer += 16;
                        }
                        break;
                case CBC:
#if STRICT_ALIGN 
                        memcpy(iv,m_initVector,16);
#else
                        iv[0] = miv[0];
                        iv[1] = miv[1];
                        iv[2] = miv[2];
                        iv[3] = miv[3];
#endif
                        for (i = numBlocks; i > 0; i--)
                        {
                                r_decrypt(input, (UINT8 *)block);
                                block[0] ^= iv[0];
                                block[1] ^= iv[1];
                                block[2] ^= iv[2];
                                block[3] ^= iv[3];
#if STRICT_ALIGN
                                memcpy(iv, input, 16);
                                memcpy(outBuf, block, 16);
#else
iv[0] = input32[0];
                                ((UINT32*)outBuffer)[0] = block[0];
                                iv[1] = input32[1];
                                ((UINT32*)outBuffer)[1] = block[1];
                                iv[2] = input32[2];
                                ((UINT32*)outBuffer)[2] = block[2];
                                iv[3] = input32[3];
                                ((UINT32*)outBuffer)[3] = block[3];

#endif
                                input += 16;
                                input32 += 16;
                                outBuffer += 16;
                        }
                        break;
                case CFB1:
#if STRICT_ALIGN 
                        memcpy(iv, m_initVector, 16);
#else
                        iv[0] = miv[0];
                        iv[1] = miv[1];
                        iv[2] = miv[2];
                        iv[3] = miv[3];
#endif
                        for(i = numBlocks; i > 0; i--)
                        {
                                for(k = 0; k < 128; k++)
                                {
                                        block[0] = iv[0];
                                        block[1] = iv[1];
                                        block[2] = iv[2];
                                        block[3] = iv[3];
                                        r_encrypt((UINT8 *)block, (UINT8 *)block);
                                        ivb = (UINT8 *)iv;
ivb[0] = (ivb[0] << 1) | (ivb[1] >> 7);
                                        ivb[1] = (ivb[1] << 1) | (ivb[2] >> 7);
                                        ivb[2] = (ivb[2] << 1) | (ivb[3] >> 7);
                                        ivb[3] = (ivb[3] << 1) | (ivb[4] >> 7);
                                        ivb[4] = (ivb[4] << 1) | (ivb[5] >> 7);
                                        ivb[5] = (ivb[5] << 1) | (ivb[6] >> 7);
                                        ivb[6] = (ivb[6] << 1) | (ivb[7] >> 7);
                                        ivb[7] = (ivb[7] << 1) | (ivb[8] >> 7);
                                        ivb[8] = (ivb[8] << 1) | (ivb[9] >> 7);
                                        ivb[9] = (ivb[9] << 1) | (ivb[10] >> 7);
                                        ivb[10] = (ivb[10] << 1) | (ivb[11] >> 7);
                                        ivb[11] = (ivb[11] << 1) | (ivb[12] >> 7);
                                        ivb[12] = (ivb[12] << 1) | (ivb[13] >> 7);
                                        ivb[13] = (ivb[13] << 1) | (ivb[14] >> 7);
                                        ivb[14] = (ivb[14] << 1) | (ivb[15] >> 7);
                                        ivb[15] = (ivb[15] << 1) | ((input[k >> 3] >> (7-(k&7))) & 1);
                                        outBuffer[k/8] ^= (((UINT8 *)block)[0] & 0x80) >> (k & 7);
                                }
                        }
                        break;
                default:
                        return -1;
                        break;
        }

        return 128*numBlocks;
}