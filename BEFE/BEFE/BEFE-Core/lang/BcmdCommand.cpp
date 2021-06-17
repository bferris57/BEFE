//----------------------------------------------------------------------
// File: BcmdCommand.h - Implementation of the BcmdCommand Struct
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "BEFEMacros.h"
#include "BEFEWrappers.h"
#include "Bcmd.h"
#include "cmdutils.h"
#include <stddef.h>       // For offsetof()

namespace BEFE {// Namespace BEFE...

// Forward function prototypes
static UInt FindBooleanOption(String const &option);

//----------------------------------------------------------------------
//
// C++ Lifecycle...
//

String BcmdCommand::operator[](Int index) {
  String  str;
  str = String(args[index].value);
  return str.Consumable();
}

//----------------------------------------------------------------------
//
// BEFE Lifecycle...
//

Status BcmdCommand::StartUp() {

  Status retStatus;  
  Status status;

  isConsumable = false;
  BEFE::SetNull(lineNo);
  retStatus = fullSpan.StartUp();
  status = nonWhiteSpan.StartUp();
  if (status && !retStatus) retStatus = status;
  status = args.StartUp();
  if (status && !retStatus) retStatus = status;
  numArgs = 0;
  numOptions  = 0;
  numMessages = 0;
  numErrors   = 0;
  numWarnings = 0;  
  status = messages.StartUp();
  if (status && !retStatus) retStatus = status;

  return retStatus;
  
}

Status BcmdCommand::ShutDown() {

  Status retStatus;  
  Status status;
  
  retStatus = args.ShutDown();
  status = messages.ShutDown();
  if (status && !retStatus) retStatus = status;
  
  return retStatus;
  
}

Status BcmdCommand::Reset() {

  Status retStatus;
  Status status;

  retStatus = ShutDown();
  status    = StartUp();

  if (status && !retStatus) retStatus = status;

  return retStatus;

}

Status BcmdCommand::MoveFrom(BcmdCommand const &that) {
  
  Status retStatus;
  Status status;

  if ((BcmdCommand *)&that == this)
    retStatus = Error::None;
  else {
    retStatus = ShutDown();
    MemoryCopyRaw((Byte *)&that, (Byte *)this, sizeof(BcmdCommand));
    status = ((BcmdCommand *)&that)->StartUp();
    if (status && !retStatus) retStatus = status;
  }
  NotConsumable();
  
  return retStatus;
  
}

Status BcmdCommand::CopyFrom(BcmdCommand const &that) {

  Status retStatus;  
  Status status;
  
  if (this == (BcmdCommand *)&that)
    retStatus = Error::None;
  else {
    if (IsConsumable())
      retStatus = MoveFrom(that);
    else {
      lineNo       = that.lineNo;
      fullSpan     = that.fullSpan;
      nonWhiteSpan = that.nonWhiteSpan;
      numOptions   = that.numOptions;
      numMessages  = that.numMessages;
      numErrors    = that.numErrors;
      numWarnings  = that.numWarnings;
      numArgs      = that.numArgs;
      retStatus    = args.CopyFrom(that.args);
      status       = messages.CopyFrom(that.messages);
      if (status && !retStatus) retStatus = status;
    }
  }
  
  return retStatus;
  
}

Boolean BcmdCommand::IsNull() const {
  return (
    fullSpan.IsNull()     &&
    nonWhiteSpan.IsNull() &&
    BEFE::IsNull(lineNo)  &&
    args.IsNull()
  );
}

Status BcmdCommand::SetNull() {
  return Reset();
}

Boolean BcmdCommand::IsEmpty() const {
  return IsNull();
}

Status BcmdCommand::SetEmpty() {
  return Reset();
}

UInt BcmdCommand::Length() const {
  return args.Length();
}

UInt BcmdCommand::Size() const {
  return UNaN;
}

Boolean BcmdCommand::IsConsumable() const {
  return isConsumable;
}

BcmdCommand &BcmdCommand::Consumable() const {
  ((BcmdCommand *)this)->isConsumable = true;
  return *((BcmdCommand *)this);
}

BcmdCommand &BcmdCommand::NotConsumable() const {
  ((BcmdCommand *)this)->isConsumable = false;
  return *((BcmdCommand *)this);
}

//----------------------------------------------------------------------
//
// Error reporting
//

Status BcmdCommand::NormaliseErrors() {

  Status   status;
  BcmdArg *src;
  BcmdArg *dst;
  BcmdArg  empty;
  UInt     srcIdx;

  numArgs     = args.Length();
  numMessages = messages.Length();
  numErrors   = 0;
  numWarnings = 0;

  srcIdx = 0;

  while (srcIdx < numArgs) {

    // Dereference source BcmdArg
    src = (BcmdArg *)args._GetAddress(srcIdx);
    if (BEFE::IsNull(src)) goto NULLPOINTER;

    // Skip if it's not an Error, Unexpected, or Warning
    if (src->type < BcmdArg::TypeError || src->type > BcmdArg::TypeWarning) {
      srcIdx++;
      continue;
    }

    // Append empty argument to wherever and dereference destination BcmdArg
    status = messages.Append(empty);
    if (status) goto SOMEERROR;
    numMessages++;
    dst = (BcmdArg *)messages._GetAddress(-1);
    if (BEFE::IsNull(dst)) goto NULLPOINTER;

    // Move it from source to dest
    status = dst->MoveFrom(*src);
    if (status) goto SOMEERROR;

    // Remove it from args
    status = args.Remove(srcIdx);
    if (status) goto SOMEERROR;
    numArgs--;

  }

  // Recalculate counts...
  numMessages = messages.Length();
  src = (BcmdArg *)messages._GetAddress(0);
  if (numMessages != 0 && BEFE::IsNull(src)) goto NULLPOINTER;

  for (srcIdx=0; srcIdx < numMessages; srcIdx++) {
    if (src->type < BcmdArg::TypeWarning)
      numErrors++;
    else
      numWarnings++;
    src++;
  }

  // Handle errors...
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }

