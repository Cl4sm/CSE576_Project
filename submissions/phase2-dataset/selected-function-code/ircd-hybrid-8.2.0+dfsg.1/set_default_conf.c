set_default_conf(void)
{
  /* verify init_class() ran, this should be an unnecessary check
   * but its not much work.
   */
  assert(class_default == class_get_list()->tail->data);

#ifdef HAVE_LIBCRYPTO
#if OPENSSL_VERSION_NUMBER >= 0x1000005FL && !defined(OPENSSL_NO_ECDH)
  {
    EC_KEY *key = EC_KEY_new_by_curve_name(NID_X9_62_prime256v1);

    if (key)
    {
      SSL_CTX_set_tmp_ecdh(ConfigServerInfo.server_ctx, key);
      EC_KEY_free(key);
    }
  }

  SSL_CTX_set_options(ConfigServerInfo.server_ctx, SSL_OP_SINGLE_ECDH_USE);
#endif

  ConfigServerInfo.message_digest_algorithm = EVP_sha256();
  ConfigServerInfo.rsa_private_key = NULL;
  ConfigServerInfo.rsa_private_key_file = NULL;
#endif

  /* ConfigServerInfo.name is not rehashable */
  /* ConfigServerInfo.name = ConfigServerInfo.name; */
  ConfigServerInfo.description = NULL;
  ConfigServerInfo.network_name = xstrdup(NETWORK_NAME_DEFAULT);
  ConfigServerInfo.network_desc = xstrdup(NETWORK_DESC_DEFAULT);

  memset(&ConfigServerInfo.ip, 0, sizeof(ConfigServerInfo.ip));
  ConfigServerInfo.specific_ipv4_vhost = 0;
  memset(&ConfigServerInfo.ip6, 0, sizeof(ConfigServerInfo.ip6));
  ConfigServerInfo.specific_ipv6_vhost = 0;

  ConfigServerInfo.max_clients = MAXCLIENTS_MAX;
  ConfigServerInfo.max_nick_length = 9;
  ConfigServerInfo.max_topic_length = 80;
  ConfigServerInfo.hub = 0;

  ConfigAdminInfo.name = NULL;
  ConfigAdminInfo.email = NULL;
  ConfigAdminInfo.description = NULL;

  log_del_all();

  ConfigLog.use_logging = 1;

  ConfigChannel.disable_fake_channels = 0;
  ConfigChannel.invite_client_count = 10;
  ConfigChannel.invite_client_time = 300;
  ConfigChannel.knock_client_count = 1;
  ConfigChannel.knock_client_time = 300;
  ConfigChannel.knock_delay_channel = 60;
  ConfigChannel.max_channels = 25;
  ConfigChannel.max_bans = 25;
  ConfigChannel.default_split_user_count = 0;
  ConfigChannel.default_split_server_count = 0;
  ConfigChannel.no_join_on_split = 0;
  ConfigChannel.no_create_on_split = 0;

  ConfigServerHide.flatten_links = 0;
  ConfigServerHide.links_delay = 300;
  ConfigServerHide.hidden = 0;
  ConfigServerHide.hide_servers = 0;
  ConfigServerHide.hide_services = 0;
  ConfigServerHide.hidden_name = xstrdup(NETWORK_NAME_DEFAULT);
  ConfigServerHide.hide_server_ips = 0;
  ConfigServerHide.disable_remote_commands = 0;

  ConfigGeneral.away_count = 2;
  ConfigGeneral.away_time = 10;
  ConfigGeneral.max_watch = WATCHSIZE_DEFAULT;
  ConfigGeneral.cycle_on_host_change = 1;
  ConfigGeneral.glines = 0;
  ConfigGeneral.gline_time = 12 * 3600;
  ConfigGeneral.gline_request_time = GLINE_REQUEST_EXPIRE_DEFAULT;
  ConfigGeneral.gline_min_cidr = 16;
  ConfigGeneral.gline_min_cidr6 = 48;
  ConfigGeneral.invisible_on_connect = 1;
  ConfigGeneral.tkline_expire_notices = 1;
  ConfigGeneral.hide_spoof_ips = 1;
  ConfigGeneral.ignore_bogus_ts = 0;
  ConfigGeneral.disable_auth = 0;
  ConfigGeneral.kill_chase_time_limit = 90;
  ConfigGeneral.default_floodcount = 8;
  ConfigGeneral.failed_oper_notice = 1;
  ConfigGeneral.dots_in_ident = 0;
  ConfigGeneral.min_nonwildcard = 4;
  ConfigGeneral.min_nonwildcard_simple = 3;
  ConfigGeneral.max_accept = 20;
  ConfigGeneral.anti_nick_flood = 0;
  ConfigGeneral.max_nick_time = 20;
  ConfigGeneral.max_nick_changes = 5;
  ConfigGeneral.anti_spam_exit_message_time = 0;
  ConfigGeneral.ts_warn_delta = TS_WARN_DELTA_DEFAULT;
  ConfigGeneral.ts_max_delta = TS_MAX_DELTA_DEFAULT;
  ConfigGeneral.warn_no_connect_block = 1;
  ConfigGeneral.stats_e_disabled = 0;
  ConfigGeneral.stats_o_oper_only = 0;
  ConfigGeneral.stats_k_oper_only = 1;  /* 1 = masked */
  ConfigGeneral.stats_i_oper_only = 1;  /* 1 = masked */
  ConfigGeneral.stats_P_oper_only = 0;
  ConfigGeneral.stats_u_oper_only = 0;
  ConfigGeneral.caller_id_wait = 60;
  ConfigGeneral.opers_bypass_callerid = 0;
  ConfigGeneral.pace_wait = 10;
  ConfigGeneral.pace_wait_simple = 1;
  ConfigGeneral.short_motd = 0;
  ConfigGeneral.ping_cookie = 0;
  ConfigGeneral.no_oper_flood = 0;
  ConfigGeneral.true_no_oper_flood = 0;
  ConfigGeneral.oper_pass_resv = 1;
  ConfigGeneral.max_targets = MAX_TARGETS_DEFAULT;
  ConfigGeneral.oper_only_umodes = UMODE_DEBUG;
  ConfigGeneral.oper_umodes = UMODE_BOTS | UMODE_LOCOPS | UMODE_SERVNOTICE | UMODE_WALLOP;
  ConfigGeneral.throttle_count = 1;
  ConfigGeneral.throttle_time = 1;
}
