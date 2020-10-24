static int is_tar(unsigned char *buf, apr_size_t nbytes)
{
    register union record *header = (union record *) buf;
    register int i;
    register long sum, recsum;
    register char *p;

    if (nbytes < sizeof(union record))
               return 0;

    recsum = from_oct(8, header->header.chksum);

    sum = 0;
    p = header->charptr;
    for (i = sizeof(union record); --i >= 0;) {
        /*
         * We can't use unsigned char here because of old compilers, e.g. V7.
         */
        sum += 0xFF & *p++;
    }

    /* Adjust checksum to count the "chksum" field as blanks. */
    for (i = sizeof(header->header.chksum); --i >= 0;)
        sum -= 0xFF & header->header.chksum[i];
    sum += ' ' * sizeof header->header.chksum;

    if (sum != recsum)
        return 0;   /* Not a tar archive */

    if (0 == strcmp(header->header.magic, TMAGIC))
        return 2;   /* Unix Standard tar archive */

    return 1;       /* Old fashioned tar archive */
}