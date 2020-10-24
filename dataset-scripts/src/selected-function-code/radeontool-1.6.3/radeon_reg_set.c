void radeon_reg_set(const char *inname, unsigned int value)
{
    int i;
    unsigned long address;

    if (inname[0] == '0' && inname[1] == 'x') {
        address = strtol(&(inname[2]), NULL, 16);
        set_reg(inname, "", address, value, radeon_get, radeon_set);
    }
    else if (inname[0] == 'M' && inname[1] == 'C' && inname[2] == ':') {
        address = strtol(&(inname[3]), NULL, 16);
        set_reg(inname, "MC: ", address, value, radeon_get_mc, radeon_set_mc);
    }
    else if (inname[0] == 'C' && inname[1] == 'L' && inname[2] == ':') {
        address = strtol(&(inname[3]), NULL, 16);
        set_reg(inname, "CL: ", address, value, radeon_get_clk, radeon_set_clk);
    }
    else if (inname[0] == 'P' && inname[1] == 'C' && inname[2] == ':') {
        address = strtol(&(inname[3]), NULL, 16);
        set_reg(inname, "PCIE: ", address, value, radeon_get_pcie, radeon_set_pcie);
    }
}