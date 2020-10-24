    static unsigned char dh2048_p[]={ /* OpenSSL DH parameters */
        0xED,0x92,0x89,0x35,0x82,0x45,0x55,0xCB,0x3B,0xFB,0xA2,0x76,
        0x5A,0x69,0x04,0x61,0xBF,0x21,0xF3,0xAB,0x53,0xD2,0xCD,0x21,
        0xDA,0xFF,0x78,0x19,0x11,0x52,0xF1,0x0E,0xC1,0xE2,0x55,0xBD,
        0x68,0x6F,0x68,0x00,0x53,0xB9,0x22,0x6A,0x2F,0xE4,0x9A,0x34,
        0x1F,0x65,0xCC,0x59,0x32,0x8A,0xBD,0xB1,0xDB,0x49,0xED,0xDF,
        0xA7,0x12,0x66,0xC3,0xFD,0x21,0x04,0x70,0x18,0xF0,0x7F,0xD6,
        0xF7,0x58,0x51,0x19,0x72,0x82,0x7B,0x22,0xA9,0x34,0x18,0x1D,
        0x2F,0xCB,0x21,0xCF,0x6D,0x92,0xAE,0x43,0xB6,0xA8,0x29,0xC7,
        0x27,0xA3,0xCB,0x00,0xC5,0xF2,0xE5,0xFB,0x0A,0xA4,0x59,0x85,
        0xA2,0xBD,0xAD,0x45,0xF0,0xB3,0xAD,0xF9,0xE0,0x81,0x35,0xEE,
        0xD9,0x83,0xB3,0xCC,0xAE,0xEA,0xEB,0x66,0xE6,0xA9,0x57,0x66,
        0xB9,0xF1,0x28,0xA5,0x3F,0x22,0x80,0xD7,0x0B,0xA6,0xF6,0x71,
        0x93,0x9B,0x81,0x0E,0xF8,0x5A,0x90,0xE6,0xCC,0xCA,0x6F,0x66,
        0x5F,0x7A,0xC0,0x10,0x1A,0x1E,0xF0,0xFC,0x2D,0xB6,0x08,0x0C,
        0x62,0x28,0xB0,0xEC,0xDB,0x89,0x28,0xEE,0x0C,0xA8,0x3D,0x65,
        0x94,0x69,0x16,0x69,0x53,0x3C,0x53,0x60,0x13,0xB0,0x2B,0xA7,
        0xD4,0x82,0x87,0xAD,0x1C,0x72,0x9E,0x41,0x35,0xFC,0xC2,0x7C,
        0xE9,0x51,0xDE,0x61,0x85,0xFC,0x19,0x9B,0x76,0x60,0x0F,0x33,
        0xF8,0x6B,0xB3,0xCA,0x52,0x0E,0x29,0xC3,0x07,0xE8,0x90,0x16,
        0xCC,0xCC,0x00,0x19,0xB6,0xAD,0xC3,0xA4,0x30,0x8B,0x33,0xA1,
        0xAF,0xD8,0x8C,0x8D,0x9D,0x01,0xDB,0xA4,0xC4,0xDD,0x7F,0x0B,
        0xBD,0x6F,0x38,0xC3,};
    static unsigned char dh2048_g[]={0x02,};
    DH *dh;

    dh=DH_new();
    if(!dh)
        return NULL;
    dh->p=BN_bin2bn(dh2048_p, sizeof dh2048_p, NULL);
    dh->g=BN_bin2bn(dh2048_g, sizeof dh2048_g, NULL);
    if(!dh->p || !dh->g) {
        DH_free(dh);
        return NULL;
    }
    s_log(LOG_DEBUG, "Using hardcoded DH parameters");
    return dh;
}
