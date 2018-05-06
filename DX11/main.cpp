
#include <Windows.h> 



LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;  HDC hDC;

	switch (message) {
	case WM_PAINT:   hDC = BeginPaint(hwnd, &ps);   EndPaint(hwnd, &ps);   break;

	case WM_DESTROY:   PostQuitMessage(0);   break;

	default:   return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmd, int nShow) {
	WNDCLASSEX cls; 
	cls.cbSize = sizeof(cls);  
	cls.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH); 
	cls.hCursor = LoadCursor(NULL, IDC_ARROW);  
	cls.style = CS_HREDRAW | CS_VREDRAW;  
	cls.lpfnWndProc = WndProc; 
	cls.hInstance = hInstance; 
	cls.lpszMenuName = NULL;  
	cls.lpszClassName = "DX11BookWindowClass";
	cls.hIconSm=NULL;
	cls.hIcon=NULL;
	cls.cbWndExtra=32;
	cls.cbClsExtra=0;
	if (!RegisterClassEx(&cls)) return -1;

	RECT rc = { 0, 0, 640, 480 };  AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindow(cls.lpszClassName, "Blank Win32 Window", WS_OVERLAPPEDWINDOW, 0, 0, rc.right - rc.left, rc.bottom - rc.top, 0, 0, hInstance, 0);

	if (!hwnd) return -1;

	ShowWindow(hwnd, nShow);

	//Demo Initialize 

	MSG msg = { 0 };  
	while (msg.message != WM_QUIT) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) { 
			TranslateMessage(&msg);   
			DispatchMessage(&msg); 
		}   //Update   //Draw  
	} 

	 //Demo Shutdown  
	return static_cast<int>(msg.wParam);
} 
