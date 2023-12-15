#include "Threads.h"

Thread::Thread(LPSECURITY_ATTRIBUTES   lpThreadAttributes0, SIZE_T dwStackSize0, LPTHREAD_START_ROUTINE  lpStartAddress0, __drv_aliasesMem LPVOID lpParameter0, DWORD dwCreationFlags0, LPDWORD lpThreadId0) {
	lpThreadAttributes = lpThreadAttributes0;
	dwStackSize = dwStackSize0;
	lpStartAddress = lpStartAddress0;
	lpParameter = lpParameter0;
	dwCreationFlags = dwCreationFlags0;
	lpThreadId = lpThreadId0;
	RemoteFlag = 0;
	ExFlag = 0;
}
Thread::Thread(HANDLE hProcess0, LPSECURITY_ATTRIBUTES   lpThreadAttributes0, SIZE_T dwStackSize0, LPTHREAD_START_ROUTINE  lpStartAddress0, __drv_aliasesMem LPVOID lpParameter0, DWORD dwCreationFlags0, LPDWORD lpThreadId0) {
	lpThreadAttributes = lpThreadAttributes0;
	dwStackSize = dwStackSize0;
	lpStartAddress = lpStartAddress0;
	lpParameter = lpParameter0;
	dwCreationFlags = dwCreationFlags0;
	lpThreadId = lpThreadId0;
	hProcess = hProcess0;
	RemoteFlag = 1;
	ExFlag = 0;
}
Thread::Thread(HANDLE hProcess0, LPSECURITY_ATTRIBUTES   lpThreadAttributes0, SIZE_T dwStackSize0, LPTHREAD_START_ROUTINE  lpStartAddress0, __drv_aliasesMem LPVOID lpParameter0, DWORD dwCreationFlags0, LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList0,LPDWORD lpThreadId0) {
	lpThreadAttributes = lpThreadAttributes0;
	dwStackSize = dwStackSize0;
	lpStartAddress = lpStartAddress0;
	lpParameter = lpParameter0;
	dwCreationFlags = dwCreationFlags0;
	lpThreadId = lpThreadId0;
	hProcess = hProcess0;
	lpAttributeList = lpAttributeList0;
	RemoteFlag = 1;
	ExFlag = 1;
}
Thread::Thread() {

}
int Thread::Create() {
	if (ExFlag == 0) {
		if (RemoteFlag == 1) {
			hThread = CreateRemoteThread(hProcess, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
		}
		if (RemoteFlag == 0) {
			hThread = CreateThread(lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags, lpThreadId);
		}
	}
	if (ExFlag == 1) {
		if (RemoteFlag == 1) {
			hThread = CreateRemoteThreadEx(hProcess, lpThreadAttributes, dwStackSize, lpStartAddress, lpParameter, dwCreationFlags,lpAttributeList, lpThreadId);
		}
		if (RemoteFlag == 0) {
			return -2;
		}
	}
	if (hThread == NULL) {
		return -1;
	}
	return 0;
}
/*class Thread {
private:

public:
	LPSECURITY_ATTRIBUTES   lpThreadAttributes;
	SIZE_T                  dwStackSize;
	LPTHREAD_START_ROUTINE  lpStartAddress;
	__drv_aliasesMem LPVOID lpParameter;
	DWORD                   dwCreationFlags;
	LPDWORD                 lpThreadId;
	HANDLE hThread;
	HANDLE hProcess;

	Thread(LPSECURITY_ATTRIBUTES   lpThreadAttributes,SIZE_T dwStackSize,LPTHREAD_START_ROUTINE  lpStartAddress,__drv_aliasesMem LPVOID lpParameter,DWORD dwCreationFlags,LPDWORD lpThreadId);
	Thread(HANDLE hProcess,LPSECURITY_ATTRIBUTES   lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE  lpStartAddress, __drv_aliasesMem LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
	int Create();
	int Terminate();
	int Suspend();
	int Resume();
	DWORD GetID();
	HANDLE GetHandle();
	void GetStackLimits(PULONG_PTR LowLimit,PULONG_PTR HighLimit);
	HRESULT GetDescription(PWSTR* ppszThreadDescription);
	BOOL GetIdealProcessorEx(PPROCESSOR_NUMBER lpIdealProcessor);
	BOOL GetInformation(THREAD_INFORMATION_CLASS ThreadInformationClass,LPVOID ThreadInformation,DWORD ThreadInformationSize);
	BOOL GetThreadIOPendingFlag(PBOOL  lpIOIsPending);
	BOOL SetPriorityBoost(BOOL   bDisablePriorityBoost);
};*/
BOOL Thread::Terminate(DWORD  dwExitCode) {
	return TerminateThread(hThread,dwExitCode);
}
BOOL Thread::Suspend() {
	return SuspendThread(hThread);
}
BOOL Thread::Resume() {
	return ResumeThread(hThread);
}
DWORD Thread::GetID() {
	return GetThreadId(hThread);
}
HANDLE Thread::GetHandle() {
	return hThread;
}
HRESULT  Thread::GetDescription(PWSTR * d) {
	return GetThreadDescription(hThread, d);
}
BOOL Thread::GetIdealProcessorEx(PPROCESSOR_NUMBER lpIdealProcessor) {
	return GetThreadIdealProcessorEx(hThread,lpIdealProcessor);
}
BOOL Thread::GetInformation(THREAD_INFORMATION_CLASS ThreadInformationClass, LPVOID ThreadInformation, DWORD ThreadInformationSize) {
	return GetThreadInformation(hThread, ThreadInformationClass, ThreadInformation,ThreadInformationSize);
}
BOOL Thread::GetIOPendingFlag(PBOOL  lpIOIsPending) {
	return GetThreadIOPendingFlag(hThread, lpIOIsPending);
}
BOOL Thread::SetPriorityBoost(BOOL   bDisablePriorityBoost) {
	return SetThreadPriorityBoost(hThread,bDisablePriorityBoost);
}
BOOL Thread::SetContext(const CONTEXT* lpContext) {
	return SetThreadContext(hThread,lpContext);
}
BOOL Thread::GetContext(CONTEXT* lpContext) {
	return GetThreadContext(hThread, lpContext);
}
