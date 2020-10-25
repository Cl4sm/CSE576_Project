uint32_t pp_anonymize(const uint32_t orig_addr) {
        uint8_t rin_output[16];
        uint8_t rin_input[16];

        uint32_t result = 0;
        uint32_t first4bytes_pad, first4bytes_input;
        int pos;

        memcpy(rin_input, m_pad, 16);
        first4bytes_pad = (((uint32_t) m_pad[0]) << 24) + 
                (((uint32_t) m_pad[1]) << 16 ) + 
                (((uint32_t) m_pad[2]) << 8) + 
                (uint32_t) m_pad[3];

        // For each prefix with length 0 to 31, generate a bit using the 
        // rijndael cipher, which is used as a pseudorandom function here. 
        // The bits generated in every round are combined into a pseudorandom 
        // one-time-pad.

        for (pos = 0; pos <= 31; pos++) {
                // Padding: The most significant pos bits are taken from orig_addr.
                // The other 128-pos bits are taken from m_pad. The variables 
                // first4bytes_pad and first4bytes_input are used to handle the annoying
                // byte order problem

                if (pos == 0) {
                        first4bytes_input = first4bytes_pad;
                } else {
                        first4bytes_input = ((orig_addr >> (32 - pos)) << (32 - pos)) |
                                ((first4bytes_pad << pos) >> pos);
                }
                rin_input[0] = (uint8_t) (first4bytes_input >> 24);
                rin_input[1] = (uint8_t) ((first4bytes_input << 8) >> 24);
                rin_input[2] = (uint8_t) ((first4bytes_input << 16) >> 24);
                rin_input[3] = (uint8_t) ((first4bytes_input << 24) >> 24);

                // Encryption: The rijndael cipher is used as a pseudorandom function.
                // During each round, only the first bit of rin_output is used.
                blockEncrypt(rin_input, 128, rin_output);

                // Combination: the bits are combined into a pseudorandom one-time-pad.
                result |= (rin_output[0] >> 7) << (31 - pos);
        }

        return result ^ orig_addr;
}