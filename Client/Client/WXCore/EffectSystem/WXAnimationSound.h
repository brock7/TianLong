/********************************************************************
filename:   WXAnimationSound.h

purpose:    the ribbon that attach to the bone or a locator.
*********************************************************************/

#ifndef _ANIMATIONSOUND_H_
#define _ANIMATIONSOUND_H_

#include "Core/WXPrerequisites.h"

#include <OgreStringInterface.h>

namespace WX	{

	class AnimationSound : public Ogre::StringInterface
	{
	public:
		class CmdAttachTime : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		class CmdSoundName : public Ogre::ParamCommand
		{
		public:
			String doGet(const void* target) const;
			void doSet(void* target, const String& val);
#if OGRE_STRING_INTERFACE_COPY
            virtual void doCopy(void* target, const void* source);
#endif	
        };

		AnimationSound(void);

		void setAttachTime(float time)
		{
			mAttachTime = time;
		}
		float getAttachTime(void) const
		{
			return mAttachTime;
		}

		void setSoundName( const Ogre::String &name )
		{
			mSoundName = name;
		}
		const Ogre::String& getSoundName(void) const
		{
			return mSoundName;
		}

        /** ����������Ч��������
        @remarks ����ԭ����Ogre::StringInterface::copyParametersTo�������
        �����в����Ŀ����ٶ�̫������Ϊ������һЩ�ַ����Ĳ�����������
        ��һ���������������еĲ���������ÿ��һ����������Ҫ���������
        ����������
        */
        void copyParameters(AnimationSound& newInfo) const;

	public:

		/// the handle of sound, will set by play sound function.
		int mSoundHandle;
		bool mPlayed;

	protected:

		float mAttachTime;
		String mSoundName;


	//////////////////////////////////////////////////////////////////////////
		static CmdAttachTime msAttachTimeCmd;
		static CmdSoundName msSoundNameCmd;
	};
}

#endif