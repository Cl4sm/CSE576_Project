unsigned char linear2ulaw(short pcm_val)  /* 2's complement (16-bit range) */
{
	static short seg_uend[8] = {0x3F, 0x7F, 0xFF, 0x1FF, 0x3FF, 0x7FF, 0xFFF, 0x1FFF};
        short           mask;
        short           seg;
        unsigned char   uval;

        /* Get the sign and the magnitude of the value. */
        pcm_val = pcm_val >> 2;
        if (pcm_val < 0) {
                pcm_val = -pcm_val;
                mask = 0x7F;
        } else {
                mask = 0xFF;
        }
        if ( pcm_val > 8159 ) pcm_val = 8159;           /* clip the magnitude */
        pcm_val += (0x84 >> 2);

        /* Convert the scaled magnitude to segment number. */
        seg = search(pcm_val, seg_uend, 8);

        /*
         * Combine the sign, segment, quantization bits;
         * and complement the code word.
         */
        if (seg >= 8)           /* out of range, return maximum value. */
                return (unsigned char) (0x7F ^ mask);
        else {
                uval = (unsigned char) (seg << 4) | ((pcm_val >> (seg + 1)) & 0xF);
                return (uval ^ mask);
        }

}