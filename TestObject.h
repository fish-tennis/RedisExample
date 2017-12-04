#pragma once

#include <Util/FmConfig.h>
#include <Util/FmStream.h>

NS_FM_BEGIN

class TestObject
{
public:
	uint16		m_DeckIndex;		// 卡组索引
	string		m_DeckName;			// 卡组名
	uint16		m_HeroCardId;		// 英雄卡
	std::map<uint16, uint8> m_MainCards;
public:
	TestObject();
	~TestObject();

	// 序列化
	void Serialize(Stream& stream)
	{
		stream << m_DeckIndex << m_DeckName << m_HeroCardId << (uint16)m_MainCards.size();
		for (std::map<uint16, uint8>::iterator it=m_MainCards.begin(); it!=m_MainCards.end(); ++it)
		{
			stream << it->first << it->second;
		}
	}
	// 反序列化
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