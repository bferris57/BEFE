//----------------------------------------------------------------------
// File: CommandLayer_Lang.cpp - Implementaion of the CommandLayer_Lang class
//----------------------------------------------------------------------
//
// The CommandLayer class is responsible for parsing and executing
// BEFE-Lex command line arguments and command lines for a closely
// related set of commands.
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Error.h"
#include "StringGeneral.h"
#include "MinExp.h"
#include "CommandShell.h"
#include "CommandLayer_BEFE.h"
#include "CommandLayer_OS.h"
#include "CommandLayer_Lang.h"
#include "cmdutils.h"
#include "Help.h"
#include "LexFormat.h"
#include "LexLanguage.h"
#include "LexLang.h"
#include "ToString.h"
#include "TestLexicon.h"

namespace BEFE { // Namespace BEFE...

//----------------------------------------------------------------------
//
// Command Specifications
//

static CommandSpec commandSpecs[] = {
  // Exit...
   {"e[xit]"                          , (CommandHandler)CommandLayer_BEFE_Exit       , true}
  ,{"x"                               , (CommandHandler)CommandLayer_BEFE_Exit       , true}
  ,{"q[uit]"                          , (CommandHandler)CommandLayer_BEFE_Exit       , true}
  // Finished...
  ,{".."                              , (CommandHandler)CommandLayer_BEFE_Finish     , true}
  ,{"done"                            , (CommandHandler)CommandLayer_BEFE_Finish     , true}
  // Help...
  ,{"h[elp]"                          , (CommandHandler)CommandLayer_Lang_Help       , true}
  ,{"?"                               , (CommandHandler)CommandLayer_Lang_Help       , true}
  // Switches...
  ,{"Sw[itch[es]]"                    , (CommandHandler)CommandLayer_BEFE_Switches   , true}
  ,{"Arg[ument][s]"                   , (CommandHandler)CommandLayer_BEFE_Switches   , true}
  ,{"Opt[ion][s]"                     , (CommandHandler)CommandLayer_BEFE_Switches   , true}
  ,{"S[how]A[rg[ument][s]]"           , (CommandHandler)CommandLayer_BEFE_ShowArgs   , true}
  ,{"S[how]Sw[itch[es]]"              , (CommandHandler)CommandLayer_BEFE_ShowArgs   , true}
  ,{"S[how]Opt[ion][s]"               , (CommandHandler)CommandLayer_BEFE_ShowArgs   , true}
  // Date/Time...
  ,{"Date"                            , (CommandHandler)CommandLayer_BEFE_DateTime   , true}
  ,{"Time"                            , (CommandHandler)CommandLayer_BEFE_DateTime   , true}
  // OS Directory commands
  ,{"pwd"                             , (CommandHandler)CommandLayer_OS_pwd          , true}
  ,{"cwd"                             , (CommandHandler)CommandLayer_OS_cwd          , true}
  ,{"cd"                              , (CommandHandler)CommandLayer_OS_cwd          , true}
  ,{"dir"                             , (CommandHandler)CommandLayer_OS_ls           , true}
  ,{"ls"                              , (CommandHandler)CommandLayer_OS_ls           , true}
  ,{"m[o]v[e]"                        , (CommandHandler)CommandLayer_OS_mv           , true}
  ,{"ren[ame]"                        , (CommandHandler)CommandLayer_OS_mv           , true}
  ,{"c[o]p[y]"                        , (CommandHandler)CommandLayer_OS_cp           , true}
  ,{"r[e]m[ove]"                      , (CommandHandler)CommandLayer_OS_rm           , true}
  ,{"del[ete]"                        , (CommandHandler)CommandLayer_OS_rm           , true}
  ,{"path"                            , (CommandHandler)CommandLayer_OS_path         , true}
  ,{"which"                           , (CommandHandler)CommandLayer_OS_which        , true}
  ,{"cat"                             , (CommandHandler)CommandLayer_OS_cat          , true}
  ,{"type"                            , (CommandHandler)CommandLayer_OS_cat          , true}
  // LexWs commands
  ,{"New"                             , (CommandHandler)CommandLayer_Lang_New        , true}
  ,{"Op[e]n"                          , (CommandHandler)CommandLayer_Lang_Open       , true}
  ,{"Cl[ose]"                         , (CommandHandler)CommandLayer_Lang_Close      , true}
  ,{"D[i][sp][lay]"                   , (CommandHandler)CommandLayer_Lang_Display    , true}
  ,{"P[r[i]n[t]]"                     , (CommandHandler)CommandLayer_Lang_Display    , true}
  ,{"Show"                            , (CommandHandler)CommandLayer_Lang_Display    , true}
  ,{"Bl[oc]k"                         , (CommandHandler)CommandLayer_Lang_Block      , true}
  ,{"Cur[rent]Lang[uage]"             , (CommandHandler)CommandLayer_Lang_CurLang    , true}
  ,{"Lex[icon]"                       , (CommandHandler)CommandLayer_Lang_Lex        , true}
  ,{"Lexical"                         , (CommandHandler)CommandLayer_Lang_Lex        , true}
  ,{"Exp[r[ess[ion]]]"                , (CommandHandler)CommandLayer_Lang_Expr       , true}
  ,{"Mat[ch]T[yp[e]]"                 , (CommandHandler)CommandLayer_Lang_MatchType  , true}
  ,{"Pat[tern]"                       , (CommandHandler)CommandLayer_Lang_Pattern    , true}
  ,{"Mat[ch][es]"                     , (CommandHandler)CommandLayer_Lang_Match      , true}
  ,{"Min[imal[ist]]Exp[ress[ion]]"    , (CommandHandler)CommandLayer_Lang_MinExp     , true}
  ,{"Test"                            , (CommandHandler)CommandLayer_Lang_Test       , true}
  // Other
	,{"Echo"                            , (CommandHandler)CommandLayer_BEFE_Echo       , true}
  ,{"Cl[ear]S[creen]"                 , (CommandHandler)CommandLayer_BEFE_ClearScreen, true}


  ,{NULL,NULL,true}
};

//----------------------------------------------------------------------
//
// C++ Lifecycle
//

CommandLayer_Lang::CommandLayer_Lang() {
  StartUp();
}

CommandLayer_Lang::~CommandLayer_Lang() {
  ShutDown();
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle methods...
//

Status CommandLayer_Lang::StartUp() { // CommandLayer_Lang.StartUp...

  Status retStatus;
  Status status;

  shell = NULL;
  retStatus = CommandLayer::StartUp();
  name   = "Lang";
  prompt = "Lang";

  status = lex.StartUp();
  if (status && !retStatus) retStatus = status;
  status = ws.StartUp();
  if (status && !retStatus) retStatus = status;  
  status = curLang.StartUp();
  if (status && !retStatus) retStatus = status;  
  status = lexStream.StartUp();
  if (status && !retStatus) retStatus = status;  
  status = match.StartUp();
  if (status && !retStatus) retStatus = status;  

  return retStatus;

} // ...CommandLayer.StartUp

Status CommandLayer_Lang::ShutDown() { // CommandLayer_Lang.ShutDown...

  Status retStatus;
  Status status;

  retStatus = CommandLayer::ShutDown();
  status = lex.ShutDown();
  if (status && !retStatus) retStatus = status;  
  status = ws.ShutDown();
  if (status && !retStatus) retStatus = status;  
  status = curLang.ShutDown();
  if (status && !retStatus) retStatus = status;  
  status = lexStream.ShutDown();
  if (status && !retStatus) retStatus = status;  
  status = lexStream.ShutDown();
  if (status && !retStatus) retStatus = status;  
  
  return retStatus;

} // ...CommandLayer_Lang.ShutDown

//----------------------------------------------------------------------
//
// Command Flow overrides
//

Status CommandLayer_Lang::Enter(BcmdCommand &cmd) {

  Status  status;
  BcmdArg arg;

  // If no arguments (other than the first one), don't do anything
  if (cmd.numArgs < 2) goto OK;

  // Make sure there's only 2 args
  if (cmd.numArgs > 2) {
    status = Error::CmdLang_ExpectedOneLang;
    status = cmd.SetError(2, status);
    if (status) goto SOMEERROR;
    goto OK;
  }

  // Quick hack to execute "Cur[rent]Lang[uage]" instead...
  arg = cmd.args[0];
  arg.value = "curlang";
  status = cmd.args.Set(0,arg);
  if (status) goto SOMEERROR;
  status = Execute(cmd);
  if (status) goto SOMEERROR;

  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Command Handler Registry
//

Status CommandLayer_Lang::RegisterHandlers() {

  Status status;

  status = CommandLayer::RegisterHandlers(commandSpecs);
  if (status) goto SOMEERROR;

  // Handle errors
  while (false) {
    SOMEERROR: break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Function: CommandLayer_Lang_Help - Handle Language 'help' command
//            

Status CommandLayer_Lang_Help(CommandLayer_Lang *layer, BcmdCommand &cmd) {

  Help("Lang");

  return Error::None;

}

//----------------------------------------------------------------------
//
// Function: CommandLayer_Lang_EnterLang - Handle BEFE 'Language' command
//            

Status CommandLayer_Lang_EnterLang(CommandLayer_Lang *layer, BcmdCommand &cmd) {

  Status        status;
  CommandShell *shell;

  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;
  
  status = shell->PushLayer("Lang", cmd);
  if (status) goto SOMEERROR;
  
  // Handle Errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }
  
  return status;

}

//----------------------------------------------------------------------
//
// Functions: CommandLayer_Lang_New   - Handle Language 'New' command
//            CommandLayer_Lang_Open  - Handle Language 'Open' command
//            CommandLayer_Lang_Close - Handle Language 'Close' command
//

Status CommandLayer_Lang_New(CommandLayer_Lang *layer, BcmdCommand &cmd) {  // CommandLayer_Lang_New...

  Status        status;
  CommandShell *shell;
  String        cwd;
  String        fileName;
  
  // Sanity checks...  
  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;
  
  // Pull arguments apart
  if (cmd.numArgs < 1) goto INTERNAL;
  if (cmd.numArgs > 2) {
    status = Error::CmdLang_ExpectedOneFile;
    status = cmd.SetError(2, status);
    goto SOMEERROR;
  }

  // Validate the new file name...
  fileName = cmd[1];
  status = ValidateNewFileName(fileName);
  if (status) goto SOMEERROR;
  
  // Close it if it's already open
  if (layer->ws.IsOpen()) {
    status = layer->ws._Close();
    if (status) goto SOMEERROR;
  }
  
  // Open it...
  status = layer->ws.SetFileName(fileName);
  if (status) goto SOMEERROR;
  if (!layer->lex.IsAttached(layer->ws)) {
    status = layer->lex.Attach(layer->ws);
    if (status) goto SOMEERROR;
  }
  status = layer->ws._Open();
  if (status) goto SOMEERROR;
  
  // Handle Errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;    break;
    INTERNAL:    status = Error::Internal;               break;
    SOMEERROR:                                           break;
  }
  if (status)
    status = cmd.SetError(0, status);

  return status;

} // ...CommandLayer_Lang_New

Status CommandLayer_Lang_Open(CommandLayer_Lang *layer, BcmdCommand &cmd) {  // CommandLayer_Lang_Open...

  Status        status;
  CommandShell *shell;
  String        cwd;
  String        fileName;
  
  // Sanity checks...  
  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;
  
  // Pull arguments apart
  if (cmd.numArgs < 1) goto INTERNAL;
  if (cmd.numArgs > 2) goto ONEARGONLY;
  
  // Make sure it's not already open
  if (layer->ws.IsOpen()) goto ISOPEN;
  
  // Validate the file name...
  fileName = cmd[1];
  status = ValidateExistingFileName(fileName);
  if (status) goto SOMEERROR;
  
  // Close it if it's already open
  if (layer->ws.IsOpen()) {
    status = layer->ws._Close();
    if (status) goto SOMEERROR;
  }
  
  // Open it...
  status = layer->ws.SetFileName(fileName);
  if (status) goto SOMEERROR;
  if (!layer->lex.IsAttached(layer->ws)) {
    status = layer->lex.Attach(layer->ws);
    if (status) goto SOMEERROR;
  }
  status = layer->ws._Open();
  if (status) goto SOMEERROR;
  
  // Handle Errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;        break;
    INTERNAL:    status = Error::Internal;                   break;
    ISOPEN:      status = Error::CmdLang_WorkspaceIsOpen;    break;
    ONEARGONLY:  status = Error::CmdLang_ExpectedOneBlockNo; break;
    SOMEERROR:                                               break;
  }
  if (status)
    status = cmd.SetError(0, status);

  return status;

} // ...CommandLayer_Lang_Open

Status CommandLayer_Lang_Close(CommandLayer_Lang *layer, BcmdCommand &cmd) {

  Status        status;
  CommandShell *shell;
  
  // Sanity checks...  
  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;
  
  // Pull arguments apart
  if (cmd.numArgs < 1) goto INTERNAL;
  if (cmd.numArgs > 1) goto HAVEARGS;
  
  // Make sure it's open
  if (!layer->ws.IsOpen()) goto NOTOPEN;
  
  // Close it
  status = layer->ws._Close();
  if (status) goto SOMEERROR;
  
  // Handle Errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;      break;
    INTERNAL:    status = Error::Internal;                 break;
    HAVEARGS:    status = Error::CmdLang_ExpectedNoArgs;   break;
    NOTOPEN:     status = Error::CmdLang_WorkspaceNotOpen; break;
    SOMEERROR:                                             break;
  }
  if (status)
    status = cmd.SetError(0, status);
  
  return status;

}

//----------------------------------------------------------------------
//
// Settings
//
// NOTE: We intentionally don't change the prompt in SetPrompt because
//       the CommandLayer_OS functions try setting it to 'cwd' but we
//       don't want that behaviour here.
//

Status CommandLayer_Lang::SetPrompt(String const &str) {
  return Error::None;
}

//----------------------------------------------------------------------
//
// Internal Methods
//

Status CommandLayer_Lang::_InitLex() {
  
  Status      status;
  LexLanguage theLang;
  
  // If not already initialised...
  if (lex.GetLanguageCount()) goto OK;
  
  // Build C++ Language...
  theLang.Reset();
  status = LexLangCpp(theLang);
  if (status) goto SOMEERROR;
  status = lex.RegisterLanguage(theLang);
  if (status) goto SOMEERROR;
  
  // Build BEFE Languages...
  theLang.Reset();
  status = LexLangBefe(theLang);
  if (status) goto SOMEERROR;
  status = lex.RegisterLanguage(theLang);
  if (status) goto SOMEERROR;
  theLang.Reset();

  // Attach the stream
  status = lex.Attach(lexStream);
  if (status) goto SOMEERROR;
  
  // Handle errors
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }
  
  return status;
  
}

//----------------------------------------------------------------------
//
// Functions: CommandLayer_Lang_Display - Handle Language 'Display' command
//

Status CommandLayer_Lang_Display(CommandLayer_Lang *layer, BcmdCommand &cmd) {  // CommandLayer_Lang_Display...

  Status        status;
  CommandShell *shell;
  String        arg;
  MinExp        what;
  String        msg;
  NamedStrings  subst;
  LexFormatOut  fmt;
  String        leadIn;
  String        leadOut;
  
  // Sanity checks...  
  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;
  
  if (cmd.numArgs < 1) goto INTERNAL;
  if (cmd.numArgs > 2) goto ONEARGONLY;

  // Default if no "what" to specify
  if (cmd.numArgs == 1)
    arg = "workspace";
  else
    arg = cmd[1];
    
  //
  // Depending on what was said...
  //

  fmt.indentCols = layer->indent.Length()*layer->indent.Multiplier() + 2;
  fmt.linesBefore = 1;
  fmt.linesAfter = 1;
  
  // Display Workspace...
  what.Set("W[ork]S[pace]");
  if (what.MatchInsensitive(arg)) {
    status = fmt.Display(layer->ws);
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Display Cache...
  what.Set("Cac[he]");
  if (what.MatchInsensitive(arg)) {
    status = fmt.DisplayCache();
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Display Languages...
  what.Set("Lang[uage]s");
  if (what.MatchInsensitive(arg)) {

    Strings     langNames;
    LexLanguage lang;
    UInt        maxLen;
    UInt        langIdx;
    Id          langId;
    String      langName;
    
    status = layer->_InitLex();
    if (status) goto SOMEERROR;
    langNames = layer->lex.GetLanguageNames();
    if (langNames.Length() == 0) goto NOLANGS;
    
    // Compute maximum language name length...
    maxLen = 0;
    for (langIdx=0; langIdx < langNames.Length(); langIdx++) {
      maxLen = Max(maxLen,langNames.Get(langIdx).Length());
    }
    
    // Display them...
    Cout << '\n';
    for (langIdx=0; langIdx < langNames.Length(); langIdx++) {
      langName = langNames.Get(langIdx);
      langId = layer->lex.GetLanguageId(langName);
      Cout << String(' ')*fmt.indentCols
           << "Language '" << langName << "': " << String(' ')*(maxLen-langName.Length())
           << "id = 0x" << ToHexString(langId, 8)
           << '\n';
    }
    
    Cout << '\n';    
    goto OK;

  }
  
  // Display Current Language...
  what.Set("Lang[uage]");
  if (what.MatchInsensitive(arg)) {
    
    // Make sure there's a current language
    status = layer->_InitLex();
    if (status) goto SOMEERROR;
    if (BEFE::IsNull(layer->curLang.GetId())) goto NOCURLANG;

    // Display it...
    status = fmt.DisplayLanguage(layer->curLang);
    if (status) goto SOMEERROR;

    goto OK;
  
  }
  
  // Display Current Match Type and Pattern...
  what.Set("Match");
  if (what.MatchInsensitive(arg)) {
    
    // Make sure there's a current Match...
    if (layer->match.type == Match::TypeUnknown ||
        layer->match.type >= Match::NumTypes) goto NOCURMATCH;
        
    // Display it...
    leadIn = GetEnvironmentValue("BEFE_QuoteLeadIn");
    if (leadIn.Length() == 0)
      leadIn = '"';
    leadOut = GetEnvironmentValue("BEFE_QuoteLeadOut");
    if (leadOut.Length() == 0)
      leadOut = '"';
    Cout << layer->indent << "Match: type = ";
    switch (layer->match.type) {
      case Match::TypeStar:     Cout << "Star";          break;
      case Match::TypeMinExp:   Cout << "MinExp";        break;
      case Match::TypeSQL:      Cout << "SQL";           break;
      case Match::TypePosixBRE: Cout << "PosixBRE";      break;
      case Match::TypePosixERE: Cout << "PosixERE";      break;
      default:                  Cout << CONSTR_StarNone; break;
    }
    Cout << ", pattern = " << leadIn << layer->match.pattern << leadOut << '\n';
    
    goto OK;
    
  }
  
  // Don't know what they said
  status = Error::CmdLang_CantDisplayWhat;
  msg = CmdStringMessage(status, arg);
  status = cmd.SetError(1, status, msg);
  
  // Handle Errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;       break;
    INTERNAL:    status = Error::Internal;                  break;
    ONEARGONLY:  status = Error::CmdLang_ExpectedOneFile;   break;
    NOLANGS:     status = Error::CmdLang_NoLanguages;       break;
    NOCURLANG:   status = Error::CmdLang_NoCurrentLanguage; break;
    NOCURMATCH:  status = Error::CmdLang_NoCurrentMatch;    break;
    OK:          status = Error::None;                      break;
    SOMEERROR:                                              break;
  }
  if (status)
    status = cmd.SetError(0, status);

  return status;

} // ...CommandLayer_Lang_Display

//----------------------------------------------------------------------
//
// Functions: CommandLayer_Lang_Block - Handle Language 'Block' command
//

Status CommandLayer_Lang_Block(CommandLayer_Lang *layer, BcmdCommand &cmd) {  // CommandLayer_Lang_Block...

  Status        status;
  CommandShell *shell;
  String        arg;
  Int32         blockNo;
  LexFormatOut  fmt;
  
  // Sanity checks...  
  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;
  
  if (cmd.numArgs != 2) goto ONEARGONLY;

  // Get the block number
  status = Int32FromString(cmd[1], blockNo);
  if (status) goto SOMEERROR;
  
  // Display it
  fmt.indentCols = layer->indent.Length()*layer->indent.Multiplier() + 2;
  fmt.linesBefore = 1;
  fmt.linesAfter = 1;
  status = fmt.DisplayBlock(layer->ws, blockNo);
  if (status) goto SOMEERROR;
  
  // Handle Errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;        break;
    ONEARGONLY:  status = Error::CmdLang_ExpectedOneBlockNo; break;
    SOMEERROR:                                               break;
  }
  if (status)
    status = cmd.SetError(0, status);
  
  return status;

} // ...CommandLayer_Lang_Block

//----------------------------------------------------------------------
//
// Functions: CommandLayer_Lang_CurLang - Handle Language 'CurLang' command
//

Status CommandLayer_Lang_CurLang(CommandLayer_Lang *layer, BcmdCommand &cmd) {  // CommandLayer_Lang_Lang...

  Status        status;
  CommandShell *shell;
  String        langName;
  Id            langId;
  LexFormatOut  fmt;
  
  // Sanity checks...  
  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;

  if (cmd.numArgs > 2) goto ONEARGMAX;

  // Make sure languages are registered  
  status = layer->_InitLex();
  if (status) goto SOMEERROR;

  // If no argument, display current language
  if (cmd.numArgs < 2) {
    if (layer->curLang.GetName().IsNull())
      goto NOCURLANG;
    fmt.indentCols = layer->indent.Length()*layer->indent.Multiplier() + 2;
    fmt.linesBefore = 1;
    fmt.linesAfter = 1;
    status = fmt.DisplayLanguage(layer->curLang);
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Get the language
  langName = cmd[1];
  status = layer->lex.GetLanguage(langName, layer->curLang);
  if (status) goto SOMEERROR;
    
  // Handle Errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;       break;
    ONEARGMAX:   status = Error::CmdLang_ExpectedOneLang;   break;
    NOCURLANG:   status = Error::CmdLang_NoCurrentLanguage; break;
    OK:          status = Error::None;                      break;
    SOMEERROR:                                              break;
  }
  if (status)
    status = cmd.SetError(0, status);
  
  return status;

} // ...CommandLayer_Lang_Lang

//----------------------------------------------------------------------
//
// Functions: CommandLayer_Lang_Lex - Handle Language 'Lex' command
//

Status CommandLayer_Lang_Lex(CommandLayer_Lang *layer, BcmdCommand &cmd) { // CommandLayer_Lang_Lex...

  Status        status;
  CommandShell *shell;
  String        cwd;
  String        fileName;
  LexFormatOut  fmt;
  LexToken      token;
  UInt          tokenNo;
  String        leadIn;
  String        leadOut;
  
  // Sanity checks...  
  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;
  
  // Get quote leadIn/leadOut...
  leadIn = GetEnvironmentValue("BEFE_QuoteLeadIn");
  if (leadIn.Length() == 0)
    leadIn = '"';
  leadOut = GetEnvironmentValue("BEFE_QuoteLeadOut");
  if (leadOut.Length() == 0)
    leadOut = '"';

  // Pull arguments apart
  if (cmd.numArgs < 1) goto INTERNAL;
  if (cmd.numArgs > 2) goto ONEARGONLY;
  
  // Validate the file name...
  fileName = cmd[1];
  status = ValidateExistingFileName(fileName);
  if (status) goto SOMEERROR;
  
  // Make sure there's a current language
  status = layer->_InitLex();
  if (status) goto SOMEERROR;
  if (BEFE::IsNull(layer->curLang.GetId())) goto NOCURLANG;

  // Setup the LexStream
  status = layer->lexStream.SetLanguageName(layer->curLang.GetName().Consumable());
  if (status) goto SOMEERROR;
  status = layer->lexStream.SetInputFileName(fileName);
  if (status) goto SOMEERROR;
  
  //
  // Display the tokens...
  //
  
  
  // Python display (it's easy this)...
  if (gPythonOutput) {

    LexFormatOut lexOut;
    Strings      output;
    UInt         numStrings;

    output = lexOut.ToStrings(layer->lexStream);
    numStrings = output.Length();
    for (UInt i=0; i < numStrings; i++)
    	Cout << output[i] << '\n';

  }

  // Normal display...
  else {

		tokenNo = 0;
		fmt.indentCols = 2;

		do {
			status = layer->lexStream.NextToken(token);
			if (status) goto SOMEERROR;
			status = fmt.DisplayToken(tokenNo, token);
			if (status) goto SOMEERROR;

			// TEMP...
			if (gVerbose && token.type != LexToken::EndOfLine) {
				String tokString;
				status = TheBefe->TheCache.ReadStringSpan(layer->lexStream.fileId, token.span, tokString);
				if (status) goto SOMEERROR;
				Cout << "    token.value = " << leadIn << tokString.Escape() << leadOut << '\n';
			}

			tokenNo++;
		} while (token.type != LexToken::EndOfInput);

		// Reset the LexStream...
		layer->lexStream.Reset();

  }
  
  // Handle Errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;       break;
    INTERNAL:    status = Error::Internal;                  break;
    ONEARGONLY:  status = Error::CmdLang_ExpectedOneFile;   break;
    NOCURLANG:   status = Error::CmdLang_NoCurrentLanguage; break;
    SOMEERROR:                                              break;
  }
  if (status)
    status = cmd.SetError(0, status);
  
