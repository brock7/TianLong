/********************************************************************
filename:   FairyEffectManager.h

purpose:    effect manager manage all effects, skills and path files.
*********************************************************************/

#ifndef __EffectManager_H__
#define __EffectManager_H__

// ogre header
#include <OgreSingleton.h>
#include <OgreScriptLoader.h>
#include <OgreStringVector.h>
#include <OgreDataStream.h>
#include "OgreIteratorWrappers.h"

// fairy header
#include "Core/WXPrerequisites.h"

// stl header
#include <map>

// ǰ������
namespace Ogre	{
	class SceneNode;
}

namespace WX	{
	class Effect;
	class EffectElementFactory;
	class EffectElement;
	class System;
	class EffectBasic;
	class Skill;
	class AnimationEffectInfo;
	class AnimationRibbon;
	class AnimationSound;
    class AnimationSceneLightInfo;
    class MainSceneLight;
}
namespace WX	{	

    /// ��Ч����Ĭ��ΪHIGH
    enum EffectLevel
    {
        EL_LOW,
        EL_MID,
        EL_HIGH,
    };

	class EffectManager : public Ogre::Singleton<EffectManager>, Ogre::ScriptLoader
	{
	public:

		typedef std::map<String, Effect *> EffectTemplateMap;
		//typedef std::map<String, Effect *> EffectMap;
		typedef std::list<Effect *> ActiveEffectList;

		typedef std::list<Effect *> FreeEffectList;
		typedef std::map<String, FreeEffectList *> FreeEffectMap;

		typedef std::map<String, EffectElementFactory *> ElementFactoryMap;

		typedef std::map<String, String> EffectTemplateScriptFileMap;

		typedef std::vector<String> LoadedScripts;

		typedef std::map<String, Skill *> SkillMap;

	public:

		EffectManager( System *system );
		~EffectManager();

		/// @copydoc ScriptLoader::getScriptPatterns
		const Ogre::StringVector& getScriptPatterns(void) const;
        /// @copydoc ScriptLoader::parseScript
		void parseScript(Ogre::DataStreamPtr& stream, const String& groupName);
        /// @copydoc ScriptLoader::getLoadingOrder
        Real getLoadingOrder(void) const;
		
        static EffectManager& getSingleton(void);       
        static EffectManager* getSingletonPtr(void);
		
		/// debug
		void dump(const String &fileName);

		/// ������Чģ��������һ���µ���Ч
		Effect *createEffect( const String &templateName );

		Skill *getSkill( const String &skillName );

		/// ������ЧԪ�ص�����������һ����ЧԪ��
		EffectElement * createElement(const String &type);

		/// ������ɾ��һ����Ч
		ActiveEffectList::iterator destroyEffect( Effect *effect );

		/** ��ʱ��ɾ��һ����Ч
			������������ɾ����Ч�����ǰ����ӻ����Ч������ɾ��������
			���е���Ч�������Ա�����
		@param removeParentNode �Ƿ�Ҫ�Ѹ�effect�Ľڵ�����ĸ��ڵ���ж������
			�������effect�Ľڵ���õ���createSceneNode���������Ϊfalse��
			�������effect�Ľڵ���õ���createSceneNode( Ogre::SceneNode *parentNode )���������Ϊtrue
        @param removeNow �Ƿ����Ͼ�ɾ�����effect�����Ϊfalse����ô���effect��������ȫ�������
                         ��ɾ��

        @remarks ���removeNowΪfalse����������ɾ��effect����ʱҪ��֤effect��SceneNode��������effectɾ����
                 ���������ҪremoveNowΪfalse������effect��scnenode���ʹ��createSceneNode()��������scenenode����
                 ��effect�ڲ������Ա�֤scenenode��ɾ��ʱ��
		*/
		void removeEffect( Effect *effect, bool removeParentNode = true, bool removeNow = true );

		/**	����һ��������������ÿ֡����effect	*/
		void createController(void);

		/// �ɿ��������ã�ÿ֡���»����Ч
		void updateActiveEffects(Real time);

		size_t getNumEffectTemplates(void);

		typedef Ogre::MapIterator<EffectTemplateMap> EffectTemplateIterator;
		EffectTemplateIterator getEffectTemplateIterator(void);

		typedef Ogre::MapIterator<ElementFactoryMap> ElementFactoryIterator;
		ElementFactoryIterator getElementFactoryIterator(void);

		typedef Ogre::MapIterator<SkillMap> SkillIterator;
		SkillIterator getSkillIterator(void);

		const String & getFileNameByTemplateName( const String &templateName );

		void getTemplatesFromScriptFile( const String &fileName, std::vector<String> &templates );

		void getLoadedScripts( std::vector<String> &files )	{	files = mLoadedScripts;	}

		/// ����ģ����������ȡģ��
		Effect * getTemplate(const String& name);

		Skill * createSkillTemplate(const String &name);

		/** ����һ���µ�effectģ��
		@param name ��Ч����
		*/
		Effect * createEffectTemplate(const String &name);

