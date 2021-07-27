//!befe-publish src
//----------------------------------------------------------------------
// File: TestVerTerm.cpp - Implementation of the TestMinExp Class
//----------------------------------------------------------------------
//
// The TestLexicon Class tests the functionality of Lexical Analysis
// related classes.
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"

#include "Test.h"
#include "MinExp.h"
#include "_MinExpStack.h"

namespace BEFE { // Namespace BEFE...

Status TestMinExp::Run() {

  struct TestData {

    const char *string;
    UInt        strings;

  } tests[] = {
  		          {"abc)def",                      1},
		            {"(def",                         1},
    		        {"abc)",                         1},
                {"[abc|bca)def(ghi|jkl",         5},
            	  {"(abc|def)ghi(jkl|mno)",        4},
            	  {")\\)\\[)([(",                  1},
                {"e]x[it",                       4},
                {"[e]x[it]",                     4},
                {"q[uit]",                       2},
                {"[e]x[it]|q[uit]",              6},
                {"[e]x[it]|q[uit]|",             7},
                {"a|(bc)",                       2},
                {"[e]x",                         2},
                {"a|b|c",                        3},
                {"|a|b|c",                       4},
                {"a|b|c|",                       4},
                {"(a|b|c|)",                     4},
                {"norm[al(ise|ize)]",            3},
    		        {"normal(ise|ize)",              2},
    		        {"empty[[]]braces",              1},
    		        {"\\xhi there\\",                1},
    		        {"e\\[xit\\]|q\\[uit\\]",        2},
    		        {"[def[ghi]]xxx|jkl",            4},
    		        {"[def[ghi]]xxx|",               4},
    		        {"[def[ghi]]xxx",                3},
    		        {"abc[def|ghi]xxx[jkl|mno]",     9},
    		        {"abcxxx[jkl|mno]",              3},
    		        {"abc[def|ghi]xxx[jkl|mno]|def", 10},
    		        {"abc(def|ghi)|jkl(mno|pqr)",    4},
    		        {"abc(def|ghi)",                 2},
    		        {"abc[def|ghi]",                 3},
                {"a[b[c[d[e[f[g][h]]]]]]",       9},
								{"[[[[[[[a]b]c]d]e]f]g]h",       8},
								{"cross[-]ref([erenc][ed])",     8},
    				    {NULL,                           0},
              };
  Status    status;
  UInt      numFailed;
  TestData *test;
  String    msg;
  MinExp    var;
  UInt      i;
  String    quoteLeadIn;
  String    quoteLeadOut;

	quoteLeadIn  = GetEnvironmentValue(String("BEFE_QuoteLeadIn"));
	quoteLeadOut = GetEnvironmentValue(String("BEFE_QuoteLeadOut"));

	if (quoteLeadIn.IsNull())  quoteLeadIn = '"';
	if (quoteLeadOut.IsNull()) quoteLeadOut = '"';

  numFailed = 0;

  if (!gQuiet)
    out << indent << "***Testing MinExp..." << Endl;

  for (test=tests; !IsNull(test->string); test++) { // For each test...

    msg = "      ";

    status = var.Set(test->string);

    if (gVerbose)
      Cout << "    MinExp " << quoteLeadIn << test->string << quoteLeadOut << "...";
    if (gVeryVerbose)
    	Cout << Endl;

    // If VeryVerbose, output the strings...
    if (gVeryVerbose) {
    	Strings formatted;
    	var.ToStrings(formatted);
      Cout << '\n';
      for (i=0; i < formatted.Length(); i++) {
        Cout << "      " << formatted[i] << Endl;
      }
      Cout << Endl;
    }
    if (gVerbose && !gVeryVerbose)
    	Cout << ' ' << var.Length() << " match" << ((var.Length() == 1)?"":"es") << Endl;
    if (status) goto SETFAILED;
    
    // A little validation...
    if (var.theStrings.Length() != test->strings) goto BADNUMSTRINGS;
    
    continue;

  SETFAILED:
    msg += "!!! ";
    msg += ErrorString(status);
    msg += " from var.Set";
    Warn(msg);
    numFailed++;
    continue;
  BADNUMSTRINGS:
  /**/
    msg += "!!! Expected ";
    msg += test->strings;
    msg += " strings, got ";
    msg += var.theStrings.Length();
    Warn(msg);
    numFailed++;
    continue;

  } // ...For each test

  return (numFailed) ? Error::Failed : Error::None;

}

} // ...Namespace BEFE
