#include <windows.h>
#include <windowsx.h>
//#include "BEFE.h"

const char g_szClassName[] = "myWindowClass";

#define IDC_MAIN_EDIT	101

// Step 4: the Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

  switch(msg) {
    case WM_CLOSE:
      DestroyWindow(hwnd);
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      break;
    case WM_CREATE: {
      HFONT hfDefault;
      HWND hEdit;

      hEdit = CreateWindowEx(WS_EX_CLIENTEDGE, "EDIT", "", 
                             WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL, 
                             0, 0, 100, 100, hwnd, (HMENU)IDC_MAIN_EDIT, GetModuleHandle(NULL), NULL);
      if (hEdit == NULL)
        MessageBox(hwnd, "Could not create edit box.", "Error", MB_OK | MB_ICONERROR);

      //hfDefault = (HFONT)GetStockObject(DEFAULT_GUI_FONT);
      hfDefault = (HFONT)GetStockObject(OEM_FIXED_FONT);
      SendMessage(hEdit, WM_SETFONT, (WPARAM)hfDefault, MAKELPARAM(FALSE, 0));
      }
      break;
    case WM_SIZE: {
      HWND hEdit;
      RECT rcClient;

      GetClientRect(hwnd, &rcClient);

      hEdit = GetDlgItem(hwnd, IDC_MAIN_EDIT);
      SetWindowPos(hEdit, NULL, 0, 0, rcClient.right, rcClient.bottom, SWP_NOZORDER);
      }
      break;

    default:
      return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    
    return 0;
    
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
  WNDCLASSEX wc;
  HWND       hwnd;
  MSG        msg;

  //Step 1: Registering the Window Class
  wc.cbSize        = sizeof(WNDCLASSEX);
  wc.style         = 0;
  wc.lpfnWndProc   = WndProc;
  wc.cbClsExtra    = 0;
  wc.cbWndExtra    = 0;
  wc.hInstance     = hInstance;
  wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
  wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
  wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
  wc.lpszMenuName  = NULL;
  wc.lpszClassName = g_szClassName;
  wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

  if (!RegisterClassEx(&wc)) {
    MessageBox(NULL, "Window Registration Failed!", "Error!",
               MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  // Step 2: Creating the Window
  hwnd = CreateWindowEx(WS_EX_CLIENTEDGE,
                        g_szClassName,
                        "BEFEGui",
                        WS_OVERLAPPEDWINDOW,
                        CW_USEDEFAULT, CW_USEDEFAULT, 240, 120,
                        NULL, NULL, hInstance, NULL);

  if (hwnd == NULL) {
    MessageBox(NULL, "Window Creation Failed!", "Error!",
               MB_ICONEXCLAMATION | MB_OK);
    return 0;
  }

  ShowWindow(hwnd, nCmdShow);
  UpdateWindow(hwnd);

  // Step 3: The Message Loop
  while (GetMessage(&msg, NULL, 0, 0) > 0) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
  
  return msg.wParam;
  
}

/*
void myFontFunc() {

  // The code fragment shows the usage of CFont::operator HFONT.

  // Initialize a CFont object with the characteristics given 
  // in a LOGFONT structure.
  LOGFONT lf;

  // clear out structure
  memset(&lf, 0, sizeof(LOGFONT)); 

  // request a 12-pixel-height font
  lf.lfHeight = 12;                

  // request a face name "Arial"
  _tcsncpy_s(lf.lfFaceName, LF_FACESIZE, _T("Arial"), 7);

  CFont font1;
  font1.CreateFontIndirect(&lf);  // create the font

  // CFont::operator HFONT automatically converts font1 from 
  // CFont* to HFONT.
  CFont* font2 = CFont::FromHandle(font1);

  // Do something with the font just created...
  CClientDC dc(this);
  CFont* def_font = dc.SelectObject(font2);
  dc.TextOut(5, 5, _T("Hello"), 5);
  dc.SelectObject(def_font);

  // Done with the font. Delete the font object.
  font1.DeleteObject();  
}
*/