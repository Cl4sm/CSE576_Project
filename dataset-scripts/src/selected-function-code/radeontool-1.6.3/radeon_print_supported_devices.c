static void radeon_print_supported_devices(uint16_t sd)
{
    if (sd & ATOM_DEVICE_CRT1_SUPPORT)
	printf("CRT1 ");
    if (sd & ATOM_DEVICE_LCD1_SUPPORT)
	printf("LCD1 ");
    if (sd & ATOM_DEVICE_TV1_SUPPORT)
	printf("TV1 ");
    if (sd & ATOM_DEVICE_DFP1_SUPPORT)
	printf("DFP1 ");
    if (sd & ATOM_DEVICE_CRT2_SUPPORT)
	printf("CRT2 ");
    if (sd & ATOM_DEVICE_LCD2_SUPPORT)
	printf("LCD2 ");
    if (sd & ATOM_DEVICE_TV2_SUPPORT)
	printf("TV2 ");
    if (sd & ATOM_DEVICE_DFP2_SUPPORT)
	printf("DFP2 ");
    if (sd & ATOM_DEVICE_CV_SUPPORT)
	printf("CV ");
    if (sd & ATOM_DEVICE_DFP3_SUPPORT)
	printf("DFP3 ");
    if (sd & ATOM_DEVICE_DFP4_SUPPORT)
	printf("DFP4 ");
    if (sd & ATOM_DEVICE_DFP5_SUPPORT)
	printf("DFP5 ");
}