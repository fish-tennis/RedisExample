/********************************************************************
created:	2012-04-15
author:		Fish (于国平)
summary:	流数据的操作
*********************************************************************/
#pragma once

#include "FmConfig.h"

NS_FM_BEGIN

// 错误枚举
enum EStreamError
{
	EStreamError_Error,
	EStreamError_OK,
	EStreamError_BufferFull,
	EStreamError_BadArg,
	EStreamError_ContainerTooBig,
};

class FmUtil_EXPORTS Stream
{
public:
	static uint s_MaxContainerSize;	// 容器最大大小,防止序列化时,数据错误,导致分配一个很大的容器,直接内存爆掉,默认值:10000
	static uint	s_MaxStringLength;	// 字符串最大大小,防止序列化时,数据错误,导致分配一个很大的字符串,直接内存爆掉,默认值:1024*256
	uint8	m_StreamError;	// 错误
public:
	Stream()
	{
		m_StreamError = EStreamError_OK;
	}
	// 偏移
	virtual size_t GetOffset() = 0;
	// 修改偏移
	virtual void SetOffset( size_t offset ) = 0;
	// 空闲大小
	virtual size_t GetSpace() = 0;
	// 总大小
	virtual size_t GetSize() = 0;

	// 序列化接口
	virtual void SerializeData( void* data, size_t size ) = 0;
	// 反序列化接口
	virtual void DeserializeData( void* data, size_t size ) = 0;
	// 插入一个数值
	virtual void Insert( uint value, uint offset ) = 0;

	virtual const uint8* GetBuffPoint() = 0;
	// 错误
	EStreamError GetError() const { return (EStreamError)m_StreamError; }
	bool IsOK() { return (m_StreamError == EStreamError_OK); }

	Stream& operator<<( bool value );
	Stream& operator>>( bool& value );

	Stream& operator<<( int8 value );
	Stream& operator>>( int8& value );

	Stream& operator<<( uint8 value );
	Stream& operator>>( uint8& value );

	Stream& operator<<( int16 value );
	Stream& operator>>( int16& value );

	Stream& operator<<( uint16 value );
	Stream& operator>>( uint16& value );

	Stream& operator<<( int value );
	Stream& operator>>( int& value );

	Stream& operator<<( uint value );
	Stream& operator>>( uint& value );

	Stream& operator<<( int64 value );
	Stream& operator>>( int64& value );

	Stream& operator<<( uint64 value );
	Stream& operator>>( uint64& value );

	Stream& operator<<( float value );
	Stream& operator>>( float& value );

	Stream& operator<<( double value );
	Stream& operator>>( double& value );

	Stream& operator<<( const char* value );
	Stream& operator<<( const string& value );
	Stream& operator<<( string& value );
	Stream& operator>>( string& value );

	bool Read_bool();
	int8 Read_int8();
	uint8 Read_uint8();
	int16 Read_int16();
	uint16 Read_uint16();
	int Read_int();
	uint Read_uint();
	int64 Read_int64();
	uint64 Read_uint64();
	float Read_float();
	double Read_double();
	string Read_string();

	void Write_bool( bool value );
	void Write_int8( int8 value );
	void Write_uint8( uint8 value );
	void Write_int16( int16 value );
	void Write_uint16( uint16 value );
	void Write_int( int value );
	void Write_uint( uint value );
	void Write_int64( int64 value );
	void Write_uint64( uint64 value );
	void Write_float( float value );
	void Write_double( double value );
	void Write_string( const char* value );

};

// 模板化
template<class T>
Stream& operator << ( Stream& stream, T& t )
{
	t.Serialize( stream );
	return stream;
}

// 模板化
template<class T>
Stream& operator >> ( Stream& stream, T& t )
{
	t.Deserialize( stream );
	return stream;
}

// 模板化vector
template<class T>
Stream& operator << ( Stream& stream, vector<T>& vec )
{
	uint size = (uint)vec.size();
	stream << size;
	for ( uint i=0; i<size; ++i )
	{
		stream << vec[i];
		if( !stream.IsOK() )
			break;
	}
	return stream;
}

template<class T>
Stream& operator >> ( Stream& stream, vector<T>& vec )
{
	uint size = 0;
	stream >> size;
	if( size > Stream::s_MaxContainerSize )
	{
		stream.m_StreamError = EStreamError_ContainerTooBig;
		Assert( false) ;
		return stream;
	}
	vec.resize( size );
	for ( uint i=0; i<size; ++i )
	{
		stream >> vec[i];
		if( !stream.IsOK() )
			break;
	}
	return stream;
}

// 模板化map
template<class K, class V>
Stream& operator << ( Stream& stream, map<K,V>& m )
{
	uint size = m.size();
	stream << size;
	for ( typename map<K,V>::iterator it=m.begin(); it!=m.end(); ++it )
	{
		stream << it->first;
		stream << it->second;
		if( !stream.IsOK() )
			break;
	}
	return stream;
}

template<class K, class V>
Stream& operator >> ( Stream& stream, map<K,V>& m )
{
	uint size = 0;
	stream >> size;
	if( size > Stream::s_MaxContainerSize )
	{
		stream.m_StreamError = EStreamError_ContainerTooBig;
		Assert( false) ;
		return stream;
	}
	for ( uint i=0; i<size; ++i )
	{
		K k;
		stream >> k;
		stream >> m[k];
		if( !stream.IsOK() )
			break;
	}
	return stream;
}

