#ifndef IMGUI_ASSET_BROWSER_H
#define IMGUI_ASSET_BROWSER_H

#include <imgui.h>

#include <algorithm>
#include <array>
#include <cctype>
#include <cstdint>
#include <filesystem>
#include <functional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace ImGui {

struct AssetEntry {
    std::filesystem::path path;
    bool isDirectory = false;
    std::string extension;
};
enum AssetBrowserFlags : std::uint32_t {
    AssetBrowserFlags_None = 0,
    AssetBrowserFlags_MultiSelect = 1u << 0,
    AssetBrowserFlags_AutoRefresh = 1u << 1,
};

inline AssetBrowserFlags operator|(AssetBrowserFlags lhs, AssetBrowserFlags rhs)
{
    return static_cast<AssetBrowserFlags>(
        static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs));
}

inline AssetBrowserFlags operator&(AssetBrowserFlags lhs, AssetBrowserFlags rhs)
{
    return static_cast<AssetBrowserFlags>(
        static_cast<std::uint32_t>(lhs) & static_cast<std::uint32_t>(rhs));
}

inline AssetBrowserFlags operator~(AssetBrowserFlags flag)
{
    return static_cast<AssetBrowserFlags>(~static_cast<std::uint32_t>(flag));
}

inline AssetBrowserFlags& operator|=(AssetBrowserFlags& lhs, AssetBrowserFlags rhs)
{
    lhs = lhs | rhs;
    return lhs;
}

inline AssetBrowserFlags& operator&=(AssetBrowserFlags& lhs, AssetBrowserFlags rhs)
{
    lhs = lhs & rhs;
    return lhs;
}

class AssetBrowser {
public:
    using ThumbnailCallback = std::function<ImTextureID(const std::filesystem::path&)>;
    using ContextMenuCallback = std::function<void(const std::filesystem::path&)>;
    using CreateAssetCallback = std::function<void(const std::filesystem::path&)>;


public:
    AssetBrowser()
    {
        m_CurrentPath = std::filesystem::current_path();
        m_RootPath = m_CurrentPath;
        m_PathHistory.push_back(m_CurrentPath);
        Refresh();
    }

    void SetRoot(const std::filesystem::path& path)
    {
        m_RootPath = path;
        m_PathHistory.clear();
        m_PathHistory.push_back(path);
        m_HistoryIndex = 0;
        NavigateTo(path, false);
    }

    void SetCurrent(const std::filesystem::path& path)
    {
        NavigateTo(path, true);
    }

    void SetTypeFilters(const std::vector<std::string>& filters)
    {
        m_Filters.clear();
        m_Filters.reserve(filters.size());

        for (auto filter : filters) {
            if (filter.empty())
                continue;

            if (filter.front() != '.')
                filter.insert(filter.begin(), '.');

            ToLower(filter);
            m_Filters.push_back(filter);
        }

        Refresh();
    }


    void SetThumbnailSize(float size) { m_ThumbnailSize = size; }
    void EnableMultiSelect(bool enable) { enable ? AddFlags(AssetBrowserFlags_MultiSelect) : RemoveFlags(AssetBrowserFlags_MultiSelect); }
    void EnableAutoRefresh(bool enable) { enable ? AddFlags(AssetBrowserFlags_AutoRefresh) : RemoveFlags(AssetBrowserFlags_AutoRefresh); }

    void SetFlags(AssetBrowserFlags flags) { m_Flags = flags; }
    void AddFlags(AssetBrowserFlags flags) { m_Flags |= flags; }
    void RemoveFlags(AssetBrowserFlags flags) { m_Flags &= ~flags; }
    AssetBrowserFlags GetFlags() const { return m_Flags; }
    bool HasFlag(AssetBrowserFlags flag) const
    {
        return (static_cast<std::uint32_t>(m_Flags) & static_cast<std::uint32_t>(flag)) != 0u;
    }

    void SetThumbnailCallback(ThumbnailCallback cb) { m_ThumbnailCallback = cb; }
    void SetContextMenuCallback(ContextMenuCallback cb) { m_ContextMenuCallback = cb; }
    void SetCreateAssetCallback(CreateAssetCallback cb) { m_CreateAssetCallback = cb; }

    bool CanNavigateBack() const { return m_HistoryIndex > 0; }
    bool CanNavigateForward() const { return m_HistoryIndex + 1 < m_PathHistory.size(); }

    void NavigateBack()
    {
        if (!CanNavigateBack())
            return;

        --m_HistoryIndex;
        NavigateTo(m_PathHistory[m_HistoryIndex], false);
    }

    void NavigateForward()
    {
        if (!CanNavigateForward())
            return;

        ++m_HistoryIndex;
        NavigateTo(m_PathHistory[m_HistoryIndex], false);
    }

    const std::vector<std::filesystem::path> GetSelected() const
    {
        return std::vector<std::filesystem::path>(m_Selected.begin(), m_Selected.end());
    }

    const std::filesystem::path& GetCurrentPath() const { return m_CurrentPath; }

    void Refresh()
    {
        m_Entries.clear();

        std::error_code ec;
        for (auto& entry : std::filesystem::directory_iterator(m_CurrentPath, ec)) {
            AssetEntry e;
            e.path = entry.path();
            e.isDirectory = entry.is_directory();
            e.extension = entry.path().extension().string();
            ToLower(e.extension);

            if (!PassFilter(e)) continue;

            m_Entries.push_back(e);
        }
    }

    void Display()
    {
        if (HasFlag(AssetBrowserFlags_AutoRefresh))
            Refresh();

        DrawToolbar();
        DrawBreadcrumbs();
        DrawGrid();
    }

private:
    bool PassFilter(const AssetEntry& e) const
    {
        if (!MatchesSearch(e))
            return false;

        if (m_Filters.empty() || e.isDirectory)
            return true;

        for (const auto& f : m_Filters)
            if (e.extension == f)
                return true;

        return false;
    }

