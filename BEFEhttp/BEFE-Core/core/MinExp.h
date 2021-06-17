//!befe-publish inc
//----------------------------------------------------------------------
// File: MinExp.h - Declarations for the MinExp lexical class
//----------------------------------------------------------------------
//
// The MinExp class implements Lexical "minimalist expression" expressed
// as a string.
//
// The following is an example of Minimalist Expression...
//
//   'e[x[it]]|q[uit]'
//
// This indicates the possible options...
//
//   'e'
//   'ex'
//   'exit'
//   'q'
//   'quit'
//
// Minimalist Expressions may have nested optional and/or conditional parts.
//
// For example...
//
//   "U[niversal]T[ext]F[ormat][[-]8]"
//
// So, the following are possible combinations of that one...
//
//   UTF
//   UniversalTF
//   UniversalTextF
//   UniversalTextFormat
//   UniversalTextFormat8
//   ...
//   UniversalTextFormat-8
//
// Normalisation...
//
//   When missing/unmatched parens ('[','(','}',']') are encountered they
//   are "normalised". This involves automatic reasonable placing of
//   matching parens at appropriate places.  For example, "...(..."
//   becomes "...(...)" etc.
// 
//   Leading and trailing '|' produces an empty string. So "|abc"
//   produces "" and "abc".
//
//   Original        Normalised
//   --------        ----------
//   "[...)...]"     "[(...)...]"
//   "[(...[...]..." "[([...[...]...)]"
//   
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef MINEXP_H // MINEXP_H...
#define MINEXP_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

//
// Constant declarations
//

#define LEX_MAXMINEXPDEPTH 32

//----------------------------------------------------------------------
//
// Class MinExp
//

class MinExp { // Class MinExp...

  // Friend classes
  friend class TestMinExp;

  //
  // Public instance methods
  //

  // C++ Lifecycle methods
  public: MinExp();                            // Default constructor
  public: MinExp(String const &str);           // String constructor
  public: MinExp(char const *str);             // char const * constructor
  public: ~MinExp();                           // Destructor
  
  // BEFE Lifecycle methods
  public: Status  StartUp();
  public: Status  ShutDown();
  public: Status  Reset();
  public: Boolean IsNull() const;
  public: Status  SetNull();
  public: Boolean IsEmpty() const;
  public: Status  SetEmpty();  
  public: Status  MoveFrom(MinExp const &that);
  public: Status  CopyFrom(MinExp const &that);
  public: MinExp &Consumable() const;
  public: MinExp &NotConsumable() const;
  public: Boolean IsConsumable() const;
  public: Int     Length() const;               // Return number of permutations in the MinExp
  public: Int     Count() const;                // Return number of permutations in the MinExp
  
  // Getting/Setting
  public: Status  Set(String const &str);       // Set the MinExp specification
  public: Status  Set(char const *str);         // Set the MinExp specification
  
  // Query
  public: String  Get(Int i) const;              // Get given Exp (permutation)
  public: Status  Get(Int i, String &exp) const; // Get given Exp (permutation)

  public: String const &Longest();               // Get the "longest" one
  public: String const &Shortest();              // Get the "shortest" one
  
  // Matching
  public: Boolean Contains(String const &that) const;
  public: Boolean Contains(char const *that) const;
  public: Boolean Match(String const &that) const;
  public: Boolean MatchSensitive(String const &that) const;
  public: Boolean MatchInsensitive(String const &that) const;
  
  // Formatting (for now)
  public: Status  ToStrings(Strings &theStrings);
  public: Strings ToStrings();

  // C++ Operators: []
  //
  // Note: Invalid subscripts return NULL (as opposed to empty) String
  public: String const &operator[] (Int index) const;

  //
  // Protected instance members
  //

  protected: String  theSpec;
  protected: Strings theStrings;
  protected: Int     shortIdx;
  protected: Int     longIdx;
  
  // Class members...
  protected: static String  bogusIndex; // Used when bogus index passed

  //
  // Protected instance methods
  //

  protected: Status  Parse();

}; // ...Class MinExp

} // ...Namespace BEFE

#endif // ...MINEXP_H
