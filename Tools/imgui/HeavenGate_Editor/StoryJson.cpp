#include "StoryJson.h"
#include "CharacterUtility.h"

#include "StoryJsonStoryNode.h"
#include "StoryJsonWordNode.h"
#include "StoryJsonJumpNode.h"
#include "StoryJsonLabelNode.h"
#include "StoryJsonExhibitNode.h"

#include <utility>

#include <stdexcept>


namespace HeavenGateEditor {
//using NodeType = StoryNode::NodeType;
enum class StructLayout {
    Info,
    Value
};
enum class InfoLayout {
    Chapter,
    Scene
};





extern char StructString[][MAX_ENUM_LENGTH] = {
    "info",
    "value"
};
extern char infoString[][MAX_ENUM_LENGTH] = {
    "chapter",
    "scene"
};




int HeavenGateEditor::StoryJson::AddNode(StoryNode* const node) {
    if (node->m_nodeType == NodeType::None) {
        return -1;
    }

    m_nodes.push_back(node);
    return static_cast<int>(m_nodes.size());
}

int HeavenGateEditor::StoryJson::AddWord(StoryWord * const word)
{
    m_nodes.push_back(word);
    return static_cast<int>(m_nodes.size());
}

int  HeavenGateEditor::StoryJson::AddWord(const char* name, const char* content) {
    StoryWord* word = new StoryWord;
    word->m_nodeType = NodeType::Word;
    strcpy(word->m_name, name);
    strcpy(word->m_content, content);

    return  AddNode(word);
}

int StoryJson::InsertWord(const char* name, const char* content, int index)
{
    int i = 0;
    for (auto iter = m_nodes.cbegin(); iter != m_nodes.cend(); iter++)
    {
        if (i == index)
        {
            StoryWord* word = new StoryWord;
            word->m_nodeType = NodeType::Word;
            strcpy(word->m_name, name);
            strcpy(word->m_content, content);

            m_nodes.insert(iter, word);
        }

        i++;
    }
    return i;
}

int HeavenGateEditor::StoryJson::AddLabel(const char* labelName) {
    StoryLabel* label = new StoryLabel;
    label->m_nodeType = NodeType::Label;
    strcpy(label->m_labelId, labelName);

    return  AddNode(label);
}
int StoryJson::InsertLabel(const char* labelName, int index)
{
    int i = 0;
    for (auto iter = m_nodes.cbegin(); iter != m_nodes.cend(); iter++)
    {
        if (i == index)
        {
            StoryLabel* label = new StoryLabel;
            label->m_nodeType = NodeType::Label;
            strcpy(label->m_labelId, labelName);

            m_nodes.insert(iter, label);
        }

        i++;
    }
    return i;
}


int HeavenGateEditor::StoryJson::AddJump(const char* jumpName, const char* jumpContent) {
    StoryJump* jump = new StoryJump;
    jump->m_nodeType = NodeType::Jump;
    strcpy(jump->m_jumpId, jumpName);
    strcpy(jump->m_jumpContent, jumpContent);

    return  AddNode(jump);
}
int StoryJson::InsertJump(const char* jumpName, const char* jumpContent, int index)
{
    int i = 0;
    for (auto iter = m_nodes.cbegin(); iter != m_nodes.cend(); iter++)
    {
        if (i == index)
        {
            StoryJump* jump = new StoryJump;
            jump->m_nodeType = NodeType::Jump;
            strcpy(jump->m_jumpId, jumpName);
            strcpy(jump->m_jumpContent, jumpContent);

            m_nodes.insert(iter, jump);
        }

        i++;
    }

    return i;
}


int StoryJson::AddExhibit(const char *exhibitName) {
    StoryExhibit* jump = new StoryExhibit;
    jump->m_nodeType = NodeType::Exhibit;
    strcpy(jump->m_exhibitName, exhibitName);

    return  AddNode(jump);
}

int StoryJson::InsertExhibit(const char* exhibitName, int index){
    int i = 0;
    for (auto iter = m_nodes.cbegin(); iter != m_nodes.cend(); iter++)
    {
        if (i == index)
        {
            StoryExhibit* exhibit = new StoryExhibit;
            exhibit->m_nodeType = NodeType::Exhibit;
            strcpy(exhibit->m_exhibitName, exhibitName);


            m_nodes.insert(iter, exhibit);
        }

        i++;
    }

    return i;
}


void StoryJson::Swap(int lhs, int rhs)
{
    if (lhs < 0 || rhs < 0 || lhs >= m_nodes.size() || rhs >= m_nodes.size())
    {
        return;
    }

    int i = 0;
    list<StoryNode*>::iterator lIter = m_nodes.begin();
    list<StoryNode*>::iterator rIter = m_nodes.begin();

    for (; lIter != m_nodes.end(); lIter++)
    {
        if (lhs == i)
        {
            break;
        }

        i++;
    }

    i = 0;

    for (; rIter != m_nodes.end(); rIter++)
    {
        if (rhs == i)
        {
            break;
        }

        i++;
    }

    StoryNode* node = *lIter;
    *lIter = *rIter;
    *rIter = node;
}

const HeavenGateEditor::StoryNode * const HeavenGateEditor::StoryJson::GetNode(int index) const
{
    int i = 0;
    if (index >= m_nodes.size())
    {
        return nullptr;
    }

    for (auto iter = m_nodes.cbegin(); iter != m_nodes.cend(); iter++)
    {
        if (i == index)
        {
            return *iter;
        }

        i++;
    }
    return nullptr;
}

void StoryJson::Remove(int index) {
    int i = 0;
    for (auto iter = m_nodes.cbegin(); iter != m_nodes.cend(); iter++) {
        if (i == index)
        {
            iter = m_nodes.erase(iter);
        }
        i++;
    }
}

HeavenGateEditor::StoryNode * const HeavenGateEditor::StoryJson::GetNode(int index)
{
    return const_cast<StoryNode*>(
                                  static_cast<const StoryJson&>(*this).GetNode(index));
}

int StoryJson::Size() const
{
    return  static_cast<int>(m_nodes.size());

}

StoryJson::StoryJson()
{
    memset(m_fullPath, '\0', sizeof(m_fullPath));
    memset(m_chapter, '\0', sizeof(m_chapter));
    memset(m_scene, '\0', sizeof(m_scene));

}

StoryJson::StoryJson(const StoryJson& storyJson)
{
    if (this == &storyJson)
    {
        return;
    }
    memset(m_fullPath, '\0', sizeof(m_fullPath));
    strcpy(m_fullPath, storyJson.m_fullPath);

    memset(m_chapter, '\0', sizeof(m_chapter));
    strcpy(m_chapter, storyJson.m_chapter);

    memset(m_scene, '\0', sizeof(m_scene));
    strcpy(m_scene, storyJson.m_scene);

    for (auto iter = storyJson.m_nodes.cbegin(); iter != storyJson.m_nodes.cend(); iter++) {
        if ((*iter)->m_nodeType == NodeType::Word)
        {
            const StoryNode* node = *iter;
            StoryWord* word = new StoryWord(static_cast<const StoryWord&>(*node));
            m_nodes.push_back(word);
        }
        else if ((*iter)->m_nodeType == NodeType::Jump)
        {
            const StoryNode* node = *iter;
            StoryJump* jump = new StoryJump(static_cast<const StoryJump&>(*node));
            m_nodes.push_back(jump);

        }
        else if ((*iter)->m_nodeType == NodeType::Label)
        {
            const StoryNode* node = *iter;
            StoryLabel* label = new StoryLabel(static_cast<const StoryLabel&>(*node));
            m_nodes.push_back(label);

        }
    }
}

StoryJson::StoryJson(StoryJson&& storyJson) noexcept
{
    if (this == &storyJson)
    {
        return;
    }
    memset(m_fullPath, '\0', sizeof(m_fullPath));
    strcpy(m_fullPath, storyJson.m_fullPath);

    memset(m_chapter, '\0', sizeof(m_chapter));
    strcpy(m_chapter, storyJson.m_chapter);

    memset(m_scene, '\0', sizeof(m_scene));
    strcpy(m_scene, storyJson.m_scene);

    m_nodes = storyJson.m_nodes;

    for (auto iter = storyJson.m_nodes.begin(); iter != storyJson.m_nodes.end(); iter++) {
        *iter = nullptr;
    }


}

StoryJson::~StoryJson()
{
    Clear();
}



StoryJson& StoryJson::operator=(StoryJson&& storyJson) noexcept
{
    if (this == &storyJson) {
        return *this;
    }
    Clear();
    memset(m_fullPath, '\0', sizeof(m_fullPath));
    strcpy(m_fullPath, storyJson.m_fullPath);

    memset(m_chapter, '\0', sizeof(m_chapter));
    strcpy(m_chapter, storyJson.m_chapter);

    memset(m_scene, '\0', sizeof(m_scene));
    strcpy(m_scene, storyJson.m_scene);
    m_nodes = storyJson.m_nodes;

    for (auto iter = storyJson.m_nodes.begin(); iter != storyJson.m_nodes.end(); iter++) {
        *iter = nullptr;
    }

    return *this;
}



void StoryJson::Clear()
{

    for (auto iter = m_nodes.begin(); iter != m_nodes.end(); iter++)
    {
        if (*iter == nullptr)
        {
            continue;
        }
        delete *iter;
        *iter = nullptr;
    }

    m_nodes.clear();

    memset(m_fullPath, '\0', sizeof(m_fullPath));
    memset(m_chapter, '\0', sizeof(m_chapter));
    memset(m_scene, '\0', sizeof(m_scene));
}
bool StoryJson::SetFileName(const char* const fileName) {

    size_t lengthOfFolderPath = strlen(PATH_FROM_PROJECT_ROOT_TO_STORY_FOLDER);

    int pos = CharacterUtility::Find(m_fullPath, strlen(m_fullPath), PATH_FROM_PROJECT_ROOT_TO_STORY_FOLDER, strlen(PATH_FROM_PROJECT_ROOT_TO_STORY_FOLDER));

    if (pos == -1) { return false; }
    strcpy(m_fullPath + pos + lengthOfFolderPath + 1, fileName);

    return true;
}

bool StoryJson::GetFileName(char* const outFileName) const {
    memset(outFileName, '\0', sizeof(outFileName));
    size_t lengthOfFolderPath = strlen(PATH_FROM_PROJECT_ROOT_TO_STORY_FOLDER);
    int pos = CharacterUtility::Find(m_fullPath, strlen(m_fullPath), PATH_FROM_PROJECT_ROOT_TO_STORY_FOLDER, lengthOfFolderPath);

    if (pos == -1) { return false; }
    strcpy(outFileName, m_fullPath + pos + strlen(DELIMITER) + lengthOfFolderPath);

    return true;
}

void StoryJson::SetFullPath(const char* fullPath) {
    strcpy(m_fullPath, fullPath);
}
const char* StoryJson::GetFullPath() const
{
    return m_fullPath;
}

bool StoryJson::IsExistFullPath()const {
    if (strlen(m_fullPath) == 0)
    {
        return false;
    }
    char filePath[MAX_FILE_NAME];
    char defaultName[MAX_FILE_NAME];

    GetFileName(filePath);
    strcpy(defaultName, DEFAULT_FILE_NAME);
    strcpy(defaultName, ".json");
    if (strcmp(filePath, defaultName) == 0)
    {
        return false;
    }
    return true;
}




//=========================DATA STRUCTURE============================




void to_json(json & j, const StoryJson & story)
{
    j[StructString[(int)StructLayout::Info]] = json::array();
    j[StructString[(int)StructLayout::Info]] = json{
        {infoString[(int)InfoLayout::Chapter],       story.GetChapter()},
        {infoString[(int)InfoLayout::Scene],         story.GetScene()} };


    j[StructString[(int)StructLayout::Value]] = json::array();
    ToJsonFactory(j[StructString[(int)StructLayout::Value]], story);
}


/* String version, Archived */

//void from_json(const json & j, StoryWord & p)
//{
//    j.at("name").get_to(p.m_name);
//    j.at("content").get_to(p.m_content);
//}

//=========================Exception===========================




void from_json(const json & j, StoryJson & p)
{
    json tmpJson;

    GetJsonException(tmpJson, j, StructString[(int)StructLayout::Info]);
    if (!tmpJson.empty())
    {
        GetContentException(p.GetChapter(), tmpJson, infoString[(int)InfoLayout::Chapter]);
        GetContentException(p.GetScene(), tmpJson, infoString[(int)InfoLayout::Scene]);
    }

    GetJsonException(tmpJson, j, StructString[(int)StructLayout::Value]);
    if (!tmpJson.empty())
    {
        FromJsonFactory(tmpJson, p);
    }

    //Adapt for old struct
    if (j.is_array())
    {
        FromJsonFactory(j, p);
    }


    // iterate the array
    /* for (json::iterator it = j.begin(); it != j.end(); ++it) {
     StoryWord* word = new StoryWord();
     *word = *it;
     p.AddWord(word);
     }*/
}


void ToJsonFactory(json& j, const StoryJson& story)
{
    for (int i = 0; i < story.Size(); i++)
    {
        const StoryNode* const pNode = story.GetNode(i);

        if (pNode->m_nodeType == NodeType::Word)
        {
            const StoryWord*const pWord = static_cast<const StoryWord*const>(pNode);
            j.push_back(*pWord);
        }

        if (pNode->m_nodeType == NodeType::Label)
        {
            const StoryLabel*const pLabel = static_cast<const StoryLabel*const>(pNode);
            j.push_back(*pLabel);
        }

        if (pNode->m_nodeType == NodeType::Jump)
        {
            const StoryJump*const pJump = static_cast<const StoryJump*const>(pNode);
            j.push_back(*pJump);
        }

        if (pNode->m_nodeType == NodeType::Exhibit)
        {
            const StoryExhibit*const pExhibit = static_cast<const StoryExhibit*const>(pNode);
            j.push_back(*pExhibit);
        }
    }
}

void FromJsonFactory(const json& j, StoryJson & storyJson)
{
    for (int i = 0; i < j.size(); i++)
    {
        char enumString[MAX_ENUM_LENGTH];
        //strcpy(enumString, j[i].at(wordNodeString[(int)WordLayout::NodeTypeName]).get_ptr<const json::string_t *>()->c_str());
        GetContentException(enumString, j[i], wordNodeString[0]);

        if (strcmp(enumString, nodeTypeString[(int)NodeType::Word]) == 0) {
            StoryWord* node = new StoryWord;
            *node = j[i];
            storyJson.AddNode(node);
        }

        if (strcmp(enumString, nodeTypeString[(int)NodeType::Label]) == 0) {
            StoryLabel* node = new StoryLabel;
            *node = j[i];
            storyJson.AddNode(node);
        }

        if (strcmp(enumString, nodeTypeString[(int)NodeType::Jump]) == 0) {
            StoryJump* node = new StoryJump;
            *node = j[i];
            storyJson.AddNode(node);
        }

        if(strcmp(enumString, nodeTypeString[(int)NodeType::Exhibit]) == 0){
            StoryExhibit* node = new StoryExhibit;
            *node = j[i];
            storyJson.AddNode(node);
        }
    }
}


void IdOperator::ParseStringId(const char* const pinStringId, char(*pOutIdArray)[MAX_ID_COUNT])
{
    int strLength = strlen(pinStringId);

    char tmpPartId[MAX_ID];
    memset(tmpPartId, '\0', MAX_ID);

    int j = 0;
    int indexOfArray = 0;

    for (int i = 0; i < strLength; i++)
    {
        if (pinStringId[i] == '_')
        {

            strcpy(pOutIdArray[indexOfArray++], tmpPartId);

            j = 0;
            memset(tmpPartId, '\0', MAX_ID);
        }
        else {
            tmpPartId[j++] = pinStringId[i];
        }
    }
    if (indexOfArray > 3)
    {
        return;
    }
    strcpy(pOutIdArray[indexOfArray++], tmpPartId);
}

void IdOperator::CombineStringId(char* const pOutStringId, char(*pInIdArray)[MAX_ID_COUNT])
{

    memset(pOutStringId, '\0', MAX_ID);
    for (int i = 0; i < NUM_OF_ID_PART - 1; i++)
    {
        strcat(pOutStringId, pInIdArray[i]);
        strcat(pOutStringId, "_");
    }
    strcat(pOutStringId, pInIdArray[NUM_OF_ID_PART - 1]);

}

}
