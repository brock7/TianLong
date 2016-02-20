/********************************************************************
filename:   WXLogicModelObject.cpp

purpose:    ��LogicModelObject��װ��WXObjectϵ����
            ��actor object���ƣ����Ƕ�LogicModel��ԭDObject���ķ�װ��
            ��ͬ�����ڣ�actor object�Ǵ�placement object��������Ҫ
            ���ڳ�������İڷţ���LogicModelObjectֱ�Ӵ�object������
            �Լ�����λ�ã��������Ϣ����Ҫ���ڿͻ��˴������ǣ�npc��
            ��������塣
*********************************************************************/

#include "WXLogicModelObject.h"
#include "WXObjectFactory.h"
#include "WXObjectProxy.h"

#include "ModelSystem/WXLogicModelManager.h"
#include "ModelSystem/WXLogicModel.h"

namespace WX {

    const String LogicModelObject::msType = "LogicModel";
    const String LogicModelObject::msCategory = "LogicModel";

    //////////////////////////////////////////////////////////////////////////
    // Factory
    //////////////////////////////////////////////////////////////////////////

    class LogicModelObject::Factory : public ObjectFactory
    {
    public:
        const String& getType(void) const
        {
            return msType;
        }

        Object* createInstance(void)
        {
            return new LogicModelObject;
        }
    };

    //////////////////////////////////////////////////////////////////////////
    // Commands
    //////////////////////////////////////////////////////////////////////////
    namespace
    {
        struct LogicModelNameCmd : PropertyCommand
        {
            Variant doGet(const void* source) const
            {
                const LogicModelObject* object = static_cast<const LogicModelObject*>(source);
                return object->getLogicModelName();
            }

            String doGetAsString(const void* source) const
            {
                const LogicModelObject* object = static_cast<const LogicModelObject*>(source);
                return object->getLogicModelName();
            }

            void doSet(void* target, const Variant& value)
            {
                LogicModelObject* object = static_cast<LogicModelObject*>(target);
                String actorName = VariantCast<String>(value);
                object->setLogicModelName(actorName);
            }

            void doSetAsString(void* target, const String& value)
            {
                LogicModelObject* object = static_cast<LogicModelObject*>(target);
                object->setLogicModelName(value);
            }
        };

    }

    //////////////////////////////////////////////////////////////////////////
    LogicModelObject::LogicModelObject() :
    mLogicModel(NULL),
    mLogicModelName(""),
    mProxy(NULL)
    {
        if (createPropertyDictionary("LogicModel"))
        {
            static LogicModelNameCmd logicModelNameCmd;
            defineProperty(
                "logic model name",
                "The logic model filename use to create this logic model.",
                "LogicModelName",
                &logicModelNameCmd,
                PF_PRIMARY);
        }
    }

    LogicModelObject::~LogicModelObject()
    {
        destroyRenderInstance();
    }

    //-----------------------------------------------------------------------
    const String& LogicModelObject::getType(void) const
    {
        return msType;
    }
    //-----------------------------------------------------------------------
    const String& LogicModelObject::getCategory(void) const
    {
        return msCategory;
    }
    //-----------------------------------------------------------------------
    void LogicModelObject::createRenderInstance(System* system)
    {
        assert(system);
        assert(!mLogicModel);
        assert(!mProxy);

        mProxy = new ObjectProxy(shared_from_this());

        if (!mLogicModelName.empty())
        {
            _createLogicModel();
        }
    }
    //-----------------------------------------------------------------------
    void LogicModelObject::destroyRenderInstance(void)
    {
        if (mLogicModel)
        {
            _destroyLogicModel();
        }

        if (mProxy)
        {
            delete mProxy;
            mProxy = NULL;
        }
    }
    //-----------------------------------------------------------------------
    void LogicModelObject::setLogicModelName(const String& modelName)
    {
        if (mLogicModelName != modelName)
        {
            if (mLogicModel)
            {
                _destroyLogicModel();
            }

            mLogicModelName = modelName;

            if (mProxy && !mLogicModelName.empty())
            {
                _createLogicModel();
            }
        }
    }
    //-----------------------------------------------------------------------
    ObjectFactory* LogicModelObject::getFactory(void)
    {
        static Factory factory;
        return &factory;
    }
    //-----------------------------------------------------------------------
    void LogicModelObject::_createLogicModel(void)
    {
        assert(!mLogicModel);
        assert(mProxy && !mLogicModelName.empty());

        mLogicModel = LogicModelManager::getSingleton().createLogicModel(mLogicModelName);
        mLogicModel->setUserObject(mProxy);
    }
    //-----------------------------------------------------------------------
    void LogicModelObject::_destroyLogicModel(void)
    {
        assert (mLogicModel);

        mLogicModel->setUserObject(NULL);
        LogicModelManager::getSingleton().destroyLogicModel(mLogicModel);
        mLogicModel = NULL;
    }
}