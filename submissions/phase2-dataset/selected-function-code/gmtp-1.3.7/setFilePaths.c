    // Get our executable location.
    applicationpath = getRuntimePath(argc, argv);

    // Set our image locations.
    file_logo_png = g_strdup_printf("%s/../share/gmtp/logo.png", applicationpath);
    file_icon48_png = g_strdup_printf("%s/../share/gmtp/gmtpicon.png", applicationpath);
    file_icon16_png = g_strdup_printf("%s/../share/gmtp/icon-16.png", applicationpath);
    file_about_png = g_strdup_printf("%s/../share/gmtp/stock-about-16.png", applicationpath);
    file_format_png = g_strdup_printf("%s/../share/gmtp/view-refresh.png", applicationpath);

    file_audio_png = g_strdup_printf("%s/../share/gmtp/audio-x-mpeg.png", applicationpath);
    file_video_png = g_strdup_printf("%s/../share/gmtp/video-x-generic.png", applicationpath);
    file_playlist_png = g_strdup_printf("%s/../share/gmtp/audio-x-mp3-playlist.png", applicationpath);
    file_album_png = g_strdup_printf("%s/../share/gmtp/media-cdrom-audio.png", applicationpath);
    file_textfile_png = g_strdup_printf("%s/../share/gmtp/text-plain.png", applicationpath);
    file_generic_png = g_strdup_printf("%s/../share/gmtp/empty.png", applicationpath);
    file_folder_png = g_strdup_printf("%s/../share/gmtp/folder.png", applicationpath);
    file_image_png = g_strdup_printf("%s/../share/gmtp/image-x-generic.png", applicationpath);
} // end setFilePaths()