  return status;

}

Status BcmdCommand::SetError(Status pStatus) {

  Status  status;
  BcmdArg arg;

  arg.type   = BcmdArg::TypeError;  
  arg.status = pStatus;
  status = messages.Append(arg);
  if (status) goto SOMEERROR;
  numMessages++;

  // Handle errors
  while (false) {
    SOMEERROR: break;
  }

  return status;

}

Status BcmdCommand::SetError(Int argNo, Status pStatus) {

  Status   status;
  BcmdArg *arg;

  // Get the BcmdArg's address
  arg = (BcmdArg *)args._GetAddress(argNo);
  if (BEFE::IsNull(arg)) goto OUTOFRANGE;

  // Populate the errors
  arg->type   = BcmdArg::TypeError;
  arg->status = pStatus;
  arg->value  = String();

  // Handle errors
  status = Error::None;
  while (false) {
    OUTOFRANGE: status = Error::IndexOutOfRange; break;
  }

  return status;

}

Status BcmdCommand::SetError(Int argNo, Status pStatus, String const &errMsg) {

  Status   status;
  BcmdArg *arg;

  // Get the BcmdArg's address
  arg = (BcmdArg *)args._GetAddress(argNo);
  if (BEFE::IsNull(arg)) goto OUTOFRANGE;

  // Populate the errors
  arg->type  = BcmdArg::TypeError;
  arg->status = pStatus;
  arg->value  = errMsg;

  // Handle errors
  status = Error::None;
  while (false) {
    OUTOFRANGE: status = Error::IndexOutOfRange; break;
  }

  return status;

}

Status BcmdCommand::SetError(UInt lineNo, UInt colNo, Status pStatus) {

  Status  status;
  BcmdArg arg;

  // Populate the errors
  arg.type = BcmdArg::TypeError;
  arg.status = pStatus;

  status = Error::None;

  return status;

}

Status BcmdCommand::SetError(UInt lineNo, UInt colNo, Status pStatus, String const &errMsg) {

  Status  status;
  BcmdArg arg;

  // Populate the errors
  arg.type = BcmdArg::TypeError;
  arg.lineNo = lineNo;
  arg.colNo = colNo;
  arg.status = pStatus;
  arg.value = errMsg;

  status = Error::None;

  return status;

}

Status BcmdCommand::AppendError(Status pStatus) {

  Status  status;
  BcmdArg arg;

  // Populate the error
  arg.type   = BcmdArg::TypeError;
  arg.status = pStatus;
  arg.value  = String();

  // Append it
  status = args.Append(arg.Consumable());

  return status;

}

Status BcmdCommand::AppendError(Status pStatus, String const &errMsg) {

  Status  status;
  BcmdArg arg;

  // Populate the error
  arg.type = BcmdArg::TypeError;
  arg.status = pStatus;
  arg.value = errMsg;

  // Append it
  status = args.Append(arg.Consumable());

  return status;

}

//----------------------------------------------------------------------
//
// Messages
//

Status BcmdCommand::DisplayMessages(String const &prefix) {

  Status   status;
  Int      argNo;
  BcmdArg *arg0;
  String   message;
  String   lineColMsg;

  numArgs = args.Length();
  numMessages = messages.Length();

  // Display any in BcmdCommand.args...
  if (numArgs) {
    arg0 = (BcmdArg *)args._GetAddress(0);
    if (BEFE::IsNull(arg0)) goto NULLPOINTER;
    for (argNo=0; argNo < (Int)numArgs; argNo++) {
      if (arg0[argNo].type >= BcmdArg::TypeError && arg0[argNo].type <= BcmdArg::TypeWarning) {
        status = DisplayMessage(prefix, arg0[argNo]);
        if (status) goto SOMEERROR;
      }
    }
  }

  // Display any in BcmdCommand.messages...
  if (numMessages) {
    arg0 = (BcmdArg *)messages._GetAddress(0);
    if (BEFE::IsNull(arg0)) goto NULLPOINTER;
    for (argNo=0; argNo < (Int)numMessages; argNo++) {
      status = DisplayMessage(prefix, arg0[argNo]);
      if (status) goto SOMEERROR;
    }
  }

  // Handle errors
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }

  return status;

}