  return status;

} // ...CommandLayer_Lang_Lex

//----------------------------------------------------------------------
//
// Functions: CommandLayer_Lang_Expr - Handle Language 'Expression' command
//

Status CommandLayer_Lang_Expr(CommandLayer_Lang *layer, BcmdCommand &cmd) {  // CommandLayer_Lang_Expr...

  Status        status;
  CommandShell *shell;
  LexFormatOut  fmt;
  Char          c;
  LexToken      token;
  UInt          tokenNo;
  
  // Sanity checks...  
  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;

  if (cmd.numArgs < 2) goto NOEXPR;
  if (cmd.numArgs > 2) goto EXPECTED1;

  // Make sure there's a current language
  status = layer->_InitLex();
  if (status) goto SOMEERROR;
  if (BEFE::IsNull(layer->curLang.GetId())) goto NOCURLANG;

  // Setup the LexStream
  status = layer->lexStream.SetLanguageName(layer->curLang.GetName().Consumable());
  if (status) goto SOMEERROR;
  status = layer->lexStream.SetInputString(cmd[1]);
  if (status) goto SOMEERROR;
  
  //
  // Display the tokens...
  //
  
  tokenNo = 0;
  fmt.indentCols = 2;
  
  do {
    status = layer->lexStream.NextToken(token);
    if (status) goto SOMEERROR;
    status = fmt.DisplayToken(tokenNo, token);
    if (status) goto SOMEERROR;
    tokenNo++;
  } while (token.type != LexToken::EndOfInput);
  
  // Handle Errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;       break;
    NOCURLANG:   status = Error::CmdLang_NoCurrentLanguage; break;
    NOEXPR:      status = Error::CmdLang_NoExpression;      break;
    EXPECTED1:   status = Error::CmdLang_ExpectedOneExpr;   break;
    SOMEERROR:                                              break;
  }
  if (status)
    status = cmd.SetError(0, status);
  
  return status;
  
} // ...CommandLayer_Lang_Expr

