#pragma once
#include <irrlicht.h>
#include "LightDistanceElement.h"

using namespace irr;
using namespace core;

class CMyLightManager : public scene::ILightManager, public IEventReceiver
{
    typedef enum
    {
        NO_MANAGEMENT,
        LIGHTS_NEAREST_NODE,
        LIGHTS_IN_ZONE
    }
    LightManagementMode;

    LightManagementMode Mode;
    LightManagementMode RequestedMode;

    // These data represent the state information that this light manager
    // is interested in.
    scene::ISceneManager * SceneManager;
    core::array<scene::ISceneNode*> * SceneLightList;
    scene::E_SCENE_NODE_RENDER_PASS CurrentRenderPass;
    scene::ISceneNode * CurrentSceneNode;

public:
    CMyLightManager(scene::ISceneManager* sceneManager)
        : Mode(NO_MANAGEMENT), RequestedMode(NO_MANAGEMENT),
        SceneManager(sceneManager), SceneLightList(0),
        CurrentRenderPass(scene::ESNRP_NONE), CurrentSceneNode(0)
    { }

    // The input receiver interface, which just switches light management strategy
    bool OnEvent(const SEvent & event)
    {
        bool handled = false;

        if (event.EventType == irr::EET_KEY_INPUT_EVENT && event.KeyInput.PressedDown)
        {
            handled = true;
            switch(event.KeyInput.Key)
            {
            case irr::KEY_KEY_1:
                RequestedMode = NO_MANAGEMENT;
                break;
            case irr::KEY_KEY_2:
                RequestedMode = LIGHTS_NEAREST_NODE;
                break;
            case irr::KEY_KEY_3:
                RequestedMode = LIGHTS_IN_ZONE;
                break;
            default:
                handled = false;
                break;
            }

            if(NO_MANAGEMENT == RequestedMode)
                SceneManager->setLightManager(0); // Show that it's safe to register the light manager
            else
                SceneManager->setLightManager(this);
        }

        return handled;
    }


    // This is called before the first scene node is rendered.
    virtual void OnPreRender(core::array<scene::ISceneNode*> & lightList)
    {
        // Update the mode; changing it here ensures that it's consistent throughout a render
        Mode = RequestedMode;

        // Store the light list. I am free to alter this list until the end of OnPostRender().
        SceneLightList = &lightList;
    }

    // Called after the last scene node is rendered.
    virtual void OnPostRender()
    {
        // Since light management might be switched off in the event handler, we'll turn all
        // lights on to ensure that they are in a consistent state. You wouldn't normally have
        // to do this when using a light manager, since you'd continue to do light management
        // yourself.
        for (u32 i = 0; i < SceneLightList->size(); i++)
            (*SceneLightList)[i]->setVisible(true);
    }

    virtual void OnRenderPassPreRender(scene::E_SCENE_NODE_RENDER_PASS renderPass)
    {
        // I don't have to do anything here except remember which render pass I am in.
        CurrentRenderPass = renderPass;
    }

    virtual void OnRenderPassPostRender(scene::E_SCENE_NODE_RENDER_PASS renderPass)
    {
        // I only want solid nodes to be lit, so after the solid pass, turn all lights off.
        if (scene::ESNRP_SOLID == renderPass)
        {
            for (u32 i = 0; i < SceneLightList->size(); ++i)
                (*SceneLightList)[i]->setVisible(false);
        }
    }

    // This is called before the specified scene node is rendered
    virtual void OnNodePreRender(scene::ISceneNode* node)
    {
        CurrentSceneNode = node;

        // This light manager only considers solid objects, but you are free to manipulate
        // lights during any phase, depending on your requirements.
        if (scene::ESNRP_SOLID != CurrentRenderPass)
            return;

        // And in fact for this example, I only want to consider lighting for cube scene
        // nodes.  You will probably want to deal with lighting for (at least) mesh /
        // animated mesh scene nodes as well.
        if (node->getType() != scene::ESNT_CUBE)
            return;

        if (LIGHTS_NEAREST_NODE == Mode)
        {
            // This is a naive implementation that prioritises every light in the scene
            // by its proximity to the node being rendered.  This produces some flickering
            // when lights orbit closer to a cube than its 'zone' lights.
            const vector3df nodePosition = node->getAbsolutePosition();

            // Sort the light list by prioritising them based on their distance from the node
            // that's about to be rendered.
            array<LightDistanceElement> sortingArray;
            sortingArray.reallocate(SceneLightList->size());

            u32 i;
            for(i = 0; i < SceneLightList->size(); ++i)
            {
                scene::ISceneNode* lightNode = (*SceneLightList)[i];
                const f64 distance = lightNode->getAbsolutePosition().getDistanceFromSQ(nodePosition);
                sortingArray.push_back(LightDistanceElement(lightNode, distance));
            }

            sortingArray.sort();

            // The list is now sorted by proximity to the node.
            // Turn on the three nearest lights, and turn the others off.
            for(i = 0; i < sortingArray.size(); ++i)
                sortingArray[i].node->setVisible(i < 3);
        }
        else if(LIGHTS_IN_ZONE == Mode)
        {
            // Empty scene nodes are used to represent 'zones'.  For each solid mesh that
            // is being rendered, turn off all lights, then find its 'zone' parent, and turn
            // on all lights that are found under that node in the scene graph.
            // This is a general purpose algorithm that doesn't use any special
            // knowledge of how this particular scene graph is organised.
            for (u32 i = 0; i < SceneLightList->size(); ++i)
            {
                if ((*SceneLightList)[i]->getType() != scene::ESNT_LIGHT)
                    continue;
                scene::ILightSceneNode* lightNode = static_cast<scene::ILightSceneNode*>((*SceneLightList)[i]);
                video::SLight & lightData = lightNode->getLightData();

                if (video::ELT_DIRECTIONAL != lightData.Type)
                    lightNode->setVisible(false);
            }

            scene::ISceneNode * parentZone = findZone(node);
            if (parentZone)
                turnOnZoneLights(parentZone);
        }
    }

    // Called after the specified scene node is rendered
    virtual void OnNodePostRender(scene::ISceneNode* node)
    {
        // I don't need to do any light management after individual node rendering.
    }

private:

    // Find the empty scene node that is the parent of the specified node
    scene::ISceneNode * findZone(scene::ISceneNode * node)
    {
        if (!node)
            return 0;

        if (node->getType() == scene::ESNT_EMPTY)
            return node;

        return findZone(node->getParent());
    }

    // Turn on all lights that are children (directly or indirectly) of the
    // specified scene node.
    void turnOnZoneLights(scene::ISceneNode * node)
    {
        core::list<scene::ISceneNode*> const & children = node->getChildren();
        for (core::list<scene::ISceneNode*>::ConstIterator child = children.begin();
            child != children.end(); ++child)
        {
            if ((*child)->getType() == scene::ESNT_LIGHT)
                (*child)->setVisible(true);
            else // Assume that lights don't have any children that are also lights
                turnOnZoneLights(*child);
        }
    }

};