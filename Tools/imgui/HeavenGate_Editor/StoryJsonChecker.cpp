#include "StoryJsonChecker.h"

#include "StoryJson.h"
#include "StoryJsonStoryNode.h"
#include "StoryJsonLabelNode.h"
#include "StoryJsonJumpNode.h"

#include <vector>

namespace HeavenGateEditor {

    using std::vector;



    bool StoryJsonChecker::CheckJsonStory(const StoryJson* const story) const
    {
        bool result = true;
        result = CheckLabelAndJumpPosition(story);
        if (!result)
        {
            printf("Label and Jump position is illegal");
            return false;
        }

        return true;

    }

    bool StoryJsonChecker::CheckLabelAndJumpPosition(const StoryJson* const story) const
    {
        vector<const StoryLabel* > readedList;
        for (int i = 0 ; i < story->Size(); i++)
        {
            const StoryNode* const node  = story->GetNode(i);
            if (node->m_nodeType == NodeType::Label)
            {
                const StoryLabel* const label = static_cast<const StoryLabel*const>(node);
                readedList.push_back(label);

            }
            else if(node->m_nodeType == NodeType::Jump)
            {
                const StoryJump*const jump = static_cast<const StoryJump*const>(node);
                if (!readedList.empty())
                {
                    for (auto iter = readedList.cbegin(); iter != readedList.cend(); iter++)
                    {
                        if (strcmp((*iter)->m_labelId, jump->m_jumpId) == 0)
                        {
                            return false;
                        }
                    }
                }
            }
            else
            {
                readedList.clear();
            }
        }

        return true;
    }


}