//----------------------------------------------------------------------
//
// Functions: CommandLayer_Lang_MatchType - Handle Language 'MatchType' Command
//

Status CommandLayer_Lang_MatchType(CommandLayer_Lang *layer, BcmdCommand &cmd) {  // CommandLayer_Lang_MatchType...

  Status        status;
  CommandShell *shell;
  UInt          theType;
  String        typeName;
  MinExp        exp;
  
  // Sanity checks...  
  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;
  
  // Pull arguments apart
  if (cmd.numArgs != 2) goto NUMARGS;
  
  // Validate the type...
  typeName = cmd[1];
  theType = Match::TypeUnknown;
  exp.Set("Star");
  if (exp.MatchInsensitive(typeName))
    theType = Match::TypeStar;
  exp.Set("Min[imal[ist]]Exp[ress[ion]");
  if (exp.MatchInsensitive(typeName))
    theType = Match::TypeMinExp;
  exp.Set("SQL");
  if (exp.MatchInsensitive(typeName))
    theType = Match::TypeSQL;
  exp.Set("[P[os[ix]]]B[asic]R[eg[ular]]E[xp[ress[ion]]]");
  if (exp.MatchInsensitive(typeName))
    theType = Match::TypePosixBRE;
  if (theType == Match::TypeUnknown) goto INVTYPE;
  exp.Set("[P[os[ix]]]E[xt[end[ed]]]R[eg[ular]]E[xp[ress[ion]]]");
  if (exp.MatchInsensitive(typeName))
    theType = Match::TypePosixERE;
  if (theType == Match::TypeUnknown) goto INVTYPE;
    
  // Set it...
  layer->match.type = theType;
  
  // Handle Errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;          break;
    NUMARGS:     status = Error::CmdLang_ExpectedOneMatchType; break;
    INVTYPE:     status = Error::CmdLang_UnknownMatchType;     break;
  }
  if (status)
    status = cmd.SetError(0, status);
  
  return status;
  
} // ...CommandLayer_Lang_MatchType

