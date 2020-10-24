static void cache_update(uint32_t scan) {
        uint8_t rin_output[16];
        uint8_t rin_input[16];
        uint32_t orig_addr = 0;
        uint32_t result = 0;
        uint32_t first4bytes_pad, first4bytes_input;
        int pos;

        memcpy(rin_input, m_pad, 16);
        first4bytes_pad = (((uint32_t) m_pad[0]) << 24) + 
                (((uint32_t) m_pad[1]) << 16 ) + 
                (((uint32_t) m_pad[2]) << 8) + 
                (uint32_t) m_pad[3];


        memcpy(rin_input, m_pad, 16);
        orig_addr = (scan << (32 - CACHEBITS));
        result = 0;
        for (pos = 0; pos < CACHEBITS; pos++) {

                if (pos == 0) {
                        first4bytes_input = first4bytes_pad;
                } else {
                        first4bytes_input = 
                                ((orig_addr >> (32 - pos)) << (32 - pos)) |
                                ((first4bytes_pad << pos) >> pos);
                }
                rin_input[0] = (uint8_t) (first4bytes_input >> 24);
                rin_input[1] = (uint8_t) ((first4bytes_input << 8) >> 24);
                rin_input[2] = (uint8_t) ((first4bytes_input << 16) >> 24);
                rin_input[3] = (uint8_t) ((first4bytes_input << 24) >> 24);

                blockEncrypt(rin_input, 128, rin_output);

                result |= (rin_output[0] >> 7) << (31 - pos);
        }
        enc_cache[scan] = (result >> (32 - CACHEBITS));

}