Status BcmdCommand::DisplayMessage(String const &prefix, BcmdArg &arg) {

  Status status;
  UInt   fg;
  String message;
  String lineColMsg;

  message = prefix;
  fg = Cout.GetForegroundColour();

  if (!BEFE::IsNull(arg.lineNo) || !BEFE::IsNull(arg.colNo)) {
    lineColMsg = GetMessage(Error::CommandLineColHead);
    lineColMsg = CmdUIntMessage(lineColMsg.Consumable(), arg.lineNo+1, arg.colNo+1);
  }
  else
    lineColMsg = String();
  message += lineColMsg;

  // Form Error/Warning part
  if (arg.type < BcmdArg::TypeWarning)
    message += GetMessage(Error::CommandErrorHead);
  else
    message += GetMessage(Error::CommandWarningHead);

  // Form the actual message part...
  if (!arg.value.IsNull())
    status = message.Append(arg.value);
  else
    status = message.Append(ErrorString(arg.status).Consumable());
  if (status) goto SOMEERROR;

  // Display the message...
  Cout.SetForegroundColour((arg.type < BcmdArg::TypeWarning) ? Console::Red : Console::Yellow);
  Cout << message << '\n';
  Cout.SetForegroundColour(fg);

  // Handle errors
  status = Error::None;
  while (false) {
    SOMEERROR: break;
  }

  return status;

}

//----------------------------------------------------------------------
//
// Option Declarations - Used in Options methods below
//

enum BoolDeclAction {
  BoolActionNone,
  BoolActionQuiet,
  BoolActionVerbose,
  BoolActionVVerbose,
  BoolActionVVVerbose,
  BoolActionAll,
  BoolActionM,
  BoolActionMS,
  BoolActionMAS,
  BoolActionMVA,
  BoolActionLem,
  BoolActionUTFFold,
  BoolActionUTFTurk,
  BoolNumBoolActions
};

struct BoolDecl {
  char const     *name;           // Name of option
  UInt            index;          // Index into Globals.<type>.array
  BoolDeclAction  action;         // Action enum (for external use)
  Status          msgNo;          // Display message number (for '?xxx')
};

#define OFFSET(X) offsetof(Globals, bools.X)/sizeof(Boolean) - offsetof(Globals,bools)

static struct BoolDecl boolDecls[] = {

//  name              index                          action              msgNo
//  ----------------- ------------------------------ ------------------- ---------------

   {"x",              OFFSET(FlowQuitNow),           BoolActionNone,     Error::GlobOpt_q}
  ,{"q",              OFFSET(FlowQuitNow),           BoolActionQuiet,    Error::GlobOpt_q}

  ,{"v",              OFFSET(Verbose),               BoolActionVerbose,  Error::GlobOpt_v}
  ,{"vv",             OFFSET(VeryVerbose),           BoolActionVVerbose, Error::GlobOpt_vv}
  ,{"vvv",            OFFSET(VeryVeryVerbose),       BoolActionVVerbose, Error::GlobOpt_vvv}

  ,{"et",             OFFSET(ShowElapsed),           BoolActionNone,     Error::GlobOpt_et}
  ,{"ts",             OFFSET(ShowTimeStamps),        BoolActionNone,     Error::GlobOpt_ts}
  ,{"localtime",      OFFSET(ShowLocalTime),         BoolActionNone,     Error::GlobOpt_localtime}
  ,{"dep",            OFFSET(ShowDeprecated),        BoolActionNone,     Error::GlobOpt_dep}
  ,{"deprecate",      OFFSET(ShowDeprecated),        BoolActionNone,     Error::GlobOpt_dep}
  ,{"w",              OFFSET(ShowWarnings),          BoolActionNone,     Error::GlobOpt_w}
  ,{"warn",           OFFSET(ShowWarnings),          BoolActionNone,     Error::GlobOpt_w}
  ,{"warning",        OFFSET(ShowWarnings),          BoolActionNone,     Error::GlobOpt_w}
  ,{"warnings",       OFFSET(ShowWarnings),          BoolActionNone,     Error::GlobOpt_w}
  ,{"info",           OFFSET(ShowInfo),              BoolActionNone,     Error::GlobOpt_info}
  ,{"information",    OFFSET(ShowInfo),              BoolActionNone,     Error::GlobOpt_info}
  ,{"info",           OFFSET(ShowInfo),              BoolActionNone,     Error::GlobOpt_info}
  ,{"information",    OFFSET(ShowInfo),              BoolActionNone,     Error::GlobOpt_info}
  ,{"d",              OFFSET(ShowDebug),             BoolActionNone,     Error::GlobOpt_debug}
  ,{"debug",          OFFSET(ShowDebug),             BoolActionNone,     Error::GlobOpt_debug}
  ,{"head",           OFFSET(ShowHeaders),           BoolActionNone,     Error::GlobOpt_head}
  ,{"header",         OFFSET(ShowHeaders),           BoolActionNone,     Error::GlobOpt_head}
  ,{"headers",        OFFSET(ShowHeaders),           BoolActionNone,     Error::GlobOpt_head}

  ,{"indent",         OFFSET(CommandIndent),         BoolActionNone,     Error::GlobOpt_indent}
  ,{"es",             OFFSET(CommandEnvSubst),       BoolActionNone,     Error::GlobOpt_envsubst}
  ,{"envs",           OFFSET(CommandEnvSubst),       BoolActionNone,     Error::GlobOpt_envsubst}
  ,{"esubst",         OFFSET(CommandEnvSubst),       BoolActionNone,     Error::GlobOpt_envsubst}
  ,{"envsubst",       OFFSET(CommandEnvSubst),       BoolActionNone,     Error::GlobOpt_envsubst}
  
