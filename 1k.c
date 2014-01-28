#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <gl/gl.h>
static const int  WWIDTH = 800;
static const int  WHEIGHT = 600;
static const LPCWSTR Title = "h";
static char Running;
static HDC g_HDC;
static HGLRC hRC;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CREATE:
			g_HDC = GetDC(hwnd);
			int nPixelFormat;
			PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, PFD_TYPE_RGBA, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0 };
			nPixelFormat = ChoosePixelFormat(g_HDC, &pfd);
			SetPixelFormat(g_HDC, nPixelFormat, &pfd);
			hRC = wglCreateContext(g_HDC);
			wglMakeCurrent(g_HDC, hRC);
			glViewport(0, 0, WWIDTH, WHEIGHT);
			glMatrixMode(GL_PROJECTION);
			glOrtho(0, WWIDTH, 0, WHEIGHT, -1.0, 1.0);
			glPointSize(3);
			return 0;
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			break;
	}
	return (DefWindowProc(hwnd, message, wParam, lParam));
}

int APIENTRY WinMainCRTStartup(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX windowClass;
	HWND    hwnd;
	MSG             msg;
	Running = 1;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = WndProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hbrBackground = NULL;
	windowClass.lpszClassName = Title;
	RegisterClassEx(&windowClass);
	hwnd = CreateWindowEx(NULL, Title, Title, WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, WWIDTH, WHEIGHT, NULL, NULL, hInstance, NULL);
	ShowWindow(hwnd, SW_SHOW);
	double zoom = 0.0002f;
	while (Running != 0)
	{
		PeekMessage(&msg, hwnd, NULL, NULL, PM_REMOVE);
		if (msg.message == WM_QUIT)	Running = 0;
		else
		{
			glBegin(GL_POINTS);
			int maxIt = 255;
			for (int iy = 0; iy < WHEIGHT; iy+=5) for (int ix = 0; ix < WWIDTH; ix+=5)
				{
					double cx = 0.274f + ix * zoom;
					double cy = 0.482f + iy * zoom;
					double x = 0;
					double y = 0;
					int it;
					for (it = 0; it<maxIt; it++)
					{
						double x2 = x*x;
						double y2 = y*y;
						if (x2 + y2 > 4.0f) break;
						double twoxy = 2.0*x*y;
						x = x2 - y2 + cx;
						y = twoxy + cy;
					}
					if (it > 3)
					{
						glColor3b(it, 0, 0);
						glVertex2d(ix, iy);
					}
				}
			glEnd();
			SwapBuffers(g_HDC);
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	ExitProcess(0);
}