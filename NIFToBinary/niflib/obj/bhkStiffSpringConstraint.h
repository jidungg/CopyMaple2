/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#ifndef _BHKSTIFFSPRINGCONSTRAINT_H_
#define _BHKSTIFFSPRINGCONSTRAINT_H_

#include "AbhkConstraint.h"
namespace Niflib {

#include "../gen/obj_defines.h"

class bhkStiffSpringConstraint;
typedef Ref<bhkStiffSpringConstraint> bhkStiffSpringConstraintRef;

/*!
 * bhkStiffSpringConstraint - A spring constraint.
 */

class NIFLIB_API bhkStiffSpringConstraint : public BHK_STIFF_SPRING_CONSTRAINT_PARENT {
public:
	bhkStiffSpringConstraint();
	~bhkStiffSpringConstraint();
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
	BHK_STIFF_SPRING_CONSTRAINT_MEMBERS
private:
	void InternalRead( istream& in, list<uint> & link_stack, unsigned int version, unsigned int user_version );
	void InternalWrite( ostream& out, map<NiObjectRef,uint> link_map, unsigned int version, unsigned int user_version ) const;
	string InternalAsString( bool verbose ) const;
	void InternalFixLinks( const map<unsigned,NiObjectRef> & objects, list<uint> & link_stack, unsigned int version, unsigned int user_version );
	list<NiObjectRef> InternalGetRefs() const;
};

}
#endif
