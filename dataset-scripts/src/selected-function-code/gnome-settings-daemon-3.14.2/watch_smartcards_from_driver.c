static void
watch_smartcards_from_driver (GTask                    *task,
                              GsdSmartcardManager      *self,
                              WatchSmartcardsOperation *operation,
                              GCancellable             *cancellable)
{
        g_debug ("watching for smartcard events");
        while (!g_cancellable_is_cancelled (cancellable)) {
                gboolean watch_succeeded;
                GError *error = NULL;

                watch_succeeded = watch_one_event_from_driver (self, operation, cancellable, &error);

                if (g_task_return_error_if_cancelled (task)) {
                        break;
                }

                if (!watch_succeeded) {
                        g_task_return_error (task, error);
                        break;
                }
        }
}