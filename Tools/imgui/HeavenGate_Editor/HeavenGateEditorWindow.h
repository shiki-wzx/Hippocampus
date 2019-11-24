//
//Copyright (c) 2019 Star Platinum
//
//Created by Kong Wei Hang, 2019-11-17
//example_win32_directx11, HeavenGateEditor
//
//Add Description
//
#ifndef HeavenGateEditorWindow_h
#define HeavenGateEditorWindow_h


#include "HeavenGateEditorConstant.h"
#include "StoryJson.h"

using std::string;
using json = nlohmann::json;

namespace HeavenGateEditor {
    class HeavenGateEditor
    {
    public:

        //Max number of display folders
        static const int MAX_NUM_OF_DISPLAY_FORLDERS;

        //Tool folder name
        static const char* const TOOL_FOLDER_NAME;

        //Relative path form project root to story folder
        static const char* const PATH_FROM_PROJECT_ROOT_TO_STORY_FOLDER;

    public:
        HeavenGateEditor();
        ~HeavenGateEditor();
        void ShowEditorWindow(bool* isOpenPoint);
        void ShowEditorMenuFile();
        void OpenSelectStoryWindow(bool* p_open);

    private:
        void ExePath(char* const outExePath);

        //Is Open Select Story Window
        bool show_app_layout;

        bool isSavedFile = false;
        char storyPath[MAX_FOLDER_PATH];



        json currentStory;
        StoryJson* m_story;



    };

#ifndef _WIN32
        bool GetModuleFileNameOSX(char *  pOutCurrentPath);
#endif

}


#endif /* HeavenGateEditorWindow_h */
