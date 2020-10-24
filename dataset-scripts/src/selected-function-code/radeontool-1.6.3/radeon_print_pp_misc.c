static void radeon_print_pp_misc(uint32_t misc)
{
    if (misc & ATOM_PM_MISCINFO_SPLIT_CLOCK)
	printf("\t\tSPLIT_CLOCK\n");
    if (misc & ATOM_PM_MISCINFO_USING_MCLK_SRC)
	printf("\t\tUSING_MCLK_SRC\n");
    if (misc & ATOM_PM_MISCINFO_USING_SCLK_SRC)
	printf("\t\tUSING_SCLK_SRC\n");
    if (misc & ATOM_PM_MISCINFO_VOLTAGE_DROP_SUPPORT)
	printf("\t\tVOLTAGE_DROP_SUPPORT\n");
    if (misc & ATOM_PM_MISCINFO_VOLTAGE_DROP_ACTIVE_HIGH)
	printf("\t\tVOLTAGE_DROP_ACTIVE_HIGH\n");
    if (misc & ATOM_PM_MISCINFO_LOAD_PERFORMANCE_EN)
	printf("\t\tLOAD_PERFORMANCE_EN\n");
    if (misc & ATOM_PM_MISCINFO_ENGINE_CLOCK_CONTRL_EN)
	printf("\t\tENGINE_CLOCK_CONTRL_EN\n");
    if (misc & ATOM_PM_MISCINFO_MEMORY_CLOCK_CONTRL_EN)
	printf("\t\tMEMORY_CLOCK_CONTRL_EN\n");
    if (misc & ATOM_PM_MISCINFO_PROGRAM_VOLTAGE)
	printf("\t\tPROGRAM_VOLTAGE\n");
    if (misc & ATOM_PM_MISCINFO_ASIC_REDUCED_SPEED_SCLK_EN)
	printf("\t\tASIC_REDUCED_SPEED_SCLK_EN\n");
    if (misc & ATOM_PM_MISCINFO_ASIC_DYNAMIC_VOLTAGE_EN)
	printf("\t\tASIC_DYNAMIC_VOLTAGE_EN\n");
    if (misc & ATOM_PM_MISCINFO_ASIC_SLEEP_MODE_EN)
	printf("\t\tASIC_SLEEP_MODE_EN\n");
    if (misc & ATOM_PM_MISCINFO_LOAD_BALANCE_EN)
	printf("\t\tLOAD_BALANCE_EN\n");
    if (misc & ATOM_PM_MISCINFO_DEFAULT_DC_STATE_ENTRY_TRUE)
	printf("\t\tDEFAULT_DC_STATE_ENTRY_TRUE\n");
    if (misc & ATOM_PM_MISCINFO_DEFAULT_LOW_DC_STATE_ENTRY_TRUE)
	printf("\t\tDEFAULT_LOW_DC_STATE_ENTRY_TRUE\n");
    if (misc & ATOM_PM_MISCINFO_LOW_LCD_REFRESH_RATE)
	printf("\t\tLOW_LCD_REFRESH_RATE\n");
    if (misc & ATOM_PM_MISCINFO_DRIVER_DEFAULT_MODE)
	printf("\t\tDRIVER_DEFAULT_MODE\n");
    if (misc & ATOM_PM_MISCINFO_OVER_CLOCK_MODE)
	printf("\t\tOVER_CLOCK_MODE\n");
    if (misc & ATOM_PM_MISCINFO_OVER_DRIVE_MODE)
	printf("\t\tOVER_DRIVE_MODE\n");
    if (misc & ATOM_PM_MISCINFO_POWER_SAVING_MODE)
	printf("\t\tPOWER_SAVING_MODE\n");
    if (misc & ATOM_PM_MISCINFO_THERMAL_DIODE_MODE)
	printf("\t\tTHERMAL_DIODE_MODE\n");
    if (misc & ATOM_PM_MISCINFO_FRAME_MODULATION_MASK)
	printf("\t\tFM: 0x%x\n",
	       (misc & ATOM_PM_MISCINFO_FRAME_MODULATION_MASK) >> ATOM_PM_MISCINFO_FRAME_MODULATION_SHIFT);
    if (misc & ATOM_PM_MISCINFO_DYN_CLK_3D_IDLE)
	printf("\t\tDYN_CLK_3D_IDLE\n");
    if (misc & ATOM_PM_MISCINFO_DYNAMIC_CLOCK_DIVIDER_BY_2)
	printf("\t\tDYNAMIC_CLOCK_DIVIDER_BY_2\n");
    if (misc & ATOM_PM_MISCINFO_DYNAMIC_CLOCK_DIVIDER_BY_4)
	printf("\t\tDYNAMIC_CLOCK_DIVIDER_BY_4\n");
    if (misc & ATOM_PM_MISCINFO_DYNAMIC_HDP_BLOCK_EN)
	printf("\t\tDYNAMIC_HDP_BLOCK_EN\n");
    if (misc & ATOM_PM_MISCINFO_DYNAMIC_MC_HOST_BLOCK_EN)
	printf("\t\tDYNAMIC_MC_HOST_BLOCK_EN\n");
    if (misc & ATOM_PM_MISCINFO_3D_ACCELERATION_EN)
	printf("\t\t3D_ACCELERATION_EN\n");
    switch ((misc & ATOM_PM_MISCINFO_POWERPLAY_SETTINGS_GROUP_MASK) >>
	    ATOM_PM_MISCINFO_POWERPLAY_SETTINGS_GROUP_SHIFT) {
    case 1:
	printf("\t\tOptimal Battery Life\n");
	break;
    case 2:
	printf("\t\tHigh Battery\n");
	break;
    case 3:
	printf("\t\tBalanced\n");
	break;
    case 4:
	printf("\t\tHigh Performance\n");
	break;
    case 5:
	printf("\t\tOptimal Performance (Default state with Default clocks)\n");
	break;
    default:
	break;
    }
    if (misc & ATOM_PM_MISCINFO_ENABLE_BACK_BIAS)
	printf("\t\tENABLE_BACK_BIAS\n");
}