  ,{"k",              OFFSET(KeepTemporaryFiles),    BoolActionNone,     Error::GlobOpt_keep}
  ,{"keep",           OFFSET(KeepTemporaryFiles),    BoolActionNone,     Error::GlobOpt_keep}
  ,{"a",              OFFSET(ShowHiddenFiles),       BoolActionAll,      Error::None}
  ,{"all",            OFFSET(ShowHiddenFiles),       BoolActionAll,      Error::None}
  ,{"af",             OFFSET(ShowHiddenFiles ),      BoolActionNone,     Error::GlobOpt_allfiles}
  ,{"allf",           OFFSET(ShowHiddenFiles),       BoolActionNone,     Error::GlobOpt_allfiles}
  ,{"afiles",         OFFSET(ShowHiddenFiles),       BoolActionNone,     Error::GlobOpt_allfiles}
  ,{"allfiles",       OFFSET(ShowHiddenFiles),       BoolActionNone,     Error::GlobOpt_allfiles}
  ,{"ad",             OFFSET(ShowHiddenDirectories), BoolActionNone,     Error::GlobOpt_alldirs}
  ,{"alld",           OFFSET(ShowHiddenDirectories), BoolActionNone,     Error::GlobOpt_alldirs}
  ,{"adirs",          OFFSET(ShowHiddenDirectories), BoolActionNone,     Error::GlobOpt_alldirs}
  ,{"alldirs",        OFFSET(ShowHiddenDirectories), BoolActionNone,     Error::GlobOpt_alldirs}
    
  ,{"UF",             OFFSET(UnicodeFolding),        BoolActionUTFFold,  Error::GlobOpt_ufolding}
  ,{"UnicodeF",       OFFSET(UnicodeFolding),        BoolActionUTFFold,  Error::GlobOpt_ufolding}
  ,{"UfoldingF",      OFFSET(UnicodeFolding),        BoolActionUTFFold,  Error::GlobOpt_ufolding}
  ,{"UnicodeFolding", OFFSET(UnicodeFolding),        BoolActionUTFFold,  Error::GlobOpt_ufolding}
  ,{"UT",             OFFSET(UnicodeTurkish),        BoolActionUTFTurk,  Error::GlobOpt_uturkish}
  ,{"UnicodeT",       OFFSET(UnicodeTurkish),        BoolActionUTFTurk,  Error::GlobOpt_uturkish}
  ,{"UTurkish",       OFFSET(UnicodeTurkish),        BoolActionUTFTurk,  Error::GlobOpt_uturkish}
  ,{"UnicodeTurkish", OFFSET(UnicodeTurkish),        BoolActionUTFTurk,  Error::GlobOpt_uturkish}

  ,{"m",              OFFSET(LogMemory),             BoolActionM,        Error::GlobOpt_m}
  ,{"ma",             OFFSET(LogMemoryAlloc),        BoolActionNone,     Error::GlobOpt_ma}
  ,{"mas",            OFFSET(LogMemoryAllocSizes),   BoolActionMAS,      Error::GlobOpt_ms}
  ,{"mf",             OFFSET(LogMemoryFree),         BoolActionNone,     Error::GlobOpt_mf}
  ,{"maf",            UNaN,                          BoolActionNone,     Error::None}
  ,{"mfill",          OFFSET(LogMemoryFill),         BoolActionNone,     Error::GlobOpt_mfill}
  ,{"mcopy",          OFFSET(LogMemoryCopy),         BoolActionNone,     Error::GlobOpt_mcopy}
  ,{"ms",             OFFSET(ShowMemoryStats),       BoolActionMS,       Error::GlobOpt_ms}
  ,{"mv",             OFFSET(MemoryValidate),        BoolActionMVA,      Error::GlobOpt_mv}
  ,{"mva",            OFFSET(MemoryValidate),        BoolActionNone,     Error::GlobOpt_mva}

  ,{"lem",            UNaN,                          BoolActionLem,      Error::None}

  ,{"cc",             OFFSET(ConsoleColour),         BoolActionNone,     Error::GlobOpt_cc}
  ,{"ccolour",        OFFSET(ConsoleColour),         BoolActionNone,     Error::GlobOpt_cc}
  ,{"ci",             OFFSET(ConsoleColour),         BoolActionNone,     Error::GlobOpt_ci}
  ,{"cu",             OFFSET(ConsoleUTF8),           BoolActionNone,     Error::GlobOpt_cutf}
  ,{"cutf",           OFFSET(ConsoleUTF8),           BoolActionNone,     Error::GlobOpt_cutf}
  ,{"cutf8",          OFFSET(ConsoleUTF8),           BoolActionNone,     Error::GlobOpt_cutf}
  ,{"cutf-8",         OFFSET(ConsoleUTF8),           BoolActionNone,     Error::GlobOpt_cutf}
  ,{"cb",             OFFSET(ConsoleBeep),           BoolActionNone,     Error::GlobOpt_cb}
  ,{"cbeep",          OFFSET(ConsoleBeep),           BoolActionNone,     Error::GlobOpt_cb}
  ,{"cint",           OFFSET(ConsoleInteractive),    BoolActionNone,     Error::GLobOpt_cint}
  ,{"pyout",          OFFSET(PythonOutput),          BoolActionNone,     Error::GlobOpt_pyout}
  ,{0,                0,                             BoolActionNone,     Error::None}
  };