// 模板化set
template<class T>
Stream& operator >> ( Stream& stream, set<T>& s )
{
	uint size = 0;
	stream >> size;
	if( size > Stream::s_MaxContainerSize )
	{
		stream.m_StreamError = EStreamError_ContainerTooBig;
		Assert( false) ;
		return stream;
	}
	T data;
	for ( uint i=0; i<size; ++i )
	{
		stream >> data;
		if( !stream.IsOK() )
			break;
		s.insert( data );
	}
	return stream;
}

template<class T>
Stream& operator << ( Stream& stream, set<T>& s )
{
	uint size = (uint)s.size();
	stream << size;
	for ( typename set<T>::iterator it=s.begin(); it!=s.end(); ++it )
	{
		stream << *it;
		if( !stream.IsOK() )
			break;
	}
	return stream;
}

// 模板化UNORDERED_MAP
template<class K, class V>
Stream& operator << ( Stream& stream, UNORDERED_MAP< K, V > & m )
{
	uint size = (uint)m.size();
	stream << size;
	for ( typename UNORDERED_MAP<K,V>::iterator it=m.begin(); it!=m.end(); ++it )
	{
		stream << it->first;
		stream << it->second;
		if( !stream.IsOK() )
			break;
	}
	return stream;
}

template<class K, class V>
Stream& operator >> ( Stream& stream, UNORDERED_MAP<K,V>& m )
{
	uint size = 0;
	stream >> size;
	if( size > Stream::s_MaxContainerSize )
	{
		stream.m_StreamError = EStreamError_ContainerTooBig;
		Assert( false) ;
		return stream;
	}
	for ( uint i=0; i<size; ++i )
	{
		K k;
		stream >> k;
		stream >> m[k];
		if( !stream.IsOK() )
			break;
	}
	return stream;
}

// 固定缓存的流
class FmUtil_EXPORTS MemStream : public Stream
{
protected:
	uint8*	m_Buffer;		// 起始地址
	uint8*	m_Pointer;		// 当前地址
	size_t	m_Size;			// 总大小
public:
	// 外部缓存
	MemStream( uint8* pBuffer, size_t size );

	// 内部分配内存
	MemStream( size_t size );

	// 释放内存
	void ReleaseBuffer();

	// 偏移
	virtual size_t GetOffset();
	// 修改偏移
	virtual void SetOffset( size_t offset );
	// 空闲大小
	virtual size_t GetSpace();
	// 总大小
	virtual size_t GetSize()    { return m_Size; }

	// 指针接口
	uint8* GetBuffer()	{ return m_Buffer; }
	uint8* GetOffsetPointer()	{ return m_Pointer; }

	// 序列化接口
	virtual void SerializeData( void* data, size_t size );
	// 反序列化接口
	virtual void DeserializeData( void* data, size_t size );
	// 插入一个数值
	virtual void Insert( uint value, uint offset );

	virtual const uint8* GetBuffPoint();
	// 偏移向前
	bool Ignore( size_t size );

public:
	// 在偏移处写入一个值
	template<class T>
	void SetData( T value, uint offset )
	{
		if( offset + sizeof(T) > m_Size )
		{
			Assert( false );
			m_StreamError = EStreamError_BufferFull;
			return;
		}
		uint8* buffer = m_Buffer + offset;
		*((T*)buffer) = value;
	}
};


// 固定大小的缓存流
template <uint BufferSize>
class FmUtil_EXPORTS BufferStream : public MemStream
{
protected:
	uint8	m_MemBuffer[BufferSize];
public:
	BufferStream() : MemStream( m_MemBuffer, BufferSize ) {}
};

class FmUtil_EXPORTS AutoReleaseMemStream
{
public:
	MemStream* m_MemStream;
public:
	AutoReleaseMemStream()
	{
		m_MemStream = nullptr;
	}
	~AutoReleaseMemStream()
	{
		if( m_MemStream != nullptr )
		{
			m_MemStream->ReleaseBuffer();
			m_MemStream = nullptr;
		}
	}
};

// 动态流
class FmUtil_EXPORTS StrStream : public Stream
{
protected:
	string	m_Buffer;
	size_t	m_Offset;
public:
	StrStream()
	{
		m_Offset = 0;
	}
	// 偏移
	virtual size_t GetOffset();
	virtual void SetOffset( size_t offset );
	// 空闲大小
	virtual size_t GetSpace();
	// 总大小
	virtual size_t GetSize();

	string& GetBuffer() { return m_Buffer; }
	void SetBuffer( const string& buffer );

	// 序列化接口
	virtual void SerializeData( void* data, size_t size );
	// 反序列化接口
	virtual void DeserializeData( void* data, size_t size );
	// 插入一个数值
	virtual void Insert( uint value, uint offset );
	//得到当前数据指针
	virtual const uint8* GetBuffPoint();
	// 序列化自己
	void Serialize( Stream& stream );
	// 反序列化自己
	void Deserialize( Stream& stream );
};

NS_FM_END
