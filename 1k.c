// //////////////////////
//
// GL1k - Is a small piece of C code created by Steven batchelor-Manning demonstating the
//      posibility to achieve Opengl Rendering within the confinds of a 1kb exe.
// https://github.com/sbatman/Cranium.git
//
// This work is covered under the Creative Commons Attribution-ShareAlike 3.0 Unported (CC BY-SA 3.0) licence.
// More information can be found about the liecence here http://creativecommons.org/licenses/by-sa/3.0/
// If you wish to discuss the licencing terms please contact Steven Batchelor-Manning
//
// //////////////////////

#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <gl/gl.h>
#define WWIDTH 800
#define WHEIGHT 600
#define one 1.0
static double Running;
static HDC g_HDC;
static HGLRC hRC;


LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			g_HDC = GetDC(hwnd);
			int nPixelFormat;
			PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1,  PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0 };
			nPixelFormat = ChoosePixelFormat(g_HDC, &pfd);
			SetPixelFormat(g_HDC, nPixelFormat, &pfd);
			hRC = wglCreateContext(g_HDC);
			wglMakeCurrent(g_HDC, hRC);
			glViewport(0, 0, WWIDTH, WHEIGHT);
			glOrtho(0, WWIDTH, 0, WHEIGHT, -one, one);
			glPointSize(2);
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
			break;
	}
	return (DefWindowProc(hwnd, message, wParam, lParam));
}

int APIENTRY WinMainCRTStartup(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX windowClass;
	HWND    hwnd;
	MSG             msg;
	Running = one;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = WndProc;
	windowClass.style =  CS_HREDRAW | CS_VREDRAW;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hbrBackground = 0;
	windowClass.lpszClassName = "h";
	RegisterClassEx(&windowClass);
	hwnd = CreateWindowEx(0, "h", "h", WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, WWIDTH, WHEIGHT, 0, 0, hInstance, 0);
	ShowWindow(hwnd, SW_SHOW);
	while (Running != 0)
	{
		PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
		if (msg.message == WM_QUIT || msg.message == WM_CLOSE) break;
		//Drawing code here
		SwapBuffers(g_HDC);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ExitProcess(0);
}