//----------------------------------------------------------------------
//
// Functions: CommandLayer_Lang_Pattern - Handle Language 'Pattern' Command
//

Status CommandLayer_Lang_Pattern(CommandLayer_Lang *layer, BcmdCommand &cmd) {  // CommandLayer_Lang_Pattern...

  Status        status;
  CommandShell *shell;
  String        typeName;
  
  // Sanity checks...  
  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;
  
  if (cmd.numArgs != 2) goto NUMARGS;
  
  //
  // Handle "Pattern <pattern>"
  //
  
  // Set it...
  layer->match.pattern = cmd[1];
  if ( (layer->match.pattern.Get(0) == '\'' || layer->match.pattern.Get(0) == '"' || layer->match.pattern.Get(0) == '`') &&
       layer->match.pattern.Get(-1) == layer->match.pattern.Get(0)) {
    layer->match.pattern.Remove(0);
    layer->match.pattern.Remove(-1);
  }
  
  // Handle Errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;       break;
    NUMARGS:     status = Error::CmdLang_MissingPattern;    break;
  }
  if (status)
    status = cmd.SetError(0, status);
  
  return status;
  
} // ...CommandLayer_Lang_Pattern

//----------------------------------------------------------------------
//
// Functions: CommandLayer_Lang_Match - Handle Language 'Match' Command
//

