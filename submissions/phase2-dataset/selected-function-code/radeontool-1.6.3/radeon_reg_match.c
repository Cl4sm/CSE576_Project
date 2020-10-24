void radeon_reg_match(const char *pattern)
{
    int i;
    unsigned long address;
    unsigned int value;

    if (pattern[0] == '0' && pattern[1] == 'x') {
        address = strtol(&(pattern[2]), NULL, 16);
        value = radeon_get(address, pattern);
        printf("%s\t0x%08x (%d)\n", pattern, value, value);
    }
    else if (pattern[0] == 'M' && pattern[1] == 'C' && pattern[2] == ':') {
        address = strtol(&(pattern[3]), NULL, 16);
        value = radeon_get_mc(address, pattern);
        printf("%s\t0x%08x (%d)\n", pattern, value, value);
    }
    else if (pattern[0] == 'C' && pattern[1] == 'L' && pattern[2] == ':') {
        address = strtol(&(pattern[3]), NULL, 16);
        value = radeon_get_clk(address, pattern);
        printf("%s\t0x%08x (%d)\n", pattern, value, value);
    }
    else if (pattern[0] == 'P' && pattern[1] == 'C' && pattern[2] == ':') {
        address = strtol(&(pattern[3]), NULL, 16);
        value = radeon_get_pcie(address, pattern);
        printf("%s\t0x%08x (%d)\n", pattern, value, value);
    }
}