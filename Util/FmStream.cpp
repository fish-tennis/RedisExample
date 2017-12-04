#include "FmStream.h"

NS_FM_BEGIN

uint Stream::s_MaxContainerSize = 10000;
uint Stream::s_MaxStringLength  = 1024*1024*2;
//------------------Stream-------------------//
Stream& Stream::operator<<( bool value )
{
	SerializeData( &value, sizeof(bool) );
	return *this;
}
Stream& Stream::operator>>( bool& value )
{
	DeserializeData( &value, sizeof(bool) );
	return *this;
}

Stream& Stream::operator<<( int8 value )
{
	SerializeData( &value, sizeof(int8) );
	return *this;
}
Stream& Stream::operator>>( int8& value )
{
	DeserializeData( &value, sizeof(int8) );
	return *this;
}

Stream& Stream::operator<<( uint8 value )
{
	SerializeData( &value, sizeof(uint8) );
	return *this;
}
Stream& Stream::operator>>( uint8& value )
{
	DeserializeData( &value, sizeof(uint8) );
	return *this;
}

Stream& Stream::operator<<( int16 value )
{
	SerializeData( &value, sizeof(int16) );
	return *this;
}
Stream& Stream::operator>>( int16& value )
{
	DeserializeData( &value, sizeof(int16) );
	return *this;
}

Stream& Stream::operator<<( uint16 value )
{
	SerializeData( &value, sizeof(uint16) );
	return *this;
}
Stream& Stream::operator>>( uint16& value )
{
	DeserializeData( &value, sizeof(uint16) );
	return *this;
}

Stream& Stream::operator<<( int value )
{
	SerializeData( &value, sizeof(int) );
	return *this;
}
Stream& Stream::operator>>( int& value )
{
	DeserializeData( &value, sizeof(int) );
	return *this;
}

Stream& Stream::operator<<( uint value )
{
	SerializeData( &value, sizeof(uint) );
	return *this;
}
Stream& Stream::operator>>( uint& value )
{
	DeserializeData( &value, sizeof(uint) );
	return *this;
}

Stream& Stream::operator<<( int64 value )
{
	SerializeData( &value, sizeof(int64) );
	return *this;
}
Stream& Stream::operator>>( int64& value )
{
	DeserializeData( &value, sizeof(int64) );
	return *this;
}

Stream& Stream::operator<<( uint64 value )
{
	SerializeData( &value, sizeof(uint64) );
	return *this;
}
Stream& Stream::operator>>( uint64& value )
{
	DeserializeData( &value, sizeof(uint64) );
	return *this;
}

Stream& Stream::operator<<( float value )
{
	SerializeData( &value, sizeof(float) );
	return *this;
}
Stream& Stream::operator>>( float& value )
{
	DeserializeData( &value, sizeof(float) );
	return *this;
}

Stream& Stream::operator<<( double value )
{
	SerializeData( &value, sizeof(double) );
	return *this;
}
Stream& Stream::operator>>( double& value )
{
	DeserializeData( &value, sizeof(double) );
	return *this;
}

Stream& Stream::operator<<( const char* value )
{
	if( value == NULL )
	{
		m_StreamError = EStreamError_BadArg;
		return *this;
	}
	uint len = (uint)strlen( value );
	SerializeData( &len, sizeof(uint) );
	SerializeData( (void*)value, len );
	return *this;
}
Stream& Stream::operator<<( const string& value )
{
	uint len = (uint)value.length();
	SerializeData( &len, sizeof(uint) );
	SerializeData( (void*)(value.data()), value.length() );
	return *this;
}
Stream& Stream::operator<<( string& value )
{
	uint len = (uint)value.length();
	SerializeData( &len, sizeof(uint) );
	SerializeData( (void*)(value.data()), value.length() );
	return *this;
}
Stream& Stream::operator>>( string& value )
{
	uint len = 0;
	DeserializeData( &len, sizeof(uint) );
	if( len > 0 )
	{
		if( len > s_MaxStringLength )
		{
			m_StreamError = EStreamError_ContainerTooBig;
			Assert( false) ;
			return *this;
		}
		value.resize( len );
		DeserializeData( &(value[0]), len );
	}
	else
	{
		value.clear();
	}
	return *this;
}

bool Stream::Read_bool()
{
	bool value = false;
	(*this) >> value;
	return value;
}

int8 Stream::Read_int8()
{
	int8 value = 0;
	(*this) >> value;
	return value;
}
uint8 Stream::Read_uint8()
{
	uint8 value = 0;
	(*this) >> value;
	return value;
}
int16 Stream::Read_int16()
{
	int16 value = 0;
	(*this) >> value;
	return value;
}
uint16 Stream::Read_uint16()
{
	uint16 value = 0;
	(*this) >> value;
	return value;
}
int Stream::Read_int()
{
	int value = 0;
	(*this) >> value;
	return value;
}
uint Stream::Read_uint()
{
	uint value = 0;
	(*this) >> value;
	return value;
}
int64 Stream::Read_int64()
{
	int64 value = 0;
	(*this) >> value;
	return value;
}
uint64 Stream::Read_uint64()
{
	uint64 value = 0;
	(*this) >> value;
	return value;
}
float Stream::Read_float()
{
	float value = 0.0f;
	(*this) >> value;
	return value;
}
double Stream::Read_double()
{
	double value = 0.0f;
	(*this) >> value;
	return value;
}
string Stream::Read_string()
{
	string value;
	(*this) >> value;
	return value;
}

