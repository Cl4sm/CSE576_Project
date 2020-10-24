unsigned char linear2alaw(int pcm_val)  /* 2's complement (16-bit range) */
{
	static short seg_aend[8] = {0x1F, 0x3F, 0x7F, 0xFF, 0x1FF, 0x3FF, 0x7FF, 0xFFF};
        int             mask; 
        int             seg;  
        unsigned char   aval;

        pcm_val = pcm_val >> 3;

        if (pcm_val >= 0) {
                mask = 0xD5;            /* sign (7th) bit = 1 */
        } else {
                mask = 0x55;            /* sign bit = 0 */
                pcm_val = -pcm_val - 1;
        }

        /* Convert the scaled magnitude to segment number. */
        seg = search(pcm_val, seg_aend, 8);

        /* Combine the sign, segment, and quantization bits. */

        if (seg >= 8)           /* out of range, return maximum value. */
                return (unsigned char) (0x7F ^ mask);
        else {
                aval = (unsigned char) seg << 4;
                if (seg < 2)
                        aval |= (pcm_val >> 1) & 0xf;
                else
                        aval |= (pcm_val >> seg) & 0xf;
		return (aval ^ mask);
        }
}