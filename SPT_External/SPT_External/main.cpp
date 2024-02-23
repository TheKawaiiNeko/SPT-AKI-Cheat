#include "overlay.h"
#include "Utils\Memory\Memory.h"
#include "Utils\SimpleMath\SimpleMath.h"
#pragma warning(disable : 4996) 

Overlay Ov, * v = &Ov;

bool Run = false;
int MenuKey = VK_END;
HWND GameHwnd = nullptr;
HWND OverlayHwnd = nullptr;

void ManagerTH();

//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
int main()
{
    AllocConsole();
    freopen("CONIN$", "r", stdin);
    freopen("CONOUT$", "w", stdout);
    freopen("CONOUT$", "w", stderr);

    //system("color 0c");

    std::cout << "" << std::endl;
    std::cout << " _____          _                  _____      _                        _ " << std::endl;
    std::cout << "|_   _|_ _ _ __| | _______   __   | ____|_  _| |_ ___ _ __ _ __   __ _| |" << std::endl;
    std::cout << "  | |/ _` | '__| |/ / _ \\ \\ / /   |  _| \\ \\/ / __/ _ \\ '__| '_ \\ / _` | |" << std::endl;
    std::cout << "  | | (_| | |  |   < (_) \\ V /    | |___ >  <| ||  __/ |  | | | | (_| | |" << std::endl;
    std::cout << "  |_|\\__,_|_|  |_|\\_\\___/ \\_/     |_____/_/\\_\\\\__\\___|_|  |_| |_|\\__,_|_|" << std::endl;
    std::cout << "      " << std::endl;

    for (int i = 0; i < 2; i++)
        printf("\n");

    // Init
    if (!m.Init())
        return 0;

    // Apexのウィンドウサイズ/POINTを取得
    GameHwnd = FindWindowA(NULL, "EscapeFromTarkov");

    if (!GameHwnd)
    {
        while (!GameHwnd)
        {
            GameHwnd = FindWindowA(NULL, "EscapeFromTarkov");

            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);
    GetClientRect(GameHwnd, &v->GameRect);

    // ウィンドウが最小化されていたら
    while (v->GameRect.right == 0 && v->GameRect.bottom == 0)
    {
        GetClientRect(GameHwnd, &v->GameRect);

        std::this_thread::sleep_for(std::chrono::seconds(1));
    } 

    ClientToScreen(GameHwnd, &v->GamePoint);

    // Create Overlay
    WNDCLASSEXA wc = { sizeof(WNDCLASSEXA), 0, WndProc, 0, 0, NULL, NULL, NULL, NULL, "A Overlay", "WND", NULL };
    RegisterClassExA(&wc);
    OverlayHwnd = CreateWindowExA(WS_EX_LAYERED | WS_EX_TRANSPARENT | WS_EX_TOPMOST, wc.lpszClassName, wc.lpszMenuName, WS_POPUP | WS_VISIBLE, v->GamePoint.x, v->GamePoint.y, v->GameRect.right, v->GameRect.bottom, NULL, NULL, wc.hInstance, NULL);
    SetLayeredWindowAttributes(OverlayHwnd, RGB(0, 0, 0), 255, LWA_ALPHA);
    MARGINS margin = { -1 };
    DwmExtendFrameIntoClientArea(OverlayHwnd, &margin);

    if (!CreateDeviceD3D(OverlayHwnd))
    {
        CleanupDeviceD3D();
        UnregisterClassA(wc.lpszClassName, wc.hInstance);
        exit(0);
    }

    ShowWindow(OverlayHwnd, SW_SHOW);
    UpdateWindow(OverlayHwnd);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;

    //LoadStyle();

    ImGui_ImplWin32_Init(OverlayHwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Background color
    ImVec4 clear_color = ImVec4(1.f, 1.f, 1.f, 0.f);

    // Main loop
    Run = true;
    std::thread(ManagerTH).detach();

    std::cout << "  [+] Successful Initialization!" << std::endl;
    printf("\n");
    printf("  [+] BaseAddress : 0x%I64x\n", m.BaseAddress);
    printf("  [+]  Process ID : %d\n", m.PID);
    printf("  [+] WindowSize  : %d, %d\n\n", Ov.GameRect.right, Ov.GameRect.bottom);

    std::cout << "  [+] Ready!" << std::endl;
    printf("\n");
    std::cout << "  MenuKey : VK_END" << std::endl;

    while (Run)
    {
        MSG msg;
        while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // Info
        v->m_Info();

        // Menu
        if (v->ShowMenu)
            v->m_Menu();

        // ESP
       if (v->ESP)
            v->m_ESP();

        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        g_pSwapChain->Present(1, 0);
    }

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    DestroyWindow(OverlayHwnd);
    UnregisterClassA(wc.lpszClassName, wc.hInstance);

    return 0;
}

// 主にオーバーレイの管理用スレッド
void ManagerTH()
{
    while (Run)
    {
        // ゲームが実行されているかをチェックする
        HWND CheckHwnd = FindWindowA(NULL, "EscapeFromTarkov");
        if (!CheckHwnd)
            Run = false;

        // ShowMenu
        static bool menu_key = false;
        if (IsKeyDown(MenuKey) && !menu_key)
        {
            v->ShowMenu = !v->ShowMenu;

            if (v->ShowMenu)
                SetWindowLong(OverlayHwnd, GWL_EXSTYLE, WS_EX_LAYERED | WS_EX_TOPMOST);
            else 
                SetWindowLong(OverlayHwnd, GWL_EXSTYLE, WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOPMOST);

            menu_key = true;
        }
        else if (!IsKeyDown(MenuKey) && menu_key)
        {
            menu_key = false;
        }

        RECT TmpRect = {};
        POINT TmpPoint = {};
        GetClientRect(GameHwnd, &TmpRect);
        ClientToScreen(GameHwnd, &TmpPoint);

        // Resizer
        if (TmpRect.left != v->GameRect.left || TmpRect.bottom != v->GameRect.bottom || TmpRect.top != v->GameRect.top || TmpRect.right != v->GameRect.right || TmpPoint.x != v->GamePoint.x || TmpPoint.y != v->GamePoint.y)
        {
            v->GameRect = TmpRect;
            SetWindowPos(OverlayHwnd, nullptr, TmpPoint.x, TmpPoint.y, v->GameRect.right, v->GameRect.bottom, SWP_NOREDRAW);
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}