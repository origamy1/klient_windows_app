// Project2.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "Project2.h"
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include "Message.h"
#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <io.h>
// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define MAX_LOADSTRING 100

// definicie pre sokety
#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "27015"
#define NEXT_BLOCK_USER 50
#define TOP_DEFAULT 150
#define CHARACTERS_IN_MESSAGE_COLUMN 15
#define CHARACTERS_IN_MESSAGE_ROW 6
// definicia caret
#define TEXTMATRIX(x, y) *(pTextMatrix + (y * nWindowCharsX) + x) 

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
int SendMessageToServer(Message* message);
void SetStdOutToNewConsole();
void init(RECT& usersRect, RECT& writeMessageRect, RECT& sendRect, RECT& choseUserIDRect, RECT& receivedMessagesRect, RECT& clearMsgRect, TEXTMETRIC& tm, HWND hwnd, int& nWidth, int& nHeight);
void paint(HWND hwnd, PAINTSTRUCT* ps, RECT& usersRect, RECT& writeMesageRect, RECT& sendRect, RECT& choseUserIDRect, RECT& receivedMessagesRect, RECT& clearMsgRect, TCHAR* text, int Clicked_user_id
, int nWindowCharsY, int nWindowCharsX,int nHeight, char* pTextMatrix, int my_chosen_User_id);
void switchingKeyDown(HWND hWnd,WPARAM wParam, int& nCaretPosX, int& nCaretPosY, int& nWindowCharsX, int& nWindowCharsY
    , char* pTextMatrix, RECT& writeMessageRect, int nWidth, int nHeight);
void SwitchingChar(HWND hWnd, WPARAM wParam, int& nCaretPosX, int& nCaretPosY, int& nWindowCharsX, int& nWindowCharsY, char* pTextMatrix
    , RECT& writeMessageRect, int nWidth, int nHeight);
