//----------------------------------------------------------------------
// File: BEFEEnviron.cpp - Class Befe Environment Variable methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"

namespace BEFE { // Namespace BEFE...

// Handy Macros...
#define THIS (*(Befe *)this)

Status Befe::RefreshEnvironment() {
  
  Status       status;
  NamedStrings values;
  
  if (!TheProcess) goto NOPROCESS;

  haveEnvValues = true;
  status = TheProcess->GetEnvironmentValues(values);
  if (status) goto SOMEERROR;
  status = envValues.Mutable();
  if (status) goto SOMEERROR;
  status = envValues.MoveFrom(values);
  if (status) goto SOMEERROR;
  //status = envValues.MakeImmutable();
  //if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    NOPROCESS: status = Error::InternalNoTheProcess; break;
    SOMEERROR:                                       break;
  }
  
  return status;
  
}

Status Befe::ClearEnvironment() {
  
  Status       status;
  
  if (!TheProcess) goto NOPROCESS;
  
  status = envValues.Mutable();
  if (status) goto SOMEERROR;
  status = envValues.SetEmpty();
  if (status) goto SOMEERROR;
  //status = envValues.MakeImmutable();
  //if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    NOPROCESS: status = Error::InternalNoTheProcess; break;
    SOMEERROR:                                       break;
  }
  
  return status;
  
}

Boolean Befe::IsEnvironmentName(String const &name) {
  LoadEnvValues();
  return envValues.HasKey(name);
}

Strings Befe::GetEnvironmentNames() {
  
  //Status  status;
  Strings envNames;

  /*status =*/ LoadEnvValues();
  /*status =*/ GetEnvironmentNames(envNames);
  
  return envNames.Consumable();
  
}

Status Befe::GetEnvironmentNames(Strings &names) {
  
  Status status;

  status = LoadEnvValues();
  if (status) goto SOMEERROR;
  status = envValues.Keys(names);
  if (status) goto SOMEERROR;  
  status = names.Sort();
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    SOMEERROR:                                       break;
  }
  
  return status;
  
}

String Befe::GetEnvironmentValue(String const &name) {
  LoadEnvValues();
  return envValues.Get(name);
}

Status Befe::SetEnvironmentValue(String const &name, String const &value) {
  LoadEnvValues();
  return envValues.Set(name, value);
}

Status Befe::RemoveEnvironmentName(String const &name) {
  LoadEnvValues();
  return envValues.Remove(name);
}

NamedStrings const *Befe::GetEnvironmentValues() {
  LoadEnvValues();
  return (NamedStrings const *)&envValues;
}

Status Befe::GetEnvironmentValues(NamedStrings &values) {
  
  Status status;

  LoadEnvValues();  
  status = values.CopyFrom(envValues);
  
  return status;
  
}

String Befe::SubstituteEnvironmentValues(String const &inStr) {
  
  Status status;
  String outStr;
  
  LoadEnvValues();
  
  status = Befe::SubstituteEnvironmentValues(inStr, outStr);
  if (status)
    outStr = inStr;
    
  return outStr.Consumable();
  
}

Status Befe::SubstituteEnvironmentValues(String const &inStr, String &outStr) {
  
  Status status;
  String substBegin;
  String substEnd;
  
  substBegin = envValues.Get("BEFE_SubstBegin");
  substEnd   = envValues.Get("BEFE_SubstEnd");
  if (substBegin.Length() == 0) substBegin = "${";
  if (substEnd.Length()   == 0) substEnd   = '}';

  status = BEFE::Substitute(inStr, envValues, substBegin, substEnd, outStr);
  if (status)
    outStr = inStr;

  return status;
  
}

//----------------------------------------------------------------------
//
// Protected methods
//

Status Befe::LoadEnvValues() const {

  Status status;
  
  if (!haveEnvValues) {
    THIS.haveEnvValues = true;
    status = TheProcess->GetEnvironmentValues(THIS.envValues);
    if (status) goto SOMEERROR;
    //status = THIS.envValues.MakeImmutable();
    //if (status) goto SOMEERROR;
  }
  
  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }
  
  return status;

}

} // ...Namespace BEFE