		void addToEffectTemplateScriptFileMap( const String &templateName, const String &fileName );

		Skill * createSkill(const String &skillName);
		void removeSkill(Skill *skill);

		/** ����ÿ�����е�effect�������ɱ���Ŀ���effect����
		@remarks effect��removeEffectʱ�ᱻ���գ�����һ��map�У�map��key��effect��ģ�����ƣ�
				 value��һ�����е�effect��������������õľ�������������������Ҳ����˵����
				 ����effect���У�ͬ��ģ�������Ա���mMaxNumFreeEffectPerList�����е�effect
		*/
		void setMaxNumFreeEffectPerList(unsigned short num)
		{
			mMaxNumFreeEffectPerList = num;
		}
		unsigned short getMaxNumFreeEffectPerList(void)
		{
			return mMaxNumFreeEffectPerList;
		}

        MainSceneLight* getMainSceneLight(void)
        {
            return mMainSceneLight;
        }

        /// �����Ч���е���Ч
        void clearAllFreeEffects(void);

        /// ������Ч����
        void setEffectLevel(EffectLevel level);
        EffectLevel getEffectLevel(void)
        {
            return mEffectLevel;
        }

        void _destroyEffectTemplates(void);

        void _destroySkillTemplates(void);

	protected:

		/** ע���µ���ЧԪ�ع���
			ÿ����һ�������͵���ЧԪ�أ���������������������Ӧ�Ĵ���
		*/
		void registerElementFactory(void);

		void addElementFactory( EffectElementFactory *factory );

		void skipToNextCloseBrace(Ogre::DataStreamPtr& chunk);
        void skipToNextOpenBrace(Ogre::DataStreamPtr& chunk);

		/// �����ű��е��µ���ЧԪ��
		void parseNewElement(const String &type, Ogre::DataStreamPtr &stream, Effect *effectParent);

		/// ������ЧԪ�ص����� 
		void parseElementAttrib(const String& line, EffectElement *element);

		/// ����.effect�ļ�
		void parseEffectFile( Ogre::DataStreamPtr &stream );
		void parseEffectAttrib(const String& line, Effect *effect);

		/// ����.skill�ļ�
		void parseSkillFile( Ogre::DataStreamPtr &stream );
		void parseSkillAttrib(const String& line, Skill *skill);
        void parseAnimEffectInfo(Ogre::DataStreamPtr &stream, Skill *skill);	
		void parseAnimEffectInfoAttrib(const String& line, AnimationEffectInfo *effectInfo);
		void parseAnimRibbon(Ogre::DataStreamPtr &stream, Skill *skill);	
		void parseAnimRibbonAttrib(const String& line, AnimationRibbon *ribbon);
		void parseAnimSound(Ogre::DataStreamPtr &stream, Skill *skill);	
		void parseAnimSoundAttrib(const String& line, AnimationSound *sound);
        void parseAnimSceneLightInfo(Ogre::DataStreamPtr &stream, Skill *skill);	
        void parseAnimSceneLightInfoAttrib(const String& line, AnimationSceneLightInfo *sceneLightInfo);

		/** �ӿ��е���Ч�����л�ȡһ����ָ��ģ�����Ч
			һ����˵������Ϸ����ʱ���Ҫɾ��һ����Ч������˵һ���ҽ�
			��Ч�Ķ�������ˣ���������ֱ��ɾ����Ч�ģ����ǰ�������һЩ�����
			�����³�ʼ�������뵽һ�����е������У��´����Ҫ�󴴽�ͬ�����͵���Ч��
			���ȴӿ����������ң����û�У������´���һ��ȫ�µ�
		*/
		Effect * getFromFreeMap( const String &templateName );

		/// ��һ����Ч���뵽����������
		void addToFreeEffectMap( Effect *effect );	

		void addToLoadedScripts( const String &fileName )	{	mLoadedScripts.push_back(fileName);	}

        /// ���������Ϣ
        void _logErrorInfo(const String& errorInfo, const String& lineContent, 
            const String& functionName);
		
	private:

		ElementFactoryMap mElementFactoryMap;
		EffectTemplateMap mEffectTemplateMap;
		FreeEffectMap mFreeEffectMap;
		SkillMap mSkillMap;

		EffectTemplateScriptFileMap mEffectTemplateScriptFileMap;

		LoadedScripts mLoadedScripts;

		ActiveEffectList mActiveEffectList;

		Ogre::StringVector mScriptPatterns;

		System *mSystem;

		Ogre::Controller<Real> *mController;

		unsigned short mMaxNumFreeEffectPerList;

        /// ÿ�ν���getLine��ʱ��͵����кţ���Ϊ����������Ϣ
        int mWrongLineNum;

        /// ��ǰ�������ļ�����
        String mParsingFileName;

        /// ��ǰ�����ϵĵƹ�仯��Ϣ
        MainSceneLight* mMainSceneLight;

        /// ��Ч����
        EffectLevel mEffectLevel;
	};
}

#endif