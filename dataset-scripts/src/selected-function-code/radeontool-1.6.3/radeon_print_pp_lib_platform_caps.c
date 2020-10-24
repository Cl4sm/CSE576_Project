static void radeon_print_pp_lib_platform_caps(uint32_t caps)
{
    if (caps & ATOM_PP_PLATFORM_CAP_BACKBIAS)
	printf("\tBACKBIAS\n");
    if (caps & ATOM_PP_PLATFORM_CAP_POWERPLAY)
	printf("\tPOWERPLAY\n");
    if (caps & ATOM_PP_PLATFORM_CAP_SBIOSPOWERSOURCE)
	printf("\tSBIOSPOWERSOURCE\n");
    if (caps & ATOM_PP_PLATFORM_CAP_ASPM_L0s)
	printf("\tASPM_L0s\n");
    if (caps & ATOM_PP_PLATFORM_CAP_ASPM_L1)
	printf("\tASPM_L1\n");
    if (caps & ATOM_PP_PLATFORM_CAP_HARDWAREDC)
	printf("\tHARDWAREDC\n");
    if (caps & ATOM_PP_PLATFORM_CAP_GEMINIPRIMARY)
	printf("\tGEMINIPRIMARY\n");
    if (caps & ATOM_PP_PLATFORM_CAP_STEPVDDC)
	printf("\tSTEPVDDC\n");
    if (caps & ATOM_PP_PLATFORM_CAP_VOLTAGECONTROL)
	printf("\tVOLTAGECONTROL\n");
    if (caps & ATOM_PP_PLATFORM_CAP_SIDEPORTCONTROL)
	printf("\tSIDEPORTCONTROL\n");
    if (caps & ATOM_PP_PLATFORM_CAP_TURNOFFPLL_ASPML1)
	printf("\tTURNOFFPLL_ASPML1\n");
    if (caps & ATOM_PP_PLATFORM_CAP_HTLINKCONTROL)
	printf("\tHTLINKCONTROL\n");
}