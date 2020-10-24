    s_log(level, "stunnel " STUNNEL_VERSION " on " HOST " platform");
    if(SSLeay()==SSLEAY_VERSION_NUMBER) {
        s_log(level, "Compiled/running with " OPENSSL_VERSION_TEXT);
    } else {
        s_log(level, "Compiled with " OPENSSL_VERSION_TEXT);
        s_log(level, "Running  with %s", SSLeay_version(SSLEAY_VERSION));
        s_log(level, "Update OpenSSL shared libraries or rebuild stunnel");
    }
    s_log(level,

        "Threading:"
#ifdef USE_UCONTEXT
        "UCONTEXT"
#endif
#ifdef USE_PTHREAD
        "PTHREAD"
#endif
#ifdef USE_WIN32
        "WIN32"
#endif
#ifdef USE_FORK
        "FORK"
#endif

        " Sockets:"
#ifdef USE_POLL
        "POLL"
#else /* defined(USE_POLL) */
        "SELECT"
#endif /* defined(USE_POLL) */
        ",IPv%c"
#ifdef HAVE_SYSTEMD_SD_DAEMON_H
        ",SYSTEMD"
#endif /* HAVE_SYSTEMD_SD_DAEMON_H */

#if defined HAVE_OSSL_ENGINE_H || defined HAVE_OSSL_OCSP_H || defined USE_FIPS
        " SSL:"
#define ITEM_SEPARATOR ""
#ifdef HAVE_OSSL_ENGINE_H
        "ENGINE"
#undef ITEM_SEPARATOR
#define ITEM_SEPARATOR ","
#endif /* defined(HAVE_OSSL_ENGINE_H) */
#ifdef HAVE_OSSL_OCSP_H
        ITEM_SEPARATOR "OCSP"
#undef ITEM_SEPARATOR
#define ITEM_SEPARATOR ","
#endif /* HAVE_OSSL_OCSP_H */
#ifdef USE_FIPS
        ITEM_SEPARATOR "FIPS"
#endif /* USE_FIPS */
#endif /* an SSL optional feature enabled */

#ifdef USE_LIBWRAP
        " Auth:LIBWRAP"
#endif

        , /* supported IP version parameter */
#if defined(USE_WIN32) && !defined(_WIN32_WCE)
        s_getaddrinfo ? '6' : '4'
#else /* defined(USE_WIN32) */
#if defined(USE_IPv6)
        '6'
#else /* defined(USE_IPv6) */
        '4'
#endif /* defined(USE_IPv6) */
#endif /* defined(USE_WIN32) */
        );
#ifdef errno
#define xstr(a) str(a)
#define str(a) #a
    s_log(LOG_DEBUG, "errno: " xstr(errno));
#endif /* errno */
}
