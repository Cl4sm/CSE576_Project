int padEncrypt(const UINT8 *input, int inputOctets, UINT8 *outBuffer)
{
        int i, numBlocks, padLen;
        UINT32 block[4], *iv;
        UINT32 *input32 = (UINT32 *)input;
        UINT8 *ivb, *blockb;

        if(m_state != Valid)return RIJNDAEL_NOT_INITIALIZED;
        if(m_direction != Encrypt)return RIJNDAEL_NOT_INITIALIZED;

        if(input == 0 || inputOctets <= 0)return 0;

        numBlocks = inputOctets/16;

        switch(m_mode)
        {
                case ECB:
                        for(i = numBlocks; i > 0; i--)
                        {
                                r_encrypt(input, outBuffer);
                                input += 16;
                                input32 += 4;
                                outBuffer += 16;
                        }
                        padLen = 16 - (inputOctets - 16*numBlocks);
                        memcpy(block, input, 16 - padLen);
                        memset(block + 4 - padLen, padLen, padLen);
                        r_encrypt((UINT8 *)block,outBuffer);
                        break;
                case CBC:
                        iv = (UINT32 *)m_initVector;
                        for(i = numBlocks; i > 0; i--)
                        {
                                (block)[0] = (input32)[0] ^ (iv)[0];
                                (block)[1] = (input32)[1] ^ (iv)[1];
                                (block)[2] = (input32)[2] ^ (iv)[2];
                                (block)[3] = (input32)[3] ^ (iv)[3];
                                r_encrypt((UINT8 *)block, outBuffer);
                                iv = (UINT32 *)outBuffer;
                                input += 16;
                                outBuffer += 16;
                        }
                        padLen = 16 - (inputOctets - 16*numBlocks);
                        blockb = (UINT8 *)block;
                        ivb = (UINT8 *)iv;
                        for (i = 0; i < 16 - padLen; i++) {
                                blockb[i] = input[i] ^ ivb[i];
                        }
                        for (i = 16 - padLen; i < 16; i++) {
                                blockb[i] = (UINT8)padLen ^ ivb[i];
                        }
                        r_encrypt((UINT8 *)block,outBuffer);
                        break;
                default:
                        return -1;
                        break;
        }

        return 16*(numBlocks + 1);
}