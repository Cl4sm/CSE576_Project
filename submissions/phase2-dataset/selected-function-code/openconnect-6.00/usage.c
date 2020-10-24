static void usage(void)
{
	printf(_("Usage:  openconnect [options] <server>\n"));
	printf(_("Open client for Cisco AnyConnect VPN, version %s\n\n"), openconnect_version_str);
	print_build_opts();
	printf("      --config=CONFIGFILE         %s\n", _("Read options from config file"));
#ifndef _WIN32
	printf("  -b, --background                %s\n", _("Continue in background after startup"));
	printf("      --pid-file=PIDFILE          %s\n", _("Write the daemon's PID to this file"));
#endif
	printf("  -c, --certificate=CERT          %s\n", _("Use SSL client certificate CERT"));
	printf("  -e, --cert-expire-warning=DAYS  %s\n", _("Warn when certificate lifetime < DAYS"));
	printf("  -k, --sslkey=KEY                %s\n", _("Use SSL private key file KEY"));
	printf("  -C, --cookie=COOKIE             %s\n", _("Use WebVPN cookie COOKIE"));
	printf("      --cookie-on-stdin           %s\n", _("Read cookie from standard input"));
	printf("  -d, --deflate                   %s\n", _("Enable compression (default)"));
	printf("  -D, --no-deflate                %s\n", _("Disable compression"));
	printf("      --force-dpd=INTERVAL        %s\n", _("Set minimum Dead Peer Detection interval"));
	printf("  -g, --usergroup=GROUP           %s\n", _("Set login usergroup"));
	printf("  -h, --help                      %s\n", _("Display help text"));
	printf("  -i, --interface=IFNAME          %s\n", _("Use IFNAME for tunnel interface"));
#ifndef _WIN32
	printf("  -l, --syslog                    %s\n", _("Use syslog for progress messages"));
#endif
	printf("      --timestamp                 %s\n", _("Prepend timestamp to progress messages"));
#ifndef _WIN32
	printf("  -U, --setuid=USER               %s\n", _("Drop privileges after connecting"));
	printf("      --csd-user=USER             %s\n", _("Drop privileges during CSD execution"));
	printf("      --csd-wrapper=SCRIPT        %s\n", _("Run SCRIPT instead of CSD binary"));
#endif
	printf("  -m, --mtu=MTU                   %s\n", _("Request MTU from server"));
	printf("      --base-mtu=MTU              %s\n", _("Indicate path MTU to/from server"));
	printf("  -p, --key-password=PASS         %s\n", _("Set key passphrase or TPM SRK PIN"));
	printf("      --key-password-from-fsid    %s\n", _("Key passphrase is fsid of file system"));
	printf("  -P, --proxy=URL                 %s\n", _("Set proxy server"));
	printf("      --proxy-auth=METHODS        %s\n", _("Set proxy authentication methods"));
	printf("      --no-proxy                  %s\n", _("Disable proxy"));
	printf("      --libproxy                  %s\n", _("Use libproxy to automatically configure proxy"));
#ifndef LIBPROXY_HDR
	printf("                                  %s\n", _("(NOTE: libproxy disabled in this build)"));
#endif
	printf("      --pfs                       %s\n", _("Require perfect forward secrecy"));
	printf("  -q, --quiet                     %s\n", _("Less output"));
	printf("  -Q, --queue-len=LEN             %s\n", _("Set packet queue limit to LEN pkts"));
	printf("  -s, --script=SCRIPT             %s\n", _("Shell command line for using a vpnc-compatible config script"));
	printf("                                  %s: \"%s\"\n", _("default"), default_vpncscript);
#ifndef _WIN32
	printf("  -S, --script-tun                %s\n", _("Pass traffic to 'script' program, not tun"));
#endif
	printf("  -u, --user=NAME                 %s\n", _("Set login username"));
	printf("  -V, --version                   %s\n", _("Report version number"));
	printf("  -v, --verbose                   %s\n", _("More output"));
	printf("      --dump-http-traffic         %s\n", _("Dump HTTP authentication traffic (implies --verbose"));
	printf("  -x, --xmlconfig=CONFIG          %s\n", _("XML config file"));
	printf("      --authgroup=GROUP           %s\n", _("Choose authentication login selection"));
	printf("      --authenticate              %s\n", _("Authenticate only and print login info"));
	printf("      --cookieonly                %s\n", _("Fetch webvpn cookie only; don't connect"));
	printf("      --printcookie               %s\n", _("Print webvpn cookie before connecting"));
	printf("      --cafile=FILE               %s\n", _("Cert file for server verification"));
	printf("      --disable-ipv6              %s\n", _("Do not ask for IPv6 connectivity"));
	printf("      --dtls-ciphers=LIST         %s\n", _("OpenSSL ciphers to support for DTLS"));
	printf("      --no-dtls                   %s\n", _("Disable DTLS"));
	printf("      --no-http-keepalive         %s\n", _("Disable HTTP connection re-use"));
	printf("      --no-passwd                 %s\n", _("Disable password/SecurID authentication"));
	printf("      --no-cert-check             %s\n", _("Do not require server SSL cert to be valid"));
	printf("      --no-xmlpost                %s\n", _("Do not attempt XML POST authentication"));
	printf("      --non-inter                 %s\n", _("Do not expect user input; exit if it is required"));
	printf("      --passwd-on-stdin           %s\n", _("Read password from standard input"));
	printf("      --token-mode=MODE           %s\n", _("Software token type: rsa, totp or hotp"));
	printf("      --token-secret=STRING       %s\n", _("Software token secret"));
#ifndef HAVE_LIBSTOKEN
	printf("                                  %s\n", _("(NOTE: libstoken (RSA SecurID) disabled in this build)"));
#endif
#ifndef HAVE_LIBOATH
	printf("                                  %s\n", _("(NOTE: liboath (TOTP,HOTP) disabled in this build)"));
#endif
	printf("      --reconnect-timeout         %s\n", _("Connection retry timeout in seconds"));
	printf("      --servercert=FINGERPRINT    %s\n", _("Server's certificate SHA1 fingerprint"));
	printf("      --useragent=STRING          %s\n", _("HTTP header User-Agent: field"));
	printf("      --os=STRING                 %s\n", _("OS type (linux,linux-64,win,...) to report"));
	printf("      --dtls-local-port=PORT      %s\n", _("Set local port for DTLS datagrams"));
	printf("\n");

	helpmessage();
	exit(1);
}