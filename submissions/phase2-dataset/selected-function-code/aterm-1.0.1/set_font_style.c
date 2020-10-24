void
set_font_style(void)
{
    rstyle &= ~RS_fontMask;
    switch (charsets[screen.charset]) {
    case '0':			/* DEC Special Character & Line Drawing Set */
	rstyle |= RS_acsFont;
	break;
    case 'A':			/* United Kingdom (UK) */
	rstyle |= RS_ukFont;
	break;
    case 'B':			/* United States (USASCII) */
	break;
    case '<':			/* Multinational character set */
	break;
    case '5':			/* Finnish character set */
	break;
    case 'C':			/* Finnish character set */
	break;
    case 'K':			/* German character set */
	break;
    }
}