#undef OFFSET

/*  
enum QueryDeclAction {
  ActionQ_Os,
  ActionQ_Network,
  ActionQ_Platform,
  ActionQ_Process,
  ActionQ_Processor,
  ActionQ_Cpu,
  ActionQ_Header,
  ActionQ_Footer,
  ActionQ_Switches,
  ActionQ_Environ,
  NumQueryActions
};

{"os",             TypeQuery,   UNaN,
{"net",            TypeQuery,   UNaN,
{"network",        TypeQuery,   UNaN,
{"platform",       TypeQuery,   UNaN,
{"process",        TypeQuery,   UNaN,
{"processor",      TypeQuery,   UNaN,
{"cpu",            TypeQuery,   UNaN,

{"head",           TypeQuery,   UNaN,
{"header",         TypeQuery,   UNaN,
{"foot",           TypeQuery,   UNaN,
{"footer",         TypeQuery,   UNaN,
{"trail",          TypeQuery,   UNaN,
{"trailer",        TypeQuery,   UNaN,

{"sw",             TypeQuery,   UNaN,
{"switch",         TypeQuery,   UNaN,
{"switches",       TypeQuery,   UNaN,
{"opt",            TypeQuery,   UNaN,
{"opts",           TypeQuery,   UNaN,
{"option",         TypeQuery,   UNaN,
{"options",        TypeQuery,   UNaN,
*/

//----------------------------------------------------------------------
//
// Options
//

Boolean BcmdCommand::IsOption(Int argNo) const { // BcmdCommand.IsOption...

  Boolean  answer;
  BcmdArg *arg;

  arg = (BcmdArg *)((BcmdArgs *)&args)->_GetAddress(argNo);
  answer = !BEFE::IsNull(arg)                &&
           arg->type == BcmdArg::TypeOption  &&
           (Bcmd::GetOptionString().Find(arg->value[0]) >= 0);

  return answer;

} // ...BcmdCommand.IsOption

Status BcmdCommand::ExecuteOptions() { // BcmdCommand.ExecuteOptions...

  Status   status;
  BcmdArg *arg0;
  UInt     argIdx;
  BcmdArg *message;
  BcmdArg  empty;

  // Nothing to do if no arguments
  numArgs = args.Length();
  if (numArgs == 0) goto OK;
  numMessages = messages.Length();

  // Dereference &args[0]
  arg0 = (BcmdArg *)args._GetAddress(0);
  if (BEFE::IsNull(arg0)) goto NULLPOINTER;

  // For each argument...
  argIdx = 0;
  while (argIdx < numArgs) {

    // Ignore it if not an option
    if (arg0[argIdx].type != BcmdArg::TypeOption) {
      argIdx++;
      continue;
    }

    // Execute it...
    status = ExecuteOption(argIdx);
    if (status) goto SOMEERROR;

    // If it's an error/warning now, move it...
    if (arg0[argIdx].type >= BcmdArg::TypeError && arg0[argIdx].type <= BcmdArg::TypeWarning) {

      // Append an empty message
      status = messages.Append(empty);
      if (status) goto SOMEERROR;
      numMessages++;

      // Dereference &messages[-1]
      message = (BcmdArg *)messages._GetAddress(-1);
      if (BEFE::IsNull(message)) goto NULLPOINTER;

      // Move it
      status = message->MoveFrom(arg0[argIdx]);
      if (status) goto SOMEERROR;

      // Remove the argument
      status = args.Remove(argIdx);
      if (status) goto SOMEERROR;
      numArgs--;
      arg0 = (BcmdArg *)args._GetAddress(0);
      if (BEFE::IsNull(arg0) && numArgs != 0) goto NULLPOINTER;

      // Update counts
      if (message->type < BcmdArg::TypeWarning)
        numErrors++;
      else
        numWarnings++;

      continue;

    }

    // Bump index for next one
    argIdx++;

  }

  // Handle errors
  status = Error::None;
  while (false) {
    OK:          status = Error::None;                break;
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }

  return status;

} // ...BcmdCommand.ExecuteOptions

Status BcmdCommand::ExecuteOption(Int argNo) { // BcmdCommand.ExecuteOption...

  Status   status;
  Char     flag;
  String   name;
  String   value;

  // Make sure we have TheBefe...
  if (BEFE::IsNull(TheBefe)) goto NOBEFE;

  // Validate the argument number
  numArgs = args.Length();
  if (argNo < 0 || argNo >= (Int)numArgs) goto BADPARAM;

  // Pull it apart
  status = _SplitOption(argNo, flag, name, value);
  if (status) goto SOMEERROR;

  // Make sure it's a known option flag
  if (Bcmd::GetOptionString().Find(flag) < 0) {
    status = SetError(argNo, Error::CommandUnknownOptionFlag);
    if (status) goto SOMEERROR;
    goto OK;
  }

  // Do it...
  if (flag == '?')

    status = _QueryOption(argNo, name, value);

  else {

    if (name == "env")
      status = _ExecuteEnvOption(argNo, flag, name, value);
    else
      status = _ExecuteBooleanOption(argNo, flag, name, value);

  }

  // Handle Error::CommandUnknownOption...
  if (status && status != Error::CommandUnknownOption) goto SOMEERROR;
  if (status) {
    status = SetError(argNo, status, CmdStringMessage(ErrorString(status), name));
    if (status) goto SOMEERROR;    
  }

  // Handle errors
  while (false) {
    OK:        status = Error::None;                break;
    NOBEFE:    status = Error::NoBefe;              break;
    BADPARAM:  status = Error::InvalidParameter;    break;
    SOMEERROR:                                      break;
  }

  return status;

} // ...BcmdCommand.ExecuteOption

