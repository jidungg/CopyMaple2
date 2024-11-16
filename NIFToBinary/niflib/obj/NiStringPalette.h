/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#ifndef _NISTRINGPALETTE_H_
#define _NISTRINGPALETTE_H_

#include "NiObject.h"

// Include structures
#include "../gen/StringPalette.h"
namespace Niflib {


#include "../gen/obj_defines.h"

class NiStringPalette;
typedef Ref<NiStringPalette> NiStringPaletteRef;

/*!
 * NiStringPalette - List of 0x00-seperated strings, which are names of
 * controlled blocks and controller types. Used in .kf files in
 * conjunction with NiControllerSequence.
 */

class NIFLIB_API NiStringPalette : public NI_STRING_PALETTE_PARENT {
public:
	NiStringPalette();
	~NiStringPalette();
	//Run-Time Type Information
	static const Type & TypeConst() { return TYPE; }
private:
	static const Type TYPE;
public:
	virtual void Read( istream& in, list<uint> & link_stack, unsigned int version, unsigned int user_version );
	virtual void Write( ostream& out, map<NiObjectRef,uint> link_map, unsigned int version, unsigned int user_version ) const;
	virtual string asString( bool verbose = false ) const;
	virtual void FixLinks( const map<unsigned,NiObjectRef> & objects, list<uint> & link_stack, unsigned int version, unsigned int user_version );
	virtual list<NiObjectRef> GetRefs() const;
	virtual const Type & GetType() const;

	string GetPaletteString() const;
	void SetPaletteString( const string & n );
	string GetSubStr( short offset ) const;
	unsigned int AddSubStr( const string & n );

protected:
	NI_STRING_PALETTE_MEMBERS
private:
	void InternalRead( istream& in, list<uint> & link_stack, unsigned int version, unsigned int user_version );
	void InternalWrite( ostream& out, map<NiObjectRef,uint> link_map, unsigned int version, unsigned int user_version ) const;
	string InternalAsString( bool verbose ) const;
	void InternalFixLinks( const map<unsigned,NiObjectRef> & objects, list<uint> & link_stack, unsigned int version, unsigned int user_version );
	list<NiObjectRef> InternalGetRefs() const;
};

}
#endif
