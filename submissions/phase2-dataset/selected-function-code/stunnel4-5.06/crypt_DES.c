NOEXPORT void crypt_DES(DES_cblock dst, const_DES_cblock src, DES_cblock hash) {
    DES_cblock key;
    DES_key_schedule sched;

    /* convert key from 56 to 64 bits */
    key[0]=hash[0];
    key[1]=((hash[0]&1)<<7)|(hash[1]>>1);
    key[2]=((hash[1]&3)<<6)|(hash[2]>>2);
    key[3]=((hash[2]&7)<<5)|(hash[3]>>3);
    key[4]=((hash[3]&15)<<4)|(hash[4]>>4);
    key[5]=((hash[4]&31)<<3)|(hash[5]>>5);
    key[6]=((hash[5]&63)<<2)|(hash[6]>>6);
    key[7]=((hash[6]&127)<<1);
    DES_set_odd_parity(&key);

    /* encrypt */
    DES_set_key_unchecked(&key, &sched);
    DES_ecb_encrypt((const_DES_cblock *)src,
        (DES_cblock *)dst, &sched, DES_ENCRYPT);
}
