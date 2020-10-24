static void
watch_smartcards (GTask               *task,
                  GsdSmartcardManager *self,
                  gpointer             data,
                  GCancellable        *cancellable)
{
        GMainContext *context;
        GMainLoop *loop;

        g_debug ("Getting list of suitable drivers");
        context = g_main_context_new ();
        g_main_context_push_thread_default (context);

        activate_all_drivers_async (self,
                                    cancellable,
                                    (GAsyncReadyCallback) on_all_drivers_activated,
                                    task);

        loop = g_main_loop_new (context, FALSE);
        g_main_loop_run (loop);
        g_main_loop_unref (loop);

        g_main_context_pop_thread_default (context);
        g_main_context_unref (context);
}