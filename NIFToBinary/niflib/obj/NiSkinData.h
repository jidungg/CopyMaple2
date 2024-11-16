/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#ifndef _NISKINDATA_H_
#define _NISKINDATA_H_

#include "NiObject.h"
// Include structures
#include "../Ref.h"
#include "../gen/SkinData.h"
namespace Niflib {

// Forward define of referenced blocks
class NiSkinPartition;
class NiGeometry;

#include "../gen/obj_defines.h"

class NiSkinData;
typedef Ref<NiSkinData> NiSkinDataRef;

/*!
 * NiSkinData - Skinning data.
 */

class NIFLIB_API NiSkinData : public NI_SKIN_DATA_PARENT {
public:
	NiSkinData();

	/*!
	 * This constructor is called by NiGeometry when it creates a new skin
	 * instance using the BindSkin function.
	 */
	NiSkinData( const Ref<NiGeometry> & owner );

	~NiSkinData();

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

	void ResetOffsets( const Ref<NiGeometry> & owner );
	Matrix44 GetOverallTransform() const;
	void SetOverallTransform( const Matrix44 & transform );
	uint GetBoneCount() const;
	Matrix44 GetBoneTransform( uint bone_index ) const;
	vector<SkinWeight> GetBoneWeights( uint bone_index ) const;
	void SetBoneWeights( uint bone_index, const vector<SkinWeight> & n, Vector3 center, float radius );

	void NormalizeWeights( unsigned numVertices );
	
	Ref<NiSkinPartition> GetSkinPartition() const;
	void SetSkinPartition(Ref<NiSkinPartition> skinPartition);

protected:
	NI_SKIN_DATA_MEMBERS
private:
	void InternalRead( istream& in, list<uint> & link_stack, unsigned int version, unsigned int user_version );
	void InternalWrite( ostream& out, map<NiObjectRef,uint> link_map, unsigned int version, unsigned int user_version ) const;
	string InternalAsString( bool verbose ) const;
	void InternalFixLinks( const map<unsigned,NiObjectRef> & objects, list<uint> & link_stack, unsigned int version, unsigned int user_version );
	list<NiObjectRef> InternalGetRefs() const;
};

}
#endif
