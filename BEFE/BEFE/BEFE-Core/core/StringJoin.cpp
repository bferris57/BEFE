//----------------------------------------------------------------------
// File: StringJoin.h - Implementation of the String.Join methods
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// Note: Especially helpful is the technique of doing the following...
//
//         ...
//         fullString = String().Join(somestrings);
//         ...
//
//       This joins all the separate strings together without any
//       separator.
//
//       And, another trick for doing Multiple String Replaceent in
//       a String, instead of using String.Find and String.Replace
//       single string in a loop is to do this in one fell swoop...
//
//         String source;
//         String oldVal;
//         String newVal;
//         ...
//         source = "abc;def;%replace%;efg;%replace%...";
//         oldVal = "%replace";
//         newVal = "╟─╢";
//         if (source.Count(oldVal))
//           source = source.SplitToStrings(oldVal).Join(newVal);
//         ...
//       
//       Once that fragment is executed, and assuming you're not
//       out of memory or some other error occurs internally,
//       you'll end up with the string in 'source' containing the
//       following String...
//
//         "abc;def;╟─╢;efg;╟─╢..."
//
//       Pretty cool, eh?  BTW... The 'if (source.Count...' was at
//       little show of attention to speed because, if the replacement
//       string doesn't occur in the source string, there's no reason
//       to go to the bother of splitting and rejoining.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"

namespace BEFE { // Namespace BEFE...

Status String::Join(Strings const &strings, String &outString) const {
  
  Status status;
  
  status = strings.Join(*this, outString);
  
  return status;
  
}

String String::Join(Strings const &strings) const {

  String that;
  
  strings.Join(*this, that);
  
  return that.Consumable();

}

} // ...Namespace BEFE