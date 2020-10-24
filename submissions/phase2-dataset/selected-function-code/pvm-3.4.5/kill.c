kill(pid, p_handle, signal)
int pid;
int p_handle;
int signal;
{
	HANDLE hProcess;

	// DebugBreak();
	if (p_handle)
	if (!TerminateProcess((HANDLE)p_handle,signal)) {
		pvmlogprintf("Unable to terminate process: %d \n",p_handle);
	} else return TRUE;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,pid);

	if (hProcess) {
		if (!TerminateProcess(hProcess,signal)) {
			pvmlogprintf("Unable to terminate process: %d \n",hProcess);
		} else {CloseHandle (hProcess);return TRUE;}
	}
	CloseHandle (hProcess);
	return TRUE;
}
