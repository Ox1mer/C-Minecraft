#include "framework.h"
#include "Mine2.h"

#include <cstdlib>
#include <ctime>
#include <vector>
#include <array>
#include <windows.h>
#include <cassert>

#include "Player.h"
#include "Camera.h"
#include "CameraConfig.h"
#include "Block.h"
#include "Position3D.h"
#include "ScreenPoint.h"
#include "Chunk.h"

#define MAX_LOADSTRING 100

bool keys[256] = {};
HINSTANCE hInst;
WCHAR szTitle[] = L"OxiCraft";
WCHAR szWindowClass[] = L"OxiCraft";

static HBITMAP      hDIB = NULL;
static uint32_t* frameBuffer = nullptr;
static BITMAPINFO   bmi = {};
static int screenW = GetSystemMetrics(SM_CXSCREEN);
static int screenH = GetSystemMetrics(SM_CYSCREEN);

static std::vector<double> zBuffer(screenW* screenH);

static Camera      camera{ CameraConfig(screenH, screenW, 90.0, 0.1, 1000.0) };
static Player player(10, 7, camera, Transform());
static Chunk       chunk{ Position3D(0,0,0) };
static std::vector<Block*> worldBlocks;
static HWND        hWndMain;
double deltaTime;

// FPS
static int         targetFPS = 144;

ATOM        MyRegisterClass(HINSTANCE);
BOOL        InitInstance(HINSTANCE, int);
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void        onStart();
void        frameUpdate();
void        drawDebugText(HWND hWnd);
void        clearFrame();
void        drawFrame();
void        renderBlocks();
void        drawFilledTriangleToBuffer(const ScreenPoint&, const ScreenPoint&, const ScreenPoint&);
bool        isShouldRenderTri(std::array<Position3D, 3> tri);

template<typename T>
T myMin(T a, T b) { return (a < b) ? a : b; }
template<typename T>
T myMax(T a, T b) { return (a > b) ? a : b; }


int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE, LPWSTR, int nCmdShow) {
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MINE2, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
    if (!InitInstance(hInstance, nCmdShow))
        return FALSE;

    MSG            msg = {};
    LARGE_INTEGER  freq, start, end, prev;
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&prev);

    double         frameTime = 1.0 / targetFPS;

    while (true) {
        QueryPerformanceCounter(&start);

        deltaTime = double(start.QuadPart - prev.QuadPart) / freq.QuadPart;
        prev = start;

        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT)
                return (int)msg.wParam;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        frameUpdate();
        clearFrame();
        renderBlocks();
        drawFrame();
        drawDebugText(hWndMain);

        QueryPerformanceCounter(&end);
        double elapsed = double(end.QuadPart - start.QuadPart) / freq.QuadPart;
        double delay = frameTime - elapsed;
        if (delay > 0)
            Sleep(static_cast<DWORD>(delay * 1000.0));
    }
}