void Stream::Write_bool( bool value )
{
	(*this) << value;
}
void Stream::Write_int8( int8 value )
{
	(*this) << value;
}
void Stream::Write_uint8( uint8 value )
{
	(*this) << value;
}
void Stream::Write_int16( int16 value )
{
	(*this) << value;
}
void Stream::Write_uint16( uint16 value )
{
	(*this) << value;
}
void Stream::Write_int( int value )
{
	(*this) << value;
}
void Stream::Write_uint( uint value )
{
	(*this) << value;
}
void Stream::Write_int64( int64 value )
{
	(*this) << value;
}
void Stream::Write_uint64( uint64 value )
{
	(*this) << value;
}
void Stream::Write_float( float value )
{
	(*this) << value;
}
void Stream::Write_double( double value )
{
	(*this) << value;
}
void Stream::Write_string( const char* value )
{
	if( value )
		(*this) << value;
}

//-----------------MemStream-----------------//
MemStream::MemStream( uint8* pBuffer, size_t size )
	: m_Buffer(pBuffer), m_Pointer(pBuffer), m_Size(size)
{
	Assert( m_Buffer );
	//Assert( size > 0 );
}

MemStream::MemStream( size_t size )
{
	m_Size = size;
	m_Buffer = (m_Size>0?(new uint8[m_Size]):nullptr);
	m_Pointer = m_Buffer;
}

void MemStream::ReleaseBuffer()
{
	FM_SAFE_DELETE_ARRAY( m_Buffer );
	m_Pointer = m_Buffer;
	m_Size = 0;
}

size_t MemStream::GetOffset()
{
	return (size_t)(m_Pointer-m_Buffer);
}

void MemStream::SetOffset( size_t offset )
{
	if( (offset > 0 && offset > m_Size) || m_Buffer == NULL )
	{
		Assert( false );
		return;
	}
	m_Pointer = m_Buffer + offset;
}

size_t MemStream::GetSpace()
{
	return (size_t)(m_Size-(m_Pointer-m_Buffer));
}

bool MemStream::Ignore( size_t size )
{
	if( GetSpace() < size )
	{
		Assert( false );
		m_StreamError = EStreamError_BufferFull;
		return false;
	}
	m_Pointer += size;
	return true;
}

void MemStream::SerializeData( void* data, size_t size )
{
	if( GetSpace() < size )
	{
		m_StreamError = EStreamError_BufferFull;
		return;
	}
	memcpy( m_Pointer, data, size );
	m_Pointer += size;
}

void MemStream::DeserializeData( void* data, size_t size )
{
	if( GetSpace() < size )
	{
		m_StreamError = EStreamError_BufferFull;
		return;
	}
	memcpy( data, m_Pointer, size );
	m_Pointer += size;
}

void MemStream::Insert( uint value, uint offset )
{
	if( offset + sizeof(uint) > m_Size )
	{
		Assert( false );
		m_StreamError = EStreamError_BufferFull;
		return;
	}
	uint8* buffer = m_Buffer + offset;
	*((uint*)buffer) = value;
}
const uint8* MemStream::GetBuffPoint()
{
	return m_Pointer;
}
size_t StrStream::GetOffset()
{
	return m_Offset;
}

void StrStream::SetOffset( size_t offset )
{
	if( offset > 0 && offset > GetSize() )
	{
		Assert( false );
		return;
	}
	m_Offset = offset;
}

size_t StrStream::GetSpace()
{
	return (size_t)(m_Buffer.length()-m_Offset);
}

size_t StrStream::GetSize()
{
	return m_Buffer.length();
}

void StrStream::SetBuffer( const string& buffer )
{
	m_Buffer = buffer;
	m_Offset = 0;
}

void StrStream::SerializeData( void* data, size_t size )
{
	if( GetSpace() < size )
	{
		m_Buffer.insert( m_Offset, size-GetSpace(), 0 );
	}
	if( size > 0 )
	{
		memcpy( &(m_Buffer[m_Offset]), data, size );
		m_Offset += size;
	}
}

void StrStream::DeserializeData( void* data, size_t size )
{
	if( GetSpace() < size )
	{
		m_StreamError = EStreamError_BufferFull;
		return;
	}
	if( size > 0 )
	{
		memcpy( data, &(m_Buffer[m_Offset]), size );
		m_Offset += size;
	}
}

void StrStream::Insert( uint value, uint offset )
{
	if( offset + sizeof(uint) > GetSize() )
	{
		Assert( false );
		m_StreamError = EStreamError_BufferFull;
		return;
	}
	memcpy( &(m_Buffer[offset]), &value, sizeof(uint) );
}
const uint8* StrStream::GetBuffPoint()
{
	return (uint8*)&(m_Buffer[m_Offset]);
}
void StrStream::Serialize( Stream& stream )
{
	// 只序列化有效的数据
	stream << (uint)m_Offset;
	if( m_Offset > 0 )
	{
		stream.SerializeData( &(m_Buffer[0]), m_Offset );
	}
}

void StrStream::Deserialize( Stream& stream )
{
	stream >> m_Buffer;
	m_Offset = m_Buffer.length();
}

NS_FM_END