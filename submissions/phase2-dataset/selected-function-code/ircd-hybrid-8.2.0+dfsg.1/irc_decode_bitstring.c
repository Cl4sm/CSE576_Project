irc_decode_bitstring(const unsigned char **cpp, char *dn, const char *eom)
{
        const unsigned char *cp = *cpp;
        char *beg = dn, tc;
        int b, blen, plen;

        if ((blen = (*cp & 0xff)) == 0)
                blen = 256;
        plen = (blen + 3) / 4;
        plen += sizeof("\\[x/]") + (blen > 99 ? 3 : (blen > 9) ? 2 : 1);
        if (dn + plen >= eom)
                return(-1);

        cp++;
        dn += sprintf(dn, "\\[x");
        for (b = blen; b > 7; b -= 8, cp++)
                dn += sprintf(dn, "%02x", *cp & 0xff);
        if (b > 4) {
                tc = *cp++;
                dn += sprintf(dn, "%02x", tc & (0xff << (8 - b)));
        } else if (b > 0) {
                tc = *cp++;
               dn += sprintf(dn, "%1x",
                               ((tc >> 4) & 0x0f) & (0x0f << (4 - b)));
        }
        dn += sprintf(dn, "/%d]", blen);

        *cpp = cp;
        return(dn - beg);
} /*2*/
