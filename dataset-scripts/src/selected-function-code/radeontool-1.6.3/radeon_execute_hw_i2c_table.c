void radeon_execute_hw_i2c_table(struct atom_context *ctx)
{
	int index = GetIndexIntoMasterTable(COMMAND, ReadEDIDFromHWAssistedI2C);
	READ_EDID_FROM_HW_I2C_DATA_PS_ALLOCATION edid_data;
	int prescale;
	int i2c_clock = 50;
	int engine_clk = 0; //info->sclk * 100;

//	if (info->ChipFamily == CHIP_FAMILY_R520)
		prescale = (127 << 8) + (engine_clk * 10) / (4 * 127 * i2c_clock);
//	else if (info->ChipFamily < CHIP_FAMILY_R600)
		prescale = (((engine_clk * 10)/(4 * 128 * 100) + 1) << 8) + 128;
//	else
//		prescale = (info->pll.reference_freq * 10) / i2c_clock;

	edid_data.usPrescale = prescale;
	edid_data.usVRAMAddress = 0;
	edid_data.ucSlaveAddr = 0xa0;
	//edid_data.ucLineNumber = radeon_output->ddc_i2c.hw_line;	

	atom_execute_table(ctx, index, (uint32_t *)&edid_data);


}