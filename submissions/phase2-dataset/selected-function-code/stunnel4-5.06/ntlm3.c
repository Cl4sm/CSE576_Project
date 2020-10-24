NOEXPORT char *ntlm3(char *username, char *password, char *phase2) {
    MD4_CTX md4;
    char *decoded; /* decoded reply from proxy */
    char phase3[146];
    unsigned char md4_hash[21];
    unsigned int userlen=strlen(username);
    unsigned int phase3len=s_min(88+userlen, sizeof phase3);

    /* setup phase3 structure */
    memset(phase3, 0, sizeof phase3);
    strcpy(phase3, "NTLMSSP");
    phase3[8]=3;            /* type: 3 */
    phase3[16]=phase3len;   /* LM-resp off */
    phase3[20]=24;          /* NT-resp len */
    phase3[22]=24;          /* NT-Resp len */
    phase3[24]=64;          /* NT-resp off */
    phase3[32]=phase3len;   /* domain offset */
    phase3[36]=userlen;     /* user length */
    phase3[38]=userlen;     /* user length */
    phase3[40]=88;          /* user offset */
    phase3[48]=phase3len;   /* host offset */
    phase3[56]=phase3len;   /* message len */
    phase3[60]=2;           /* flag: negotiate OEM */
    phase3[61]=2;           /* flag: negotiate NTLM */

    /* calculate MD4 of UTF-16 encoded password */
    MD4_Init(&md4);
    while(*password) {
        MD4_Update(&md4, password++, 1);
        MD4_Update(&md4, "", 1); /* UTF-16 */
    }
    MD4_Final(md4_hash, &md4);
    memset(md4_hash+16, 0, 5); /* pad to 21 bytes */

    /* decode challenge and calculate response */
    decoded=base64(0, phase2, strlen(phase2)); /* decode */
    if(!decoded)
        return NULL;
    crypt_DES((unsigned char *)phase3+64,
        (unsigned char *)decoded+24, md4_hash);
    crypt_DES((unsigned char *)phase3+72,
        (unsigned char *)decoded+24, md4_hash+7);
    crypt_DES((unsigned char *)phase3+80,
        (unsigned char *)decoded+24, md4_hash+14);
    str_free(decoded);

    strncpy(phase3+88, username, sizeof phase3-88);

    return base64(1, phase3, phase3len); /* encode */
}
