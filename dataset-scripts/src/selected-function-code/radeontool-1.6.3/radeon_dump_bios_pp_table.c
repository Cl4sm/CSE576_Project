void radeon_dump_bios_pp_table(struct atom_context *ctx)
{
	uint16_t size, data_offset;
	uint8_t frev, crev;
	int index = GetIndexIntoMasterTable(DATA, PowerPlayInfo);
	union pp_info *pp_info;
	union clock_info *clock_info;
	ATOM_PPLIB_NONCLOCK_INFO *non_clock_info;
	ATOM_PPLIB_STATE *power_state;
	int num_modes = 0, i, j;
	uint32_t sclk, mclk;

	atom_parse_data_header(ctx, index, &size, &frev, &crev, &data_offset);

	pp_info = (union pp_info *)(ctx->bios + data_offset);

	printf("\nPOWERPLAY\n");
	printf("pp v %d %d\n", frev, crev);

	if (frev < 4) {
	    printf("thermal controller: %s on 0x%02x at 0x%02x - int bitmap 0x%02x\n",
		   thermal_controller_names[pp_info->info.ucOverdriveThermalController],
		   pp_info->info.ucOverdriveI2cLine,
		   pp_info->info.ucOverdriveControllerAddress,
		   pp_info->info.ucOverdriveIntBitmap);
	    printf("%d power modes (lowest to highest)\n", pp_info->info.ucNumOfPowerModeEntries);
	    num_modes = pp_info->info.ucNumOfPowerModeEntries;
	    if (num_modes > ATOM_MAX_NUMBEROF_POWER_BLOCK)
		num_modes = ATOM_MAX_NUMBEROF_POWER_BLOCK;

	    for (i = 0; i < num_modes; i++) {
		printf("== power mode %d ==\n", i);
		switch (frev) {
		case 1:
		    printf("\tengine: %d, memory: %d\n",
			   pp_info->info.asPowerPlayInfo[i].usEngineClock,
			   pp_info->info.asPowerPlayInfo[i].usMemoryClock);
		    printf("\tpcie lanes: %d\n",
			   pp_info->info.asPowerPlayInfo[i].ucNumPciELanes);
		    printf("\ttemp: %d - %d\n",
			   pp_info->info.asPowerPlayInfo[i].ucMinTemperature,
			   pp_info->info.asPowerPlayInfo[i].ucMaxTemperature);
		    printf("\tpanel refresh: %d\n",
			   pp_info->info.asPowerPlayInfo[i].ucSelectedPanel_RefreshRate);
		    printf("\tvoltage gpio id: 0x%02x\n",
			   pp_info->info.asPowerPlayInfo[i].ucVoltageDropIndex);
		    printf("\tflags:\n");
		    radeon_print_pp_misc(pp_info->info.asPowerPlayInfo[i].ulMiscInfo);
		    break;
		case 2:
		    printf("\tengine: %d, memory: %d\n",
			   pp_info->info_2.asPowerPlayInfo[i].ulEngineClock,
			   pp_info->info_2.asPowerPlayInfo[i].ulMemoryClock);
		    printf("\tpcie lanes: %d\n",
			   pp_info->info_2.asPowerPlayInfo[i].ucNumPciELanes);
		    printf("\ttemp: %d - %d\n",
			   pp_info->info_2.asPowerPlayInfo[i].ucMinTemperature,
			   pp_info->info_2.asPowerPlayInfo[i].ucMaxTemperature);
		    printf("\tpanel refresh: %d\n",
			   pp_info->info_2.asPowerPlayInfo[i].ucSelectedPanel_RefreshRate);
		    printf("\tvoltage gpio id: 0x%02x\n",
			   pp_info->info_2.asPowerPlayInfo[i].ucVoltageDropIndex);
		    printf("\tflags:\n");
		    radeon_print_pp_misc(pp_info->info_2.asPowerPlayInfo[i].ulMiscInfo);
		    radeon_print_pp_misc2(pp_info->info_2.asPowerPlayInfo[i].ulMiscInfo2);
		    break;
		case 3:
		    printf("\tengine: %d, memory: %d\n",
			   pp_info->info_3.asPowerPlayInfo[i].ulEngineClock,
			   pp_info->info_3.asPowerPlayInfo[i].ulMemoryClock);
		    printf("\tpcie lanes: %d\n",
			   pp_info->info_3.asPowerPlayInfo[i].ucNumPciELanes);
		    printf("\ttemp: %d - %d\n",
			   pp_info->info_3.asPowerPlayInfo[i].ucMinTemperature,
			   pp_info->info_3.asPowerPlayInfo[i].ucMaxTemperature);
		    printf("\tpanel refresh: %d\n",
			   pp_info->info_3.asPowerPlayInfo[i].ucSelectedPanel_RefreshRate);
		    printf("\tVDDC index: 0x%02x\n",
			   pp_info->info_3.asPowerPlayInfo[i].ucVoltageDropIndex);
		    printf("\tVDDCI index: 0x%02x\n",
			   pp_info->info_3.asPowerPlayInfo[i].ucVDDCI_VoltageDropIndex);
		    printf("\tflags:\n");
		    radeon_print_pp_misc(pp_info->info_3.asPowerPlayInfo[i].ulMiscInfo);
		    radeon_print_pp_misc2(pp_info->info_3.asPowerPlayInfo[i].ulMiscInfo2);
		    break;
		default:
		    break;
		}
	    }
	} else {
	    printf("thermal controller: %s on 0x%02x at 0x%02x\n",
		   pp_lib_thermal_controller_names[pp_info->info_4.sThermalController.ucType],
		   pp_info->info_4.sThermalController.ucI2cLine,
		   pp_info->info_4.sThermalController.ucI2cAddress);
	    if (pp_info->info_4.sThermalController.ucType != 0) {
		if (pp_info->info_4.sThermalController.ucFanParameters)
		    printf("\tno fan connected\n");
		else {
		    printf("\tfan connected\n");
		    printf("\ttach pulse per rev: %d\n",
			   pp_info->info_4.sThermalController.ucFanParameters &
			   ATOM_PP_FANPARAMETERS_TACHOMETER_PULSES_PER_REVOLUTION_MASK);
		    printf("\trpm: %d - %d\n", pp_info->info_4.sThermalController.ucFanMinRPM,
			   pp_info->info_4.sThermalController.ucFanMaxRPM);
		}
	    }
	    printf("back bias time: %d usec\n",pp_info->info_4.usBackbiasTime);
	    printf("voltage time: %d usec\n",pp_info->info_4.usVoltageTime);
	    printf("platform caps:\n");
	    radeon_print_pp_lib_platform_caps(pp_info->info_4.ulPlatformCaps);
	    printf("%d power modes\n", pp_info->info_4.ucNumStates);
	    for (i = 0; i < pp_info->info_4.ucNumStates; i++) {
		printf("== power mode %d ==\n", i);
		power_state = (ATOM_PPLIB_STATE *)(ctx->bios + data_offset +
						   pp_info->info_4.usStateArrayOffset +
						   i * pp_info->info_4.ucStateEntrySize);
		non_clock_info = (ATOM_PPLIB_NONCLOCK_INFO *)(ctx->bios + data_offset +
							      pp_info->info_4.usNonClockInfoArrayOffset +
							      power_state->ucNonClockStateIndex *
							      pp_info->info_4.ucNonClockSize);
		printf("  non-clock info\n");
		printf("\ttemp: %d - %d\n", non_clock_info->ucMinTemperature,
		       non_clock_info->ucMaxTemperature);
		printf("\trequired power: %d\n", non_clock_info->ucRequiredPower);
		printf("\tflags: \n");
		radeon_print_pp_lib_non_clock_flags(non_clock_info->ulCapsAndSettings);
		printf("\tclass info: \n");
		radeon_print_pp_lib_non_clock_class(non_clock_info->usClassification);

		for (j = 0; j < (pp_info->info_4.ucStateEntrySize - 1); j++) {
		    clock_info = (union clock_info *)(ctx->bios + data_offset +
						      pp_info->info_4.usClockInfoArrayOffset +
						      power_state->ucClockStateIndices[j] *
						      pp_info->info_4.ucClockInfoSize);

		    if (1) {
			//discrete
			printf("  clock info %d\n", j);
			sclk = clock_info->r600.usEngineClockLow;
			sclk |= clock_info->r600.ucEngineClockHigh << 16;
			mclk = clock_info->r600.usMemoryClockLow;
			mclk |= clock_info->r600.ucMemoryClockHigh << 16;
			printf("\tengine %d, memory %d\n", sclk, mclk);
			printf("\tVDDC: %d\n", clock_info->r600.usVDDC);
			printf("\tflags: \n");
			radeon_print_pp_lib_clock_flags_discrete(clock_info->r600.ulFlags);
		    } else {
			// igp
			sclk = clock_info->rs780.usLowEngineClockLow;
			sclk |= clock_info->rs780.ucLowEngineClockHigh << 16;
			mclk = clock_info->rs780.usMemoryClockLow;
			mclk |= clock_info->rs780.ucMemoryClockHigh << 16;
			printf("\tengine %d\n", sclk);
			printf("\tsideport memory %d\n", mclk);
			printf("\tHT links %d - %d\n", clock_info->rs780.ucMinHTLinkWidth,
			       clock_info->rs780.ucMaxHTLinkWidth);
			if (clock_info->rs780.usHTLinkFreq & ATOM_PPLIB_RS780_HTLINKFREQ_HIGH)
			    printf("\tHT freq HIGH\n");
			else if (clock_info->rs780.usHTLinkFreq & ATOM_PPLIB_RS780_HTLINKFREQ_LOW)
			    printf("\tHT freq LOW\n");
			else
			    printf("\tHT freq NONE\n");
			printf("\tVDDC: %d\n", clock_info->rs780.usVDDC);
			printf("\tflags: \n");
			radeon_print_pp_lib_clock_flags_igp(clock_info->rs780.ulFlags);
		    }
		}
	    }
	}
}