/********************************************************************
filename:   HardwareInfoSystem.h

purpose:    ���ڼ��ϵͳ��Ӳ����Ϣ����ȡӲ����Ϣ���ó���ǰϵͳ����
            �����ú��Ƽ����á�
            ��������ļ��ᱻ������ͬ�ĳ���ʹ��(Render.dll��tconfig.exe)��
            �ڴ�������һЩ��ͬ�ĵط�����tconfig�Ĺ����лᶨ��TCONFIG.
*********************************************************************/

#ifndef _HARDWAREINFOSYSTEM_H_
#define _HARDWAREINFOSYSTEM_H_

#include <map>
#include <string>

#include <D3D9.h>

#ifndef TCONFIG
    class tDataBase;
#else
    namespace DBC
    {
        class DBCFile;
    }
#endif

/// �����Կ���Ϣ���б������Ϣ
struct VideoCardSetupInfo
{
    VideoCardSetupInfo() :
    mId(0),
    mDeviceId(NULL),
    mDesc(NULL),
    mTerrainLightmap(0),
    mAntiAlias(0),
    mRealtimeShadow(0),
    mTextureQuality(0),
    mTextureFilter(0),
    mPostFilter(0),
    mVSync(0)
    {
    }

    int mId;
    const char* mDeviceId;
    const char* mDesc;
    int mTerrainLightmap;
    int mAntiAlias;
    int mRealtimeShadow;
    int mTextureQuality;
    int mTextureFilter;
    int mPostFilter;
    int mVSync;    
};

/// ���������ڴ���Ϣ���б������Ϣ
struct PhyMemorySetupInfo
{
    PhyMemorySetupInfo() :
    mId(0),
    mPhyMemoryRange(NULL),
    mTerrainLightMap(0)
    {
    }

    int mId;
    const char* mPhyMemoryRange;
    int mTerrainLightMap;
};

/// ����CPU��Ϣ���б������Ϣ
struct  CpuSetupInfo
{
    CpuSetupInfo() :
    mId(0),
    mCpuRange(NULL),
    mRealtimeShadow(0),
    mWeather(0),
    mWalkEffect(0)
    {
    }

    int mId;
    const char* mCpuRange;
    int mRealtimeShadow;
    int mWeather;
    int mWalkEffect;
};

/// ��ǰϵͳ��Ӳ��������Ϣ
struct HardwareInfo
{
    HardwareInfo() :
    mCpuSpeed(0),
    mPhyMemory(0.0f),
    mFreePhyMemory(0.0f),
    mVideoCardDeviceId(0xFFFF),
    mLocalVideoMemory(0.0f),
    mFreeLocalVideoMemory(0.0f),
    mNonLocalVideoMemory(0.0f),
    mFreeNonLocalVideoMemory(0.0f),
    mDXVersion(NULL),
    mVideoCardVendorId(0xFFFF),
    mVideoDriverVersion(0)
    {        
    }

    ~HardwareInfo()
    {
        if (mDXVersion)
        {
            delete [] mDXVersion;
            mDXVersion = NULL;
        }
    }

    int mCpuSpeed;                                  /// cpuƵ�ʣ���λ��MHz;
    float mPhyMemory;                               /// �����ڴ棬��λ��Mb
    float mFreePhyMemory;                           /// �����ڴ棬��λ��Mb
    DWORD mVideoCardDeviceId;                       /// �Կ�id������ʶ������ͺŵ��Կ�
    float mLocalVideoMemory;                        /// �Կ��Դ��С����λ��Mb
    float mFreeLocalVideoMemory;                    /// �����Կ��Դ��С����λ��Mb
    float mNonLocalVideoMemory;                     /// AGP�Դ��С����λ��Mb
    float mFreeNonLocalVideoMemory;                 /// ����AGP�Դ��С����λ��Mb
    char* mDXVersion;                               /// dx�汾��
    DWORD mVideoCardVendorId;                       /// �Կ�����Id
    DWORD mVideoDriverVersion;                      /// ��������汾��
};

/// ��Ϸ����ʾ�����е���
struct VideoSetupInfo
{
    void init(void)
    {
        mTerrainLightMap = 0;
        mAntiAlias = 0;
        mRealtimeShadow = 0;
        mTextureQuality = 0;
        mTextureFilter = 0;
        mPostFilter = 0;
        mVSync = 0;
        mWeather = 0;
        mWalkEffect = 0;
    }

    VideoSetupInfo()
    {
        init();
    }

    int mTerrainLightMap;
    int mAntiAlias;
    int mRealtimeShadow;
    int mTextureQuality;
    int mTextureFilter;
    int mPostFilter;
    int mVSync;
    int mWeather;
    int mWalkEffect;
};

