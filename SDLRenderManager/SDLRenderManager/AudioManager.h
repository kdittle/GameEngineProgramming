//#ifndef AUDIOMANAGER_H_INCLUDED
//#define AUDIOMANAGER_H_INCLUDED
//
//#include "EngineObject.h"
//#include "ResourceManager.h"
//#include "Bass.h"
//#include <string>
//#include <sstream>
//#include <list>
//
////-------------------------------------------------------
//
//typedef enum {AUDIO_TYPE_SAMPLE=0, AUDIO_TYPE_STREAM=1}AUDIO_TYPE;
//
//class cAudioResource : public cResource
//{
//	private:
//	protected:
//	public:
//
//		HSAMPLE m_SampleData;
//		HSTREAM m_StreamData;
//		HCHANNEL m_ChannelData;
//
//		AUDIO_TYPE m_AudioType;
//
//		void load();
//		void unload();
//};
//
////-------------------------------------------------------
//
//class cAudioPlayer : public cEngineObject
//{
//	private:
//	protected:
//	public:
//		cAudioResource* m_AudioResource;
//		int m_NumRepeats;
//		int m_RepeatCount;
//
//		cAudioPlayer()
//		{
//			m_AudioResource = NULL;
//			m_NumRepeats = m_RepeatCount = 0;
//		}
//
//		virtual void onMediaComplete()=0;
//};
//
////-------------------------------------------------------
//
//class cAudioManager :  public cEngineObject
//{
//private:
//protected:
//	cAudioManager();
//	static cAudioManager m_AudioManager;
//
//	std::list<cAudioPlayer*> m_Players;
//public:
//	BASS_DEVICEINFO m_DeviceInfo;
//	std::string m_DeviceInfoString;
//	static cAudioManager* GetAudioManager();
//	bool init(int Device=-1, DWORD SampleRate=44100, DWORD flags=0, HWND win=0);
//	void free();
//	void setVolume(float Volume) {BASS_SetVolume(Volume);}
//	void pause() {BASS_Pause();}
//	void start() {BASS_Start();}
//	cResource* loadResourceFromXML(TiXmlElement *Element);
//	
//	//Plays audio resource once
//	void playFromResource(cAudioResource* AudioResource);
//	void addAudioPlayer(cAudioPlayer *Player);
//
//	void update();
//};
//
//
////-------------------------------------------------------
//
//#endif //AUDIOMANAGER_H_INCLUDED