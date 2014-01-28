#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#include <gl/gl.h>
#define WWIDTH 640
#define WHEIGHT 480
#define start 0.005
#define end 0.0000005
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
	double zoom = start;
	while (Running != 0)
	{
		if (zoom < end)Running = -one;
		if (zoom > start)Running = one;
		PeekMessage(&msg, hwnd, 0, 0, PM_REMOVE);
		if (msg.message == WM_QUIT)	break;
		zoom *= (one - (Running*start));
		glBegin(GL_POINTS);
		int maxIt = 128;
		for (double iy = 0; iy < WHEIGHT; iy+=3) for (double ix = 0; ix < WWIDTH; ix+=3)
			{
				double targetX = 0.27499 + ix * zoom;
				double targetY = 0.48419 + iy * zoom;
				double x = 0;
				double y = 0;
				int it;
				for (it = 0; it<maxIt; it++)
				{
					double x2 = x*x;
					double y2 = y*y;
					if (x2 + y2 > 4.0) break;
					double twoxy = 2.0*x*y;
					x = x2 - y2 + targetX;
					y = twoxy + targetY;
				}
				glColor3b(it * 2,( it * 2) - 64, (it * 2) - 128);
				glVertex2d(ix, iy);
			}
		glEnd();
		SwapBuffers(g_HDC);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ExitProcess(0);
}