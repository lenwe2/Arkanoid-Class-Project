#include "ground.h"
#include <vector>


#define MAX_LOADSTRING 1000
// Globale Variablen:
HINSTANCE hInst;								// Aktuelle Instanz
TCHAR szTitle[MAX_LOADSTRING];					// Titelleistentext
TCHAR szWindowClass[MAX_LOADSTRING];			// Klassenname des Hauptfensters
HFONT editfont;
HWND hMain = NULL;
//-----------------------
static char MainWin[] = "MainWin";

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
void draw_line(HDC DC, int x, int y, int a, int b, int red, int green, int blue, int width);

HBRUSH  hWinCol = CreateSolidBrush(RGB(180, 180, 180));

HWND g_hwnd=NULL;

int dx = 0, dy = 0;

bool playing = true;

struct Ball
{
	int x, y, r;
	int dx, dy;

	void update()
	{
		x += dx;
		y += dy;
	}

	void draw(HDC dc)
	{
		draw_line(dc, x, y, x, y, 0, 0, 255, r);
	}
} ball;

struct Brick
{
	int x, y;
	int width, height;
	bool exists;
	int r, g, b;

	void draw(HDC dc)
	{
		draw_line(dc, x, y, x + width, y, r, g, b, height);
	}

	bool collidesWith(Ball& b)
	{
		if (b.x >= x - width && b.x <= x + width &&
			b.y >= y - (height / 2.0f) && b.y <= y + (height / 2.0f))
		{
			return true;
		}
		return false;
	}
};

std::vector<Brick*> bricks; //<-- like Java's ArrayList class

//timer:
#define TIMER1 111

// Vorwärtsdeklarationen der in diesem Codemodul enthaltenen Funktionen:
/*ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

*/

int APIENTRY WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{

	
	hInst = hInstance;												//						save in global variable for further use
	// TODO: Hier Code einfügen.
	MSG msg;
	
	// Globale Zeichenfolgen initialisieren
	LoadString(hInstance, 103, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, 104, szWindowClass, MAX_LOADSTRING);
	//register Window													<<<<<<<<<<			STEP ONE: REGISTER WINDOW						!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	WNDCLASSEX wcex;												//						=> Filling out struct WNDCLASSEX
	BOOL Result = TRUE;
	wcex.cbSize = sizeof(WNDCLASSEX);								//						size of this struct (don't know why
	wcex.style = CS_HREDRAW | CS_VREDRAW;							//						?
	wcex.lpfnWndProc = (WNDPROC)WndProc;							//						The corresponding Proc File -> Message loop switch-case file
	wcex.cbClsExtra = 0;											//
	wcex.cbWndExtra = 0;											//
	wcex.hInstance = hInstance;										//						The number of the program
	wcex.hIcon = LoadIcon(hInstance, NULL);							//
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);						//
	wcex.hbrBackground = hWinCol;									//						Background color
	wcex.lpszMenuName = NULL;										//
	wcex.lpszClassName = MainWin;									//						Name of the window (must the the same as later when opening the window)
	wcex.hIconSm = LoadIcon(wcex.hInstance, NULL);					//
	Result = (RegisterClassEx(&wcex) != 0);							//						Register this struct in the OS

				//													STEP TWO: OPENING THE WINDOW with x,y position and xlen, ylen !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	hMain = CreateWindow(MainWin, "CST 325 Homework 1 Arkanoid", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 500, 500, NULL, NULL, hInst, NULL);
	if (!hMain)return 0;
	ShowWindow(hMain, nCmdShow);
	UpdateWindow(hMain);

	//													STEP THRE: Going into the infinity message loop							  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

	// Hauptmeldungsschleife:
	bool quit = FALSE;
	while (!quit)
	{
		//if (GetMessage(&msg, NULL, 0, 0) > 0)
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) == TRUE)
		{
			if (msg.message == WM_QUIT){ quit = TRUE; break; }

			
				TranslateMessage(&msg);								//						IF a meessage occurs, the WinProc will be called!!!!
				DispatchMessage(&msg);
		}
	}
	return (int)msg.wParam;
}
///////////////////////////////////////////////////
void redr_win_full(HWND hwnd, bool erase)
	{
	RECT rt;
	GetClientRect(hwnd, &rt);
	InvalidateRect(hwnd, &rt, erase);
	}

