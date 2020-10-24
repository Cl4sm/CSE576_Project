static GHashTable *_bt_device_sdp_browse(const gchar *device_path, const gchar *pattern)
{
    int pipefd[2];
    pipe(pipefd);

    // Fork the process into a child process to make an exec call
    int pid = fork();

    // Child (to call sdptool)
    if (pid == 0)
    {
        // close reading end in the child
        close(pipefd[0]);    

        // send stdout to the pipe
        dup2(pipefd[1], 1);
        // send stderr to the pipe
        dup2(pipefd[1], 2);

        // this descriptor is no longer needed
        close(pipefd[1]);

        if(!g_file_test ("/bin/sdptool", G_FILE_TEST_EXISTS))
        {
            write(2, "sdptool not found\n", sizeof("sdptool not found\n"));
            exit(EXIT_FAILURE);
        }
        
        if(pattern == NULL || strlen(pattern) == 0)
            execl("/bin/sdptool", "/bin/sdptool", "browse", "--xml", device_path, (char *) 0);
        else
            execl("/bin/sdptool", "/bin/sdptool", "browse", "--xml", "--uuid", pattern, device_path, (char *) 0);
        
    }
    if(pid == -1)
    {
        perror("forking process failed");
        exit(EXIT_FAILURE);
    }

    close(pipefd[1]);  // close the write end of the pipe in the parent
    GInputStream *exec_output = (GInputStream *) g_unix_input_stream_new(pipefd[0], TRUE);
    GDataInputStream *exec_data_input_stream = g_data_input_stream_new(exec_output);
    GPtrArray *array = g_ptr_array_new();
    guint record_counter = 0;
    guint n = 0;
    GError *error = NULL;
    GString *string_buffer = g_string_new("");
    while(TRUE)
    {
        n++;
        gchar *line = g_data_input_stream_read_line_utf8(exec_data_input_stream, NULL, NULL, &error);
        exit_if_error(error);
        // If there is no content, then break out of the loop
        if(!line)
            break;
        
        if(n == 1)
        {
            if(g_regex_match_simple("Failed", line, 0, 0) || g_regex_match_simple("Error", line, 0, 0))
            {
                g_print("%s\n", line);
                exit(EXIT_FAILURE);
            }
            else if(g_regex_match_simple("not found", line, 0, 0))
            {
                g_print("Failed to start SDP discovery. Please make sure you have bluez-utils installed on your system.\n");
                exit(EXIT_FAILURE);
            }
            
            continue;
        }
        
        if(g_regex_match_simple("<\\?xml(.*)\\?>", line, 0, 0))
        {
            if(record_counter != 0)
            {
                g_ptr_array_add(array, g_string_free(string_buffer, FALSE));
                string_buffer = g_string_new("");
            }
            record_counter++;
        }
        
        g_string_append(string_buffer, line);
    }
    
    g_ptr_array_add(array, g_string_free(string_buffer, FALSE));
    
    GHashTable *sdp_hash_table = g_hash_table_new(g_int_hash, g_int_equal);
    
    int i = 0;
    for(i = 0; i < array->len; i++)
    {
        GRegex *record_id_regex = g_regex_new("<attribute id=\"0x0000\">(\\s*<uint32 value=\"(.*)\" />\\s)</attribute>", G_REGEX_CASELESS | G_REGEX_MULTILINE | G_REGEX_NEWLINE_ANYCRLF, 0, &error);
        exit_if_error(error);
        GMatchInfo *match_info;
        g_regex_match (record_id_regex, (gchar *) g_ptr_array_index(array, i), 0, &match_info);
        if(g_match_info_matches(match_info))
        {
            gchar *word = g_match_info_fetch(match_info, 2);
            guint32 *key = g_new(guint32, 1);
            *key = 0;
            sscanf(word, "0x%x", key);
            g_hash_table_insert(sdp_hash_table, key, (gchar *) g_ptr_array_index(array, i));
            g_free(word);
        }
        g_match_info_free(match_info);
        g_regex_unref(record_id_regex);
    }
    
    g_input_stream_close(exec_output, NULL, &error);
    exit_if_error(error);
    g_object_unref(exec_data_input_stream);
    g_object_unref(exec_output);
    
    g_ptr_array_unref(array);
    
    return sdp_hash_table;
}
