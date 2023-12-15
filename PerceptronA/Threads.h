#pragma once
#include <Windows.h>
#include <processthreadsapi.h>
class Thread {
private:
	bool RemoteFlag;
	bool ExFlag;
public:
	LPSECURITY_ATTRIBUTES   lpThreadAttributes;
	SIZE_T                  dwStackSize;
	LPTHREAD_START_ROUTINE  lpStartAddress;
	__drv_aliasesMem LPVOID lpParameter;
	DWORD                   dwCreationFlags;
	LPDWORD                 lpThreadId;
	LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList;
	HANDLE hThread;
	HANDLE hProcess;
	Thread();
	Thread(LPSECURITY_ATTRIBUTES   lpThreadAttributes,SIZE_T dwStackSize,LPTHREAD_START_ROUTINE  lpStartAddress,__drv_aliasesMem LPVOID lpParameter,DWORD dwCreationFlags,LPDWORD lpThreadId);
	Thread(HANDLE hProcess,LPSECURITY_ATTRIBUTES   lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE  lpStartAddress, __drv_aliasesMem LPVOID lpParameter, DWORD dwCreationFlags, LPDWORD lpThreadId);
	Thread(HANDLE hProcess, LPSECURITY_ATTRIBUTES   lpThreadAttributes, SIZE_T dwStackSize, LPTHREAD_START_ROUTINE  lpStartAddress, __drv_aliasesMem LPVOID lpParameter, DWORD dwCreationFlags, LPPROC_THREAD_ATTRIBUTE_LIST lpAttributeList, LPDWORD lpThreadId);
	int Create();
	BOOL Terminate(DWORD  dwExitCode);
	BOOL Suspend();
	BOOL Resume();
	DWORD GetID();
	HANDLE GetHandle();
	HRESULT GetDescription(PWSTR* ppszThreadDescription);
	BOOL GetIdealProcessorEx(PPROCESSOR_NUMBER lpIdealProcessor);
	BOOL GetInformation(THREAD_INFORMATION_CLASS ThreadInformationClass,LPVOID ThreadInformation,DWORD ThreadInformationSize);
	BOOL GetIOPendingFlag(PBOOL  lpIOIsPending);
	BOOL SetPriorityBoost(BOOL   bDisablePriorityBoost);
	BOOL SetContext(const CONTEXT* lpContext);
	BOOL GetContext(CONTEXT* lpContext);
};