///////////////////////////////////
//		This Function is called every time the Left Mouse Button is down
///////////////////////////////////
void OnLBD(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
{

}
///////////////////////////////////
//		This Function is called every time the Right Mouse Button is down
///////////////////////////////////
void OnRBD(HWND hwnd, BOOL fDoubleClick, int x, int y, UINT keyFlags)
	{
	
	}
///////////////////////////////////
//		This Function is called every time a character key is pressed
///////////////////////////////////
void OnChar(HWND hwnd, UINT ch, int cRepeat)
{

}
///////////////////////////////////
//		This Function is called every time the Left Mouse Button is up
///////////////////////////////////
void OnLBU(HWND hwnd, int x, int y, UINT keyFlags)
{

	
}
///////////////////////////////////
//		This Function is called every time the Right Mouse Button is up
///////////////////////////////////
void OnRBU(HWND hwnd, int x, int y, UINT keyFlags)
	{
	

	}
///////////////////////////////////
//		This Function is called every time the Mouse Moves
///////////////////////////////////
void OnMM(HWND hwnd, int x, int y, UINT keyFlags)
{

dx = x;
dy = y;

if ((keyFlags & MK_LBUTTON) == MK_LBUTTON)
	{
	}

if ((keyFlags & MK_RBUTTON) == MK_RBUTTON)
	{
	}
}
///////////////////////////////////
//		This Function is called once at the begin of a program
///////////////////////////////////
#define TIMER1 1

BOOL OnCreate(HWND hwnd, CREATESTRUCT FAR* lpCreateStruct)
{
	g_hwnd = hwnd;
	editfont = CreateFont(-10, 0, 0, 0, 0, FALSE, 0, 0, 0, 0, 0, 0, 0, "Arial");	
	if (!SetTimer(hwnd, TIMER1, 20, NULL))
	{
		MessageBox(hwnd, "No Timers Available", "Info", MB_OK);
		return FALSE;
	}

	srand(time(0));
	
	for (int j = 0; j < 5; ++j)
	{
		for (int i = 0; i < 15; ++i)
		{
			Brick* b = new Brick();

			b->width = 23;
			b->height = 5;

			b->x = i * (b->width + 8) + 15;
			b->y = j * (b->height + 2) + 5;
			
			if (j == 0) {
				b->r = 179;
				b->g = 0;
				b->b = 0;
			}
			if (j == 1) {
				b->r = 255;
				b->g = 106;
				b->b = 77;
			}
			if (j == 2) {
				b->r = 213;
				b->g = 128;
				b->b = 255;
			}
			if (j == 3) {
				b->r = 0;
				b->g = 255;
				b->b = 255;
			}
			if (j == 4) {
				b->r = 255;
				b->g = 255;
				b->b = 0;
			}
			

			b->exists = true;

			bricks.push_back(b);
		}
	}
	ball.x = 250;
	ball.y = 250;
	ball.dx = rand() % 9 + 1;
	ball.dy = rand() % 9 + 1;
	ball.r = 5;
	return TRUE;
}

void OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	HWND hwin;
	
	switch (id)
	{
	default:
		break;
	}

}
//************************************************************************
void OnTimer(HWND hwnd, UINT id)
{
	redr_win_full(hwnd, FALSE);

	ball.update();
	
	if (ball.x > 480) {
		ball.dx *= -1;
	}
	if (ball.x < 5) {
		ball.dx *= -1;
	}
	if (ball.y > 460) {
		ball.x = 250;
		ball.y = 250;
		for (int j = 0; j < 5; ++j)
		{
			for (int i = 0; i < 15; ++i)
			{
				Brick* b = new Brick();

				b->width = 23;
				b->height = 5;

				b->x = i * (b->width + 8) + 15;
				b->y = j * (b->height + 2) + 5;

				if (j == 0) {
					b->r = 179;
					b->g = 0;
					b->b = 0;
				}
				if (j == 1) {
					b->r = 255;
					b->g = 106;
					b->b = 77;
				}
				if (j == 2) {
					b->r = 213;
					b->g = 128;
					b->b = 255;
				}
				if (j == 3) {
					b->r = 0;
					b->g = 255;
					b->b = 255;
				}
				if (j == 4) {
					b->r = 255;
					b->g = 255;
					b->b = 0;
				}


				b->exists = true;

				bricks.push_back(b);
			}
		}
	}
	if (ball.y < 10) {
		ball.dy *= -1;
	}

	if (ball.y >= 445 && ball.y <= 455 && ball.x <= dx + 10 && ball.x >= dx - 10) {
		ball.dy *= -1;
	}

	for (int i = 0; i < bricks.size(); ++i)
	{
		if (bricks[i]->exists && bricks[i]->collidesWith(ball)) //<-- short circuit trick
		{
			bricks[i]->exists = false;
			ball.dy *= -1;
		}
	}
}
//************************************************************************
///////////////////////////////////
//		This Function is called every time the window has to be painted again
///////////////////////////////////