#define USERS_COUNT 7

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

  

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_PROJECT2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT2));

    MSG msg;

    // Main message loop:
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
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT2));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT2);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

    // for caret
    static LPCWSTR lpszChar;  // pointer to character 
    static char* pTextMatrix = NULL;  // points to text matrix 
    static RECT writeMessageRect;
  
    //static HINSTANCE hinst;                  // current instance 
    HBITMAP hCaret;                   // caret bitmap
    static int  nWidth,        // width of char. in logical units 
        nHeight,              // height of char. in logical units 
        nWindowX,             // width of client area 
        nWindowY,             // height of client area 
        nWindowCharsX,        // width of client area 
        nWindowCharsY,        // height of client area 
        nCaretPosX,           // x-position of caret 
        nCaretPosY;           // y-position of caret 
    static UINT uOldBlink;            // previous blink rate 
    int x, y;                         // coordinates for text matrix 
    TEXTMETRIC tm;                    // font information 

   // For window
    PAINTSTRUCT ps;
    HDC hdc;
    HBRUSH hBrush;
    HPEN hPen;
   
    //for mouse and user interface
    //int xPos, yPos;
    static RECT sendRect;
    static POINT mousePos;
    static RECT receivedMessagesRect;
    static RECT usersRect;
    static RECT clearMsgRect;
    static RECT choseUserIDRect;
    static int Clicked_user_id, my_chosen_User_id;
    static TCHAR text[25] = L"PouûÌvateæ 1";
    static bool startWriting;

    switch (message)
    {
    case WM_SETFOCUS:

        CreateCaret(hWnd, (HBITMAP)NULL, nWidth, nHeight); // CreateCaret(hWnd, hCaret, 0, 0);
        // Adjust the caret position. from left top corner of message rectangle
        // z æavÈho hornÈho rohu ötvoruholnika
        SetCaretPos(writeMessageRect.left + nCaretPosX * nWidth, writeMessageRect.top + nCaretPosY * nHeight);

        break;

    case WM_KILLFOCUS:

        // The window is losing the keyboard focus, so destroy the caret. 
        DestroyCaret();

        break;

    case WM_CREATE:
        mousePos.x = 0;
        mousePos.y = 0;
        Clicked_user_id = 0;
        my_chosen_User_id = 0;
        nCaretPosX = 0;//usersRect.right + (usersRect.right - usersRect.left) / 2 ;
        nCaretPosY = 0;//usersRect.top;
        startWriting = false;
        // Select a fixed-width system font, and get its text metrics.
        init(usersRect, writeMessageRect, sendRect, choseUserIDRect,receivedMessagesRect,clearMsgRect,tm, hWnd, nWidth, nHeight);

        return 0;
        break;

    case WM_SIZE:
        // Determine the width of the client area, in pixels 
        // and in number of characters. 
        nWindowX = writeMessageRect.right - writeMessageRect.left;//LOWORD(lParam);
        nWindowCharsX = max(1, nWindowX / nWidth);

        // Determine the height of the client area, in 
        // pixels and in number of characters. 
        nWindowY = writeMessageRect.bottom - writeMessageRect.top;// HIWORD(lParam);
        nWindowCharsY = max(1, nWindowY / nHeight);

        // Clear the buffer that holds the text input. 
        if (pTextMatrix != NULL)
            free(pTextMatrix);

        // If there is enough memory, allocate space for the 
        // text input buffer. 
        pTextMatrix = (char*)malloc(nWindowCharsX * nWindowCharsY + 1); // +1 pre nulu ukonËenie reùazca. 
        pTextMatrix[nWindowCharsX * nWindowCharsY] = 0; // ukonËenie reùazca
        if (pTextMatrix == NULL) {
            //ErrorHandler("Not enough memory.");
        }
        else {
            for (y = 0; y < nWindowCharsY; y++)
                for (x = 0; x < nWindowCharsX; x++)
                    TEXTMATRIX(x, y) = ' ';
        }
       // Move the caret to the origin. 
       //SetCaretPos(writeMessageRect.left + nCaretPosX * nWidth, writeMessageRect.top + nCaretPosY * nHeight);
        SetCaretPos(writeMessageRect.left, writeMessageRect.top);
                return 0; 
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
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
        }
        break;


    case WM_PAINT:
        { 
        
        paint(hWnd, &ps, usersRect, writeMessageRect, sendRect,choseUserIDRect, receivedMessagesRect, clearMsgRect, text, Clicked_user_id, nWindowCharsY, nWindowCharsX, nHeight, pTextMatrix,
             my_chosen_User_id);
        
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_MOUSEMOVE:
        mousePos.x = GET_X_LPARAM(lParam);
        mousePos.y = GET_Y_LPARAM(lParam);
        break;

    case WM_LBUTTONDOWN:
        if (wParam == MK_LBUTTON) {

            usersRect.top = TOP_DEFAULT;
            choseUserIDRect.top = TOP_DEFAULT;
            bool InRect = PtInRect(&usersRect, mousePos);
            // ak boli stlaËenÌ pouûÌvatelia
            if (InRect) {
               Clicked_user_id = (mousePos.y - usersRect.top) / NEXT_BLOCK_USER;

               InvalidateRect(hWnd, NULL, TRUE); // malo by to prekresliù celÈ okno. zavol· sa wm paint nad celym oknom
               startWriting = true;
               ShowCaret(hWnd);  // Display the caret.
            }
            InRect = PtInRect(&writeMessageRect, mousePos);
            if (InRect) {
                nCaretPosX = ( mousePos.x - writeMessageRect.left ) / nWidth;
                nCaretPosY = ( mousePos.y - writeMessageRect.top) / nHeight;
                SetCaretPos(writeMessageRect.left + nCaretPosX * nWidth, writeMessageRect.top + nCaretPosY * nHeight);
                ShowCaret(hWnd);  // Display the caret.
            }
            InRect = PtInRect(&sendRect, mousePos);
            if (InRect) {
                /*TODO(sad): 
                Teraz treba implementovaù, ée server si uloûÌ prijat˙ spr·vu do s˙boru. appendne ju na koniec texù·ku.
                takto bude appendovaù od kaûdÈho klienta. 

                Treba zariadiù, aby sa na server mohlo pripojiù viacero klientov
                a vöetky prijatÈ spr·vy od vöetkych klientov bude ukladaù do toho istÈho s˙boru. (multiThreads?)

                Vûdy keÔ sa aktualizuje s˙bor, tak aktualizuje vybran˝m dvom chatom ich spr·vy, teda prehæad· textov˝ s˙bor
                a odoöle nim urËenÈ spr·vy nazad dvom klientom, ktorych sa t˝kala posledn· spr·va.

                N·sledne, keÔ tieto spr·vy prÌjme klient, tak ich spracuje do poæa spr·v.
                Teraz treba vymyslieù metÛdu, ktor· toto pole spr·v vykreslÌ v rect pre chat

                »o moûno upraviù, to je spraviù objekt plocha, kde bud˙ objekty ako RECT pohromade.
                 
                */
                //pTextMatrix = (char*)malloc(nWindowCharsX * nWindowCharsY);
               
                Message* sendThisMessage = new Message(my_chosen_User_id, Clicked_user_id, pTextMatrix, strlen(pTextMatrix));
                SendMessageToServer(sendThisMessage);
            }

            InRect = PtInRect(&choseUserIDRect, mousePos);
            if (InRect) {
               
                my_chosen_User_id = (mousePos.y - choseUserIDRect.top)/(2*nHeight);
                InvalidateRect(hWnd, NULL, TRUE);
            }

            InRect = PtInRect(&clearMsgRect, mousePos);
            if (InRect) {
                //TODO moze byt nebezpecne keÔ sa tam poöl˙ nespr·vne lparam a wParam
                SendMessage(hWnd, WM_SIZE, NULL, NULL);
                InvalidateRect(hWnd, NULL, TRUE); // malo by to prekresliù celÈ okno. zavol· sa wm paint nad celym oknom
            }


        }
        break;

    case WM_KEYDOWN:

        switchingKeyDown(hWnd, wParam, nCaretPosX, nCaretPosY, nWindowCharsX, nWindowCharsY, pTextMatrix, writeMessageRect, nWidth, nHeight);

        // Adjust the caret position based on the 
        // virtual-key processing. 

        SetCaretPos(writeMessageRect.left + nCaretPosX * nWidth, writeMessageRect.top + nCaretPosY * nHeight);

        return 0;

    case WM_CHAR:

        // wParam = 'A'// 0x41  A
        // wParam obsahuje stlaËenÈ kl·vesy
        if (startWriting) {
            
            SwitchingChar(hWnd, wParam, nCaretPosX, nCaretPosY, nWindowCharsX, nWindowCharsY, pTextMatrix, writeMessageRect, nWidth, nHeight);
           
            // Adjust the caret position based on the 
            // character processing. 

            SetCaretPos(writeMessageRect.left + nCaretPosX * nWidth, writeMessageRect.top + nCaretPosY * nHeight);
        }
        else {
            break;
        }
        //  return ConnectToServer();

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


// Message handler for about box.
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


int SendMessageToServer(Message* message) {

    if (message->getLength() == 0) {
        return 0; // nieje Ëo poslaù
    }
    WSADATA wsaData;
    SOCKET ConnectSocket = INVALID_SOCKET;
    struct addrinfo* result = NULL,
        * ptr = NULL,
        hints;
    const char* sendbuf = "this is ";

    
    char recvbuf[DEFAULT_BUFLEN];
    int iResult;
    int recvbuflen = DEFAULT_BUFLEN;
  
    // Validate the parameters
 /*   if (argc != 2) {
        printf("usage: %s server-name\n", argv[0]);
        return 1;
    }*/

    // Initialize Winsock
    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
       
        printf("WSAStartup failed with error: %d\n", iResult);
        return 1;
    }

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    // Resolve the server address and port
    iResult = getaddrinfo("localhost", DEFAULT_PORT, &hints, &result);
    if (iResult != 0) {
        printf("getaddrinfo failed with error: %d\n", iResult);
        WSACleanup();
        return 1;
    }

    // Attempt to connect to an address until one succeeds
    for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

        // Create a SOCKET for connecting to server
        ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype,
            ptr->ai_protocol);
        if (ConnectSocket == INVALID_SOCKET) {
            printf("socket failed with error: %ld\n", WSAGetLastError());
            WSACleanup();
            return 1;
        }

        // Connect to server.
        iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
        if (iResult == SOCKET_ERROR) {
            closesocket(ConnectSocket);
            ConnectSocket = INVALID_SOCKET;
            continue;
        }
        break;
    }

    freeaddrinfo(result);

    if (ConnectSocket == INVALID_SOCKET) {
        printf("Unable to connect to server!\n");
        WSACleanup();
        return 1;
    }

    // Send an initial buffer
    //iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
    iResult = send(ConnectSocket, message->getMessage(),(int)strlen(message->getMessage())+1, 0);
    if (iResult == SOCKET_ERROR) {
        printf("send failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    printf("Bytes Sent: %ld\n", iResult);

    // shutdown the connection since no more data will be sent
    iResult = shutdown(ConnectSocket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        printf("shutdown failed with error: %d\n", WSAGetLastError());
        closesocket(ConnectSocket);
        WSACleanup();
        return 1;
    }

    // Receive until the peer closes the connection
    do {

        iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
        if (iResult > 0)
            printf("Bytes received: %d\n", iResult);
        else if (iResult == 0)
            printf("Connection closed\n");
        else
            printf("recv failed with error: %d\n", WSAGetLastError());

    } while (iResult > 0);

    // cleanup
    closesocket(ConnectSocket);
    WSACleanup();

    delete(message); // uvolnenie pam‰te, zavolanie deötruktora
    message = nullptr;
    return 0;
}

void init(RECT& usersRect, RECT& writeMessageRect, RECT& sendRect, RECT &choseUserIDRect, RECT& receivedMessagesRect, RECT& clearMsgRect, TEXTMETRIC& tm, HWND hwnd, int& nWidth, int& nHeight) {
   
    HDC hdc = GetDC(hwnd);
    SelectObject(hdc,
        GetStockObject(SYSTEM_FIXED_FONT));
    GetTextMetrics(hdc, &tm);
    ReleaseDC(hwnd, hdc);

    nWidth = tm.tmAveCharWidth;;
    nHeight = tm.tmHeight;

    choseUserIDRect.left = 100;
    choseUserIDRect.right = 150;
    choseUserIDRect.top = TOP_DEFAULT;
    choseUserIDRect.bottom = TOP_DEFAULT+ USERS_COUNT * nHeight * 2; // akokeby 2 riadky textu pre kazdeho usera
    
    usersRect.left = choseUserIDRect.left +100; // ätvorec pre zobrazenie pouûÌvateæov
    usersRect.right = choseUserIDRect.left + 300;
    usersRect.bottom = 500;
    usersRect.top = TOP_DEFAULT;

  

    // ötvorec v ktorom sa bude daù pÌsaù spr·va
    writeMessageRect.left = usersRect.right + 100;
    writeMessageRect.right = usersRect.right + 100 + nWidth * CHARACTERS_IN_MESSAGE_COLUMN;
    writeMessageRect.top = usersRect.top;
    writeMessageRect.bottom = usersRect.top + nHeight * CHARACTERS_IN_MESSAGE_ROW;

    sendRect.left = writeMessageRect.right + 50;
    sendRect.right = writeMessageRect.right + 50 + 120;
    sendRect.top = writeMessageRect.top;
    sendRect.bottom = writeMessageRect.top + 20;
    
    clearMsgRect.left = sendRect.left;
    clearMsgRect.right = sendRect.right;
    clearMsgRect.top = sendRect.bottom + 20;
    clearMsgRect.bottom = sendRect.bottom + 40;

    receivedMessagesRect.left = sendRect.right + 50;
    receivedMessagesRect.right = sendRect.right + 50 + nWidth * CHARACTERS_IN_MESSAGE_COLUMN;
    receivedMessagesRect.top = writeMessageRect.top - 100;
    receivedMessagesRect.bottom = usersRect.bottom + 100;
    
}

void paint(HWND hwnd, PAINTSTRUCT* ps, RECT& usersRect, RECT& writeMessageRect, RECT& sendRect, RECT& choseUserIDRect, RECT& receivedMessagesRect, RECT& clearMsgRect, TCHAR* text, int Clicked_user_id
,int nWindowCharsY, int nWindowCharsX,int nHeight, char* pTextMatrix, int my_chosen_User_id) {
    

        //Text("") funkcia pre draw Text

        HDC hdc = BeginPaint(hwnd, ps);

        HPEN hPen = CreatePen(PS_DOT, 1, RGB(0, 255, 0));
        SelectObject(hdc, hPen);
        HBRUSH hBrush = CreateSolidBrush(RGB(200, 200, 200));
        SelectObject(hdc, hBrush);
        usersRect.top = TOP_DEFAULT; // musÌ sa vûdy nastaviù pri prekreslenÌ na default. inak to bude zle vytv·raù rectangle. ten top bude eöte v‰ËöÌ   ako bottom tak sa nevykreslÌ ten rect vÙbec
        choseUserIDRect.top = TOP_DEFAULT;
        //SetDCPenColor(hdc, RGB(0, 0, 255));
        //SetDCBrushColor(hdc, RGB(150, 150, 150));
        Rectangle(hdc, usersRect.left, usersRect.top, usersRect.right, usersRect.bottom);
 
        Rectangle(hdc, writeMessageRect.left, writeMessageRect.top, writeMessageRect.right, writeMessageRect.bottom);
        Rectangle(hdc, choseUserIDRect.left, choseUserIDRect.top, choseUserIDRect.right, choseUserIDRect.bottom);
        Rectangle(hdc, receivedMessagesRect.left, receivedMessagesRect.top, receivedMessagesRect.right, receivedMessagesRect.bottom);
        HBRUSH brushTwo;
        brushTwo = CreateSolidBrush(RGB(255, 100, 100));
        SelectObject(hdc, brushTwo);
        Rectangle(hdc, sendRect.left, sendRect.top, sendRect.right, sendRect.bottom);
        Rectangle(hdc, clearMsgRect.left, clearMsgRect.top, clearMsgRect.right, clearMsgRect.bottom);
        DeleteObject(brushTwo);
        GetStockObject(WHITE_BRUSH);

        SetBkMode(hdc, TRANSPARENT);
        for (size_t i = 0; i < USERS_COUNT; i++)
        {
            //choseUserIDRect.bottom = TOP_DEFAULT + USERS_COUNT * nHeight * 2;
            
            char pom[2] ;
            pom[0] = i + '0';
            pom[1] = 0;
            DrawTextA(hdc,pom, -1, &choseUserIDRect, DT_NOCLIP|DT_CENTER);
            choseUserIDRect.top = TOP_DEFAULT + 2 * nHeight + 2 * nHeight * i;

            memcpy(text, L"PouûÌvateæ X\0", sizeof(TCHAR) * (_tcslen(L"PouûÌvateæ X") + 1));
            text[_tcslen(text) - 1] = i + '0';
            DrawText(hdc, text, -1, &usersRect, DT_NOCLIP | DT_CENTER);
            usersRect.top = TOP_DEFAULT + NEXT_BLOCK_USER + i * NEXT_BLOCK_USER;
        }
        SetBkMode(hdc, OPAQUE);
        {
            char pom[20] = "Tvoje ID:";
            char id = my_chosen_User_id + '0';
            pom[strlen(pom)] = id;
            SelectObject(hdc,
                GetStockObject(SYSTEM_FIXED_FONT));
            SetBkColor(hdc, RGB(200, 250, 250));
            TextOutA(hdc, choseUserIDRect.left, TOP_DEFAULT - 2 * NEXT_BLOCK_USER, pom, strlen(pom));
          
        }

        SetBkMode(hdc, TRANSPARENT);
        {
            char pom[20] = "Poöli spr·vu";
            DrawTextA(hdc, pom, -1, &sendRect, DT_CENTER);
            char pom2[20] = "Clearni spr·vu";
            DrawTextA(hdc, pom2, -1, &clearMsgRect, DT_CENTER);

        }
        SetBkMode(hdc, OPAQUE);
        {
            RECT ShowClicked;
            ShowClicked.left = usersRect.left;
            ShowClicked.right = usersRect.right;
            ShowClicked.top = TOP_DEFAULT - 2 * NEXT_BLOCK_USER; // vyööie o dva bloky
            ShowClicked.bottom = ShowClicked.top + NEXT_BLOCK_USER;
            memcpy(text, L"Ideö pÌsaù\nPouûÌvateæovi: 1", sizeof(TCHAR) * _tcslen(L"Ideö pÌsaù\nPouûÌvateæovi: 1"));
            text[_tcslen(text) - 1] = Clicked_user_id + '0';
            DrawText(hdc, text, -1, &ShowClicked, DT_NOCLIP);

        }

        /*    TextOutA(hdc, writeMessageRect.left + nCaretPosX * nWidth,
                writeMessageRect.top + nCaretPosY * nHeight,
                &TEXTMATRIX(nCaretPosX, nCaretPosY),
                nWindowCharsX - nCaretPosX);*/
        SelectObject(hdc,
            GetStockObject(SYSTEM_FIXED_FONT));
        SetBkMode(hdc, TRANSPARENT);
        for (int y = 0; y < nWindowCharsY; y++)
            TextOutA(hdc, writeMessageRect.left, writeMessageRect.top + y * nHeight, &TEXTMATRIX(0, y),
                nWindowCharsX);
        SetBkMode(hdc, OPAQUE);
        // Clean up
        DeleteObject(hBrush);
        DeleteObject(hPen);
        // set Stock
        GetStockObject(WHITE_BRUSH);
        GetStockObject(DC_PEN);

        EndPaint(hwnd, ps);
    
}


void switchingKeyDown(HWND hWnd, WPARAM wParam, int &nCaretPosX, int &nCaretPosY, int & nWindowCharsX,int & nWindowCharsY
, char* pTextMatrix,RECT & writeMessageRect, int nWidth, int nHeight) {
    switch (wParam)
    {
    case VK_HOME:       // Home 
        nCaretPosX = 0;
        break;

    case VK_END:        // End 
        nCaretPosX = nWindowCharsX - 1;
        break;

    case VK_PRIOR:      // Page Up 
        nCaretPosY = 0;
        break;

    case VK_NEXT:       // Page Down 
        nCaretPosY = nWindowCharsY - 1;
        break;

    case VK_LEFT:       // Left arrow 
        nCaretPosX = max(nCaretPosX - 1, 0);
        break;

    case VK_RIGHT:      // Right arrow 
        nCaretPosX = min(nCaretPosX + 1,
            nWindowCharsX - 1);
        break;

    case VK_UP:         // Up arrow 
        nCaretPosY = max(nCaretPosY - 1, 0);
        break;

    case VK_DOWN:       // Down arrow 
        nCaretPosY = min(nCaretPosY + 1,
            nWindowCharsY - 1);
        break;

    case VK_DELETE:     // Delete 

    // Move all the characters that followed the 
    // deleted character (on the same line) one 
    // space back (to the left) in the matrix. 

        for (int x = nCaretPosX; x < nWindowCharsX; x++)
            TEXTMATRIX(x, nCaretPosY) = TEXTMATRIX(x + 1, nCaretPosY);

        // Replace the last character on the 
        // line with a space. 

        TEXTMATRIX(nWindowCharsX - 1, nCaretPosY) = ' ';

        // The application will draw outside the 
        // WM_PAINT message processing, so hide the caret. 

        HideCaret(hWnd);

        // Redraw the line, adjusted for the 
        // deleted character. 

        HDC hdc = GetDC(hWnd);
        SelectObject(hdc,
            GetStockObject(SYSTEM_FIXED_FONT));
        SetBkColor(hdc, RGB(200, 200, 200));
     
        // toto moûno nepÙjde <<<<<<<<<<<<<<<<<<<<<<<<<<<< za textOut som dal textOutA
        TextOutA(hdc, writeMessageRect.left + nCaretPosX * nWidth,
            writeMessageRect.top + nCaretPosY * nHeight,
            &TEXTMATRIX(nCaretPosX, nCaretPosY),
            nWindowCharsX - nCaretPosX);
    
        ReleaseDC(hWnd, hdc);

        // Display the caret. 

        ShowCaret(hWnd);

        break;
    }
}

void SwitchingChar(HWND hWnd,WPARAM wParam, int &nCaretPosX, int &nCaretPosY,int &nWindowCharsX,int &nWindowCharsY, char* pTextMatrix
,RECT &writeMessageRect, int nWidth,int nHeight) {
    switch (wParam)
    {
    case 0x08:          // Backspace 
    // Move the caret back one space, and then 
    // process this like the DEL key. 

        if (nCaretPosX > 0)
        {
            nCaretPosX--;
            SendMessage(hWnd, WM_KEYDOWN,
                VK_DELETE, 1L);
        }
        break;

    case 0x09:          // Tab 
    // Tab stops exist every four spaces, so add 
    // spaces until the user hits the next tab. 

        do
        {
            SendMessage(hWnd, WM_CHAR, ' ', 1L);
        } while (nCaretPosX % 4 != 0);
        break;

    case 0x0D:          // Carriage return 
    // Go to the beginning of the next line. 
    // The bottom line wraps around to the top. 

        nCaretPosX = 0;

        if (++nCaretPosY == nWindowCharsY)
            nCaretPosY = 0;
        break;

    case 0x1B:        // Escape 
    case 0x0A:        // Linefeed 
        MessageBeep((UINT)-1);
        break;

    default:
        // Add the character to the text buffer. 

        TEXTMATRIX(nCaretPosX, nCaretPosY) = (char)wParam;

        // The application will draw outside the 
        // WM_PAINT message processing, so hide the caret. 

        HideCaret(hWnd);

        // Draw the character on the screen. 

        HDC hdc = GetDC(hWnd);
        SelectObject(hdc,
            GetStockObject(SYSTEM_FIXED_FONT));
        SetBkColor(hdc, RGB(200, 200, 200));
       
        TextOutA(hdc, writeMessageRect.left + nCaretPosX * nWidth,
            writeMessageRect.top + nCaretPosY * nHeight,
            &TEXTMATRIX(nCaretPosX, nCaretPosY), 1);
      
        ReleaseDC(hWnd, hdc);

        // Display the caret. 

        ShowCaret(hWnd);

        // Prepare to wrap around if you reached the 
        // end of the line. 

        if (++nCaretPosX == nWindowCharsX)
        {
            nCaretPosX = 0;
            if (++nCaretPosY == nWindowCharsY)
                nCaretPosY = 0;
        }
        break;
    }
}



/*


Windows (/SUBSYSTEM:WINDOWS)
Toto je pÙvodnÈ nastavenie v  project, Linker, System, SubSystem.


teraz je nastavenÈ 
Console (/SUBSYSTEM:CONSOLE)

toto nove nastavenie Console. Nevol· rovno vytvorenie okna aplikacie cez metÛdu wWinMain. Tak preto
je potrebnÈ ju volaù z novej funkcie int main() {} ktor˙ potrebuje toto nastavenie pre klasick˙
konzolu

int main() {
    return _tWinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOW);
}

Tak˝mto spÙsobom sa pre konzolovÈ nastavenie zavol· naöa metÛda okna wWinMain.
takûe je moûnÈ pouûÌvaù aj konzolu a v˝pisy na nej aj windows·cku apku


*/
int main() {
    return _tWinMain(GetModuleHandle(NULL), NULL, GetCommandLine(), SW_SHOW);
}