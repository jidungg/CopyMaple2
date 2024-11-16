/* Copyright (c) 2006, NIF File Format Library and Tools
All rights reserved.  Please see niflib.h for licence. */

#ifndef _CONTROLLERLINK_H_
#define _CONTROLLERLINK_H_

#include "../NIF_IO.h"

// Include structures
#include "../Ref.h"
namespace Niflib {

// Forward define of referenced blocks
class NiTimeController;
class NiInterpolator;
class NiObject;
class NiStringPalette;

/*!
 * In a .kf file, this links to a controllable object, via its name (or
 * for version 10.2.0.0 and up, a link and offset to a NiStringPalette
 * that contains the name), and a sequence of interpolators that apply to
 * this controllable object, via links.
 */
struct NIFLIB_API ControllerLink {
	/*! Default Constructor */
	ControllerLink();
	/*! Default Destructor */
	~ControllerLink();
	/*!
	 * Name of a controllable object in another NIF file.
	 */
	string targetName;
	/*!
	 * Link to a controller.
	 */
	Ref<NiTimeController > controller;
	/*!
	 * Link to an interpolator.
	 */
	Ref<NiInterpolator > interpolator;
	/*!
	 * Unknown.
	 */
	Ref<NiObject > unknownLink2;
	/*!
	 * Unknown.
	 */
	ushort unknownShort0;
	/*!
	 * Idle animations tend to have low values for this, and NIF objects that
	 * have high values tend to correspond with the important parts of the
	 * animation.
	 */
	byte priority_;
	/*!
	 * Refers to the NiStringPalette which contains the name of the
	 * controlled NIF object.
	 */
	Ref<NiStringPalette > stringPalette;
	/*!
	 * The name of the animated node.
	 */
	string nodeName;
	/*!
	 * Offset in the string palette where the name of the controlled node
	 * (NiNode, NiTriShape, ...) starts.
	 */
	uint nodeNameOffset;
	/*!
	 * Name of the property (NiMaterialProperty, ...), if this controller
	 * controls a property.
	 */
	string propertyType;
	/*!
	 * Offset in the string palette where the property (NiMaterialProperty,
	 * ...) starts, if this controller controls a property. Otherwise, -1.
	 */
	uint propertyTypeOffset;
	/*!
	 * Probably the object type name of the controller in the NIF file that
	 * is child of the controlled object.
	 */
	string controllerType;
	/*!
	 * Apparently the offset in the string palette of some type of controller
	 * related to Interpolator (for example, a 'NiTransformInterpolator' will
	 * have here a 'NiTransformController', etc.). Sometimes the type of
	 * controller that links to the interpolator. Probably it refers to the
	 * controller in the NIF file that is child of the controlled object, via
	 * its type name.
	 */
	uint controllerTypeOffset;
	/*!
	 * Some variable string (such as 'SELF_ILLUM', '0-0-TT_TRANSLATE_U',
	 * 'tongue_out', etc.).
	 */
	string variable1;
	/*!
	 * Offset in the string palette where some variable string starts (such
	 * as 'SELF_ILLUM', '0-0-TT_TRANSLATE_U', 'tongue_out', etc.). Usually,
	 * -1.
	 */
	uint variableOffset1;
	/*!
	 * Another variable string, apparently used for particle system
	 * controllers.
	 */
	string variable2;
	/*!
	 * Offset in the string palette where some variable string starts (so far
	 * only 'EmitterActive' and 'BirthRate' have been observed in official
	 * files, used for particle system controllers). Usually, -1.
	 */
	uint variableOffset2;
};

}
#endif