Status BcmdCommand::RemoveOptions() { // BcmdCommand.RemoveOptions...

  Status   status;
  UInt     argIdx;
  BcmdArg *arg;

  numArgs = args.Length();
  argIdx = 0;

  while (argIdx < numArgs) {
    arg = (BcmdArg *)args._GetAddress(argIdx);
    if (BEFE::IsNull(arg)) goto NULLPOINTER;
    if (arg->type == BcmdArg::TypeOption) {
      status = args.Remove(argIdx);
      if (status) goto SOMEERROR;
      numArgs--;
    }
    else
      argIdx++;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    NULLPOINTER: status = Error::InternalNullPointer; break;
    SOMEERROR:                                        break;
  }

  return status;

} // ...BcmdCommand.RemoveOptions

Status BcmdCommand::_SplitOption(Int argNo, Char &flag, String &name, String &value) { // BcmdCommand._SplitOption...

  Status status;
  String string;
  Int    colonPos;

  // Initialise some stuff
  string = args[argNo].value;
  flag = Char(UNaN);
  name.Reset();
  value.Reset();

  // Get the flag Char...
  if (Bcmd::GetOptionString().Find(string[0]) >= 0) {
    flag = string[0];
    string.Remove(0);
  }
  else {
    status = SetError(argNo, Error::CommandUnknownOptionFlag);
    if (status) goto SOMEERROR;
    goto OK;
  }

  // Get the name...
  colonPos = string.Find(':');
  name = string[Span(0,colonPos)];

  // Get the value...
  if (colonPos != NaN)
    value = string[Span(colonPos+1, NaN)];

  // Handle errors
  status = Error::None;
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }

  return status;
  
} // ...BcmdCommand._SplitOption

Status BcmdCommand::_ExecuteBooleanOption(Int argNo, Char &flag, String &name, String &value) {

  Status    status;
  Globals   emptyGlobs;  // For default global values
  Boolean   theBool;
  UInt      boolOptIdx;
  BoolDecl *decl;
  BcmdArg  *arg;

  // Validate parameters...
  if (argNo < 0 || argNo >= (Int)args.Length() ||
      (flag !='+' && flag != '-' && flag != '!')
     ) goto BADPARAM;

  // Make sure it's a boolean option...
  boolOptIdx = FindBooleanOption(name);
  if (BEFE::IsNull(boolOptIdx)) goto NOTBOOL;

  decl = &boolDecls[boolOptIdx];

  // Get the BcmdArg's address
  arg = (BcmdArg *)args._GetAddress(argNo);
  if (BEFE::IsNull(arg)) goto NULLPOINTER;

  // Make sure it hasn't got a value
  if (!value.IsNull()) {
    arg->type   = BcmdArg::TypeError;
    arg->status = Error::CommandBoolHasValue;
    arg->value  = String();
    goto OK;
  }

  // Display the value if '?'
  if (flag == '?') {
    arg->type   = BcmdArg::TypeError;
    arg->status = Error::NotCapable;
    arg->value  = String();
    goto OK;
  }

  // Set the value (if it has an index)...
  theBool = false;

  if (!BEFE::IsNull(decl->index)) {

    if (flag == '!')
      theBool = emptyGlobs.bools.array[decl->index];
    else if (flag == '+')
      theBool = true;
    else
      theBool = false;

    TheBefe->TheGlobals.bools.array[decl->index] = theBool;

  }

  // Perform the action (if any)...
  status = Error::None;
  switch (boolDecls[boolOptIdx].action) { // BoolDecl actions...

    case BoolActionNone:

      break;

    case BoolActionQuiet:

      gVerbose = gVeryVerbose = gVeryVeryVerbose = false;
      break;

    case BoolActionVerbose:

      if (!theBool)
        gVeryVerbose = gVeryVeryVerbose = false;
      break;

    case BoolActionVVerbose:
      if (!theBool)
        gVeryVerbose = false;
      else
        gVerbose = gVeryVerbose = true;
      break;

    case BoolActionVVVerbose:
      if (theBool)
        gVerbose = gVeryVerbose = true;
      break;

    case BoolActionAll:
      gShowHiddenFiles = gShowHiddenDirectories = theBool;
      break;

    case BoolActionM:
      break;

    case BoolActionLem:

      if (flag == '+')
        status = LoadErrorMessages();
      else
        status = UnloadErrorMessages();
      if (status)
        status = SetError(argNo, status);

      break;

    case BoolActionUTFFold:
    case BoolActionUTFTurk:
      status = SetError(argNo, Error::NotCapable);
      break;

    case BoolActionMS:

      if (theBool)
       gShowMemoryStats = gLogMemory = theBool;
      break;

    case BoolActionMAS:

      if (theBool)
        gLogMemoryAllocSizes = gLogMemory = theBool;
      gShowMemoryStats = theBool;

      break;

    case BoolActionMVA:

      gMemoryValidate = gMemoryValidateAbort = theBool;
      break;

    default:

      goto INTERNAL;

  } // ...BoolDecl actions

  // If we've got a Status now, turn it into an error
  if (status) {
    arg->type   = BcmdArg::TypeError;
    arg->status = status;

    arg->value  = String();
  }

  // Handle errors
  while (false) {
    OK:          status = Error::None;                 break;
    BADPARAM:    status = Error::InvalidParameter;     break;
    NULLPOINTER: status = Error::InternalNullPointer;  break;
    INTERNAL:    status = Error::Internal;             break;
    NOTBOOL:     status = Error::CommandUnknownOption; break;
  }

  return status;

}

