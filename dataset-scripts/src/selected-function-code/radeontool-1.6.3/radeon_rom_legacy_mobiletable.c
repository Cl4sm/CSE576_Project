static void radeon_rom_legacy_mobiletable(unsigned char *bios, int hdr)
{
	int offset, rev, i, gpiooffset, blocks, powerplayoffset, lcdddcoffset, tmdspoweroffset;
	unsigned int reg, andmask, ormask;

	offset = BIOS16(hdr + 0x42);
	if (offset == 0) {
		printf("No mobile info table\n");
		return;
	}
	rev = BIOS8(offset);
	printf("mobile table revision: %d\n", rev);

	switch (BIOS8(offset + 0x8) & 0xf) {
	case 1:
	    printf("CRT i2c clk=GPIO0, data=GPIO1\n");
	    break;
	case 2:
	    printf("CRT i2c clk=GPIO4, data=GPIO3\n");
	    break;
	default:
	    printf("%d\n", BIOS8(offset + 0x8) & 0xf);
	    break;
	}

	gpiooffset = BIOS16(offset+0x13);
	if (gpiooffset) {
	    printf("gpio table revision: %d\n", BIOS8(gpiooffset));
	    blocks = BIOS8(gpiooffset + 0x2);
	    printf("gpio blocks: %d\n", blocks);

	    for (i = 0; i < blocks; i++) {
		reg = BIOS16(gpiooffset + 3 + (i * 10) + 0);
		andmask = BIOS32(gpiooffset + 3 + (i * 10) + 0x2);
		ormask = BIOS32(gpiooffset + 3 + (i * 10) + 0x6);
		printf("reg: 0x%x, AND: 0x%x, OR: 0x%x\n", reg * 4, andmask, ormask);
	    }
	}
	powerplayoffset = BIOS16(offset+0x11);
	if (powerplayoffset) {
	    unsigned int powerplay_flags;
	    int powerplay_rev = BIOS8(powerplayoffset);
	    printf("powerplay table revision: %d\n", powerplay_rev);
	    printf("powerplay table size: %d bytes\n", BIOS8(powerplayoffset) + 1);
	    blocks = BIOS8(powerplayoffset+0x2);
	    printf("powerplay states: %d\n", blocks);
	    for (i = 0; i < blocks; i++) {
		printf("state %d\n", i);
		powerplay_flags = BIOS16(powerplayoffset + 5 + (i * 0x10));
		if (powerplay_flags & 0x1)
		    printf("\tMCLK_SRC_SPLL\n");
		else if (powerplay_flags & 0x2)
		    printf("\tSCLK_SRC_MPLL\n");
		else
		    printf("\tsplit clock\n");
		if (powerplay_flags & 0x4) {
		    printf("\tvoltage drop supported\n");
		    if (powerplay_flags & 0x8)
			printf("\tvoltage drop active high\n");
		    else
			printf("\tvoltage drop active low\n");
		}
		if (powerplay_flags & 0x10)
		    printf("\tlcd refresh rate change\n");
		if (powerplay_flags & 0x20)
		    printf("\tengine clk control enabled\n");
		if (powerplay_flags & 0x40)
		    printf("\tmemory clk control enabled\n");
		if (powerplay_flags & 0x80)
		    printf("\toverclock mode\n");
		else if (powerplay_rev <= 3)
		    printf("\tpowersave mode\n");
		if (powerplay_flags & 0x100)
		    printf("\thalf engine clk\n");
		if (powerplay_flags & 0x200)
		    printf("\tdynamic voltage control enabled\n");
		if (powerplay_flags & 0x400)
		    printf("\tpower state sleep mode enabled\n");
		if (powerplay_flags & 0x800)
		    printf("\tpower state load balancing enabled\n");
		if (powerplay_flags & 0x1000)
		    printf("\tdefault DC state\n");
		if (powerplay_flags & 0x2000)
		    printf("\tdefault DC low state\n");
		if (powerplay_rev > 3) {
		    if (powerplay_flags & 0x4000)
			printf("\tpowersave mode\n");
		    if (powerplay_flags & 0x8000)
			printf("\tthermal diode mode\n");
		}
		printf("\tmclk/sclk: %f/%f\n",
		       BIOS32(powerplayoffset + 5 + (i * 0x10) + 0x2) / 100.0,
		       BIOS32(powerplayoffset + 5 + (i * 0x10) + 0x6) / 100.0);
		switch (BIOS8(powerplayoffset + 5 + (i * 0x10) + 0xa)) {
		case 0:
		    printf("\tLCD refresh: 50 Hz\n");
		    break;
		case 2:
		    printf("\tLCD refresh: 56 Hz\n");
		    break;
		case 4:
		    printf("\tLCD refresh: 60 Hz\n");
		    break;
		case 6:
		    printf("\tLCD refresh: 67 Hz\n");
		    break;
		case 8:
		    printf("\tLCD refresh: 70 Hz\n");
		    break;
		case 10:
		    printf("\tLCD refresh: 72 Hz\n");
		    break;
		case 12:
		    printf("\tLCD refresh: 75 Hz\n");
		    break;
		case 14:
		    printf("\tLCD refresh: 76 Hz\n");
		    break;
		case 16:
		    printf("\tLCD refresh: 85 Hz\n");
			break;
		case 18:
		    printf("\tLCD refresh: 90 Hz\n");
		    break;
		case 20:
		    printf("\tLCD refresh: 100 Hz\n");
		    break;
		case 22:
		    printf("\tLCD refresh: 120 Hz\n");
		    break;
		case 24:
		    printf("\tLCD refresh: 140 Hz\n");
		    break;
		case 26:
		    printf("\tLCD refresh: 150 Hz\n");
		    break;
		case 28:
		    printf("\tLCD refresh: 160 Hz\n");
		    break;
		case 30:
		    printf("\tLCD refresh: 200 Hz\n");
		    break;
		case 1:
		    printf("\tLCD refresh: 40 Hz\n");
		    break;
		case 3:
		    printf("\tLCD refresh: 30 Hz\n");
		    break;
		default:
		    break;
		}
		if (powerplay_rev < 6) {
		    printf("\tvoltage control reg: 0x%x\n", BIOS16(powerplayoffset + 5 + (i * 0x10) + 0xb) * 4);
		    printf("\tvoltage control bits: %d\n", BIOS8(powerplayoffset + 5 + (i * 0x10) + 0xd));
		} else {
		    int j;
		    int entries = BIOS8(powerplayoffset + 5 + (i * 0x10) + 0xb);
		    int voltage_table_offset = BIOS16(powerplayoffset + 5 + (i * 0x10) + 0xc);
		    if (entries && voltage_table_offset) {
			printf("\t%d entries in voltage drop table\n", entries);
			printf("\tvoltage drop table at 0x%x\n", voltage_table_offset);
			for (j = 0; j < entries; j++) {
			    printf("\t\tvoltage drop register: 0x%x\n",
				   BIOS16(voltage_table_offset) * 4);
			    printf("\t\tvoltage drop register bits: %d\n",
				   BIOS8(voltage_table_offset + 0x2));
			}
		    }
		}
		if (powerplay_rev < 5) {
		    if (BIOS16(powerplayoffset + 5 + (i * 0x10) + 0xe))
			printf("\tpowerplay memory reset table at 0x%x\n",
			       BIOS16(powerplayoffset + 5 + (i * 0x10) + 0xe));
		} else {
		    int powerplay_flags2 = BIOS16(powerplayoffset + 5 + (i * 0x10) + 0xe);
		    if (powerplay_flags2 & 0x1)
			printf("\tenable frame modulation w/ 2 level grey\n");
		    else if (powerplay_flags2 & 0x2)
			printf("\tenable frame modulation w/ 4 level grey\n");
		    else
			printf("\tdisable frame modulation\n");
		    if (powerplay_flags2 & 0x4)
			printf("\tenable dynamic half clock\n");
		    if (powerplay_flags2 & 0x8)
			printf("\tenable dynamic clock on static screen\n");
		    if (powerplay_flags2 & 0x10)
			printf("\tenable dynamic clocking in 2D\n");
		    if ((powerplay_flags2 & 0xe0) == 0x20)
			printf("\tdynamic clocking divider: 1/2\n");
		    else if ((powerplay_flags2 & 0xe0) == 0x40)
			printf("\tdynamic clocking divider: 1/3\n");
		    else if ((powerplay_flags2 & 0xe0) == 0x60)
			printf("\tdynamic clocking divider: 1/4\n");
		    printf("\tdelay between voltage drop and sclk change: ");
		    if ((powerplay_flags2 & 0x700) == 0)
			printf("no delay\n");
		    else if ((powerplay_flags2 & 0x700) == 0x100)
			printf("33 us\n");
		    else if ((powerplay_flags2 & 0x700) == 0x200)
			printf("66 us\n");
		    else if ((powerplay_flags2 & 0x700) == 0x300)
			printf("99 us\n");
		    else if ((powerplay_flags2 & 0x700) == 0x400)
			printf("132 us\n");
		    if (powerplay_rev > 6) {
			int pcie_lanes;
			if (powerplay_flags2 & 0x800)
			    printf("\tperformance mode\n");
			if (powerplay_flags2 & 0x1000)
			    printf("\tvri-brightness required\n");
			pcie_lanes = BIOS8(powerplayoffset + 5 + (i * 0x10) + 0x10);
			if (pcie_lanes == 0)
			    pcie_lanes = 16;
			printf("\t%d pcie lanes\n", pcie_lanes);
		    }
		}
	    }
	}
	lcdddcoffset = BIOS16(offset+0x15);
	if (lcdddcoffset) {
	    int i2cline;
	    printf("LCD DDC table revision: %d\n", BIOS8(lcdddcoffset));
	    i2cline = BIOS8(lcdddcoffset+0x2);
	    printf("LCD DDC table i2c line: %d\n", i2cline);
	    if (i2cline > 4) {
		printf("LCD DDC table i2c clock pin mask: 0x%x\n", BIOS32(lcdddcoffset+0x3));
		printf("LCD DDC table i2c data pin mask: 0x%x\n", BIOS32(lcdddcoffset+0x7));
	    }
	}
	tmdspoweroffset = BIOS16(offset+0x17);
	if (tmdspoweroffset) {
	    int tmdsflags = BIOS8(tmdspoweroffset + 1);
	    int ontable_offset = BIOS16(tmdspoweroffset + 2);
	    int ontable_rev = BIOS8(ontable_offset);
	    int offtable_offset = BIOS16(tmdspoweroffset + 4);
	    int offtable_rev = BIOS8(offtable_offset);
	    printf("tmds power on/off table at offset: 0x%x\n", tmdspoweroffset);
	    printf("tmds power on/off table revision: %d\n", BIOS8(tmdspoweroffset));
	    if (tmdsflags & 1)
		printf("external tmds\n");
	    else
		printf("internal tmds\n");
	    printf("tmds power on table revision: %d\n", ontable_rev);
	    if (ontable_rev == 1) {
		// fix me
	    } else if (ontable_rev == 2) {
		int j = 4;
		int type;
		int blocks = BIOS8(ontable_offset + 3);
		int start = BIOS16(ontable_offset + j);
		int size = BIOS16(ontable_offset + 1);
		printf("tmds on table size: %d bytes\n", size);
		printf("tmds on table blocks: %d \n", blocks);
		while (blocks > 0) {
		    type = (start & 0xe000) >> 13;
		    if (type == 0) {
			unsigned int reg = BIOS16(ontable_offset + j) & 0x1fff;
			unsigned int val = BIOS32(ontable_offset + j + 2);
			printf("mmio reg: 0x%x, val: 0x%x\n", reg, val);
			j += 6;
		    } else if (type == 2) {
			unsigned int reg = BIOS16(ontable_offset + j) & 0x1fff;
			unsigned int and_mask = BIOS32(ontable_offset + j + 2);
			unsigned int or_mask = BIOS32(ontable_offset + j + 6);
			printf("mmio reg: 0x%x, AND mask: 0x%x, OR mask: 0x%x\n",
			       reg * 4, and_mask, or_mask);
			j += 10;
		    } else if (type == 3) {
			printf("delay %d ms\n", BIOS16(ontable_offset + j + 2));
			j += 4;
		    } else if (type == 4) {
			printf("delay %d us\n", BIOS16(ontable_offset + j + 2));
			j += 4;
		    } else if (type == 6) {
			int i2c_slave_addr = BIOS16(ontable_offset + j) & 0xff;
			int i2c_id = BIOS8(ontable_offset + j + 2);
			int i2c_reg = BIOS8(ontable_offset + j + 3);
			int i2c_reg_val = BIOS8(ontable_offset + j + 4);
			printf("write to i2c addr 0x%x (i2c id %d) reg: 0x%x, val: 0x%x\n",
			       i2c_slave_addr, i2c_id, i2c_reg, i2c_reg_val);
			j += 5;
		    } else
			break;
		    blocks--;;
		    start = BIOS16(ontable_offset + j);
		}
	    }
	    printf("tmds power off table revision: %d\n", offtable_rev);
	    if (offtable_rev == 1) {
		// fix me
	    } else if (offtable_rev == 2) {
		int j = 4;
		int type;
		int blocks = BIOS8(offtable_offset + 3);
		int start = BIOS16(offtable_offset + j);
		int size = BIOS16(offtable_offset + 1);
		printf("tmds off table size: %d bytes\n", size);
		printf("tmds off table blocks: %d \n", blocks);
		while (blocks > 0) {
		    type = (start & 0xe000) >> 13;
		    if (type == 0) {
			unsigned int reg = BIOS16(offtable_offset + j) & 0x1fff;
			unsigned int val = BIOS32(offtable_offset + j + 2);
			printf("mmio reg: 0x%x, val: 0x%x\n", reg, val);
			j += 6;
		    } else if (type == 2) {
			unsigned int reg = BIOS16(offtable_offset + j) & 0x1fff;
			unsigned int and_mask = BIOS32(offtable_offset + j + 2);
			unsigned int or_mask = BIOS32(offtable_offset + j + 6);
			printf("pll reg: 0x%x, AND mask: 0x%x, OR mask: 0x%x\n",
			       reg * 4, and_mask, or_mask);
			j += 10;
		    } else if (type == 3) {
			printf("delay %d ms\n", BIOS16(offtable_offset + j + 2));
			j += 4;
		    } else if (type == 4) {
			printf("delay %d us\n", BIOS16(offtable_offset + j + 2));
			j += 4;
		    } else if (type == 6) {
			int i2c_slave_addr = BIOS16(offtable_offset + j) & 0xff;
			int i2c_id = BIOS8(offtable_offset + j + 2);
			int i2c_reg = BIOS8(offtable_offset + j + 3);
			int i2c_reg_val = BIOS8(offtable_offset + j + 4);
			printf("write to i2c addr 0x%x (i2c id %d) reg: 0x%x, val: 0x%x\n",
			       i2c_slave_addr, i2c_id, i2c_reg, i2c_reg_val);
			j += 5;
		    } else
			break;
		    blocks--;
		    start = BIOS16(offtable_offset + j);
		}
	    }
	}
	if (rev > 7) {
		int external_ss_table = BIOS16(offset+0x15);
		if (external_ss_table) {
			printf("External SS table present %d\n", external_ss_table);
			printf("revision %d size bytes %d block %d\n", BIOS8(external_ss_table), BIOS8(external_ss_table + 1), BIOS8(external_ss_table + 2));

			printf("header bytes %d\n", BIOS8(external_ss_table + 3));
		}
		
	}
}