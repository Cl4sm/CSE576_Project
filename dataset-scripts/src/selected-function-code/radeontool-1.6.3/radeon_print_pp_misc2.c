static void radeon_print_pp_misc2(uint32_t misc)
{
    if (misc & ATOM_PM_MISCINFO2_SYSTEM_AC_LITE_MODE)
	printf("\t\tSYSTEM_AC_LITE_MODE\n");
    if (misc & ATOM_PM_MISCINFO2_MULTI_DISPLAY_SUPPORT)
	printf("\t\tMULTI_DISPLAY_SUPPORT\n");
    if (misc & ATOM_PM_MISCINFO2_DYNAMIC_BACK_BIAS_EN)
	printf("\t\tDYNAMIC_BACK_BIAS_EN\n");
    if (misc & ATOM_PM_MISCINFO2_FS3D_OVERDRIVE_INFO)
	printf("\t\tFS3D_OVERDRIVE_INFO\n");
    if (misc & ATOM_PM_MISCINFO2_FORCEDLOWPWR_MODE)
	printf("\t\tFORCEDLOWPWR_MODE\n");
    if (misc & ATOM_PM_MISCINFO2_VDDCI_DYNAMIC_VOLTAGE_EN)
	printf("\t\tVDDCI_DYNAMIC_VOLTAGE_EN\n");
    if (misc & ATOM_PM_MISCINFO2_VIDEO_PLAYBACK_CAPABLE)
	printf("\t\tVIDEO_PLAYBACK_CAPABLE\n");
    if (misc & ATOM_PM_MISCINFO2_NOT_VALID_ON_DC)
	printf("\t\tNOT_VALID_ON_DC\n");
    if (misc & ATOM_PM_MISCINFO2_STUTTER_MODE_EN)
	printf("\t\tSTUTTER_MODE_EN\n");
    if (misc & ATOM_PM_MISCINFO2_UVD_SUPPORT_MODE)
	printf("\t\tUVD_SUPPORT_MODE\n");

}