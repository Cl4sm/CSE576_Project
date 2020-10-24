uint32_t anonymize(const uint32_t orig_addr) {
        uint8_t rin_output[16]; 
        uint8_t rin_input[16]; 

        uint32_t result = 0;

        memcpy(rin_input, m_pad, 16);

        rin_input[0] = (uint8_t) (orig_addr >> 24);
        rin_input[1] = (uint8_t) ((orig_addr << 8) >> 24);
        rin_input[2] = (uint8_t) ((orig_addr << 16) >> 24);
        rin_input[3] = (uint8_t) ((orig_addr << 24) >> 24);

        blockEncrypt(rin_input, 128, rin_output);

        result = 0;
        result += (rin_output[0] <<24);
        result += (rin_output[1] <<16);
        result += (rin_output[2] <<8);
        result += (rin_output[3]);
        return result;
}