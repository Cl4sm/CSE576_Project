#ifdef USE_FIPS
    if(FIPS_mode()!=global->option.fips) {
        RAND_set_rand_method(NULL); /* reset RAND methods */
        if(!FIPS_mode_set(global->option.fips)) {
            ERR_load_crypto_strings();
            sslerror("FIPS_mode_set");
            return 1;
        }
    }
    s_log(LOG_NOTICE, "FIPS mode %s",
        global->option.fips ? "enabled" : "disabled");
#endif /* USE_FIPS */
    if(init_compression(global))
        return 1;
    if(init_prng(global))
        return 1;
    s_log(LOG_DEBUG, "PRNG seeded successfully");
    return 0; /* SUCCESS */
}