Status CommandLayer_Lang_Match(CommandLayer_Lang *layer, BcmdCommand &cmd) {  // CommandLayer_Lang_Match...

  Status        status;
  CommandShell *shell;
  UInt          theType;
  String        typeName;
  String        pattern;
  String        matchString;
  MinExp        exp;
  Boolean       matches;
  
  // Sanity checks...  
  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;
  
  // Pull arguments apart
  if (cmd.numArgs < 2 || cmd.numArgs > 3) goto ONEORTWO;
  
  //
  // Handle "Match <string>"
  //
  
  if (cmd.numArgs == 2) {
    
    String theString;
    
    // Make sure there's a current Match
    if (layer->match.type == Match::TypeUnknown) goto NOCURMATCH;
    
    // Get the String to check and remove quote marks (if any)...
    theString = cmd[1].Insensitive();
    if ( theString.Length() >= 2 &&
         (theString.Get(0) == '"' || theString.Get(0) == '\'' || theString.Get(0) == '`') &&
         theString.Get(0) == theString.Get(-1)) {
      theString.Remove(0);
      theString.Remove(-1);
    }

    // See if a match...
    matches = layer->match.Matches(theString);
    if (!gQuiet)
      Cout << layer->indent << ((matches)?CONSTR_True:CONSTR_False) << '\n';
    
    goto OK;
    
  }
  
  //
  // Handle "Match <matchType> <pattern>"
  //
  
  // Validate the type...
  typeName = cmd[1];
  theType = Match::TypeUnknown;
  exp.Set("Star");
  if (exp.MatchInsensitive(typeName))
    theType = Match::TypeStar;
  exp.Set("Min[imal[ist]]Exp[ress[ion]]");
  if (exp.MatchInsensitive(typeName))
    theType = Match::TypeMinExp;
  exp.Set("SQL");
  if (exp.MatchInsensitive(typeName))
    theType = Match::TypeSQL;
  exp.Set("[P[os[ix]]]B[asic]R[eg[ular]]E[xp[ress[ion]]]");
  if (exp.MatchInsensitive(typeName))
    theType = Match::TypePosixBRE;
  exp.Set("[P[os[ix]]]E[xt[end[ed]]]R[eg[ular]]E[xp[ress[ion]]]");
  if (exp.MatchInsensitive(typeName))
    theType = Match::TypePosixERE;
  if (theType == Match::TypeUnknown) goto INVTYPE;
    
  // Set it...
  layer->match.type = theType;
  layer->match.pattern = cmd[2];
  if ( (layer->match.pattern.Get(0) == '\'' || layer->match.pattern.Get(0) == '"' || layer->match.pattern.Get(0) == '`') &&
       layer->match.pattern.Get(-1) == layer->match.pattern.Get(0)) {
    layer->match.pattern.Remove(0);
    layer->match.pattern.Remove(-1);
  }
  
  // Handle Errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;       break;
    ONEORTWO:    status = Error::CmdLang_ExpectedOneOrTwo;  break;
    NOCURMATCH:  status = Error::CmdLang_NoCurrentLanguage; break;
    INVTYPE:     status = Error::CmdLang_UnknownMatchType;  break;
    OK:          status = Error::None;                      break;
  }
  if (status)
    status = cmd.SetError(0, status);
  
  return status;
  
} // ...CommandLayer_Lang_Match

