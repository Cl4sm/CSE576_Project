void init_sound_drivers(void)
{
#ifdef SOUND_AHI
	register_sound_driver(&sound_ahi);
#endif
#ifdef SOUND_BEOS
	register_sound_driver(&sound_beos);
#endif
#ifdef SOUND_SNDIO
	register_sound_driver(&sound_sndio);
#endif
#ifdef SOUND_NETBSD
	register_sound_driver(&sound_netbsd);
#endif
#ifdef SOUND_BSD
	register_sound_driver(&sound_bsd);
#endif
#ifdef SOUND_SOLARIS
	register_sound_driver(&sound_solaris);
#endif
#ifdef SOUND_SGI
	register_sound_driver(&sound_sgi);
#endif
#ifdef SOUND_HPUX
	register_sound_driver(&sound_hpux);
#endif
#ifdef SOUND_AIX
	register_sound_driver(&sound_aix);
#endif
#ifdef SOUND_COREAUDIO
	register_sound_driver(&sound_coreaudio);
#endif
#ifdef SOUND_WIN32
	register_sound_driver(&sound_win32);
#endif
#ifdef SOUND_PULSEAUDIO
	register_sound_driver(&sound_pulseaudio);
#endif
#ifdef SOUND_ALSA
	register_sound_driver(&sound_alsa);
#endif
#ifdef SOUND_ALSA05
	register_sound_driver(&sound_alsa05);
#endif
#ifdef SOUND_OSS
	register_sound_driver(&sound_oss);
#endif
#ifdef SOUND_QNX
	register_sound_driver(&sound_qnx);
#endif
	register_sound_driver(&sound_wav);
	register_sound_driver(&sound_file);
	register_sound_driver(&sound_null);
}