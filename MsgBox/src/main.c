#include <windows.h>

char *AppTitle = "Win1";
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

static BOOL _registerMsgBox(HINSTANCE hInst, char* AppTitle);
static BOOL _populateWindows(HINSTANCE hInst, char* AppTitle, int nCmdShow);

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	MSG msg;

	if (_registerMsgBox(hInst, AppTitle) == FALSE)
		return 0;

	if (_populateWindows(hInst, AppTitle, nCmdShow) == FALSE)
		return 0;

	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 1;
}

static BOOL _registerMsgBox(HINSTANCE hInst, char* AppTitle)
{
    WNDCLASS wc = { 0 };

    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = WindowProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInst;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH) COLOR_WINDOWFRAME;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = AppTitle;

    return RegisterClass(&wc);
}

static BOOL _populateWindows(HINSTANCE hInst, char* AppTitle, int nCmdShow)
{
    HWND hwnd = CreateWindow(AppTitle, AppTitle
                            , WS_OVERLAPPEDWINDOW
                            , CW_USEDEFAULT, CW_USEDEFAULT
                            , 100, 100, NULL, NULL, hInst, NULL);

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    return (hwnd != NULL);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC dc;
		RECT r;
		GetClientRect(hwnd, &r);
		dc = BeginPaint(hwnd, &ps);
		DrawText(dc, "Hello World", -1, &r,
		DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint(hwnd, &ps);
		break;
	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}
	return 0;
}
