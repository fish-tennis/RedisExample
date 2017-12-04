#pragma once

#include <Util/FmConfig.h>
#include <Util/FmStream.h>

NS_FM_BEGIN

class TestObject
{
public:
	uint16		m_DeckIndex;		// ��������
	string		m_DeckName;			// ������
	uint16		m_HeroCardId;		// Ӣ�ۿ�
	std::map<uint16, uint8> m_MainCards;
public:
	TestObject();
	~TestObject();

	// ���л�
	void Serialize(Stream& stream)
	{
		stream << m_DeckIndex << m_DeckName << m_HeroCardId << (uint16)m_MainCards.size();
		for (std::map<uint16, uint8>::iterator it=m_MainCards.begin(); it!=m_MainCards.end(); ++it)
		{
			stream << it->first << it->second;
		}
	}
	// �����л�
	void Deserialize(Stream& stream)
	{
		uint16 mapSize = 0;
		stream >> m_DeckIndex >> m_DeckName >> m_HeroCardId >> mapSize;
		for ( uint16 i=0; i<mapSize; ++i )
		{
			uint16 k = 0;
			uint8 v = 0;
			stream >> k >> v;
			m_MainCards[k] = v;
		}
	}
};

NS_FM_END