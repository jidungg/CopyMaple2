/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#ifndef _NICONTROLLERSEQUENCE_H_
#define _NICONTROLLERSEQUENCE_H_

#include "NiObject.h"
// Include structures
#include "../gen/ControllerLink.h"
#include "../Ref.h"
namespace Niflib {

// Forward define of referenced blocks
class NiTextKeyExtraData;
class NiControllerManager;
class NiStringPalette;
class NiTimeController;
class NiSingleInterpolatorController;

#include "../gen/obj_defines.h"

class NiControllerSequence;
typedef Ref<NiControllerSequence> NiControllerSequenceRef;

/*!
 * NiControllerSequence - Root node in .kf files (version 10.0.1.0 and up).
 */

class NIFLIB_API NiControllerSequence : public NI_CONTROLLER_SEQUENCE_PARENT {
public:
	NiControllerSequence();
	~NiControllerSequence();
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

	/*!
	 * Name of this NiControllerSequence object. This is also the name of the action
	 * associated with this file. For instance, if the original NIF file is called
	 * "demon.nif" and this animation file contains an attack sequence, then
	 * the file would be called "demon_attack1.kf" and this field would
	 * contain the string "attack1".
	 */
	string GetName() const;
	void SetName( const string & value );

	/*! Sets the name and block reference to the NiTextKeyExtraData block which will be used by this controller sequence to specify the keyframe labels or "notes."
	 * \param[in] txt_key A reference to the NiTextKeyExtraData object to use.
	 * \sa NiTextKeyExtraData
	 */
	void SetTextKey( const Ref<NiTextKeyExtraData> & txt_key );

	/*! Attatches a controler to this KF file for a KF file of version 10.2.0.0 or below.  Versions above this use interpolators.
	 * \param[in] obj A reference to the new NiTimeController to attach.
	 * \sa NiControllerSequence::ClearChildren, NiControllersequence::AddInterpolator
	 */
	void AddController( const Ref<NiTimeController> & obj );

	/*! Attatches an interpolator to this KF file for a KF file of version greater than 10.2.0.0.  Versions below this use controllers.
	 * \param[in] obj A reference to the new controller which has an interpolator to attach.
	 * \param[in] priority Used only in Oblivion to set the priority of one controller over another when the two are merged.
	 * \sa NiControllerSequence::ClearChildren, NiControllerSequence::AddController
	 */
	void AddInterpolator( const Ref<NiSingleInterpolatorController> & obj, byte priority = 0 );

	/*! Removes all controllers and interpolators from this Kf file root object.
	 * \sa NiControllerSequence::AddController, NiControllersequence::AddInterpolator
	 */
	void ClearControllerData();

	/*! Retrieves the data for the controllers or interpolators which are attached to this controller sequence.
	 * \return A vector containing the data for all controllers.
	 * \sa NiControllerSequence::AddController, NiControllersequence::AddInterpolator, ClearKfChildren
	 */
	vector<ControllerLink> GetControllerData() const;

	Ref<NiTextKeyExtraData> GetTextKeyExtraData() const;

	/*!
	 * Gets the animation frequency.
	 * \return The animation frequency.
	 */
	float GetFrequency() const;

	/*!
	 * Sets the animation frequency.
	 * \param[in] value The animation frequency.
	 */
	void SetFrequency( float value );

	/*!
	 * Gets the controller sequence start time.
	 * \return The controller sequence start time.
	 */
	float GetStartTime() const;

	/*!
	 * Sets the controller sequence start time.
	 * \param[in] value The controller sequence start time.
	 */
	void SetStartTime( float value );

	/*!
	 * Gets the controller sequence stop time.
	 * \return The conroller sequence stop time.
	 */
	float GetStopTime() const;
	void SetStopTime( float value );

	CycleType GetCycleType() const;
	void SetCycleType( CycleType n );

   /*! 
    * Gets the  number of controllers.
	* \return Number of total controllers in this sequence
	* \sa GetControllerData
	*/
   int GetNumControllers() const;

   /*! 
    * Gets controller priority.  Oblivion Specific.
	* \return Priority of a specific controller.
	* \param[in] controller The index of the controller to get the priority for.
	* \sa GetControllerData, GetNumControllers, SetControllerPriority
	*/
   int GetControllerPriority( int controller ) const;

   /*! 
    * Sets controller priority.  Oblivion Specific.
    * \param[in] controller The index of the controller to set the priority for.
	* \param[in] priority The amount of priority the controller should have.
    * \sa GetControllerData, GetNumControllers, GetControllerPriority
    */
   void SetControllerPriority( int controller, int priority );


	/*!
	 * Gets weight/priority of animation?
	 * \return The weight/priority of the animation?
	 */
	float GetWeight() const;

	/*!
	 * Sets weight/priority of animation?
	 * \param[in] value The weight/priority of the animation?
	 */
	void SetWeight( float value );

	/*!
	 * Gets the name of target node this controller acts on.
	 * \return The target node name.
	 */
	string GetTargetName() const;

	/*!
	 * Sets the name of target node this controller acts on.
	 * \param[in] value The target node name.
	 */
	void SetTargetName( const string & value );

protected:
   friend class NiControllerManager;
   NiControllerManager * GetParent() const;
   void SetParent(NiControllerManager *parent);

	NI_CONTROLLER_SEQUENCE_MEMBERS
private:
	void InternalRead( istream& in, list<uint> & link_stack, unsigned int version, unsigned int user_version );
	void InternalWrite( ostream& out, map<NiObjectRef,uint> link_map, unsigned int version, unsigned int user_version ) const;
	string InternalAsString( bool verbose ) const;
	void InternalFixLinks( const map<unsigned,NiObjectRef> & objects, list<uint> & link_stack, unsigned int version, unsigned int user_version );
	list<NiObjectRef> InternalGetRefs() const;
};

}
#endif
