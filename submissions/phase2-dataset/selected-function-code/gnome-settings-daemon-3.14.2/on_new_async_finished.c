static void
on_new_async_finished (GObject      *source_object,
                       GAsyncResult *result,
                       GTask        *task)
{
        GError *error = NULL;
        GObject *object;

        object = g_async_initable_new_finish (G_ASYNC_INITABLE (source_object),
                                              result,
                                              &error);

        if (object == NULL) {
                g_task_return_error (task, error);
                goto out;
        }

        g_assert (GSD_IS_SMARTCARD_SERVICE (object));

        g_task_return_pointer (task, object, g_object_unref);
out:
        g_object_unref (task);
        return;
}