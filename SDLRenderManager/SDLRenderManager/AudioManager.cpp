#include "stdafx.h"
//#include "AudioManager.h"
//
//cAudioManager cAudioManager::m_AudioManager;
//
////-------------------------------------------------------
//
//void cAudioResource::load()
//{
//	if(m_AudioType == AUDIO_TYPE_SAMPLE)
//	{
//		m_SampleData = BASS_SampleLoad(FALSE, m_FileName.c_str(),0,0,1,0);
//		m_ChannelData = BASS_SampleGetChannel(m_SampleData,false);
//	}
//	else
//	{
//		m_StreamData = BASS_StreamCreateFile(FALSE, m_FileName.c_str(),0,0,0);
//		m_ChannelData = m_StreamData;
//	}
//}
//
////-------------------------------------------------------
//
//void cAudioResource::unload()
//{
//	if(m_Type == AUDIO_TYPE_SAMPLE)
//		BASS_SampleFree(m_SampleData);
//	else
//		BASS_StreamFree(m_StreamData);
//
//	m_SampleData = m_StreamData = NULL;
//}
//
////-------------------------------------------------------
//
//void cAudioManager::playFromResource(cAudioResource* AudioResource)
//{
//	if(!AudioResource)
//		return;
//
//	BASS_ChannelPlay(AudioResource->m_ChannelData, false);
//}
//
////-------------------------------------------------------
//
//cAudioManager::cAudioManager()
//{
//}
//
////-------------------------------------------------------
//
//bool cAudioManager::init(int Device, DWORD SampleRate, DWORD flags, HWND win)
//{
//	BOOL bassActive = BASS_Init(Device, SampleRate, flags, win, NULL);
//
//	if(bassActive)
//	{
//		//Get Device Info
//		std::stringstream DeviceStringStream;
//
//		if(BASS_GetDeviceInfo(Device, &m_DeviceInfo))
//		{
//			DeviceStringStream << "Audio Device Info. Name: " << m_DeviceInfo.name << " Driver: " << m_DeviceInfo.driver;
//			m_DeviceInfoString = DeviceStringStream.str();
//		}
//	}
//
//	return bassActive;
//}
//
////-------------------------------------------------------
//
//void cAudioManager::free()
//{
//	BASS_Free();
//}
//
////-------------------------------------------------------
//
//cResource* cAudioManager::loadResourceFromXML(TiXmlElement *Element)
//{
//    if(Element)
//    {
//        cAudioResource* Resource = new cAudioResource();
//
//        for(TiXmlAttribute* ElementAttrib = Element->FirstAttribute(); ElementAttrib; ElementAttrib = ElementAttrib->Next())
//        {
//            std::string AttribName = ElementAttrib->Name();
//            std::string AttribValue = ElementAttrib->Value();
//
//            if(AttribName=="UID")
//            {
//                Resource->m_ResourceID = atoi(AttribValue.c_str());
//            }
//
//            if(AttribName=="filename")
//            {
//                Resource->m_FileName = AttribValue;
//            }
//
//            if(AttribName=="scenescope")
//            {
//                Resource->m_Scope = atoi(AttribValue.c_str());
//            }
//
//            if(AttribName=="audio_type")
//            {
//				if(AttribValue=="sample")
//					Resource->m_AudioType = AUDIO_TYPE_SAMPLE;
//				else
//					Resource->m_AudioType = AUDIO_TYPE_STREAM;
//            }
//        }
//
//        return Resource;
//    }
//
//    return NULL;
//}
//
////-------------------------------------------------------
//
//cAudioManager* cAudioManager::GetAudioManager()
//{
//    return &m_AudioManager;
//}
//
////-------------------------------------------------------
//
//void cAudioManager::update()
//{
//	std::list<cAudioPlayer*>::iterator list_it;
//
//	for(list_it=m_Players.begin();list_it!=m_Players.end();list_it++)
//	{
//		if(BASS_ChannelIsActive((*list_it)->m_AudioResource->m_ChannelData)==BASS_ACTIVE_STOPPED)
//		{
//			(*list_it)->onMediaComplete();
//			
//			if((*list_it)->m_RepeatCount>=(*list_it)->m_NumRepeats)
//			{
//				m_Players.erase(list_it);
//				return;
//			}
//			else
//			{
//				(*list_it)->m_RepeatCount++;
//				BASS_ChannelPlay((*list_it)->m_AudioResource->m_ChannelData, false);
//			}
//		}
//	}
//}
//
////-------------------------------------------------------
//
//void cAudioManager::addAudioPlayer(cAudioPlayer *Player)
//{
//	m_Players.push_back(Player);
//	BASS_ChannelPlay(Player->m_AudioResource->m_ChannelData, false);
//	Player->m_RepeatCount = 0;
//}
//
////-------------------------------------------------------