ATOM MyRegisterClass(HINSTANCE hInstance) {
    WNDCLASSEXW wcex = {};
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MINE2));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow) {
    hInst = hInstance;

    hWndMain = CreateWindowW(
        szWindowClass, szTitle,
        WS_POPUP,
        0, 0, screenW, screenH,
        nullptr, nullptr, hInstance, nullptr
    );
    if (!hWndMain) return FALSE;

    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = screenW;
    bmi.bmiHeader.biHeight = -screenH;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    HDC hdc = GetDC(hWndMain);
    hDIB = CreateDIBSection(hdc, &bmi, DIB_RGB_COLORS,
        (void**)&frameBuffer, nullptr, 0);
    ReleaseDC(hWndMain, hdc);

    onStart();

    ShowWindow(hWndMain, nCmdShow);
    UpdateWindow(hWndMain);
    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_KEYDOWN:
        keys[wParam] = true;
        break;

    case WM_KEYUP:
        keys[wParam] = false;
        break;
    case WM_MOUSEMOVE: {
        static bool first = true;
        static int  lastX = screenW / 2, lastY = screenH / 2;
        int x = LOWORD(lParam), y = HIWORD(lParam);
        if (first) { lastX = x; lastY = y; first = false; break; }
        float sens = 0.1f;
        camera.rotate((x - lastX) * sens, (lastY - y) * sens);
        lastX = x; lastY = y;
        POINT c = { screenW / 2, screenH / 2 };
        ClientToScreen(hWndMain, &c);
        SetCursorPos(c.x, c.y);
        lastX = screenW / 2; lastY = screenH / 2;
    } break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

void onStart() {
    std::srand(std::time(nullptr));

    camera = Camera(CameraConfig(screenH, screenW, 90.0, 0.1, 1000.0));

    zBuffer.resize(screenW * screenH, 1e9);

    chunk.addBlockToChunk(Block(Transform(Position3D(10, 0, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(9, 0, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(8, 0, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(7, 0, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(7, 1, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(4, 0, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(4, 1, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));

    chunk.addBlockToChunk(Block(Transform(Position3D(10, 2, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(10, 3, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(10, 4, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(9, 2, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(8, 2, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(7, 2, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(7, 3, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(7, 4, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(6, 2, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(6, 4, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(5, 2, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(5, 4, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(4, 2, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(4, 4, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));


    chunk.addBlockToChunk(Block(Transform(Position3D(15, 0, 15), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(15, 0, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(0, 0, 15), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));
    chunk.addBlockToChunk(Block(Transform(Position3D(0, 0, 0), Rotation3D(0, 0, 0), Scale3D(1, 1, 1))));

    worldBlocks = chunk.getChunkBlocks();

    ShowCursor(FALSE);
    SetCapture(hWndMain);
}


void frameUpdate() {
    camera.update();

    // -- MOVEMENT -- //
    Vec3 forward = camera.getForwardVector();
    Vec3 right = camera.getRightVector();

    forward.y = right.y = 0.0;
    forward.normalize();
    right.normalize();

    double move = player.getSpeed() * deltaTime;

    if (keys['W']) {
        player.movePlayer(forward.x * move, 0, forward.z * move, 1);
    }
    if (keys['S']) {
        player.movePlayer(forward.x * move, 0, forward.z * move, -1);
    }
    if (keys['A']) {
        player.movePlayer(right.x * move, 0, right.z * move, 1);
    }
    if (keys['D']) {
        player.movePlayer(right.x * move, 0, right.z * move, -1);
    }

    // Quit button
    if (keys['O']) {
        PostQuitMessage(0);
    }
}

void drawDebugText(HWND hWnd) {
    HDC hdc = GetDC(hWnd);

    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 200, 0));

    HFONT hFont = CreateFont(
        -20, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, FALSE,
        ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, FIXED_PITCH | FF_MODERN,
        L"Consolas");
    HFONT oldFont = (HFONT)SelectObject(hdc, hFont);

    Position3D pos = player.transform.getPosition();
    Rotation3D rot = camera.transform.getRotation();

    double fps = 1.0 / deltaTime;

    wchar_t buffer[256];
    swprintf(buffer, 256,
        L"📌 Player:\n"
        L"   X: %.2f\n"
        L"   Y: %.2f\n"
        L"   Z: %.2f\n\n"
        L"🎥 Camera:\n"
        L"   Pitch: %.2f°\n"
        L"   Yaw:   %.2f°\n"
        L"   Roll:  %.2f°\n\n"
        L"⏱ FPS: %.1f",
        pos.x, pos.y, pos.z,
        rot.pitch, rot.yaw, rot.roll,
        fps
    );


    RECT rect = { 10, 10, 500, 300 };
    DrawText(hdc, buffer, -1, &rect, DT_LEFT | DT_TOP | DT_NOPREFIX);

    SelectObject(hdc, oldFont);
    DeleteObject(hFont);
    ReleaseDC(hWnd, hdc);
}

void clearFrame() {
    memset(frameBuffer, 0, screenW * screenH * sizeof(uint32_t));
    std::fill(zBuffer.begin(), zBuffer.end(), 1e9);
}


void drawFrame() {
    HDC hdc = GetDC(hWndMain);
    StretchDIBits(
        hdc,
        0, 0, screenW, screenH,
        0, 0, screenW, screenH,
        frameBuffer,
        &bmi,
        DIB_RGB_COLORS,
        SRCCOPY
    );
    ReleaseDC(hWndMain, hdc);
}

Vec3 operator-(const Position3D& a, const Position3D& b) {
    return Vec3{ a.x - b.x, a.y - b.y, a.z - b.z };
}

bool isShouldRenderTri(std::array<Position3D, 3> tri) {
    Vec3 edge1 = tri[1] - tri[0];
    Vec3 edge2 = tri[2] - tri[0];

    Vec3 normal = Vec3::cross(edge2, edge1).normalized();

    Vec3 toCamera = (camera.transform.getPosition() - tri[0]).normalized();

    double dot = Vec3::dot(normal, toCamera);

    const double EPSILON = 1e-5;

    return dot > EPSILON;
}


void renderBlocks() {
    for (Block* blockPtr : worldBlocks) {
        auto planes = blockPtr->getRenderedPlanes();
        for (auto pl : planes) {
            int ia = pl->a, ib = pl->b;
            auto triA = blockPtr->getTriangleGlobalCoords(ia);
            auto triB = blockPtr->getTriangleGlobalCoords(ib);
            for (auto& tri3d : { triA, triB }) {
                if(isShouldRenderTri(tri3d))
                {
                    ScreenPoint pts[3];
                    bool ok = true;
                    for (int i = 0; i < 3; ++i) {
                        Position3D v = tri3d[i];
                        ScreenPoint sc;
                        if (!camera.isPointInFrustum(v) ||
                            !camera.projectWorldToScreen(v, sc))
                        {
                            ok = false; break;
                        }
                        int ix = int((sc.x - screenW * 0.5f) + screenW * 0.5f);
                        int iy = int(sc.y);
                        if (ix < 0 || ix >= screenW || iy < 0 || iy >= screenH) { ok = false; break; }
                        pts[i] = ScreenPoint(ix, iy, sc.depth);
                    }
                    if (!ok) continue;
                    drawFilledTriangleToBuffer(pts[0], pts[1], pts[2]);
                }
            }
        }
    }
}

void drawFilledTriangleToBuffer(const ScreenPoint& p0,
    const ScreenPoint& p1,
    const ScreenPoint& p2)
{
    /*
    int r = rand() % 256;
    int g = rand() % 256;
    int b = rand() % 256;
    int color = (0xFF << 24) | (r << 16) | (g << 8) | b;
    */

    int color = 0xFFAA33CC;

    auto edge = [](const ScreenPoint& a, const ScreenPoint& b, float x, float y) {
        return (x - a.x) * (b.y - a.y) - (y - a.y) * (b.x - a.x);
        };

    int minX = myMax(myMin(myMin(static_cast<int>(p0.x), static_cast<int>(p1.x)), static_cast<int>(p2.x)), 0);
    int maxX = myMin(myMax(myMax(static_cast<int>(p0.x), static_cast<int>(p1.x)), static_cast<int>(p2.x)), screenW - 1);
    int minY = myMax(myMin(myMin(static_cast<int>(p0.y), static_cast<int>(p1.y)), static_cast<int>(p2.y)), 0);
    int maxY = myMin(myMax(myMax(static_cast<int>(p0.y), static_cast<int>(p1.y)), static_cast<int>(p2.y)), screenH - 1);

    double area = edge(p0, p1, p2.x, p2.y);
    if (area == 0.0) return;

    double invArea = 1.0 / area;
    const int pitch = screenW;

    for (int y = minY; y <= maxY; ++y) {
        int rowStart = y * pitch;
        float fy = static_cast<float>(y);

        for (int x = minX; x <= maxX; ++x) {
            int index = rowStart + x;
            float fx = static_cast<float>(x);

            double w0 = edge(p1, p2, fx, fy) * invArea;
            double w1 = edge(p2, p0, fx, fy) * invArea;
            double w2 = edge(p0, p1, fx, fy) * invArea;

            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                double z = w0 * p0.depth + w1 * p1.depth + w2 * p2.depth;

                if (z < zBuffer[index]) {
                    zBuffer[index] = z;
                    frameBuffer[index] = color;
                }
            }
        }
    }
}


