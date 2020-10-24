int get_reg_type(unsigned long address)
{
    int value, prev, bits = 0;
    int i, j;

    for (i = 0; i < REG_NUM_SAMPLES; i++) {
        value = radeon_get(address, "static sampling");
        if (i > 0 && (prev ^ value))
            bits |= (prev ^ value);
        usleep(REG_SLEEP);
        prev = value;
    }

    j = 0;
    for (i = 0; i < 32; i++) {
        if (bits & (1 << i))
            j++;
    }

    if (j == 0)
        i = REG_TYPE_STATIC;
    else if (j < 3)
        i = REG_TYPE_SEMI_STATIC;
    else
        i = REG_TYPE_RANDOM;

    reg_type[address] = i;

    return i;
}