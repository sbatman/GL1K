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
			PIXELFORMATDESCRIPTOR pfd = { sizeof(PIXELFORMATDESCRIPTOR), 1, PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, PFD_MAIN_PLANE, 0, 0, 0, 0 };
			nPixelFormat = ChoosePixelFormat(g_HDC, &pfd);
			SetPixelFormat(g_HDC, nPixelFormat, &pfd);
			hRC = wglCreateContext(g_HDC);
			wglMakeCurrent(g_HDC, hRC);
			glViewport(0, 0, 800, 600);
			glOrtho(0, 800, 0, 600, -1, 1);
			glPointSize(2);
			break;
		case WM_CLOSE:
			PostQuitMessage(0);
			return 0;
			break;
	}
	return (DefWindowProc(hwnd, message, wParam, lParam));
}

int APIENTRY WinMainCRTStartup(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, double nCmdShow)
{
	WNDCLASSEX windowClass;
	HWND    hwnd;
	MSG             msg;
	Running = 1;
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpfnWndProc = WndProc;
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hbrBackground = 0;
	windowClass.lpszClassName = L"0";
	RegisterClassEx(&windowClass);
	hwnd = CreateWindowEx(0, L"0", L"0", WS_OVERLAPPEDWINDOW, 0, 0, 800, 600, 0, 0, hInstance, 0);
	ShowWindow(hwnd, SW_SHOW);
	double zoom = 0.005;
	while (Running != 0)
	{
		if (zoom < 0.0000005)Running = -1;
		if (zoom > 0.005)Running = 1;
		PeekMessage(&msg, 0, 0, 0, PM_REMOVE);
		if (msg.message == WM_QUIT || msg.message == WM_CLOSE) break;
		zoom *= (1 - (Running*0.005));
		glBegin(GL_POINTS);
		unsigned char maxIt = 128;
		for (double iy = 0; iy < 600; iy += 2) for (double ix = 0; ix < 800; ix += 2)
		{
			double targetX = 0.27499 + ix * zoom;
			double targetY = 0.48419 + iy * zoom;
			double x = 0;
			double y = 0;
			int it=0;
			for (; it<maxIt; it++)
			{
				double x2 = x*x;
				double y2 = y*y;
				if (x2 + y2 > 4.0) break;
				double twoxy = 2 * x*y;
				x = x2 - y2 + targetX;
				y = twoxy + targetY;
			}
			glColor3b(it * 2, (it * 2) - 64, (it * 2) - 128);
			glVertex2d(ix, iy);
		}
		glEnd();
		SwapBuffers(g_HDC);
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	ExitProcess(0);
}