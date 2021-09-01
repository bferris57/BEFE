//----------------------------------------------------------------------
// File: BmsgLine.h - Implementation of the BmsgLine Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "Bmsg.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// BEFE Lifecycle...
//

Status BmsgLine::StartUp() {

  Status retStatus;  
  //Status status;
  
  isConsumable = false;
  BEFE::SetNull(lineNo);
  BEFE::SetNull(numTokens);
  token0 = NULL;
  retStatus = tokens.StartUp();
  
  return retStatus;
  
}

Status BmsgLine::ShutDown() {

  Status retStatus;  
  //Status status;
  
  retStatus = tokens.ShutDown();
  
  return retStatus;
  
}

Status BmsgLine::Reset() {

  Status retStatus;
  Status status;

  retStatus = ShutDown();
  status    = StartUp();

  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Status BmsgLine::MoveFrom(BmsgLine const &that) {
  
  Status retStatus;
  Status status;

  if ((BmsgLine *)&that == this)
    retStatus = Error::None;
  else {
    retStatus = ShutDown();
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(BmsgLine));
    status = ((BmsgLine *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;
  }
  NotConsumable();
  
  return retStatus;
  
}

Status BmsgLine::CopyFrom(BmsgLine const &that) {

  Status status;  
  
  if (this == (BmsgLine *)&that)
    status = Error::None;
  else {
    if (IsConsumable())
      status = MoveFrom(that);
    else {
      isConsumable = false;      
      BEFE::SetNull((void **)&token0);
      status = tokens.CopyFrom(that.tokens);
    }

  }
  
  return status;
  
}

Boolean BmsgLine::IsNull() const {
  return (!isConsumable && tokens.IsNull());
}

Status BmsgLine::SetNull() {
  return Reset();
}

Boolean BmsgLine::IsEmpty() const {
  return IsNull();
}

Status BmsgLine::SetEmpty() {
  return Reset();
}

UInt BmsgLine::Length() const {
  return tokens.Length();
}

UInt BmsgLine::Size() const {
  return tokens.Size();
}

Boolean BmsgLine::IsConsumable() const {
  return isConsumable;
}

BmsgLine &BmsgLine::Consumable() const {
  ((BmsgLine *)this)->isConsumable = true;
  return *((BmsgLine *)this);
}

BmsgLine &BmsgLine::NotConsumable() const {
  ((BmsgLine *)this)->isConsumable = false;
  return *((BmsgLine *)this);
}

#if DEBUG

Strings BmsgLine::Format() {

	Strings    out;

	Strings    idxs;
	Strings    types;
	Strings    colNos;
	Strings    values;

	UInt       idxsMax   = 0;
	UInt       colNosMax = 0;
	UInt       typesMax  = 0;

  UInt       tokIdx;
  UInt       i;
  BmsgToken *tok;

  if (numTokens == 0)
  	return out;

  // Build the output columns...
  idxs.Append("Index");
  idxs.Append("-----");
  colNos.Append("Column");
  colNos.Append("------");
  types.Append("Type");
  types.Append("----");
  values.Append("Value");
  values.Append("-----");

	for (tokIdx = 0; tokIdx < numTokens; tokIdx++) {

		String s;

		tok = token0 + tokIdx;

		idxs.Append(String((Int)tokIdx));
		colNos.Append(String((Int)tok->colNo));
		switch (tok->type) {
		  case BmsgToken::TypeUnknown:       s = String("Unknown");     break;
  		case BmsgToken::TypeError:         s = String("Error");       break;
  		case BmsgToken::TypeWarning:       s = String("Warning");     break;
  		case BmsgToken::TypeBeginEllipsis: s = String("Ellipsis");    break;
  		case BmsgToken::TypeEndEllipsis:   s = String("EndEllipsis"); break;
  		case BmsgToken::TypeName:          s = String("Name");        break;
  		case BmsgToken::TypeNumber:        s = String("Number");      break;
  		case BmsgToken::TypeEquals:        s = String("Equals");      break;
  		case BmsgToken::TypeRange:         s = String("Range");       break;
  		case BmsgToken::TypeSeparator:     s = String("Separator");   break;
  		case BmsgToken::TypeString:        s = String("String");      break;
  		default:
  			s = String("???")+String((Int)tok->type)+"???";
		}
		types.Append(s);
		values.Append(tok->value);

	}

	// Compute column max lens...
	for (i=0; i < idxs.Length();   i++) if (idxs[i].Length()   > idxsMax)   idxsMax   = idxs[i].Length();
	for (i=0; i < colNos.Length(); i++) if (colNos[i].Length() > idxsMax)   colNosMax = colNos[i].Length();
	for (i=0; i < types.Length();  i++) if (types[i].Length()  > typesMax)  typesMax  = types[i].Length();

	// Pad all the columns
	for (i=0; i < idxs.Length();   i++) idxs[i].Pad(idxsMax);
	for (i=0; i < colNos.Length(); i++) colNos[i].Pad(colNosMax);
	for (i=0; i < types.Length();  i++) types[i].Pad(typesMax);

	// Form the output...
	for (i=0; i < idxs.Length(); i++)
		out.Append(idxs[i]+' '+colNos[i]+' '+types[i]+' '+values[i]);

	return out.Consumable();

}

void BmsgLine::Display() {
	Display(String());
	return;
}

void BmsgLine::Display(String indent) {

	Strings out = Format();

	Cout << indent << "Line " << lineNo+1 << "...\n";
	if (out.Length() == 0)
		Cout << indent << "***Empty***\n";
	else {
  	for (UInt i=0; i < out.Length(); i++)
      Cout << indent << "  " << out[i] << '\n';
	}

	return;

}

#endif

} // ...Namespace BEFE
