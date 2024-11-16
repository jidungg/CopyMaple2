/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#ifndef _ABHKRAGDOLLCONSTRAINT_H_
#define _ABHKRAGDOLLCONSTRAINT_H_

#include "AbhkConstraint.h"
namespace Niflib {


#include "../gen/obj_defines.h"

class AbhkRagdollConstraint;
typedef Ref<AbhkRagdollConstraint> AbhkRagdollConstraintRef;

/*!
 * AbhkRagdollConstraint - Ragdoll constraint.
 */

class NIFLIB_API AbhkRagdollConstraint : public ABHK_RAGDOLL_CONSTRAINT_PARENT {
public:
	AbhkRagdollConstraint();
	~AbhkRagdollConstraint();
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

protected:
	ABHK_RAGDOLL_CONSTRAINT_MEMBERS
private:
	void InternalRead( istream& in, list<uint> & link_stack, unsigned int version, unsigned int user_version );
	void InternalWrite( ostream& out, map<NiObjectRef,uint> link_map, unsigned int version, unsigned int user_version ) const;
	string InternalAsString( bool verbose ) const;
	void InternalFixLinks( const map<unsigned,NiObjectRef> & objects, list<uint> & link_stack, unsigned int version, unsigned int user_version );
	list<NiObjectRef> InternalGetRefs() const;
};

}
#endif
