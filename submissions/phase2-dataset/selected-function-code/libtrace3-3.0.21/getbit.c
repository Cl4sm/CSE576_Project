static bitbuffer_t getbit(void **packet, int *packlen, uint64_t numbits)
{
    bitbuffer_t ret;
    bitbuffer_t mask;
    
    char *pktptr = NULL;

    /* While the buffer is not filled up and there is still
     * data in the packet to read, read a byte...
     * 
     * The buffer gets filled from right to left
     */
    while(bits < numbits && *packlen > 0)
    {
	uint8_t byte;
	/* read in one byte from the packet */
	byte=(*((bitbuffer_t*)*packet))&0xff;
	buffer |= (bitbuffer_t)byte << (sizeof(bitbuffer_t)*8-(bits+sizeof(byte)*8));
	/* update the position within the packet */
	pktptr = (char *)*packet;
	pktptr += 1;
	*packet = pktptr;

	//*packet = ((char*)*packet) + 1;

	bits += sizeof(byte)*8;
	*packlen -= 1;
    }

    /* our return value is the first <numbits> of the buffer */
    mask = ~((1ULL<<((sizeof(bitbuffer_t)*8-numbits)))-1);
    ret = buffer & mask;
    ret >>=(sizeof(bitbuffer_t)*8-numbits);
    
    /* remove the bits that are being returned from out buffer */
    buffer <<= numbits;

    /* and update our position inside this buffer */
    bits -= numbits;

    return ret;
}