/// ������ʾ�����е����ֵ
struct SingleOption
{
    int mMinValue;  /// �Ƽ�ֵ
    int mMaxValue;  /// ϵͳ��������ֵ
};

/// �����ļ���Ϣ
struct FileInfo
{
	std::string strName;
	DWORD dwOffset;
	DWORD dwSize;
	DWORD dwCRC;
};

typedef std::map<std::string, SingleOption> VideoOptionMap;

class HardwareInfoSystem
{
private:

    static HardwareInfoSystem* sHardwareInfoSystem;

    VideoSetupInfo mVideoSetupInfo;

#ifndef TCONFIG
    tDataBase* mVideoCardConfigDataBase;
    tDataBase* mCpuConfigDataBase;
    tDataBase* mPhyMemoryDataBase;
#else
    DBC::DBCFile* mVideoCardConfigDataBase;
    DBC::DBCFile* mCpuConfigDataBase;
    DBC::DBCFile* mPhyMemoryDataBase;
#endif

    HardwareInfo mHardwareInfo;

    VideoOptionMap mVideoOptionMap;

    HINSTANCE mD3DLibrary;
    IDirect3D9* mIDirect3D9;
    D3DADAPTER_IDENTIFIER9 mD3DIdentifier9;
    D3DCAPS9 mD3DCAPS9;

    unsigned short mMaxVSVersionMajor;  /// Ӳ��֧�ֵ�vs���汾��
    unsigned short mMaxVSVersionMinor;  /// Ӳ��֧�ֵ�vs���汾��
    unsigned short mMaxPSVersionMajor;  /// Ӳ��֧�ֵ�ps���汾��
    unsigned short mMaxPSVersionMinor;  /// Ӳ��֧�ֵ�ps���汾��

    std::string mHardwareInfoString;    /// Ӳ����Ϣ�ַ���

    std::string mErrorInfoString;       /// ����������Ϣ

    std::string mHardwareInfoValidateString; /// Ӳ����Ϣ��֤�ַ���

	std::map<std::string, FileInfo> mConfigFileInfo;
public:

    HardwareInfoSystem();
    ~HardwareInfoSystem();

    static HardwareInfoSystem* getMe()
    {
        return sHardwareInfoSystem;
    }

    /** ��ʼ��Ӳ����Ϣϵͳ
    @remarks ���л��ȡ��Ϣ�������ݵ�ǰ����������ã������ÿ��ѡ����Ƽ�ֵ���������ֵ
    */
    bool initSystem(void);

    const VideoSetupInfo& getVideoSetupInfo(void) const
    {
        return mVideoSetupInfo;
    }

    const HardwareInfo& getHardwareInfo(void) const
    {
        return mHardwareInfo;
    }

    const char* getHardwareInfoAsString(void);

    const char* getErrorInfoString(void);

    const VideoOptionMap& getVideoOptionMap(void) const
    {
        return mVideoOptionMap;
    }

    const char* getDXVersion(void);

    const std::string& validateVideoDriver(void);

private:

    bool _ifCpuId();
    void _cpuVendor(char *szVendor);
    void _cpuTFMS(int *pnType, int *pnFamily, int *pnModel, int *pnStepping);
    bool _cpuCache(int *pnL2, int *pnL3);
    int _cpuBrand();
    int _cpuSpeed();

    void _InterCpu(char *szName, int nType, int nFamily, int nModel, int nStepping, int nL2, int nL3, int nBrand);
    void _AmdCpu(char *szName, int nType, int nFamily, int nModel, int nStepping);

    void _getOSInfo(char* osInfo);
    void _getCPUInfo(char* cpuInfo);
    void _getMemoryInfo(char* memoryInfo);
    void _getVideoCardInfo(char *szDevice, char *szDriver, char *extraInfo);


    bool _openVideoConfigFile(void);
    bool _openCpuConfigFile(void);
    bool _openPhyMemoryConfigFile(void);

    bool _isInRange(const char* rangeStr, float number);

    void _initOptionMap(void);

    int _getMaxFSAA(void);
    int _getMaxShadowTechnique(void);
    int _getMaxTextureFilter(void);
    int _getPostFilterEnable(void);

    void _setSingleOption(SingleOption& option, int minValue, int maxValue);

    bool _initD3D(void);
    void _deinitD3D(void);

    bool _openConfigFiles(void);
    void _closeConfigFiles(void);

    void _buildHardwareInfoString(void);

    void _pushErrorInfo(const std::string& errorInfo);

#ifdef TCONFIG

    /// �����ݱ�����tconfig���������ڰ���򿪣�Ҳ������ֱ�Ӵ��ļ��򿪣�
    DBC::DBCFile* _openDBCText(const std::string& namFileName, const std::string& fileName);

#endif

	int _getFileFromPackage(const std::string& strPack, const std::string& strFileName, char*& pFile );
};

#endif // _HardwareInfoSystem_H_