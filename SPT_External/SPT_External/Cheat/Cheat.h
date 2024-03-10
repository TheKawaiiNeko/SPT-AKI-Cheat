#pragma once
#include "..\ImGui\imgui.h"
#include "..\ImGui\imgui_impl_win32.h"
#include "..\ImGui\imgui_impl_dx11.h"
#include "..\..\Utils\Globals\Globals.h"
#include "NotSDK\NotSDK.h"

extern bool IsKeyDown(int VK);

class Cheat
{
public:
	void RenderInfo();
	void RenderMenu();
	void RenderESP();
private:
    // Colors
    ImColor Col_ESP_PMC = { 1.f, 0.f, 0.f, 1.f };
    ImColor Col_ESP_Scav = { 0.f, 1.f, 0.f, 1.f };
    ImColor Col_ESP_Boss = { 0.f, 1.f, 0.f, 1.f };
    ImColor Col_ESP_Team = { 0.f, 1.f, 1.f, 0.f };

    void DrawLine(ImVec2 a, ImVec2 b, ImColor color, float width)
    {
        ImGui::GetWindowDrawList()->AddLine(a, b, color, width);
    }
    void DrawBox(int x, int y, int w, int h, ImColor color)
    {
        DrawLine(ImVec2(x, y), ImVec2(x + w, y), color, 1.0f);
        DrawLine(ImVec2(x, y), ImVec2(x, y + h), color, 1.0f);
        DrawLine(ImVec2(x + w, y), ImVec2(x + w, y + h), color, 1.0f);
        DrawLine(ImVec2(x, y + h), ImVec2(x + w, y + h), color, 1.0f);
    }
    void Text(ImVec2 pos, ImColor color, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect)
    {
        ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), pos, color, text_begin, text_end, wrap_width, cpu_fine_clip_rect);
    }
    void String(ImVec2 pos, ImColor color, const char* text)
    {
        Text(pos, color, text, text + strlen(text), 200, 0);
    }
    void RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)
    {
        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
    }
    void HealthBar(float x, float y, float w, float h, int value, int v_max)
    {
        ImColor barColor = ImColor(min(510 * (v_max - value) / 100, 255), min(510 * value / 100, 255), 25, 255);

        RectFilled(x - 1, y + 1, x + w + 1, y + h - 1, ImColor(0.f, 0.f, 0.f, 0.6f), 0.f, 0);
        RectFilled(x, y, x + w, y + ((h / float(v_max)) * (float)value), barColor, 0.0f, 0);
    }
    void Circle(float x, float y, float fov_size, ImColor color, float size)
    {
        ImGui::GetBackgroundDrawList()->AddCircle(ImVec2(x / 2.f, y / 2.f), fov_size, color, 100, size);
    }
};