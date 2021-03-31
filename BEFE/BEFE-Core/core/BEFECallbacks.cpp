//----------------------------------------------------------------------
// File: BEFEEnviron.cpp - Class Befe Environment Variable methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"

namespace BEFE { // Namespace BEFE...

// Handy Macros...
#define THIS (*(Befe *)this)

Status Befe::RegisterCallback(String const &name, void *callBack) {
	
	Status status;
	Int    someInt;
	
	// Make sure callbacks is set to "Insensitive" if it's time...
	if (callbacks.Length() == 0) {
		status = callbacks.SetInsensitive();
		if (status) goto SOMEERROR;
	}
	
	// See if it's there...
	status = FindCallback(name, (void **)&someInt);
 	if (status == 0) goto ISREG;
	if (status != Error::BEFECallbackNotRegistered) goto SOMEERROR;
	
	// Register it...
	someInt = (Int)callBack;
	status = callbacks.Set(name, someInt);
	if (status) goto SOMEERROR;
	
	// Handle errors
	while (false) {
		ISREG:     status = Error::BEFECallbackIsRegistered; break;
		SOMEERROR:                                           break;
	}
	
	return status;
	
}

Status Befe::RemoveCallback(String const &name) {

	Status status;

	// Make sure it's there...
	if (!callbacks.HasKey(name)) goto NOTFOUND;

  // Remove it...
  status = callbacks.Remove(name);
	if (status) goto SOMEERROR;
	
	// Handle errors
	while (false) {
		NOTFOUND:  status = Error::BEFECallbackNotRegistered; break;
		SOMEERROR:                                            break;
	}
	
	return status;
	
}

Status Befe::FindCallback(String const &name, void **callBack) {
	
	Status status;
	Int    someInt;
	
	// Make sure we were given an address to return it in...
	if (callBack == NULL) goto BADPARAMETER;
	
	// See if it's there...
	status = callbacks.Get(name, someInt);
	if (status == Error::IndexNotFound) goto NOTFOUND;
	if (status) goto SOMEERROR;
	
	// Return it
	*callBack = (void *)someInt;
	
	// Handle errors
	status = Error::None;
	while (false) {
		BADPARAMETER: status = Error::InvalidParameter;          break;
		NOTFOUND:     status = Error::BEFECallbackNotRegistered; break;
		SOMEERROR:                                               break;
	}
	
	return status;
	
}

} // ...Namespace BEFE