//----------------------------------------------------------------------
//
// Functions: CommandLayer_Lang_MinExp - Handle Language 'MinExp' Command
//

Status CommandLayer_Lang_MinExp(CommandLayer_Lang *layer, BcmdCommand &cmd) {  // CommandLayer_Lang_MinExp...

  Status        status;
  CommandShell *shell;
  MinExp        minexp;
  Int           argIdx;

  // Sanity checks...
  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;

  // Pull arguments apart
  if (cmd.numArgs < 1) goto EXPECT1;

  //
  // Handle "Match <string>"
  //

  for (argIdx = 1; argIdx < (Int)cmd.numArgs; argIdx++) {

    Strings strings;
  	String  arg;

  	arg = cmd.args[argIdx].value;
  	status = minexp.Set(arg);
  	if (status) goto ERROR;
    strings = minexp.ToStrings();
    if (argIdx > 1)
    	Cout << '\n';
    Cout << "MinExp string \"" << arg << "\"...\n\n";

    for (UInt i=0; i < strings.Length(); i++) {
    	Cout << "  " << strings[i] << '\n';
    }

  }

  // Handle Errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;    break;
    EXPECT1:     status = Error::CmdLang_ExpectedOneArg; break;
    ERROR:                                               break;
  }
  if (status)
    status = cmd.SetError(0, status);

  return status;

} // ...CommandLayer_Lang_MinExp

