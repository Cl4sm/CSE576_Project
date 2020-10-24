void *client_thread(void *arg) {
    CLI *c=arg;

#ifdef DEBUG_STACK_SIZE
    stack_info(1); /* initialize */
#endif
    client_main(c);
#ifdef DEBUG_STACK_SIZE
    stack_info(0); /* display computed value */
#endif
    str_stats(); /* client thread allocation tracking */
    str_cleanup();
    /* s_log() is not allowed after str_cleanup() */
#if defined(USE_WIN32) && !defined(_WIN32_WCE)
    _endthread();
#endif
#ifdef USE_UCONTEXT
    s_poll_wait(NULL, 0, 0); /* wait on poll() */
#endif
    return NULL;
}
