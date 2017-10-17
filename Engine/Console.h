#pragma once
#include "imgui_dock.h"
#define IM_ARRAYSIZE(_ARR)  ((int)(sizeof(_ARR)/sizeof(*_ARR)))

struct ExampleAppConsole
{
	char                  InputBuf[256];
	ImVector<char*>       Items;
	bool                  ScrollToBottom;
	ImVector<char*>       History;
	int                   HistoryPos;    // -1: new line, 0..History.Size-1 browsing history.
	ImVector<const char*> Commands;

	ExampleAppConsole()
	{
		ClearLog();
		
	}
	~ExampleAppConsole()
	{
		ClearLog();
		for (int i = 0; i < History.Size; i++)
			free(History[i]);
	}

	// Portable helpers
	static int   Stricmp(const char* str1, const char* str2) { int d; while ((d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; } return d; }
	static int   Strnicmp(const char* str1, const char* str2, int n) { int d = 0; while (n > 0 && (d = toupper(*str2) - toupper(*str1)) == 0 && *str1) { str1++; str2++; n--; } return d; }
	static char* Strdup(const char *str) { size_t len = strlen(str) + 1; void* buff = malloc(len); return (char*)memcpy(buff, (const void*)str, len); }

	void    ClearLog()
	{
		for (int i = 0; i < Items.Size; i++)
			free(Items[i]);
		Items.clear();
		ScrollToBottom = true;
	}

	void    AddLog(const char* fmt, ...) IM_FMTARGS(2)
	{
		char buf[1024];
		va_list args;
		va_start(args, fmt);
		vsnprintf(buf, IM_ARRAYSIZE(buf), fmt, args);
		buf[IM_ARRAYSIZE(buf) - 1] = 0;
		va_end(args);
		Items.push_back(Strdup(buf));
		ScrollToBottom = true;
	}

	void    Draw(const char* title, bool* p_open)
	{
		
		int console_width = 500, console_height = 200;
		//ImGui::SetNextWindowSize(ImVec2(console_width, console_height), ImGuiCond_FirstUseEver);
		//ImGui::SetNextWindowPos(ImVec2(SCREEN_WIDTH - console_width, SCREEN_HEIGHT - console_height), ImGuiCond_FirstUseEver);
		if (ImGui::BeginDock("Console", false, false, false,
			ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse |
			 ImGuiWindowFlags_ShowBorders))
		{


			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()+20), false, ImGuiWindowFlags_HorizontalScrollbar);

			ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
			static ImGuiTextFilter filter;

			ImGui::PopStyleVar();
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing()+20), false, ImGuiWindowFlags_HorizontalScrollbar);
			if (ImGui::BeginPopupContextWindow())
			{

				ImGui::EndPopup();
			}
		//	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0)); // Tighten spacing

			for (int i = 0; i < Items.Size; i++)
			{
				const char* item = Items[i];
				if (!filter.PassFilter(item))
					continue;
				ImVec4 col = ImVec4(0.0f, 0.0f, 0.0f, 10.0f); // A better implementation may store a type per-item. For the sample let's just parse the text.
				if (strstr(item, "[error]")) col = ImColor(1.0f, 0.4f, 0.4f, 1.0f);
				else if (strncmp(item, "# ", 2) == 0) col = ImColor(1.0f, 0.78f, 0.58f, 1.0f);
				ImGui::PushStyleColor(ImGuiCol_Text, col);
				ImGui::TextUnformatted(item);
				ImGui::PopStyleColor();
			}

			if (ScrollToBottom)
				ImGui::SetScrollHere();
			ScrollToBottom = false;
			//ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::EndChild();
			

			// Command-line
		}
		ImGui::EndDock();
	}

	
};

