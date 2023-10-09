#pragma once
#include <Windows.h>  
#include <iostream>
#include <python.h>
#include <tlhelp32.h>

//ִ��python�ű�
extern void python_start();
//��ֹpython����
extern void python_kill();

//���ݳ�������ȡ����Ľ���id
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

//���ݳ����id��ֹpython��exe����
void TerminatePythonProcess(DWORD processId)
{
	HANDLE processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
	if (processHandle != NULL)
	{
		// ��ֹ Python exe ����  
		TerminateProcess(processHandle, 0); 

		// �ر� Python exe �����д�����Ͷ�ȡ���  
		CloseHandle(processHandle);
	}
}

//ִ��python�ű�
void python_start()
{
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	TCHAR szCommandLine[] = _T("load_code.exe");
	// ����Python exe�ļ�
	if (!CreateProcess(NULL, szCommandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		AfxMessageBox(_T("Failed to start process"));
		return ;
	}

	//�����Ľ���id
	DWORD processId;
	// ���� Python exe ����Ľ��� ID���˳������ʱ��ʹ��  
	processId = pi.dwProcessId;

	// �ȴ�Python exe�ļ�ִ�����  
	WaitForSingleObject(pi.hProcess, INFINITE);

	// �رս��̾�����߳̾��  
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);
	
	//��ʼ��python������
	/*Py_Initialize();

	PyObject *pModule = NULL;
	PyObject *pFunc = NULL;

	pModule = PyImport_ImportModule("load_code");//���õ�python�ļ�����
	if (pModule == NULL)
		AfxMessageBox(_T("pMoudle"));
	pFunc = PyObject_GetAttrString(pModule, "py_start");//���ú�������
	if (pFunc == NULL)
		AfxMessageBox(_T("pFunc"));
	PyObject *pArgs = NULL;//python��load_code�ļ���main�����Ĳ���
	//PyObject *pArgs = PyTuple_New(0); // ����һ���յĲ����б���Ϊ���main����û�в���  
	//if (pArgs == NULL) {
		//printf("Cannot create argument list\n");
	//}
	PyObject* pRet = PyObject_CallObject(pFunc, pArgs);//���ú���
	if (pRet == NULL)
		AfxMessageBox(_T("pRet"));*/
	/*
	PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")"); // add current directory to Python path  
	PyRun_SimpleString("import load_code"); // replace with your python script name  
	PyRun_SimpleString("load_code.main()"); // replace with your function name  
	*/
	//Py_Finalize();//�ر�python������
}

//��ֹpython����
void python_kill()
{
	//�����Ľ���id
	DWORD load_code_Id;//load_code����id
	DWORD fir_exe_Id;//�̼�exe�ĳ���id
	DWORD burn_exe_Id;//��¼exe�ĳ���id
	
	while (1)
	{
		//����ر�load_code����
		load_code_Id = GetProcessIDByName(_T("load_code.exe"));
		//�˳�python��exe
		TerminatePythonProcess(load_code_Id);


		//����ر�AndroidTool.exe����
		fir_exe_Id = GetProcessIDByName(_T("AndroidTool.exe"));
		//�˳�AndroidTool.exe
		TerminatePythonProcess(fir_exe_Id);


		//����ر�AvbKeyWriter.exe����
		burn_exe_Id = GetProcessIDByName(_T("AvbKeyWriter.exe"));
		//�˳�AvbKeyWriter.exe
		TerminatePythonProcess(burn_exe_Id);

		//����������򶼱��رգ��˳�ѭ��
		if (burn_exe_Id == 0 || load_code_Id == 0|| fir_exe_Id == 0)
		{
			break;
		}
	}
	
	
		

	
	

	/*PyRun_SimpleString("import sys");
	PyRun_SimpleString("sys.path.append(\".\")"); // add current directory to Python path  
	PyRun_SimpleString("import load_code"); // replace with your python script name  
	PyRun_SimpleString("load_code.killEXE()"); // replace with your function name*/
	
	//��ʼ��python������
	//Py_Initialize();
	/*PyObject *pModule = NULL;
	PyObject *pFunc = NULL;

	pModule = PyImport_ImportModule("load_code");//���õ�python�ļ�����
	if (pModule == NULL)
		AfxMessageBox(_T("pMoudle"));
	pFunc = PyObject_GetAttrString(pModule, "killEXE");//���ú�������
	if (pFunc == NULL)
		AfxMessageBox(_T("pFunc"));
	PyObject *pArgs = NULL;//python��load_code�ļ���main�����Ĳ���
	PyObject* pRet = PyObject_CallObject(pFunc, NULL);//���ú���
	if (pRet == NULL)
		AfxMessageBox(_T("pRet"));

	Py_Finalize();//�ر�python������*/
}

