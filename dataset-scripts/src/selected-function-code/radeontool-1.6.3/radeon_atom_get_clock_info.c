int radeon_atom_get_clock_info(struct atom_context *ctx)
{

	int index = GetIndexIntoMasterTable(DATA, FirmwareInfo);
	union firmware_info *firmware_info;
	uint8_t frev, crev;
	struct radeon_pll *p1pll = &p1pll_s;
	struct radeon_pll *p2pll = &p2pll_s;
	struct radeon_pll *spll = &spll_s;
	struct radeon_pll *mpll = &mpll_s;
	uint16_t data_offset;

	atom_parse_data_header(ctx, index, NULL, &frev, &crev, &data_offset);

	firmware_info = (union firmware_info *)(ctx->bios + data_offset);

	if (firmware_info) {
		/* pixel clocks */
		p1pll->reference_freq = le16_to_cpu(firmware_info->info.usReferenceClock);
		p1pll->reference_div = 0;

		p1pll->pll_out_min = le16_to_cpu(firmware_info->info.usMinPixelClockPLL_Output);
		p1pll->pll_out_max = le32_to_cpu(firmware_info->info.ulMaxPixelClockPLL_Output);

		if (p1pll->pll_out_min == 0) {
			p1pll->pll_out_min = 64800;
		}

		p1pll->pll_in_min = le16_to_cpu(firmware_info->info.usMinPixelClockPLL_Input);
		p1pll->pll_in_max = le16_to_cpu(firmware_info->info.usMaxPixelClockPLL_Input);

		*p2pll = *p1pll;

		/* system clock */
		spll->reference_freq = le16_to_cpu(firmware_info->info.usReferenceClock);
		spll->reference_div = 0;

		spll->pll_out_min = le16_to_cpu(firmware_info->info.usMinEngineClockPLL_Output);
		spll->pll_out_max = le32_to_cpu(firmware_info->info.ulMaxEngineClockPLL_Output);

		/* ??? */
		if (spll->pll_out_min == 0) {
#if 0
			if (radeon_is_avivo(dev_priv))
				spll->pll_out_min = 64800;
			else
				spll->pll_out_min = 20000;
#endif
		}

		spll->pll_in_min = le16_to_cpu(firmware_info->info.usMinEngineClockPLL_Input);
		spll->pll_in_max = le16_to_cpu(firmware_info->info.usMaxEngineClockPLL_Input);


		/* memory clock */
		mpll->reference_freq = le16_to_cpu(firmware_info->info.usReferenceClock);
		mpll->reference_div = 0;

		mpll->pll_out_min = le16_to_cpu(firmware_info->info.usMinMemoryClockPLL_Output);
		mpll->pll_out_max = le32_to_cpu(firmware_info->info.ulMaxMemoryClockPLL_Output);

		/* ??? */
		if (mpll->pll_out_min == 0) {
			if (0) //radeon_is_avivo(dev_priv))
				mpll->pll_out_min = 64800;
			else
				mpll->pll_out_min = 20000;
		}

		mpll->pll_in_min = le16_to_cpu(firmware_info->info.usMinMemoryClockPLL_Input);
		mpll->pll_in_max = le16_to_cpu(firmware_info->info.usMaxMemoryClockPLL_Input);

	//	mode_info->sclk = le32_to_cpu(firmware_info->info.ulDefaultEngineClock);
	//	mode_info->mclk = le32_to_cpu(firmware_info->info.ulDefaultMemoryClock);

		return true;
	}
	return false;
}