Status BcmdCommand::_ExecuteEnvOption(Int argNo) {

  Status status;
  String arg;
  String env;
  String empty;
  Int    pos;

  arg = args[argNo].value;
  pos = arg.Find('=');
  env = "env";
  if (!BEFE::IsNull(pos)) {
    status = _ExecuteEnvOption(argNo, '+', env, arg);
  }
  else {
    status = _QueryEnv(argNo, env, arg);
  }

  return status;
}

Status BcmdCommand::_ExecuteEnvOption(Int argNo, Char flag, String &name, String &value) {

  Status  status;
  Boolean exists;
  Int     pos;
  String  varName;
  String  varValue;

  // Make sure it's what we expect...
  if (name != "env" || (flag != '+' && flag != '-' && flag != '!') ) {
    status = SetError(argNo, Error::Internal);
    goto SOMEERROR;
  }

  // Special case for '!env'
  if (flag == '!') {

    // Make sure no value is specified (for now)...
    if (value.Length() != 0) {
      status = SetError(argNo, Error::CommandEnvUnexpectedValue);
      goto SOMEERROR;
    }

    // Do it...
    status = TheBefe->RefreshEnvironment();
    if (status) goto SOMEERROR;
    
    goto OK;

  }

  // If we have a value, pull it apart...
  if (!value.IsNull()) {
    pos = value.Find('=');
    if (pos >= 0) {
      varName  = value[Span(0,pos)];
      varValue = value[Span(pos+1,NaN)];
    }
    else
      varName = value;
  }
  else
    pos = NaN;

  // See if it exists
  exists = IsEnvironmentName(varName);

  // Do some more validation...
  if (varName.IsNull()) {
    status = SetError(argNo, Error::CommandExpectedEnvName);
    goto SOMEERROR;
  }
  if (flag == '-' && !varValue.IsNull()) {
    status = SetError(argNo, Error::CommandUnexpectedEnvValue);
    goto SOMEERROR;
  }
  if (flag == '+' && BEFE::IsNull(pos)) {
    status = SetError(argNo, Error::CommandExpectedEnvValue);
    goto SOMEERROR;
  }
  if (flag == '-' && !exists) {
    status = Error::CommandUnknownEnv;
    status = SetError(argNo, status, CmdStringMessage(status, varName));
    goto SOMEERROR;
  }

  // Do '-env:...'
  if (flag == '-') {

    status = RemoveEnvironmentName(varName);
    goto SOMEERROR;

  }

  // Do '+env:...'
  if (flag == '+') {

    // Strip of any surrounding quotes...
    if (varValue[Span(0,3)] == "'''" && varValue[Span(-3,NaN)] == "'''") {
      varValue.Remove(Span(0,3));
      varValue.Remove(Span(-3,NaN));
    }
    // Strip of any surrounding quotes...
    if (varValue[Span(0,3)] == "\"\"\"" && varValue[Span(-3,NaN)] == "\"\"\"") {
      varValue.Remove(Span(0,3));
      varValue.Remove(Span(-3,NaN));
    }
    else if (varValue[0] == '\'' && varValue[-1] == '\'') {
      varValue.Remove(0);
      varValue.Remove(-1);
    }
    else if (varValue[0] == '"' && varValue[-1] == '"') {
      varValue.Remove(0);
      varValue.Remove(-1);
    }
    else if (varValue[0] == '`' && varValue[-1] == '`') {
      varValue.Remove(0);
      varValue.Remove(-1);
    }

    // Do any environment substitution if requested...
    if (gCommandEnvSubst)
      varValue = SubstituteEnvironmentValues(varValue);

    // Do it...
    status = SetEnvironmentValue(varName, varValue);
    goto SOMEERROR;

  }

  // We're in some weird state
  status = SetError(argNo, Error::Internal);

  // Handle errors
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }

  return status;

}