    bool MatchesSearch(const AssetEntry& e) const
    {
        if (m_SearchBuffer[0] == '\0')
            return true;

        std::string query(m_SearchBuffer.data());
        ToLower(query);

        std::string candidate = e.path.filename().string();
        ToLower(candidate);

        return candidate.find(query) != std::string::npos;
    }

    static void ToLower(std::string& text)
    {
        std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });
    }

    void DrawToolbar()
    {
        ImGui::BeginGroup();

        ImGui::BeginDisabled(!CanNavigateBack());
        if (ImGui::Button("<"))
            NavigateBack();
        ImGui::EndDisabled();

        ImGui::SameLine();

        ImGui::BeginDisabled(!CanNavigateForward());
        if (ImGui::Button(">"))
            NavigateForward();
        ImGui::EndDisabled();

        ImGui::SameLine();
        if (ImGui::Button("Refresh"))
            Refresh();

        ImGui::SameLine();
        ImGui::SliderFloat("Size", &m_ThumbnailSize, 32.0f, 128.0f);

        ImGui::SameLine();
        if (ImGui::InputText("Search", m_SearchBuffer.data(), m_SearchBuffer.size()))
            Refresh();

        ImGui::EndGroup();
    }

    void DrawBreadcrumbs()
    {
        std::filesystem::path path;
        bool first = true;

        for (auto& part : m_CurrentPath) {
            path /= part;

            if (!first) {
                ImGui::SameLine();
                ImGui::Text(">");
            }

            ImGui::SameLine();
            if (ImGui::Button(part.string().c_str())) {
                SetCurrent(path);
                break;
            }

            first = false;
        }
    }

    void DrawGrid()
    {
        const float cellSize = m_ThumbnailSize + 16.0f;
        const float panelWidth = ImGui::GetContentRegionAvail().x;

        int columnCount = static_cast<int>(panelWidth / cellSize);
        if (columnCount < 1) columnCount = 1;

        ImGui::Columns(columnCount, nullptr, false);

        for (auto& entry : m_Entries) {
            DrawEntry(entry);
            ImGui::NextColumn();
        }

        ImGui::Columns(1);
    }

    void DrawEntry(const AssetEntry& entry)
    {
        ImGui::PushID(entry.path.string().c_str());

        if (m_ThumbnailCallback) {
            ImTextureID tex = m_ThumbnailCallback(entry.path);
            ImGui::ImageButton("##img", tex, ImVec2(m_ThumbnailSize, m_ThumbnailSize));
        } else {
            ImGui::Button("##btn", ImVec2(m_ThumbnailSize, m_ThumbnailSize));
        }

        if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(0) && entry.isDirectory)
            SetCurrent(entry.path);

        if (ImGui::IsItemClicked())
            HandleSelection(entry.path);

        if (ImGui::BeginDragDropSource()) {
            auto pathStr = entry.path.string();
            ImGui::SetDragDropPayload("ASSET_PATH", pathStr.c_str(), pathStr.size() + 1);
            ImGui::Text("%s", pathStr.c_str());
            ImGui::EndDragDropSource();
        }

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete")) {
                std::filesystem::remove_all(entry.path);
                Refresh();
            }
            if (ImGui::MenuItem("Rename"))
                m_RenameTarget = entry.path;

            if (m_ContextMenuCallback)
                m_ContextMenuCallback(entry.path);

            ImGui::EndPopup();
        }

        ImGui::TextWrapped("%s", entry.path.filename().string().c_str());

        ImGui::PopID();
    }

    void HandleSelection(const std::filesystem::path& path)
    {
        if (!HasFlag(AssetBrowserFlags_MultiSelect)) {
            m_Selected.clear();
            m_Selected.insert(path);
            return;
        }

        if (m_Selected.count(path))
            m_Selected.erase(path);
        else
            m_Selected.insert(path);
    }

    void NavigateTo(const std::filesystem::path& path, bool recordHistory)
    {
        if (path.empty())
            return;

        if (recordHistory && path == m_CurrentPath)
            return;

        m_CurrentPath = path;

        if (recordHistory) {
            if (m_HistoryIndex + 1 < m_PathHistory.size()) {
                const auto eraseBegin = m_PathHistory.begin() + static_cast<std::vector<std::filesystem::path>::difference_type>(m_HistoryIndex + 1);
                m_PathHistory.erase(eraseBegin, m_PathHistory.end());
            }

            m_PathHistory.push_back(path);
            m_HistoryIndex = m_PathHistory.size() - 1;
        }

        Refresh();
    }


private:
    std::filesystem::path m_CurrentPath;
    std::filesystem::path m_RootPath;

    std::vector<std::filesystem::path> m_PathHistory;
    std::size_t m_HistoryIndex = 0;

    std::vector<AssetEntry> m_Entries;
    std::unordered_set<std::filesystem::path> m_Selected;

    std::vector<std::string> m_Filters;

    float m_ThumbnailSize = 64.0f;
    AssetBrowserFlags m_Flags = AssetBrowserFlags_MultiSelect;

    ThumbnailCallback m_ThumbnailCallback;
    ContextMenuCallback m_ContextMenuCallback;
    CreateAssetCallback m_CreateAssetCallback;

    std::unordered_map<std::filesystem::path, ImTextureID> m_ThumbnailCache;

    std::filesystem::path m_RenameTarget;

    std::array<char, 256> m_SearchBuffer{};
};



} // namespace ImGui

#endif // IMGUI_ASSET_BROWSER_H
