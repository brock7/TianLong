#ifndef __ADDEFFECTOBJECTPLUGIN_H__
#define __ADDEFFECTOBJECTPLUGIN_H__

#include "WXSceneListener.h"

namespace WX {

    class SceneManipulator;
   
	/** ��������Ч�����ָʾ��
	*/
    class AddEffectObjectPlugin : public SceneListener
    {
    public:

        AddEffectObjectPlugin(WX::SceneManipulator* sceneManipulator);

        virtual ~AddEffectObjectPlugin();

        //////////////////////////////////////////////////////////////////////////
        virtual void onSceneReset(void);
        virtual void onAddObject(const ObjectPtr& object);
        virtual void onRemoveObject(const ObjectPtr& object);
        virtual void onSelectObject(const ObjectPtr& object);
        virtual void onDeselectObject(const ObjectPtr& object);
        virtual void onDeselectAllObjects(void);
        virtual void onObjectPropertyChanged(const ObjectPtr& object, const String& name);

    protected:

        class Indicator;

        typedef std::map<ObjectPtr, Indicator *> Indicators;
        Indicators mIndicators;

        WX::SceneManipulator* mSceneManipulator;
    };
}

#endif