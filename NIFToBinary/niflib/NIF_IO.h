/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#ifndef _NIF_IO_H
#define _NIF_IO_H

//--Includes--//
#include <string>
#include <sstream>
#include <vector>
#include "Key.h"
#include "nif_basic_types.h"
#include "gen/enums_intl.h"
#include "nif_math.h"
#include "nif_versions.h"
namespace Niflib {
using namespace std;

#ifndef NULL
#define NULL 0
#endif

/*!
 * Class overridable alloc/release methods
 * \param T Type of array
 */
template<typename T>
class array_Traits
{
public:	
   /*!
    * Default Initialization method.
    * \param v Vector of types to initialize
    * \param length Length in bytes of memory to allocate
	*/
   static void Initialize( T* v, int length )   { 
      memset(v, 0, sizeof(v[0]) * length);
   }
   /*!
    * Default Finalization method.
    * \param v Vector of types to initialize
    * \param length Length in bytes of memory to allocate
	*/
   static void Finalize( T* v, int length )   { 
      memset(v, 0, sizeof(v[0]) * length);
   }
   /*
    * Default Initialization method.
    * \param s Vector of types to copy from
    * \param d Vector of types to copy to
    * \param length Length in bytes of memory to allocate
	*/
   static void Copy(T const* s, T* d, int length )   { 
      for (int i=0; i<length; ++i)
         d[i] = s[i];
   }
};

/*!
 * A fixed length array of type T.
 * Data is allocated into a array portion and the data section.
 * The array simply points to appropriate places in the data section.
 * \param T Type of array
 * \param len_ Size of the array.
 */
template<size_t len_, typename T>
class array
{
	typedef typename T * RawData;
	typedef typename T const* ConstRawData;
public:
	/*! 
	 *Default Constructor.  Allocates empty array.
	 */
	array() {
		array_Traits<T>::Initialize(v_, len_);
	}

	/*!
	 * Copy constructor.
	 * \param other The array being copied
	 */
	array(const array& other) {
		array_Traits<T>::Copy(other.v_, v_, len_);
	}

	/*!
	 * Copy constructor.
	 * \param other The array being copied
	 */   
	array(const RawData& other) {
		array_Traits<T>::Copy(other, v_, len_);
	}

	/*!
	 * Copy constructor.
	 * \param other The array being copied
	 */
	array(RawData& other) {
		array_Traits<T>::Copy(other, v_, len_);
	}

	/*!
	 * Default destructor.
	 */
	~array() { 
		array_Traits<T>::Finalize(v_, len_);
	}

	//These operators cause SWIG warnings
	#ifndef SWIG

	/*
	 * Copy assignment.
	 * \param The array to assign to this one.
	 * \return A reference to this array.
	 */
	array& operator=(const array& other) {
		array tmp( other );
		swap( tmp );
		return *this;
	}

	/*
	 * Copy assignment.
	 * \param The array to assign to this one.
	 * \return A reference to this array.
	 */
	array& operator=(const ConstRawData& other) {
		array tmp( other );
		swap( tmp );
		return *this;
	}

	T& operator[](int index) {
		// assert( index >= 0 && index < len_ )
		return v_[index];
	} 

	const T& operator[](int index) const {
		// assert( index >= 0 && index < len_ )
		return v_[index];
	} 

	T& operator[](uint index) {
		// assert( index >= 0 && index < len_ )
		return v_[index];
	} 

	const T& operator[](uint index) const {
		// assert( index >= 0 && index < len_ )
		return v_[index];
	} 

#endif
	operator T*() const {
		return v_;
	}

	/*! Number of items in the vector. */
	size_t size() const { return len_; }
	size_t count() const { return len_; }

	T* begin() { 
		return v_; 
	}

	T* end() { 
		return v_ + len_; 
	}

	const T* begin() const { 
		return v_; 
	}

	const T* end() const { 
		return v_ + len_; 
	}

