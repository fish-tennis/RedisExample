#pragma once

#include <Util/FmConfig.h>
#include <Util/FmStream.h>

NS_FM_BEGIN

class TestObject
{
public:
	int			m_Int;
	string		m_Name;
	std::map<int, int> m_Map;
public:
	TestObject();
	~TestObject();

	// ���л�
	void Serialize(Stream& stream)
	{
		stream << m_Int << m_Name << m_Map;
	}
	// �����л�
	void Deserialize(Stream& stream)
	{
		stream >> m_Int >> m_Name >> m_Map;
	}
};

NS_FM_END