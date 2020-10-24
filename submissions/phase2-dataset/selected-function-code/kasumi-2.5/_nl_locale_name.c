const char *
_nl_locale_name (category, categoryname)
     int category;
     const char *categoryname;
{
  const char *retval;

#ifndef WIN32

  /* Use the POSIX methods of looking to 'LC_ALL', 'LC_xxx', and 'LANG'.
     On some systems this can be done by the 'setlocale' function itself.  */
# if defined HAVE_SETLOCALE && defined HAVE_LC_MESSAGES && defined HAVE_LOCALE_NULL
  retval = setlocale (category, NULL);
# else
  /* Setting of LC_ALL overwrites all other.  */
  retval = getenv ("LC_ALL");
  if (retval == NULL || retval[0] == '\0')
    {
      /* Next comes the name of the desired category.  */
      retval = getenv (categoryname);
      if (retval == NULL || retval[0] == '\0')
	{
	  /* Last possibility is the LANG environment variable.  */
	  retval = getenv ("LANG");
	  if (retval == NULL || retval[0] == '\0')
	    /* We use C as the default domain.  POSIX says this is
	       implementation defined.  */
	    retval = "C";
	}
    }
# endif

  return retval;

#else /* WIN32 */

  /* Return an XPG style locale name language[_territory][@modifier].
     Don't even bother determining the codeset; it's not useful in this
     context, because message catalogs are not specific to a single
     codeset.  */

  LCID lcid;
  LANGID langid;
  int primary, sub;

  /* Let the user override the system settings through environment
     variables, as on POSIX systems.  */
  retval = getenv ("LC_ALL");
  if (retval != NULL && retval[0] != '\0')
    return retval;
  retval = getenv (categoryname);
  if (retval != NULL && retval[0] != '\0')
    return retval;
  retval = getenv ("LANG");
  if (retval != NULL && retval[0] != '\0')
    return retval;

  /* Use native Win32 API locale ID.  */
  lcid = GetThreadLocale ();

  /* Strip off the sorting rules, keep only the language part.  */
  langid = LANGIDFROMLCID (lcid);

  /* Split into language and territory part.  */
  primary = PRIMARYLANGID (langid);
  sub = SUBLANGID (langid);

  /* Dispatch on language.
     See also http://www.unicode.org/unicode/onlinedat/languages.html .
     For details about languages, see http://www.ethnologue.com/ .  */
  switch (primary)
    {
    case LANG_AFRIKAANS: return "af_ZA";
    case LANG_ALBANIAN: return "sq_AL";
    case 0x5e: /* AMHARIC */ return "am_ET";
    case LANG_ARABIC:
      switch (sub)
	{
	case SUBLANG_ARABIC_SAUDI_ARABIA: return "ar_SA";
	case SUBLANG_ARABIC_IRAQ: return "ar_IQ";
	case SUBLANG_ARABIC_EGYPT: return "ar_EG";
	case SUBLANG_ARABIC_LIBYA: return "ar_LY";
	case SUBLANG_ARABIC_ALGERIA: return "ar_DZ";
	case SUBLANG_ARABIC_MOROCCO: return "ar_MA";
	case SUBLANG_ARABIC_TUNISIA: return "ar_TN";
	case SUBLANG_ARABIC_OMAN: return "ar_OM";
	case SUBLANG_ARABIC_YEMEN: return "ar_YE";
	case SUBLANG_ARABIC_SYRIA: return "ar_SY";
	case SUBLANG_ARABIC_JORDAN: return "ar_JO";
	case SUBLANG_ARABIC_LEBANON: return "ar_LB";
	case SUBLANG_ARABIC_KUWAIT: return "ar_KW";
	case SUBLANG_ARABIC_UAE: return "ar_AE";
	case SUBLANG_ARABIC_BAHRAIN: return "ar_BH";
	case SUBLANG_ARABIC_QATAR: return "ar_QA";
	}
      return "ar";
    case LANG_ARMENIAN: return "hy_AM";
    case LANG_ASSAMESE: return "as_IN";
    case LANG_AZERI:
      switch (sub)
	{
	/* FIXME: Adjust this when Azerbaijani locales appear on Unix.  */
	case SUBLANG_AZERI_LATIN: return "az_AZ@latin";
	case SUBLANG_AZERI_CYRILLIC: return "az_AZ@cyrillic";
	}
      return "az";
    case LANG_BASQUE:
      return "eu"; /* Ambiguous: could be "eu_ES" or "eu_FR".  */
    case LANG_BELARUSIAN: return "be_BY";
    case LANG_BENGALI: return "bn_IN";
    case LANG_BULGARIAN: return "bg_BG";
    case 0x55: /* BURMESE */ return "my_MM";
    case 0x53: /* CAMBODIAN */ return "km_KH";
    case LANG_CATALAN: return "ca_ES";
    case 0x5c: /* CHEROKEE */ return "chr_US";
    case LANG_CHINESE:
      switch (sub)
	{
	case SUBLANG_CHINESE_TRADITIONAL: return "zh_TW";
	case SUBLANG_CHINESE_SIMPLIFIED: return "zh_CN";
	case SUBLANG_CHINESE_HONGKONG: return "zh_HK";
	case SUBLANG_CHINESE_SINGAPORE: return "zh_SG";
	case SUBLANG_CHINESE_MACAU: return "zh_MO";
	}
      return "zh";
    case LANG_CROATIAN:		/* LANG_CROATIAN == LANG_SERBIAN
				 * What used to be called Serbo-Croatian
				 * should really now be two separate
				 * languages because of political reasons.
				 * (Says tml, who knows nothing about Serbian
				 * or Croatian.)
				 * (I can feel those flames coming already.)
				 */
      switch (sub)
	{
	case SUBLANG_DEFAULT: return "hr_HR";
	case SUBLANG_SERBIAN_LATIN: return "sr_YU";
	case SUBLANG_SERBIAN_CYRILLIC: return "sr_YU@cyrillic";
	}
      return "hr";
    case LANG_CZECH: return "cs_CZ";
    case LANG_DANISH: return "da_DK";
    case LANG_DIVEHI: return "div_MV";
    case LANG_DUTCH:
      switch (sub)
	{
	case SUBLANG_DUTCH: return "nl_NL";
	case SUBLANG_DUTCH_BELGIAN: /* FLEMISH, VLAAMS */ return "nl_BE";
	}
      return "nl";
    case 0x66: /* EDO */ return "bin_NG";
    case LANG_ENGLISH:
      switch (sub)
	{
	/* SUBLANG_ENGLISH_US == SUBLANG_DEFAULT. Heh. I thought
	 * English was the language spoken in England.
	 * Oh well.
	 */
	case SUBLANG_ENGLISH_US: return "en_US";
	case SUBLANG_ENGLISH_UK: return "en_GB";
	case SUBLANG_ENGLISH_AUS: return "en_AU";
	case SUBLANG_ENGLISH_CAN: return "en_CA";
	case SUBLANG_ENGLISH_NZ: return "en_NZ";
	case SUBLANG_ENGLISH_EIRE: return "en_IE";
	case SUBLANG_ENGLISH_SOUTH_AFRICA: return "en_ZA";
	case SUBLANG_ENGLISH_JAMAICA: return "en_JM";
	case SUBLANG_ENGLISH_CARIBBEAN: return "en_GD"; /* Grenada? */
	case SUBLANG_ENGLISH_BELIZE: return "en_BZ";
	case SUBLANG_ENGLISH_TRINIDAD: return "en_TT";
	case SUBLANG_ENGLISH_ZIMBABWE: return "en_ZW";
	case SUBLANG_ENGLISH_PHILIPPINES: return "en_PH";
	}
      return "en";
    case LANG_ESTONIAN: return "et_EE";
    case LANG_FAEROESE: return "fo_FO";
    case LANG_FARSI: return "fa_IR";
    case LANG_FINNISH: return "fi_FI";
    case LANG_FRENCH:
      switch (sub)
	{
	case SUBLANG_FRENCH: return "fr_FR";
	case SUBLANG_FRENCH_BELGIAN: /* WALLOON */ return "fr_BE";
	case SUBLANG_FRENCH_CANADIAN: return "fr_CA";
	case SUBLANG_FRENCH_SWISS: return "fr_CH";
	case SUBLANG_FRENCH_LUXEMBOURG: return "fr_LU";
	case SUBLANG_FRENCH_MONACO: return "fr_MC";
	}
      return "fr";
    case 0x62: /* FRISIAN */ return "fy_NL";
    case 0x67: /* FULFULDE */ return "ful_NG";
    case 0x3c: /* GAELIC */
      switch (sub)
	{
	case 0x01: /* SCOTTISH */ return "gd_GB";
	case 0x02: /* IRISH */ return "ga_IE";
	}
      return "C";
    case LANG_GALICIAN: return "gl_ES";
    case LANG_GEORGIAN: return "ka_GE";
    case LANG_GERMAN:
      switch (sub)
	{
	case SUBLANG_GERMAN: return "de_DE";
	case SUBLANG_GERMAN_SWISS: return "de_CH";
	case SUBLANG_GERMAN_AUSTRIAN: return "de_AT";
	case SUBLANG_GERMAN_LUXEMBOURG: return "de_LU";
	case SUBLANG_GERMAN_LIECHTENSTEIN: return "de_LI";
	}
      return "de";
    case LANG_GREEK: return "el_GR";
    case 0x74: /* GUARANI */ return "gn_PY";
    case LANG_GUJARATI: return "gu_IN";
    case 0x68: /* HAUSA */ return "ha_NG";
    case 0x75: /* HAWAIIAN */
      /* FIXME: Do they mean Hawaiian ("haw_US", 1000 speakers)
	 or Hawaii Creole English ("cpe_US", 600000 speakers)?  */
      return "cpe_US";
    case LANG_HEBREW: return "he_IL";
    case LANG_HINDI: return "hi_IN";
    case LANG_HUNGARIAN: return "hu_HU";
    case 0x69: /* IBIBIO */ return "nic_NG";
    case LANG_ICELANDIC: return "is_IS";
    case 0x70: /* IGBO */ return "ibo_NG";
    case LANG_INDONESIAN: return "id_ID";
    case 0x5d: /* INUKTITUT */ return "iu_CA";
    case LANG_ITALIAN:
      switch (sub)
	{
	case SUBLANG_ITALIAN: return "it_IT";
	case SUBLANG_ITALIAN_SWISS: return "it_CH";
	}
      return "it";
    case LANG_JAPANESE: return "ja_JP";
    case LANG_KANNADA: return "kn_IN";
    case 0x71: /* KANURI */ return "kau_NG";
    case LANG_KASHMIRI:
      switch (sub)
	{
	case SUBLANG_DEFAULT: return "ks_PK";
	case SUBLANG_KASHMIRI_INDIA: return "ks_IN";
	}
      return "ks";
    case LANG_KAZAK: return "kk_KZ";
    case LANG_KONKANI:
      /* FIXME: Adjust this when such locales appear on Unix.  */
      return "kok_IN";
    case LANG_KOREAN: return "ko_KR";
    case LANG_KYRGYZ: return "ky_KG";
    case 0x54: /* LAO */ return "lo_LA";
    case 0x76: /* LATIN */ return "la_VA";
    case LANG_LATVIAN: return "lv_LV";
    case LANG_LITHUANIAN: return "lt_LT";
    case LANG_MACEDONIAN: return "mk_MK";
    case LANG_MALAY:
      switch (sub)
	{
	case SUBLANG_MALAY_MALAYSIA: return "ms_MY";
	case SUBLANG_MALAY_BRUNEI_DARUSSALAM: return "ms_BN";
	}
      return "ms";
    case LANG_MALAYALAM: return "ml_IN";
    case 0x3a: /* MALTESE */ return "mt_MT";
    case LANG_MANIPURI:
      /* FIXME: Adjust this when such locales appear on Unix.  */
      return "mni_IN";
    case LANG_MARATHI: return "mr_IN";
    case LANG_MONGOLIAN:
      return "mn"; /* Ambiguous: could be "mn_CN" or "mn_MN".  */
    case LANG_NEPALI:
      switch (sub)
	{
	case SUBLANG_DEFAULT: return "ne_NP";
	case SUBLANG_NEPALI_INDIA: return "ne_IN";
	}
      return "ne";
    case LANG_NORWEGIAN:
      switch (sub)
	{
	case SUBLANG_NORWEGIAN_BOKMAL: return "no_NO";
	case SUBLANG_NORWEGIAN_NYNORSK: return "nn_NO";
	}
      return "no";
    case LANG_ORIYA: return "or_IN";
    case 0x72: /* OROMO */ return "om_ET";
    case 0x79: /* PAPIAMENTU */ return "pap_AN";
    case 0x63: /* PASHTO */
      return "ps"; /* Ambiguous: could be "ps_PK" or "ps_AF".  */
    case LANG_POLISH: return "pl_PL";
    case LANG_PORTUGUESE:
      switch (sub)
	{
	case SUBLANG_PORTUGUESE: return "pt_PT";
	/* Hmm. SUBLANG_PORTUGUESE_BRAZILIAN == SUBLANG_DEFAULT.
	   Same phenomenon as SUBLANG_ENGLISH_US == SUBLANG_DEFAULT. */
	case SUBLANG_PORTUGUESE_BRAZILIAN: return "pt_BR";
	}
      return "pt";
    case LANG_PUNJABI: return "pa_IN";
    case 0x17: /* RHAETO-ROMANCE */ return "rm_CH";
    case LANG_ROMANIAN: return "ro_RO";
    case LANG_RUSSIAN:
      return "ru"; /* Ambiguous: could be "ru_RU" or "ru_UA".  */
    case 0x3b: /* SAMI */ return "se_NO";
    case LANG_SANSKRIT: return "sa_IN";
    case LANG_SINDHI: return "sd";
    case 0x5b: /* SINHALESE */ return "si_LK";
    case LANG_SLOVAK: return "sk_SK";
    case LANG_SLOVENIAN: return "sl_SI";
    case 0x77: /* SOMALI */ return "so_SO";
    case LANG_SORBIAN:
      /* FIXME: Adjust this when such locales appear on Unix.  */
      return "wen_DE";
    case LANG_SPANISH:
      switch (sub)
	{
	case SUBLANG_SPANISH: return "es_ES";
	case SUBLANG_SPANISH_MEXICAN: return "es_MX";
	case SUBLANG_SPANISH_MODERN:
	  return "es_ES@modern";	/* not seen on Unix */
	case SUBLANG_SPANISH_GUATEMALA: return "es_GT";
	case SUBLANG_SPANISH_COSTA_RICA: return "es_CR";
	case SUBLANG_SPANISH_PANAMA: return "es_PA";
	case SUBLANG_SPANISH_DOMINICAN_REPUBLIC: return "es_DO";
	case SUBLANG_SPANISH_VENEZUELA: return "es_VE";
	case SUBLANG_SPANISH_COLOMBIA: return "es_CO";
	case SUBLANG_SPANISH_PERU: return "es_PE";
	case SUBLANG_SPANISH_ARGENTINA: return "es_AR";
	case SUBLANG_SPANISH_ECUADOR: return "es_EC";
	case SUBLANG_SPANISH_CHILE: return "es_CL";
	case SUBLANG_SPANISH_URUGUAY: return "es_UY";
	case SUBLANG_SPANISH_PARAGUAY: return "es_PY";
	case SUBLANG_SPANISH_BOLIVIA: return "es_BO";
	case SUBLANG_SPANISH_EL_SALVADOR: return "es_SV";
	case SUBLANG_SPANISH_HONDURAS: return "es_HN";
	case SUBLANG_SPANISH_NICARAGUA: return "es_NI";
	case SUBLANG_SPANISH_PUERTO_RICO: return "es_PR";
	}
      return "es";
    case 0x30: /* SUTU */ return "bnt_TZ";
    case LANG_SWAHILI: return "sw_KE";
    case LANG_SWEDISH:
      switch (sub)
	{
	case SUBLANG_DEFAULT: return "sv_SE";
	case SUBLANG_SWEDISH_FINLAND: return "sv_FI";
	}
      return "sv";
    case LANG_SYRIAC: return "syr_TR"; /* An extinct language.  */
    case 0x64: /* TAGALOG */ return "tl_PH";
    case 0x28: /* TAJIK */ return "tg_TJ";
    case 0x5f: /* TAMAZIGHT */ return "ber_MA";
    case LANG_TAMIL:
      return "ta"; /* Ambiguous: could be "ta_IN" or "ta_LK" or "ta_SG".  */
    case LANG_TATAR: return "tt_RU";
    case LANG_TELUGU: return "te_IN";
    case LANG_THAI: return "th_TH";
    case 0x51: /* TIBETAN */ return "bo_CN";
    case 0x73: /* TIGRINYA */ return "ti_ET";
    case 0x31: /* TSONGA */ return "ts_ZA";
    case LANG_TURKISH: return "tr_TR";
    case 0x42: /* TURKMEN */ return "tk_TM";
    case LANG_UKRAINIAN: return "uk_UA";
    case LANG_URDU:
      switch (sub)
	{
	case SUBLANG_URDU_PAKISTAN: return "ur_PK";
	case SUBLANG_URDU_INDIA: return "ur_IN";
	}
      return "ur";
    case LANG_UZBEK:
      switch (sub)
	{
	/* FIXME: Adjust this when Uzbek locales appear on Unix.  */
	case SUBLANG_UZBEK_LATIN: return "uz_UZ@latin";
	case SUBLANG_UZBEK_CYRILLIC: return "uz_UZ@cyrillic";
	}
      return "uz";
    case 0x33: /* VENDA */ return "ven_ZA";
    case LANG_VIETNAMESE: return "vi_VN";
    case 0x52: /* WELSH */ return "cy_GB";
    case 0x34: /* XHOSA */ return "xh_ZA";
    case 0x78: /* YI */ return "sit_CN";
    case 0x3d: /* YIDDISH */ return "yi_IL";
    case 0x6a: /* YORUBA */ return "yo_NG";
    case 0x35: /* ZULU */ return "zu_ZA";
    default: return "C";
    }

#endif
}
