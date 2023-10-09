#pragma once
#include <Windows.h>  
#include <iostream>
#include <python.h>
#include <tlhelp32.h>

//执行python脚本
extern void python_start();
//终止python程序
extern void python_kill();

//根据程序名获取程序的进程id
DWORD GetProcessIDByName(CString processName)
{
	DWORD processID = 0;

	// Take a snapshot of all processes in the system.  
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap != INVALID_HANDLE_VALUE)
	{
		// Set the size of the structure before using it.  
		PROCESSENTRY32 processEntry;
		processEntry.dwSize = sizeof(PROCESSENTRY32);

		// Retrieve information about the first process.  
		if (Process32First(hProcessSnap, &processEntry))
		{
			// Now walk the process list of the system.  
			do
			{
				// If this is the process we are looking for, store its ID.  
				if (processEntry.szExeFile == processName)
				{
					processID = processEntry.th32ProcessID;
					break;
				}
			} while (Process32Next(hProcessSnap, &processEntry));
		}

		// Close the snapshot handle to prevent it from being locked in memory.  
		CloseHandle(hProcessSnap);
	}

	return processID;
}

//根据程序的id终止python的exe程序
void TerminatePythonProcess(DWORD processId)
{
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (processHandle != NULL)
	{
		// 终止 Python exe 程序  
		TerminateProcess(processHandle, 0); 

		// 关闭 Python exe 程序的写入句柄和读取句柄  
		CloseHandle(processHandle);
	}
}

//执行python脚本
void python_start()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	TCHAR szCommandLine[] = _T("load_code.exe");
	// 启动Python exe文件
	if (!CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		AfxMessageBox(_T("Failed to start process"));
		return ;
	}

	//开启的进程id
	DWORD processId;
	// 保存 Python exe 程序的进程 ID，退出程序的时候使用  
	processId = pi.dwProcessId;

	// 等待Python exe文件执行完毕  
	WaitForSingleObject(pi.hProcess, INFINITE);

	// 关闭进程句柄和线程句柄  
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	
	//初始化python解释器
	/*Py_Initialize();

	PyObject *pModule = NULL;
	PyObject *pFunc = NULL;

	pModule = PyImport_ImportModule("load_code");//调用的python文件名称
	if (pModule == NULL)
		AfxMessageBox(_T("pMoudle"));
	pFunc = PyObject_GetAttrString(pModule, "py_start");//调用函数名称
	if (pFunc == NULL)
		AfxMessageBox(_T("pFunc"));
	PyObject *pArgs = NULL;//python中load_code文件中main函数的参数
	//PyObject *pArgs = PyTuple_New(0); // 创建一个空的参数列表，因为你的main函数没有参数  
	//if (pArgs == NULL) {
		//printf("Cannot create argument list\n");
	//}
	PyObject* pRet = PyObject_CallObject(pFunc, pArgs);//调用函数
	if (pRet == NULL)
		AfxMessageBox(_T("pRet"));*/
	/*
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")"); // add current directory to Python path  
	PyRun_SimpleString("import load_code"); // replace with your python script name  
	PyRun_SimpleString("load_code.main()"); // replace with your function name  
	*/
	//Py_Finalize();//关闭python解释器
}

//终止python程序
void python_kill()
{
	//开启的进程id
	DWORD load_code_Id;//load_code程序id
	DWORD fir_exe_Id;//固件exe的程序id
	DWORD burn_exe_Id;//烧录exe的程序id
	
	while (1)
	{
		//检查后关闭load_code程序
		load_code_Id = GetProcessIDByName(_T("load_code.exe"));
		//退出python的exe
		TerminatePythonProcess(load_code_Id);


		//检查后关闭AndroidTool.exe程序
		fir_exe_Id = GetProcessIDByName(_T("AndroidTool.exe"));
		//退出AndroidTool.exe
		TerminatePythonProcess(fir_exe_Id);


		//检查后关闭AvbKeyWriter.exe程序
		burn_exe_Id = GetProcessIDByName(_T("AvbKeyWriter.exe"));
		//退出AvbKeyWriter.exe
		TerminatePythonProcess(burn_exe_Id);

		//如果三个程序都被关闭，退出循环
		if (burn_exe_Id == 0 || load_code_Id == 0|| fir_exe_Id == 0)
		{
			break;
		}
	}
	
	
		

	
	

	/*PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")"); // add current directory to Python path  
	PyRun_SimpleString("import load_code"); // replace with your python script name  
	PyRun_SimpleString("load_code.killEXE()"); // replace with your function name*/
	
	//初始化python解释器
	//Py_Initialize();
	/*PyObject *pModule = NULL;
	PyObject *pFunc = NULL;

	pModule = PyImport_ImportModule("load_code");//调用的python文件名称
	if (pModule == NULL)
		AfxMessageBox(_T("pMoudle"));
	pFunc = PyObject_GetAttrString(pModule, "killEXE");//调用函数名称
	if (pFunc == NULL)
		AfxMessageBox(_T("pFunc"));
	PyObject *pArgs = NULL;//python中load_code文件中main函数的参数
	PyObject* pRet = PyObject_CallObject(pFunc, NULL);//调用函数
	if (pRet == NULL)
		AfxMessageBox(_T("pRet"));

	Py_Finalize();//关闭python解释器*/
}