Status BcmdCommand::_QueryOption(Int argNo, String &name, String &value) {

  Status  status;
  UInt    optIdx;
  String  message;
  Strings strings;
  UInt    numStr;
  UInt    pos;
  Char    flag;
  String  onOff;

  // Sanity checks...
  if (argNo < 0 || argNo > (Int)args.Length()) goto BADPARM;

  // "?env" may have a value...
  if (name == "env" || name == "environ" || name == "environment") {
    status = _QueryEnv(argNo, name, value);
    if (status) goto SOMEERROR;
    goto OK;
  }
  
  // Other ones, nNo value should be specified...
  if (!value.IsNull()) {
    status = Error::CommandUnexpectedQValue;
    status = SetError(argNo, status, CmdStringMessage(status, value));
    if (status) goto SOMEERROR;
    goto OK;
  }

  // Do it...
  if (name == "os")        {OutputOSInfo();        goto OK; }
  if (name == "net")       {OutputNetworkInfo();   goto OK; }
  if (name == "network")   {OutputNetworkInfo();   goto OK; }
  if (name == "platform")  {OutputPlatformInfo();  goto OK; }
  if (name == "process")   {OutputProcessInfo();   goto OK; }
  if (name == "processor") {OutputProcessorInfo(); goto OK; }
  if (name == "cpu")       {OutputProcessorInfo(); goto OK; }

  if (name == "head")      {OutputHeader();        goto OK; }
  if (name == "header")    {OutputHeader();        goto OK; }
  if (name == "foot")      {OutputFooter();        goto OK; }
  if (name == "footer")    {OutputFooter();        goto OK; }
  if (name == "trail")     {OutputFooter();        goto OK; }
  if (name == "trailer")   {OutputFooter();        goto OK; }

  if (name == "sw")        {OutputSwitches();      goto OK; }
  if (name == "switch")    {OutputSwitches();      goto OK; }
  if (name == "switches")  {OutputSwitches();      goto OK; }
  if (name == "opt")       {OutputSwitches();      goto OK; }
  if (name == "opts")      {OutputSwitches();      goto OK; }
  if (name == "option")    {OutputSwitches();      goto OK; }
  if (name == "options")   {OutputSwitches();      goto OK; }

  // Find the displayable boolean option
  optIdx = FindBooleanOption(name);
  if (BEFE::IsNull(optIdx)            ||
      boolDecls[optIdx].msgNo == NaN  ||
      boolDecls[optIdx].msgNo == Error::None) {
    status = Error::CommandUnknownQOption;
    status = SetError(argNo, status, CmdStringMessage(status, name));
    if (status) goto SOMEERROR;
    goto OK;
  }

  // Format and display the message
  message = GetMessage(boolDecls[optIdx].msgNo);
  strings = message.Split(String("¶")[0]);
  numStr = strings.Length();
  if (numStr != 3) {
    status = Error::CommandMalformedMessage;
    status = SetError(argNo, status);
    if (status) goto SOMEERROR;
    goto OK;
  }
  message = strings[0] + strings[1] + strings[2];

  flag = (TheBefe->TheGlobals.bools.array[boolDecls[optIdx].index]) ? 
            '+' : '-';
  pos = message.Find("${flag}");
  if (!BEFE::IsNull(pos))
    message.Replace(Span(pos,pos+7), flag);

  onOff = (TheBefe->TheGlobals.bools.array[boolDecls[optIdx].index]) ? 
            CONSTR_On : CONSTR_Off;
  pos = message.Find("${OnOff}");
  if (!BEFE::IsNull(pos))
    message.Replace(Span(pos,pos+8), onOff);

  Cout << message << '\n';

  // Handle errors
  while (false) {
    OK:        status = Error::None;             break;
    BADPARM:   status = Error::InvalidParameter; break;
    SOMEERROR:                                   break;
  }

  return status;

}

Status BcmdCommand::_QueryEnv(Int argNo, String &name, String &value) {

  Status  status;
  String  leadIn;
  String  leadOut;
  Strings names;
  UInt    maxNameLen;
  UInt    nameIdx;

  leadIn  = GetEnvironmentValue("BEFE_QuoteLeadIn");
  leadOut = GetEnvironmentValue("BEFE_QuoteLeadOut");

  // Handle named one
  if (!value.IsEmpty()) {

    // Make sure there's no '='
    if (value.Find('=') != NaN) {
      status = SetError(argNo, Error::CommandUnexpectedEquals);
      goto SOMEERROR;
    }

    // See if it exists
    if (!IsEnvironmentName(value)) {
      status = Error::CommandEnvNotExist;
      status = SetError(argNo, status, CmdStringMessage(status, value));
      goto SOMEERROR;
    }

    // Display it
    Cout << value << " = " << leadIn << GetEnvironmentValue(value) << leadOut << '\n';

    goto OK;

  }

  // Display all of them, but first, figure out the maximum length
  names = GetEnvironmentNames();
  if (names.IsEmpty()) {
    status = SetError(argNo, Error::CommandNoEnvSet);
    goto SOMEERROR;
  }
  maxNameLen = 0;
  for (nameIdx=0; nameIdx < names.Length(); nameIdx++)
    maxNameLen = Max(maxNameLen, names[nameIdx].Length());

  // Display all of them...
  status = names.Sort();
  if (status) goto SOMEERROR;
  for (nameIdx=0; nameIdx < names.Length(); nameIdx++)
    Cout << names[nameIdx]
         << String(' ')* (maxNameLen - names[nameIdx].Length())
         << " = " << leadIn
         << GetEnvironmentValue(names[nameIdx])
         << leadOut << '\n';

  // Handle errors...
  while (false) {
    OK:        status = Error::None; break;
    SOMEERROR:                       break;
  }

  return status;
}

//----------------------------------------------------------------------
//
// Standalone Worker Functions
//

UInt FindBooleanOption(String const &option) {

  UInt opIdx;

  opIdx = 0;
  while (boolDecls[opIdx].name && String(boolDecls[opIdx].name).Insensitive() != option)
    opIdx++;

  if (boolDecls[opIdx].name == 0)
    opIdx = UNaN;

  return opIdx;

} // ...FindBooleanOption


} // ...Namespace BEFE
