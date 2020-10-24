cmd_show_status(void)
{
  byte tim[TM_DATETIME_BUFFER_SIZE];

  cli_msg(-1000, "BIRD " BIRD_VERSION);
  tm_format_datetime(tim, &config->tf_base, now);
  cli_msg(-1011, "Router ID is %R", config->router_id);
  cli_msg(-1011, "Current server time is %s", tim);
  tm_format_datetime(tim, &config->tf_base, boot_time);
  cli_msg(-1011, "Last reboot on %s", tim);
  tm_format_datetime(tim, &config->tf_base, config->load_time);
  cli_msg(-1011, "Last reconfiguration on %s", tim);

  graceful_restart_show_status();

  if (shutting_down)
    cli_msg(13, "Shutdown in progress");
  else if (configuring)
    cli_msg(13, "Reconfiguration in progress");
  else
    cli_msg(13, "Daemon is up and running");
}
