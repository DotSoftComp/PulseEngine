/**
 * @file InterfaceEditor.h
 * @author Dorian LEXTERIAQUE (dlexteriaque@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-06-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */


#ifndef INTERFACE_EDITOR_H
#define INTERFACE_EDITOR_H

#include "Common/common.h"
#include "Common/dllExport.h"
#include <unordered_map>
#include <string>

class PulseEngineBackend;
class TopBar;
class Entity;

struct LoadingPopupData
{
    std::string title;
    std::function<void()> contentFunction;
    float progressPercent = 0.0f;
};

class MA_DLL_API InterfaceEditor
{
private:
    friend class TopBar;
    TopBar* topbar;
    Entity* selectedEntity = nullptr;
    std::unordered_map<std::string, bool> windowStates;
    std::vector<LoadingPopupData> loadingPopups;

public:
    InterfaceEditor(PulseEngineBackend* engine);

    void Render(PulseEngineBackend *engine);
    void EngineConfigWindow(PulseEngineBackend *engine);
    void EntityAnalyzerWindow();
    void GenerateSceneDataWindow(PulseEngineBackend *engine);
    void Viewport(PulseEngineBackend *engine);

    void ShowLoadingPopup(std::function<void()> contentFunction, float progressPercent);
    void AddLoadingPopup(std::function<void()> contentFunction, float progressPercent, const std::string& title = "")
    {
        LoadingPopupData popupData;
        popupData.contentFunction = contentFunction;
        popupData.progressPercent = progressPercent;
        popupData.title = title.empty() ? "Loading..." : title;

        loadingPopups.push_back(popupData);
    }

    void ChangePorgressIn(std::string originText, float progress)
    {
        for (auto& popup : loadingPopups)
        {
            if (popup.title == originText)
            {
                popup.progressPercent = progress;
                return;
            }
        }
    }

    void ChangeProgressContent(std::function<void()> newContentFunction, const std::string& title)
    {
        for (auto& popup : loadingPopups)
        {
            if (popup.title == title)
            {
                popup.contentFunction = newContentFunction;
                return;
            }
        }
    }

    void RemoveLoadingPopup(const std::string& title)
    {
        loadingPopups.erase(std::remove_if(loadingPopups.begin(), loadingPopups.end(),
            [&title](const LoadingPopupData& popup) { return popup.title == title; }), loadingPopups.end());
    }
    ~InterfaceEditor() {}
};


#endif