	/*!
	 * Assign an element to array at specified index.
	 * \param index Index in array to assign.
	 * \param value Value to copy into string.
	 */
	void assign(int index, T value) {
		v_[index] = value;
	}

	/*!
	 * Resets array back to zero size.
	 */
	void clear() {
		array_Traits<T>::Finalize(v_, len_);
	}

	/*!
	 * Swap contents with another array.
	 * \param other  Other vector to swap with
	 */
	void swap( array &other ) {
		array tmp(other);
		array_Traits<T>::Copy(v_, other.v_, len_);
		array_Traits<T>::Copy(tmp.v_, v_, len_);
	}

private:

   T v_[len_]; //! Vector data
};

//--IO Functions--//

int BlockSearch( istream& in );

//-- Read Utility Functions--//
int ReadInt( istream& in );
uint ReadUInt( istream& in );
ushort ReadUShort( istream& in );
short ReadShort( istream& in );
byte ReadByte( istream& in );
float ReadFloat( istream &in );
string ReadString( istream &in );
bool ReadBool( istream &in, unsigned int version );

//-- Write Utility Functions --//
void WriteInt( int val, ostream& out );
void WriteUInt( uint val, ostream& out );
void WriteUShort( ushort val, ostream& out );
void WriteShort( short val, ostream& out );
void WriteByte( byte val, ostream& out );
void WriteFloat( float val, ostream& out );
void WriteString( string const & val, ostream& out );
void WriteBool( bool val, ostream& out, unsigned int version );

//-- NifStream And ostream Functions --//
// The NifStream functions allow each built-in type to be streamed to and from a file.
// The ostream functions are for writing out a debug string.

//--Basic Types--//

//int
void NifStream( int & val, istream& in, uint version = 0 );
void NifStream( int const & val, ostream& out, uint version = 0  );

//uint
void NifStream( uint & val, istream& in, uint version = 0 );
void NifStream( uint const & val, ostream& out, uint version = 0  );

//ushort
void NifStream( ushort & val, istream& in, uint version = 0 );
void NifStream( ushort const & val, ostream& out, uint version = 0  );

//short
void NifStream( short & val, istream& in, uint version = 0 );
void NifStream( short const & val, ostream& out, uint version = 0  );

//byte
void NifStream( byte & val, istream& in, uint version = 0 );
void NifStream( byte const & val, ostream& out, uint version = 0  );

//bool
void NifStream( bool & val, istream& in, uint version );  // version is significant
void NifStream( bool const & val, ostream& out, uint version ); // version is significant

//float
void NifStream( float & val, istream& in, uint version = 0 );
void NifStream( float const & val, ostream& out, uint version = 0  );

//string
void NifStream( string & val, istream& in, uint version = 0 );
void NifStream( string const & val, ostream& out, uint version = 0  );

//--Structs--//

//TexCoord
void NifStream( TexCoord & val, istream& in, uint version = 0 );
void NifStream( TexCoord const & val, ostream& out, uint version = 0 );

//Triangle
void NifStream( Triangle & val, istream& in, uint version = 0 );
void NifStream( Triangle const & val, ostream& out, uint version = 0 );

//Vector3
void NifStream( Vector3 & val, istream& in, uint version = 0 );
void NifStream( Vector3 const & val, ostream& out, uint version = 0  );

//Float2
void NifStream( Float2 & val, istream& in, uint version = 0 );
void NifStream( Float2 const & val, ostream& out, uint version = 0  );

//Matrix22
void NifStream( Matrix22 & val, istream& in, uint version = 0 );
void NifStream( Matrix22 const & val, ostream& out, uint version = 0 );

//Float3
void NifStream( Float3 & val, istream& in, uint version = 0 );
void NifStream( Float3 const & val, ostream& out, uint version = 0  );

//Matrix33
void NifStream( Matrix33 & val, istream& in, uint version = 0 );
void NifStream( Matrix33 const & val, ostream& out, uint version = 0 );

//Float4
void NifStream( Float4 & val, istream& in, uint version = 0 );
void NifStream( Float4 const & val, ostream& out, uint version = 0  );

//Matrix44
void NifStream( Matrix44 & val, istream& in, uint version = 0 );
void NifStream( Matrix44 const & val, ostream& out, uint version = 0 );

//Color3
void NifStream( Color3 & val, istream& in, uint version = 0 );
void NifStream( Color3 const & val, ostream& out, uint version = 0  );

//Color4
void NifStream( Color4 & val, istream& in, uint version = 0 );
void NifStream( Color4 const & val, ostream& out, uint version = 0  );

//Quaternion
void NifStream( Quaternion & val, istream& in, uint version = 0 );
void NifStream( Quaternion const & val, ostream& out, uint version = 0  );

//HeaderString
void NifStream( HeaderString & val, istream& in, uint & version ); //Sets the passed in version varible
void NifStream( HeaderString const & val, ostream& out, uint version = 0  );
ostream & operator<<( ostream & out, HeaderString const & val );

//LineString
void NifStream( LineString & val, istream& in, uint version );
void NifStream( LineString const & val, ostream& out, uint version = 0  );
ostream & operator<<( ostream & out, LineString const & val );

//ShortString
void NifStream( ShortString & val, istream& in, uint version = 0 );
void NifStream( ShortString const & val, ostream& out, uint version = 0  );
ostream & operator<<( ostream & out, ShortString const & val );

//--Templates--//

void NifStream( Key<Quaternion> & key, istream& file, uint version, KeyType type );
void NifStream( Key<Quaternion> const & key, ostream& file, uint version,  KeyType type );

//Key<T>
template <class T> 
void NifStream( Key<T> & key, istream& file, uint version, KeyType type ) {
	key.time = ReadFloat( file );

	//If key type is not 1, 2, or 3, throw an exception
	if ( type < 1 || type > 3 ) {
		type = LINEAR_KEY;
		//throw runtime_error("Invalid key type.");
	}

	//Read data based on the type of key
	NifStream( key.data, file );
	if ( type == QUADRATIC_KEY ) {
		//Uses Quadratic interpolation
		NifStream( key.forward_tangent, file );
		NifStream( key.backward_tangent, file );
	} else if ( type == TBC_KEY ) {
		//Uses TBC interpolation
		key.tension = ReadFloat( file );
		key.bias = ReadFloat( file );
		key.continuity = ReadFloat( file );
	}
}

template <class T> 
void NifStream( Key<T> & key, istream & file, uint version, int type ) {
	NifStream( key, file, version, (KeyType)type );
}

template <class T> 
void NifStream( Key<T> const & key, ostream& file, uint version, KeyType type ) {
	WriteFloat( key.time, file );

	//If key type is not 1, 2, or 3, throw an exception
	if ( type < 1 || type > 3 ) {
		type = LINEAR_KEY;
		//throw runtime_error("Invalid key type.");
	}

	//Read data based on the type of key
	NifStream( key.data, file );
	if ( type == QUADRATIC_KEY ) {
		//Uses Quadratic interpolation
		NifStream( key.forward_tangent, file );
		NifStream( key.backward_tangent, file );
	} else if ( type == TBC_KEY ) {
		//Uses TBC interpolation
		WriteFloat( key.tension, file);
		WriteFloat( key.bias, file);
		WriteFloat( key.continuity, file);
	}
}

template <class T> 
void NifStream( Key<T> const & key, ostream & file, uint version, int type ) {
	NifStream( key, file, version, (KeyType)type );
}

//The HexString function creates a formatted hex display of the given data for use in printing
//a debug string for information that is not understood
string HexString( const byte * src, uint len );

//Byte
ostream & operator<<( ostream & out, byte const & val );

}
#endif
