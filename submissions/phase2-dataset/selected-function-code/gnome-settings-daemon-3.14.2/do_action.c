static gboolean
do_action (GsdMediaKeysManager *manager,
           guint                deviceid,
           MediaKeyType         type,
           gint64               timestamp)
{
        g_debug ("Launching action for key type '%d' (on device id %d)", type, deviceid);

        switch (type) {
        case TOUCHPAD_KEY:
                do_touchpad_action (manager);
                break;
        case TOUCHPAD_ON_KEY:
                do_touchpad_osd_action (manager, TRUE);
                break;
        case TOUCHPAD_OFF_KEY:
                do_touchpad_osd_action (manager, FALSE);
                break;
        case MUTE_KEY:
        case VOLUME_DOWN_KEY:
        case VOLUME_UP_KEY:
                do_sound_action (manager, deviceid, type, TRUE, FALSE);
                break;
        case MIC_MUTE_KEY:
                do_sound_action (manager, deviceid, MUTE_KEY, FALSE, TRUE);
                break;
        case MUTE_QUIET_KEY:
                do_sound_action (manager, deviceid, MUTE_KEY, TRUE, TRUE);
                break;
        case VOLUME_DOWN_QUIET_KEY:
                do_sound_action (manager, deviceid, VOLUME_DOWN_KEY, TRUE, TRUE);
                break;
        case VOLUME_UP_QUIET_KEY:
                do_sound_action (manager, deviceid, VOLUME_UP_KEY, TRUE, TRUE);
                break;
        case LOGOUT_KEY:
                gnome_session_shutdown (manager);
                break;
        case EJECT_KEY:
                do_eject_action (manager);
                break;
        case HOME_KEY:
                do_home_key_action (manager, timestamp);
                break;
        case SEARCH_KEY:
                do_search_action (manager, timestamp);
                break;
        case EMAIL_KEY:
                do_url_action (manager, "mailto", timestamp);
                break;
        case SCREENSAVER_KEY:
                do_lock_screensaver (manager);
                break;
        case HELP_KEY:
                do_url_action (manager, "ghelp", timestamp);
                break;
        case SCREENSHOT_KEY:
        case SCREENSHOT_CLIP_KEY:
        case WINDOW_SCREENSHOT_KEY:
        case WINDOW_SCREENSHOT_CLIP_KEY:
        case AREA_SCREENSHOT_KEY:
        case AREA_SCREENSHOT_CLIP_KEY:
                gsd_screenshot_take (type);
                break;
        case SCREENCAST_KEY:
                do_screencast_action (manager);
                break;
        case WWW_KEY:
                do_url_action (manager, "http", timestamp);
                break;
        case MEDIA_KEY:
                do_media_action (manager, timestamp);
                break;
        case CALCULATOR_KEY:
                do_execute_desktop_or_desktop (manager, "gcalctool.desktop", "gnome-calculator.desktop", timestamp);
                break;
        case CONTROL_CENTER_KEY:
                do_execute_desktop_or_desktop (manager, "gnome-control-center.desktop", NULL, timestamp);
                break;
        case PLAY_KEY:
                return do_multimedia_player_action (manager, "Play");
        case PAUSE_KEY:
                return do_multimedia_player_action (manager, "Pause");
        case STOP_KEY:
                return do_multimedia_player_action (manager, "Stop");
        case PREVIOUS_KEY:
                return do_multimedia_player_action (manager, "Previous");
        case NEXT_KEY:
                return do_multimedia_player_action (manager, "Next");
        case REWIND_KEY:
                return do_multimedia_player_action (manager, "Rewind");
        case FORWARD_KEY:
                return do_multimedia_player_action (manager, "FastForward");
        case REPEAT_KEY:
                return do_multimedia_player_action (manager, "Repeat");
        case RANDOM_KEY:
                return do_multimedia_player_action (manager, "Shuffle");
        case VIDEO_OUT_KEY:
                do_video_out_action (manager, timestamp);
                break;
        case ROTATE_VIDEO_KEY:
                do_video_rotate_action (manager, timestamp);
                break;
        case ROTATE_VIDEO_LOCK_KEY:
                do_video_rotate_lock_action (manager, timestamp);
                break;
        case MAGNIFIER_KEY:
                do_magnifier_action (manager);
                break;
        case SCREENREADER_KEY:
                do_screenreader_action (manager);
                break;
        case ON_SCREEN_KEYBOARD_KEY:
                do_on_screen_keyboard_action (manager);
                break;
	case INCREASE_TEXT_KEY:
	case DECREASE_TEXT_KEY:
		do_text_size_action (manager, type);
		break;
	case MAGNIFIER_ZOOM_IN_KEY:
	case MAGNIFIER_ZOOM_OUT_KEY:
		do_magnifier_zoom_action (manager, type);
		break;
	case TOGGLE_CONTRAST_KEY:
		do_toggle_contrast_action (manager);
		break;
        case POWER_KEY:
                do_config_power_action (manager, "button-power", FALSE);
                break;
        case SLEEP_KEY:
                do_config_power_action (manager, "button-sleep", FALSE);
                break;
        case SUSPEND_KEY:
                do_config_power_action (manager, "button-suspend", FALSE);
                break;
        case HIBERNATE_KEY:
                do_config_power_action (manager, "button-hibernate", FALSE);
                break;
        case POWER_KEY_NO_DIALOG:
                do_config_power_action (manager, "button-power", TRUE);
                break;
        case SLEEP_KEY_NO_DIALOG:
                do_config_power_action (manager, "button-sleep", TRUE);
                break;
        case SUSPEND_KEY_NO_DIALOG:
                do_config_power_action (manager, "button-suspend", TRUE);
                break;
        case HIBERNATE_KEY_NO_DIALOG:
                do_config_power_action (manager, "button-hibernate", TRUE);
                break;
        case SCREEN_BRIGHTNESS_UP_KEY:
        case SCREEN_BRIGHTNESS_DOWN_KEY:
        case KEYBOARD_BRIGHTNESS_UP_KEY:
        case KEYBOARD_BRIGHTNESS_DOWN_KEY:
        case KEYBOARD_BRIGHTNESS_TOGGLE_KEY:
                do_brightness_action (manager, type);
                break;
        case BATTERY_KEY:
                do_battery_action (manager);
                break;
        /* Note, no default so compiler catches missing keys */
        case CUSTOM_KEY:
                g_assert_not_reached ();
        }

        return FALSE;
}