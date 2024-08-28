// cypherTool.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "cypherTool.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;                                // 当前实例
WCHAR szTitle[MAX_LOADSTRING];                  // 标题栏文本
WCHAR szWindowClass[MAX_LOADSTRING];            // 主窗口类名
HWND hEdit1, hEdit2, hButton,hBtncaesar;
HWND hBtnMorseEn, hBtnMorseDe, hBtnFence;

// 此代码模块中包含的函数的前向声明:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此处放置代码。

    // 初始化全局字符串
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_CYPHERTOOL, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // 执行应用程序初始化:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CYPHERTOOL));

    MSG msg;

    // 主消息循环:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目标: 注册窗口类。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CYPHERTOOL));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_CYPHERTOOL);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目标: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // 将实例句柄存储在全局变量中

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 600, 800, nullptr, nullptr, hInstance, nullptr);
   hEdit1 = CreateWindowEx(0, L"Edit", nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_BORDER | ES_WANTRETURN | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
       20, 20, 560, 300, hWnd, nullptr, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
   hButton = CreateWindowW(L"Button",L"交换", WS_VISIBLE|WS_CHILD, 20,320,40,20,hWnd, nullptr, hInst, nullptr);
   hBtncaesar = CreateWindowW(L"Button", L"凯撒", WS_VISIBLE | WS_CHILD, 60, 320, 40, 20, hWnd, nullptr, hInst, nullptr);
   hBtnMorseEn = CreateWindowW(L"Button", L"摩斯加密", WS_VISIBLE | WS_CHILD, 100, 320, 80, 20, hWnd, nullptr, hInst, nullptr);
   hBtnMorseDe = CreateWindowW(L"Button", L"摩斯解密", WS_VISIBLE | WS_CHILD, 180, 320, 80, 20, hWnd, nullptr, hInst, nullptr);
   hBtnFence = CreateWindowW(L"Button", L"栅栏", WS_VISIBLE | WS_CHILD, 260, 320, 40, 20, hWnd, nullptr, hInst, nullptr);
   hEdit2 = CreateWindowEx(0,L"Edit", nullptr, WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_WANTRETURN | WS_BORDER | ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL,
      20, 340, 560, 300, hWnd, nullptr, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), nullptr);
   SendMessage(hEdit1, WM_SETTEXT, 0, (LPARAM)L"hello");
   // SendMessage(hEdit2, WM_SETTEXT, 0, (LPARAM)L"密文");
   if (!hWnd){return FALSE;}

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

string wchar_to_string(wchar_t* pwch) {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(pwch);
}
//const wchar_t * string_to_wchar(string s) {
//    wstring_convert<codecvt_utf8<wchar_t>> converter;
//    wstring ws = converter.from_bytes(s);
//    return ws.c_str();
//}


//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目标: 处理主窗口的消息。
//
//  WM_COMMAND  - 处理应用程序菜单
//  WM_PAINT    - 绘制主窗口
//  WM_DESTROY  - 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // 分析菜单选择:
            switch (wmId)
            {
            case BN_CLICKED:
            {
                if ((HWND)lParam == hButton) {
                    int len1 = GetWindowTextLength(hEdit1);
                    LPTSTR chbuf1; chbuf1 = new wchar_t[len1 + 1];
                    GetWindowText(hEdit1, chbuf1, len1 + 1);
                    int len2 = GetWindowTextLength(hEdit2);
                    LPTSTR chbuf2; chbuf2 = new wchar_t[len2 + 1];
                    GetWindowText(hEdit2, chbuf2, len2 + 1);
                    SendMessage(hEdit1, WM_SETTEXT, 0, (LPARAM)chbuf2);
                    SendMessage(hEdit2, WM_SETTEXT, 0, (LPARAM)chbuf1);
                    delete[] chbuf1;
                    delete[] chbuf2;
                }
                else {
                    int len = GetWindowTextLength(hEdit1);
                    LPTSTR chbuf; chbuf = new wchar_t[len + 1];
                    int cplen = GetWindowText(hEdit1, chbuf, len + 1);
                    string s = wchar_to_string(chbuf),ss;
                    if ((HWND)lParam == hBtncaesar) { ss = caesar(s); }
                    else if((HWND)lParam == hBtnMorseEn){ss = morseEncode(s); }
                    else if ((HWND)lParam == hBtnMorseDe){ ss = morseDecode(s); }
                    else if ((HWND)lParam == hBtnFence) { ss = fence(s); }
                    else { return DefWindowProc(hWnd, message, wParam, lParam); }
                    wstring_convert<codecvt_utf8<wchar_t>> converter;
                    wstring ws = converter.from_bytes(ss);
                    SendMessage(hEdit2, WM_SETTEXT, 0, (LPARAM)ws.c_str());
                    delete[] chbuf;
                }
                break;
            }
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    //case WM_PAINT:
    //    {
    //        PAINTSTRUCT ps;
    //        HDC hdc = BeginPaint(hWnd, &ps);
    //        // TODO: 在此处添加使用 hdc 的任何绘图代码...
    //        EndPaint(hWnd, &ps);
    //    }
    //    break;
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