//----------------------------------------------------------------------
//
// Functions: CommandLayer_Lang_Test - Handle Language 'Test' command
//

Status CommandLayer_Lang_Test(CommandLayer_Lang *layer, BcmdCommand &cmd) {  // CommandLayer_Lang_Test...

  Status        status;
  CommandShell *shell;
  TestLexicon   test;
  
  // Sanity checks...  
  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;

  if (cmd.numArgs < 1) goto INTERNAL;
  if (cmd.numArgs > 1) goto HAVEARGS;

  // Do it
  status = test.Run();
  if (status) goto SOMEERROR;

  // Handle Errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;    break;
    INTERNAL:    status = Error::Internal;               break;
    HAVEARGS:    status = Error::CmdLang_ExpectedNoArgs; break;
    SOMEERROR:                                           break;
  }
  if (status)
    status = cmd.SetError(0, status);
  
  return status;

} // ...CommandLayer_Lang_Test

//----------------------------------------------------------------------
//
// Functions: CommandLayer_Lang_Unknown - Handle Language 'Unknown Command'
//
// This function handles the 'Unknown Command' case in CommandLayer_Lang.
//
// Note: The current case it handles is "<str> LIKE <p

Status CommandLayer_Lang_Unknown(CommandLayer_Lang *layer, BcmdCommand &cmd) {  // CommandLayer_Lang_Unknown...

  Status        status;
  CommandShell *shell;
  Match         ourMatch;
  String        ourString;
  Boolean       answer;
  
  // Sanity checks...  
  if (BEFE::IsNull(layer)) goto NULLPOINTER;
  shell = layer->GetShell();
  if (BEFE::IsNull(shell)) goto NULLPOINTER;

  //
  // Handle: "<string> LIKE <pat>"
  //
  if (cmd.numArgs >= 3 && cmd[1] == String("Like").Insensitive()) {

    // Make sure we've got a match type...
    if (layer->match.type == Match::TypeUnknown || layer->match.type >= Match::NumTypes)
      goto NOMATCHTYPE;
    ourMatch.type = layer->match.type;
    
    // Set the pattern
    ourMatch.pattern = cmd[2];
    if ( (ourMatch.pattern.Get(0) == '\'' || ourMatch.pattern.Get(0) == '"' || ourMatch.pattern.Get(0) == '`') &&
         ourMatch.pattern.Get(-1) == ourMatch.pattern.Get(0)) {
      ourMatch.pattern.Remove(0);
      ourMatch.pattern.Remove(-1);
    }
    
    // Get the string
    ourString = cmd[1];
    if ( (ourString.Get(0) == '\'' || ourString.Get(0) == '"' || ourString.Get(0) == '`') &&
         ourString.Get(-1) == ourString.Get(0)) {
      ourString.Remove(0);
      ourString.Remove(-1);
    }

    // Do it...
    answer = ourMatch.Matches(ourString);
    if (!gQuiet)
      Cout << layer->indent << ((answer)?CONSTR_True:CONSTR_False) << '\n';
    
    goto OK;
  }
  
  // Handle Errors
  status = Error::CommandUnknownCommand;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer;      break;
    NOMATCHTYPE: status = Error::CmdLang_UnknownMatchType; break;
    OK:          status = Error::None;                     break;
  }
  if (status)
    status = cmd.SetError(0, status);
  
  return status;

} // ...CommandLayer_Lang_Unknown

} // ...Namespace BEFE