void OnPaint(HWND hwnd)
	{

	//if(rglobe.EckSizing.active_sizing())return;
	PAINTSTRUCT PaintStruct;
	HDC DC_ = BeginPaint(hwnd, &PaintStruct);
	HBITMAP hbmMem, hbmOld;
	HDC DC = CreateCompatibleDC(DC_);
	RECT rc;
	GetClientRect(hwnd, &rc);
	hbmMem = CreateCompatibleBitmap(DC_, rc.right - rc.left, 2000);
	hbmOld = (HBITMAP)SelectObject(DC, hbmMem);
	COLORREF bg = RGB(0, 0, 0);
	//if(rglobe.EckSizing.active_sizing())
	//bg=RGB(255,255,0);
	HBRUSH hbrBkGnd = CreateSolidBrush(bg);
	FillRect(DC, &rc, hbrBkGnd);
	DeleteObject(hbrBkGnd);

	//draw here
	// paddle
	draw_line(DC, dx - 10, 450, dx + 10, 450, 179, 255, 242, 5);
	for (int i = 0; i < bricks.size(); ++i)
	{
		if(bricks[i]->exists) bricks[i]->draw(DC);
	}
	ball.draw(DC);
	//end draw here

	//					DOUBLE BUFFERING!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	BitBlt(DC_, rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top, DC, 0, 0, SRCCOPY);
	SelectObject(DC, hbmOld);
	DeleteObject(hbmMem);
	DeleteDC(DC);
	EndPaint(hwnd, &PaintStruct);
	}
//****************************************************************************

void draw_line(HDC DC,int x, int y, int a, int b, int red, int green, int blue, int width)
	{
	HPEN Stift = CreatePen(PS_SOLID, width, RGB(red, green, blue));
	SelectObject(DC, Stift);
	MoveToEx(DC, x, y, NULL);
	LineTo(DC, a, b);
	DeleteObject(Stift);
	}
//*************************************************************************
void OnKeyDown(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
	{

	switch (vk)
		{
			default:break;
			
		}
	}

//*************************************************************************
void OnKeyUp(HWND hwnd, UINT vk, BOOL fDown, int cRepeat, UINT flags)
	{
	switch (vk)
		{
			default:break;
			
		}

	}


//**************************************************************************
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	SCROLLINFO si;


	
	switch (message)
	{
	/*
	#define HANDLE_MSG(hwnd, message, fn)    \
    case (message): return HANDLE_##message((hwnd), (wParam), (lParam), (fn))
	*/
		
		HANDLE_MSG(hwnd, WM_CHAR, OnChar);
		HANDLE_MSG(hwnd, WM_LBUTTONDOWN, OnLBD);
		HANDLE_MSG(hwnd, WM_LBUTTONUP, OnLBU);
		HANDLE_MSG(hwnd, WM_MOUSEMOVE, OnMM);	
		HANDLE_MSG(hwnd, WM_CREATE, OnCreate);
		HANDLE_MSG(hwnd, WM_PAINT, OnPaint);
		HANDLE_MSG(hwnd, WM_COMMAND, OnCommand);
		HANDLE_MSG(hwnd, WM_TIMER, OnTimer);
		HANDLE_MSG(hwnd, WM_KEYDOWN, OnKeyDown);
		HANDLE_MSG(hwnd, WM_KEYUP, OnKeyUp);

	case WM_ERASEBKGND:
		return (LRESULT)1;
	case WM_DESTROY:
		
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0;
}
