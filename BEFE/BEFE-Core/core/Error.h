//!befe-publish inc
//----------------------------------------------------------------------
// File: Error.h - Declarations for BEFE Error/Status values
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#ifndef ERROR_H // ERROR_H...
#define ERROR_H

#include "Primitives.h"

namespace BEFE { // Namespace BEFE...

// Main Error code Status return
typedef Int32 Status;

//----------------------------------------------------------------------
//
// ErrorBase Namespace
//

namespace ErrorBase { // Namespace ErrorBase...

  enum : Int { // enum...

  //
  // First level Error Space...  (Core errors)
  //
  
   General     = 0x00000000   // GEN
  ,Befe        = 0x00000100   // BEF
  ,Buffer      = 0x00000200   // BLK
  ,Sequence    = 0x00000300   // SEQ
  ,Number      = 0x00000400   // NUM
  ,Encode      = 0x00000500   // ENC
  ,Array       = 0x00000600   // ARR
  ,String      = 0x00000700   // STR
  ,Stream      = 0x00000800   // STM
  ,File        = 0x00000900   // FIL
  ,Persist     = 0x00000A00   // PIO
  ,Cache       = 0x00000B00   // BLK
  ,Match       = 0x00000C00   // MAT
  ,GlobStr     = 0x00000D00   // GST
  ,GlobOpt     = 0x00000E00   // GLB

  //
  // Second Level (Object Model) Error Space...
  //
  
  ,Id          = 0x00008100   // IDS
  ,Address     = 0x00008200   // ADD
  
  //
  // Third Level Error Space...
  //
  
  ,Command     = 0x00010000   // CMD (CommandShell)
  ,CmdLayer    = 0x00010100   // LAY (CommandLayer)
  ,CmdBEFE     = 0x00010200   // CommandLayer_BEFE Errors
  ,CmdOS       = 0x00010300   // CommandLayer_OS   Errors
  ,CmdLang     = 0x00010400   // CommandLayer_Lang Errors

  // Cryptography Errors
  ,Crypt       = 0x00020000   // SEC - General Cryptographic Services
  ,CryptKey    = 0x00020100   // CKY - Cryptographic Keys
  
  // Language Processing Errors
  ,Language    = 0x00030000   // LNG - General Language Based Services
  ,LangBefe    = 0x00030100     // BEF - .befe language errors
  ,Lexicon     = 0x00040000   // LEX - Language Lexical Analysis
  
  //
  // Third Level Error Space...
  //
  
  // "Port specific" error bases
  ,PortWin32   = 0x01000000   // Win32
  ,PortLinux32 = 0x02000000   // Linux32
  
  // BEFE Utility errors
  ,Utility     = (Int)0x80000000   // UTL
  ,UtilBuild   = (Int)0x80010000   // UBL
  ,UtilLex     = (Int)0x80020000   // ULX
	,UtilUnused  = (Int)0x80030000   // Reserved for BEFE Utilities (0x80030000..0x80FFFFFF)
	
	// Reserved Error Range 1...
	,Reserved1   = (Int)0x80FF0000   // RS2 - Reserverd Error Space (0x81000000..0xFEFFFFFF)
	
	// Application Defined Errors
	,Application = (Int)0xFF000000   // APP - Application Error Space (0xFF000000..0xFF00FFFF)
	
	// Reserved Error Range 2...
	,Reserved2   = (Int)0xFF010000   // RS2 - Reserverd Error Space (0xFF010000..0xFFFDFFFF)
	
  // Internal errors
  ,Internal    = (Int)0xFFFE0000   // XXX

}; // ...enum

} // Namespace ErrorBase

//----------------------------------------------------------------------
//
// Error Namespace
//
namespace Error {
  
  enum : Int { // Error Status Enumeration...

    // General Errors...
    None                = ErrorBase::General,
    Failed,                       // GEN+1  Failed to perform requested operation
    Cancelled,                    // GEN+2  User/System cancelled the operation
    OSError,                      // GEN+3  OS Specific Error occurred
    NoBefe,                       // GEN+4  No Befe currently allocated
    StartUpFailed,                // GEN+5  BEFE Process start up failed!!!!
    NoClassStartUp,               // GEN+6  A ClassStartUp method has not been called
    AlreadyClassStartUp,          // GEN+7  A ClassStartUp method has already been called
    AlreadyClassShutDown,         // GEN+8  A ClassShutDown method has already been called
    NoClassShutDown,              // GEN+9  A ClassShutDown method has not been called
    NotFound,                     // GEN+10 Requested item not found
    AlreadyExists,                // GEN+11 Requested item already exists
    NotImplemented,               // GEN+12 Not implemented yet
    NotCapable,                   // GEN+13 Implementation not capable of requested action
    NotApplicable,                // GEN+14 Requestion action not applicable
    PlatformNotCapable,           // GEN+15 Platform not capable of requested action
    PlatformNotApplicable,        // GEN+16 Requestion action not applicable on this Platform
    AccessDenied,                 // GEN+17 Requested action denied on Security/Integrity Grounds
    OutOfMemory,                  // GEN+18 Out of physical memory
    InvalidParameter,             // GEN+19 Invalid parameter passed
    IndexOutOfRange,              // GEN+20 Specified index is out of valid range
    IndexNotFound,                // GEN+21 Specified index does not exist
    IndexNotDistinct,             // GEN+22 Specified index is not distinct
    NoValue,                      // GEN+23 No value at specified index
    CompositeValue,               // GEN+24 Cannot treat composite value as scalar
    NotMutable,                   // GEN+25 Requested value is not mutable
    ValueTruncated,               // GEN+26 Requested value was truncated in caller's buffer
    WouldOverflow,                // GEN+27 Buffer would overflow
    SizeFixed,                    // GEN+28 Size/Length is fixed
    BadUnicodeCodePoint,          // GEN+29 Invalid Unicode code point character specified
    OperatingSystemNotSet,        // GEN+30 No "current" operating system
    OperatingSystemError,         // GEN+31 Operating system error
    NotFullPath,                  // GEN+32 Specified path is not a full path
    NotRelativePath,              // GEN+33 Specified path is not relative
    PathDepthMismatch,            // GEN+34 Specified relative path cannot be applied to path
    UnknownOption,                // GEN+35 Unknown option specified
    InvalidNumberDigit,           // GEN+36 Invalid digit in number
    InvalidNumber,                // GEN+37 Invalid number
    InvalidInteger,               // GEN+38 Invalid Integer
    IntegerOverflow,              // GEN+39 Integer overflow
    DivideByZero,                 // GEN+40 Divide by Zero
    InvalidUUID,                  // GEN+41 Invalid UUID
    InvalidDate,                  // GEN+42 Invalid Year, Month, Day combination
    InvalidYear,                  // GEN+43 Invalid Year (-9999..9999)
    InvalidMonthOfYear,           // GEN+44 Invalid Month of year (1..12)
    InvalidDayOfMonth,            // GEN+45 Invalid Day of month (1..29 or 1..31 depending on month)
    InvalidHourOfDay,             // GEN+46 Invalid Hour (0..23)
    InvalidMinuteOfHour,          // GEN+47 Invalid Minute (0..59)
    InvalidSecondOfMinute,        // GEN+48 Invalid Second (0..59)
    InvalidMillisecond,           // GEN+49 Invalid Millisecond (0..999)
    InvalidOffset,                // GEN+50 Invalid Offset
    MinExpStackEmpty,             // GEN+51 MinExp stack empty
		MinExpStackExpected1,         // GEN+52 MinExp expected stack to have at least 1 entry
    MinExpStackExpected2,         // GEN+53 MinExp expected stack to have at least 2 entries
	  MinExpStackExpectedOpenGroup, // GEN+54 MinExp expected stack to have at least 1 'Open Group'

    UnknownTest,                  // GEN+56 Unknown Test

    // Befe errors
    BefeNone            = ErrorBase::Befe,
    NoBEFE_Temp,                // BEF+1  BEFE_Temp Environment Variable not set
    NoBEFE_Root,                // BEF+2  BEFE_Root Environment Variable not set
    NoBEFE_Path,                // BEF+3  BEFE_Path Environment Variable not set
    NoBEFE_GIT,                 // BEF+4  BEFE_GIT  Environment Variable not set
    NoBEFE_TestData,            // BEF+5  BEFE_TestData Environment Variable not set
    NoBEFE_Build,               // BEF+6  BEFE_Build Environment variable Not set
    BadBEFE_Temp,               // BEF+7  BEFE_Temp Environment Variable is invalid Path
    BadBEFE_Root,               // BEF+8  BEFE_Root Environment Variable is invalid Path
    BadBEFE_Path,               // BEF+9  BEFE_Path Environment Variable is invalid Path
    BadBEFE_GIT,                // BEF+10 BEFE_GIT  Environment Variable is invalid Path
    BadBEFE_TestData,           // BEF+11 BEFE_TestData Environment Variable is invalid Path
    BadBEFE_Build,              // BEF+12 BEFE_Build Environemtn Variable is invalid Path
    BEFECallbackIsRegistered,   // BEF+13 BEFE Callback already registered
		BEFECallbackNotRegistered,  // BEF+14 BEFE Callback not registered
    BEFEMessagesAlreadyLoaded,  // BEF+15 Messages Already Loaded
		
    // Buffer errors
    BufferNone          = ErrorBase::Buffer,
    BufferNotAllocated,
    BufferAlreadyAllocated,
    
    // Sequence errors
    SeqNone             = ErrorBase::Sequence,
    SeqRangeNotResolved,        // Invalid operation requested on Unresolved Range
    SeqBadRange,                // Invalid/malformed Range specification
    SeqRangeBadString,          // Invalid primitive Range string specification
    SeqRangeBadNumber,          // Invalid primitive Range string number
    SeqSpanNotResolved,         // Invalid operation requested on Unresolved Span
    SeqBadSpan,                 // Invalid/malformed Span  specification
    SeqSpanBadString,           // Invalid primitive Span  string specification
    SeqSpanBadNumber,           // Invalid primitive Span  string number
    SeqSliceNotResolved,        // Invalid operation requested on Unresolved Slice
    SeqBadSlice,                // Invalid/malformed Slice specification
    SeqSliceBadString,          // Invalid primitive Slice string specification
    SeqSliceBadNumber,          // Invalid primitive Slice string number
    SeqBadAlloc,                // Invalid/malformed Alloc specification
    SeqBadAllocMin,             // Invalid AllocMin specification
    SeqBadAllocMax,             // Invalid AllocMax specification
    SeqBadAllocInc,             // Invalid AllocInc specification
    SeqBadAllocDec,             // Invalid AllocDec specification

    // Number errors
    NumberNone         = ErrorBase::Number,
    NumberInvalid,              // Invalid number
    NumberBadExponent,          // Invalid/malformed exponent
    NumberBadMantissa,          // Invalid/malformed mantissa
    
    // UTF-8 Errors
    UTF8None           = ErrorBase::Encode,
    UTF8InvalidChar,            // Not a valid 0x00..0x10FFFF character
    UTF8InvalidSeq,             // Not a valid UTF-8 byte sequence
    UTF8InvalidCodePoint,       // Not a valid UTF-8 code point
    UTF8NotFound,               // No character at Specified position

    // Array Errors
    ArrayNone          = ErrorBase::Array,
    ArraySizeFixed,             // Array/Sparse/Map size is fixed
    ArrayNotEmpty,              // Array/Sparse/Map not empty
    ArrayEmpty,                 // Array/Sparse/Map is empty
    MapNotIntIndexed,           // Map not Int Indexed
    MapNotStringIndexed,        // Map not String Indexed
    
    // String Errors
    StringNone         = ErrorBase::String,
    StringNotMutable,           // String IS NOT mutable
    StringIsMutable,            // String IS mutable
    StringSizeFixed,            // Cannot change the string's size
    StringEmpty,                // String is Empty
    StringNotEmpty,             // String is not Empty
    StringTruncated,            // String was truncated
    StringRemoveInvalid,        // Range/Span for Remove either Invalid or Ambiguous
    
    // Stream Errors
    StreamNone          = ErrorBase::Stream,
    StreamAtEnd,
    StreamAtBeginning,

    // File Errors
    FileNone            = ErrorBase::File,
    FileNotFound,           // FIL+1
    FileInvalidFileName,    // FIL+2
    FileInvalidPathName,    // FIL+3
    FileNotPath,            // FIL+4
    FileIsPath,             // FIL+5
    FileNotFullPath,        // FIL+6
    FileIsFullPath,         // FIL+7
    FileNoName,             // FIL+8
    FileNameTooLong,        // FIL+9
    FileNoDirectory,        // FIL+10
    FileContentsTooLong,    // FIL+11
    FileInvalidAccessMode,  // FIL+12
    FileNameIsNotDir,       // FIL+13
    FileNameIsNotFile,      // FIL+14
    FilePathIsNotDir,       // FIL+15
    FilePathIsNotFile,      // FIL+16
    FileDirDoesNotExist,    // FIL+17
		FileDirAlreadyExists,   // FIL+18
    FileDoesNotExist,       // FIL+19
    FileAlreadyExists,      // FIL+20
    FileAlreadyOpen,        // FIL+21
    FileDeleteFailed,       // FIL+22
    FileIsOpen,             // FIL+23
    FileNotOpen,            // FIL+24
    FileOpenFailed,         // FIL+25
    FileNotOpenForRead,     // FIL+26
    FileReadFailed,         // FIL+27
    FileNotOpenForWrite,    // FIL+28
    FileWriteFailed,        // FIL+29
    FileNotOpenForAppend,   // FIL+30
    FileAppendFailed,       // FIL+31
    FileGetSizeFailed,      // FIL+32
    FileSetSizeFailed,      // FIL+33
    FileBadAddress,         // FIL+34
    FileWildCardsNotValid,  // FIL+35
    FilePathsDoNotMatch,    // FIL+36 
    FileBadInitialBOM,      // FIL+37
    
    // Persistent I/O Errors
    PersistNone         = ErrorBase::Persist,
    PersistNotBEFE,             // Not BEFE Header
    PersistBadTypeName,         // Invalid PersistHead Type Name (1..4 Bytes)
    
    // Cache I/O Errors
    CacheNone         = ErrorBase::Cache,
    CacheNoOwningCache,         // BLK+1  !!!Owning Cache not set!!!
    CacheBlkSizeNotSet,         // BLK+2  !!!Block Size not set!!!
    CacheBlkNumberNotSet,       // BLK+3  !!!Block Number not set!!!
    CacheBadMaxBlocks,          // BLK+4  !!!Invalid maximum Blocks!!!
    CacheBadFileIdx,            // BLK+5  !!!Invalid File Index!!!
    CacheFileNotFree,           // BLK+6  !!!Not in Free Chain!!!
    CacheFileNotUsed,           // BLK+7  !!!Not in Used Chain!!!
    CacheFileNoFreeFiles,       // BLK+8  !!!No Free Cached Files available!!!
    CacheBadBlockIdx,           // BLK+9  !!!Invalid Block Index!!!
    CacheBlockIsChained,        // BLK+10 !!!Block Already in Free/Used Chain!!!
    CacheBlockNotChained,       // BLK+11 !!!Block is Already Unchained!!!
    CacheBlockUsedEmpty,        // BLK+12 !!!Block Used Chain is Empty!!!
    CacheBlockFreeEmpty,        // BLK+13 !!!Block Free Chain is Empty!!!
    CacheBlockIsFileChained,    // BLK+14 !!!Block is Already File Chained!!!
    CacheBlockNotFileChained,   // BLK+15 !!!Block is not File Chained!!!
    CacheBlockNotFree,          // BLK+16 !!!Not in Free Chain!!!
    CacheBlockNotUsed,          // BLK+17 !!!Not in Used Chain!!!
    CacheBlockNotFile,          // BLK+18 !!!Not in File Chain!!!
    CacheBlockUnknownFileId,    // BLK+19 !!!Unknown File Id in Block!!!
    CacheBlockDisowned,         // BLK+20 !!!Block disowned by Cache!!!
    CacheBlockReadFailed,       // BLK+21 !!!Cache Block Read Failed!!!
    CacheBLockWriteFailed,      // BLK+22 !!!Cache Block Write Failed!!!
    CacheNoFileName,            // BLK+23 No File Name specified
    CacheExpectedFullPath,      // BLK+24 Specified File Name is not a Full Path
    CacheFileNullId,            // BLK+25 Null File Id specified
    CacheFileNotCached,         // BLK+26 File not in Cache
    CacheFileAlreadyCached,     // BLK+27 File is already being cached
    CacheInvalidFileId,         // BLK+28 Invalid (future) File Id
    CacheFileIdStale,           // BLK+29 Stale (Unused) File Id
    CacheBadBlockNo,            // BLK+30 Invalid File Block Number
		CacheBadSpan,               // BLK+31 Span not in File
		CacheBadRange,              // BLK+32 Range not in File
    
    // Match errors
    MatchNone          = ErrorBase::Match,
    MatchInvalidNode,           // MAT+01 Invalid MatchNode
    MatchInvalidNodeIndex,      // MAT+02 Invalid MatchNode Index
    MatchInvalidNodeType,       // MAT+03 Invalid MatchNode Type
    MatchInvalidNodeSpan,       // MAT+04 Invalid MatchNode Char-Range/NodeArray-Span
    MatchInvalidNodeMinMax,     // MAT+05 Invalid MatchNode Occurrences Min/Max'
    MatchNodeTypeNotLeaf,       // MAT+06 MatchNode Type not TypeLeaf
    MatchNodeTypeNotStructural, // MAT+07 MatchNode Type not TypeNonLeaf
    MatchNodesNotMutable,       // MAT+08 MatchNodes not mutable
    MatchNodesFull,             // MAT+09 MatchNodes full
    MatchNodesEmpty,            // MAT+10 MatchNodes empty
    MatchNodesMultipleRoots,    // MAT+10 MatchNodes has multiple root Nodes
    MatchNodesNoRoot,           // MAT+11 MatchNodes has no root Node
    
    // Global non-Boolean messages
    GlobStrNone        = ErrorBase::GlobStr,
    GlobStrStartedUTC,           // "BEFE...   Started at: ${String}"
    GlobStrStartedLocal,         // "          Started at: ${String}"
    GlobStrElapsed,              // "             Elapsed: ${String}"
    GlobStrConfig,               // "Configurable Settings..."
    GlobStrNonConfig,            // "Non-Configurable Settings..."
    GlobStrBufferSize,           // "  Default Buffer Size.............: ${String}"
    GlobStrDefaultMaxUnfold,     // "  Unicode Default Max Unfold Chars: ${String}"
    GlobStrCurrentMaxUnfold,     // "  Unicode Current Max Unfold Chars: ${String}"

    // Global Boolean On/Off messages
    //
    // Note: These contain the special character '¶' used to separate each
    //       part.  When these message are displayed on their own, the '¶'
    //       is simply turned into a ' '.  But, when they are displayed
    //       together (like when using the '?opt' command option), the separator
    //       is used to align the separate parts so they're the same length
    //

    GlobOptNone        = ErrorBase::GlobOpt,

    GlobOpt_q,                  // GLB+01
    GlobOpt_v,                  // GLB+02
    GlobOpt_vv,                 // GLB+03
    GlobOpt_vvv,                // GLB+04
    GlobOpt_et,                 // GLB+05
    GlobOpt_ts,                 // GLB+06
    GlobOpt_localtime,          // GLB+07
    GlobOpt_dep,                // GLB+08
    GlobOpt_w,                  // GLB+09
    GlobOpt_info,               // GLB+10
    GlobOpt_debug,              // GLB+11
    GlobOpt_head,               // GLB+12
    GlobOpt_keep,               // GLB+13
    GlobOpt_allfiles,           // GLB+14
    GlobOpt_alldirs,            // GLB+15
    GlobOpt_ufolding,           // GLB+16
    GlobOpt_uturkish,           // GLB+17
    GlobOpt_m,                  // GLB+18
    GlobOpt_ma,                 // GLB+19
    GlobOpt_ms,                 // GLB+20
    GlobOpt_mf,                 // GLB+21
    GlobOpt_mfill,              // GLB+22
    GlobOpt_mcopy,              // GLB+23
    GlobOpt_mv,                 // GLB+24
    GlobOpt_mva,                // GLB+25
    GlobOpt_indent,             // GLB+26
    GlobOpt_envsubst,           // GLB+27
    GlobOpt_cc,                 // GLB+28
    GlobOpt_ci,                 // GLB+29
    GlobOpt_cutf,               // GLB+30
    GlobOpt_cb,                 // GLB+31
    GLobOpt_cint,               // GLB+32
		GlobOpt_pyout,              // GLB+33

    // Id and IdSpace errors
    IdNone              = ErrorBase::Id,
    IdSpaceNotInitialised,      // IdSpace Class not initialised
    IdSpaceFull,                // IdSpace is full
    IdUnknown,                  // Unknown Id

    // CommandShell (Command) processor Errors
    CommandNone         = ErrorBase::Command,
    CommandAlreadyRegistered,   // CMD+01 !!!Command Is Already Registered!!!
    CommandUnknownCommand,      // CMD+02 Unknown Command
    CommandUnknownArg,          // CMD+03
    CommandMissing,             // CMD+04
    CommandOptionSyntax,        // CMD+05
    CommandUnknownShell,        // CMD+06
    CommandArgsZeroOrOne,       // CMD+07
    CommandArgsOneOnly,         // CMD+08
    CommandArgsTwoOnly,         // CMD+09
    CommandHelpFileNotSet,      // CMD+10
    CommandHelpFileNotFound,    // CMD+11
    CommandErrorHead,           // CMD+12
    CommandWarningHead,         // CMD+13
    CommandFileHead,            // CMD+14
    CommandStringHead,          // CMD+15
    CommandConsoleHead,         // CMD+16
    CommandArgHead,             // CMD+17
    CommandLineColHead,         // CMD+18
    CommandUnknownOptionFlag,   // CMD+19
    CommandUnknownOption,       // CMD+20 Unknown option '${String}'
    CommandBoolHasValue,        // CMD+21 Option value invalid here
    CommandExpectedEnvName,     // CMD+22 Expected Environment variable name
    CommandUnexpectedEnvValue,  // CMD+23 Unexpected Environment variable value
    CommandExpectedEnvValue,    // CMD+24 Expected Environment variable value
    CommandUnknownEnv,          // CMD+25 Environment variable does not exist
    CommandUnexpectedQValue,    // CMD+26 Unexpected option value '${String}'
    CommandUnknownQOption,      // CMD+27 Unknown query option '?${String}'
    CommandMalformedMessage,    // CMD+28 Internal: Malformed Global (GlobOpt_<xxx>) Message
    CommandUnexpectedEquals,    // CMD+29 '=' not expected here
    CommandEnvNotExist,         // CMD+30 Environment variable '${String}' is not set
    CommandNoEnvSet,            // CMD+31 No environment variable set!!!
    CommandEnvUnexpectedValue,  // CMD+32 Value not applicable with '!env' option

    // CmdLayer Messages...
    CmdLayerNone      = ErrorBase::CmdLayer,
    CmdLayerNoShell,            // LAY+01 Internal: No CommandShell registered with CommandLayer

    // CmdLayer_BEFE Messages (most are Messages not Errors)...
    CmdBEFENone       = ErrorBase::CmdBEFE,
    CmdBEFE_Local,              // BEF+01 "Local   Date/Time: "
    CmdBEFE_GMTUTC,             // BEF+02 "UTC/GMT Date/Time: "
    CmdBEFEUnknownOption,       // BEF+03 "Unknown option '${String}'"
    CmdBEFERunWhat,             // BEF+04 "Run what?"
    CmdBEFEUnknownHelp,         // BEF+05 "Unknown Help topic '${String}'"
    CmdBEFEUnknownTest,         // BEF+06 Unknown Test '${String}'

    // CmdLayer_OS Messages (most are Messages not Errors)...
    CmdOSNone         = ErrorBase::CmdOS,
    CmdOS_CurDir,               // COS+01 "Directory: ${String}"
    CmdOS_ExpectedWorkDir,      // COS+02 "Expected one and only one argument, the new Working Directory"
    CmdOS_DirNotExist,          // COS+03 "Directory '${String}' does not exist"
    CmdOS_DirExists,            // COS+04 "'${String}' already exists"
    CmdOS_DirNotDir,            // COS+05 "'${String}' exists but isn't a directory"
    CmdOS_ApplyRelativePath,    // COS+06 "Error from OperatingSystem.ApplyRelativePath: ${Status}"
    CmdOS_DirNotFullOrRelative, // COS+07 "Directory '${String}' is not a Full or Relative path"
    CmdOS_ls_OnlyOneArg,        // COS+08 "Expected at most one argument, the directory to list"
    CmdOS_ls_DirIsEmpty,        // COS+09 "Directory '${String}' is empty"
    CmdOS_ls_Directories,       // COS+10 "Directories..."
    CmdOS_ls_DirectoriesNone,   // COS+11 "Directories... **None**"
    CmdOS_ls_Files,             // COS+12 "Files..."
    CmdOS_ls_FilesNone,         // COS+13 "Files... **None**"
    CmdOS_mkdir_ExpectedDir,    // COS+14 "Expected new directory path"
    CmdOS_rmdir_ExpectedDir,    // COS+15 "Expected existing directory path"
    CmdOS_mv_Expected2,         // COS+16 "Move command expects two arguments (<src> <dst>)"
    CmdOS_mv_SrcNotExist,       // COS+17 "Source '${String}' does not exist"
    CmdOS_mv_DstDoesExist,      // COS+18 "Destination '${String}' already exists"
    CmdOS_cp_Expected2,         // COS+19 "Copy command expects two arguments (<src> <dst>)"
    CmdOS_cp_SrcNotExist,       // COS+20 "Source '${String}' does not exist"
    CmdOS_cp_DstDoesExist,      // COS+21 "Destination '${String}' already exists"
    CmdOS_cp_DirNotImplemented, // COS+22 "Copy not implemented for a directory"
    CmdOS_rm_Expected1,         // COS+23 "Remove command expects one argument (<file>)"
    CmdOS_rm_NotExist,          // COS+24 "File '${String}' does not exist"
    CmdOS_rm_NotFile,           // COS+25 "<file> '${String}' exists but is not a file"
    CmdOS_path_Expected0,       // COS+26 "Path command expects no arguments"
    CmdOS_path_NoPath,          // COS+27 "No path, not even current working directory!!!"
    CmdOS_which_Expected1,      // COS+28 "Command expects one and only one argument (<file>)"
    CmdOS_which_NotFound,       // COS+29 "File '${String}' not found in search path"
    CmdOS_which_Found,          // COS+30 "Found at '${String}'"
    CmdOS_cat_Expected1,        // COS+31 "Command expects one and only one argument (<file>)"
    CmdOS_cat_EmptyFileName,    // COS+32 "Command expects non-empty file name"
    CmdOS_cat_NotExist,         // COS+33 "File '${String}' does not exist"
    CmdOS_cat_NotFile,          // COS+34 "'${String}' is not a file"
    CmdOS_MD5_Expected1,        // COS+35 "Command expects one and only one argument (<file>)"
    CmdOS_MD5_EmptyFileName,    // COS+36 "Command expects non-empty file name"
    CmdOS_MD5_NotExist,         // COS+37 "File '${String}' does not exist"
    CmdOS_MD5_NotFile,          // COS+38 "'${String}' is not a file"
    CmdOS_MD5_CheckSum,         // COS+39 "MD5 Checksum = 0x${String}"
    CmdOS_Query_Expected1,      // COS+40 "Query expected at least one argument"
    CmdOS_Query_IsDevice,       // COS+41 "IS a Device Name"
    CmdOS_Query_IsNotDevice,    // COS+42 "IS NOT a Device Name"
    CmdOS_Query_IsPath,         // COS+43 "IS a valid Path Name"
    CmdOS_Query_IsNotPath,      // COS+44 "IS NOT a valid Path Name"
    CmdOS_Query_IsFile,         // COS+45 "IS a valid File Name"
    CmdOS_Query_IsNotFile,      // COS+46 "IS NOT a valid File Name"
    CmdOS_Query_IsFullPath,     // COS+47 "IS a Full Path"
    CmdOS_Query_IsNotFullPath,  // COS+48 "IS NOT a Full Path"
    CmdOS_Query_IsRelPath,      // COS+49 "IS a Relative Path"
    CmdOS_Query_IsNotRelPath,   // COS+50 "IS NOT a Relative Path"
    CmdOS_Query_Exists,         // COS+51 "DOES Exist"
    CmdOS_Query_NotExist,       // COS+52 "DOES NOT Exist"
    CmdOS_Query_Invalid,        // COS+53 "Internal: Invalid query type ${Int}"
    CmdOS_Query_AndIsFile,      // COS+54 ", and is a File"
    CmdOS_Query_AndIsDirectory, // COS+55 ", and is a Directory"

    // CmdLang Messages...
    CmdLangNone       = ErrorBase::CmdLang,
    CmdLang_NoWorkspace,         // LNG+01 No Workspace Specified
    CmdLang_WorkspaceIsOpen,     // LNG+02 Workspace is already open
    CmdLang_WorkspaceNotOpen,    // LNG+03 Workspace is not open
    CmdLang_ExpectedNoArgs,      // LNG+04 No arguments expected
    CmdLang_ExpectedOneFile,     // LNG+05 Expected only one argument (file name)
    CmdLang_ExpectedOneBlockNo,  // LNG+06 Expected only one argument (block number)
    CmdLang_ExpectedOneLang,     // LNG+07 Expected only one argument (language name)
    CmdLang_ExpectedOneMatchType,// LNG+08 Expected one argument (Match Type)
    CmdLang_ExpectedOnePat,      // LNG+09 Expected one argument (pattern)
    CmdLang_ExpectedOneArg,      // LNG+10 Expected at least one argument
    CmdLang_ExpectedOneExpr,     // LNG+11 Expected one and only one expression
    CmdLang_ExpectedOneOrTwo,    // LNG+12 Expected one, or at the most two, arguments
    CmdLang_CantDisplayWhat,     // LNG+13 Don't know how to display '${what}'
    CmdLang_NoLanguages,         // LNG+14 No Languages configured
    CmdLang_LanguageNotFound,    // LNG+15 Language not Registered
    CmdLang_NoCurrentLanguage,   // LNG+16 No Current Language Set
    CmdLang_NoExpression,        // LNG+17 No Expression
    CmdLang_UnknownMatchType,    // LNG+18 Unknown Match Type
    CmdLang_NoCurrentMatch,      // LNG+19 No current Match pattern configured
    CmdLang_MissingPattern,      // LNG+20 Missing Match Pattern
    
    // EnCRYPTion/DeCRYPTion Key Errors...
    CryptKeyNone       = ErrorBase::CryptKey,
    CryptKeyNoBEFEKeyFile,      // SEC+1  BEFE_KeyFile environment variable not set'
    CryptKeyKeyFileNotSpecified,// SEC+2  No Key File specified'
    CryptKeyKeyFileNotExist,    // SEC+2  Key File does not exist'
    CryptKeyKeyFileNotFile,     // SEC+3  Key File exists but is not a file'
    CryptKeyKeyFileExists,      // SEC+4  Key File file already exists'
    CryptKeyEncryptFailed,      // SEC+5  Encryption Failed
    CryptKeyDecryptFailed,      // SEC+6  Decryption Failed
    CryptKeyEmptyKey,           // SEC+7  Empty/Null encryption key
    CryptKeyNotKeyFile,         // SEC+8  File is not a valid BEFE Encryption Key File
    CryptKeyKeyFileCorrupt,     // SEC+9  Encryption Key File is corrupt!!!
    CryptKeyUnknownKeyType,     // SEC+10 Unknown Encryption Key Type
    CryptKeyUnknownKeyRev,      // SEC+11 Unknown Encryption Key Revision
    CryptKeyInvalidKey,         // SEC+12 Invalid/Corrupt Encryption Key
    CryptFileNotEligible,       // SEC+14 File not eligible for encrypt/decrypt
    CryptFileNotEncrypted,      // SEC+13 File is not a BEFE Encrypted File
    CryptFileCorrupt,           // SEC+15 Encrypted File Corrupt, SORRY!!!
    CryptFileWrongKey,          // SEC+16 Wrong Encryption Key for File

    //
    // Language Errors
    //
    
    LangNone = ErrorBase::Language,
    
    LangNoInputSpecified,         // LNG+01 No input (file/string) specified
    
    // .befe Lexical Errors
    
    LangBefeNone = ErrorBase::LangBefe,
    
    LangBefeEndOfStream,          // BEF+01 End of Input Stream
    LangBefeInvalidTokenType,     // BEF+02 Invalid Token type
    LangBefeUnexpectedCharacter,  // BEF+03 Unexpected Character    
    LangBefeUnexpectedToken,      // BEF+04 Unexpected Lexical Token
    LangBefeUntermStringLiteral,  // BEF+05 Unterminated String Literal
    LangBefeUntermMultiQuote,     // BEF+06 Unterminated Multi-Quote Literal
    LangBefeUntermComment,        // BEF+07 Unterminated Comment
    LangBefeExpectedErrorOrWarn,  // BEF+08 Internal: Expected Error or Warning BcmdArg
    LangBefeConsoleNotInteractive,// BEF+09 Internal: Console Not Interactive
    LangBefeNoConsolePrompt,      // BEF+10 Internal: No Console prompt set

    // These "aren't really errors" and should be separated out of here...
    LangBefeHeadFileLineCol,      // BEF+11 File '${file}' (${line}/${col}):
    LangBefeHeadStringLineCol,    // BEF+12 <String> (${line}/${col}:
    LangBefeHeadConsoleLineCol,   // BEF+13 <Console> {${line}/${col}:
    
    //
    // Lexical Analysis Errors
    //
    
    LexLangNone = ErrorBase::Lexicon,
		
    LexLangSpecErrors,            // LEX+1  Error(s) in Language Specification
    LexLangIdAlreadySet,          // LEX+2  Language Id Already Set
    LexActionNotExist,            // LEX+3  Specified LexAction does not exist
    LexStateNotExist,             // LEX+4  Specified LexState does not exist
    LexLangNotRegistered,         // LEX+5  Specified LexLanguage is not registered
    LexLangIsRegistered,          // LEX+6  Specified LexLanguage is already registered
    LexLangNoName,                // LEX+7  Specified LexLanguage name is empty
    LexActionNotRegistered,       // LEX+8  Specified LexAction does not exist
    LexStateNotRegistered,        // LEX+9  Specified LexState does not exist
    LexStateIncomplete,           // LEX+10 Incomplete state transition table
    LexStateDuplicateName,        // LEX+11 Duplicate LexState name
    LexInvalidTokenType,          // LEX+12 Invalid LexToken type
    
    // LexLanguage.Validate Errors...
    LexInvalidWhite,              // LEX+13 Invalid WhiteSpace Character
    LexDuplicateWhite,            // LEX+14 Duplicate WhiteSpace Character
    LexBadNameChar,               // LEX+15 Invalid Name Character
    LexDuplicateName,             // LEX+16 Duplicate Name Character
    LexInvalidOperator,           // LEX+17 Invalid Operator Character
    LexBadOperatorSize,           // LEX+18 Invalid Operator Size
    LexBadOperatorChar,           // LEX+19 Invalid Operator Character
    LexDupOperator,               // LEX+20 Duplicate Operator
    LexDupOperatorChar,           // LEX+21 Duplicate Operator Char
    LexInvalidComment,            // LEX+22 Invalid Comment Character
    LexBadCommentSize,            // LEX+23 Invalid Comment Size
    LexBadCommentChar,            // LEX+24 Invalid Comment Character
    LexDupComment,                // LEX+25 Duplicate Comment
    LexDupCommentChar,            // LEX+26 Duplicate Comment Char
    LexDupPreprocessorChar,       // LEX+27 Duplicate Preprocessor Char
    LexBadPairSize,               // LEX+28 Bad Pair Size (expected two ASCII chars)
    LexBadPairChar,               // LEX+29 Invalid Pair Character
    LexDupPairChar,               // LEX+30 Pair Character already used
    LexBadSepChar,                // LEX+31 Invalid Separator Character (expected ASCII special chars)
    LexDupSepChar,                // LEX+32 Duplicate Separator Character
    LexBadQuoteChar,              // LEX+33 Invalid Separator Character (expected ASCII special chars)
    LexDupQuoteChar,              // LEX+34 Duplicate Separator Character
    LexBadUTFChar,                // LEX+35 Invalid UTF-8 Character
    LexUnexpectedChar,            // LEX+36 Unexpected Character

    LexStateTooMany,              // LEX+37 Too many States (0x7f maximum)
    LexStateIdNull,               // LEX+38 Expected non-Null LexState.id
    LexStateIdNotNull,            // LEX+39 Expected Null LexState.id
    LexStateExists,               // LEX+40 Specified LexState already exists
    LexStateNoName,               // LEX+41 Specified LexState name is empty
    LexActionTooMany,             // LEX+42 Too many Actions (0x7f maximum)
    LexActionIdNull,              // LEX+43 Expected non-Null LexState.id
    LexActionIdNotNull,           // LEX+44 Expected Null LexState.id
    LexActionMissingName,         // LEX+45 Specified LexState.name not set
    LexActionExists,              // LEX+46 Specified LexState already exists
    LexActionNotHandled,          // LEX+47 Specified LexAction has no handler
    
    // LexStream Errors...
    LexStreamNotAttached,         // LEX+48 Stream not Attached to a Lex		
    LexStreamIsAttached,          // LEX+49 Stream already Attached
    LexStreamAttachedOther,       // LEX+50 Stream Attached to a different Lex
    LexStreamAtEnd,               // LEX+51 Stream is at end
    LexStreamBadType,             // LEX+52 Stream Input Type not set
    LexStreamInputEmpty,          // LEX+53 Input is empty
    LexStreamNoFileName,          // LEX+54 Specified LexStream FileName not set
    LexStreamNoLangName,          // LEX+55 Specified LexStream LanguageName not set
    LexStreamMissingState,        // LEX+56 Missing LexState (not configured properly)
    LexStreamMissingAction,       // LEX+57 Missing LexAction (not configured properly)
    
    // LexWorkspace Errors...
    LexWorkspaceNotAttached,      // LEX+58 Workspace is already Attached
    LexWorkspaceIsAttached,       // LEX+59 Workspace is already Attached
    LexWorkspaceAttachedOther,    // LEX+60 Stream Attached to a different Lex
    LexWorkspaceNoFileName,       // LEX+61 Workspace File Name not set
    LexIdOverflow,                // LEX+62 Id overflow

    // LexWs Errors...
    LexWsNullBlockHeader,         // LEX+63 Null Block Header
    LexWsInvalidBlockType,        // LEX+64 Invalid Workspace File Block Type
    LexWsBlockNotFree,            // LEX+65 Supposedly Free block isn't actually Free
    LexWsBlockIsFree,             // LEX+66 Block is already Free
    LexWsBlockWrongType,          // LEX+67 Block is wrong type
    LexWsBlockWrongLevel,         // LEX+68 Block is wrong level (should be bottom level)
    LexWsBadEntryIdx,             // LEX+69 Invalid Block Entry Index
    LexWsBadBlockOffset,          // LEX+70 Invalid Block Offset
    LexWsBlobNotFound,            // LEX+71 Specified Blob not found

    // Flow Control...
    LexExitToken = (Int)0xFFFFFFFF,

    //
    // Platform Specific Errors
    //
 
    // Win32 Platform Errors
    Win32None           = ErrorBase::PortWin32,
    Win32RegInvalidKey,
    Win32RegErrorReading,
    Win32RegInvalidRoot,
    Win32RegNoWalker,
    Win32RegTooDeep,
    Win32RegNoRoot,
    Win32RegEmptyKey,
    Win32RegKeyTooLong,
    Win32RegKeyRootUnknown,
    Win32RegValueMissingFromKey,
    Win32RegKeyNotFound,
    Win32RegValueNotFound,
    Win32RegValueNotCompatible,
    Win32FailedToInvokeProcess,
    Win32OLEError,
		Win32UnknownAction,
    Win32ConsoleNotInteractive,
		
    // Linux32 Platform Errors
    Linux32None         = ErrorBase::PortLinux32,
    Linux32NotTTY,
    Linux32UnknownTermType,
    Linux32UnconfiguredTermType,
    Linux32MissingConfiguration,
    Linux32MissingEquals,
    Linux32UntermString,
    Linux32UntermTerm,
    Linux32MissingEndTerm,
    Linux32MissingAction,
    Linux32MissingSequence,
    Linux32MalformedSequence,
    Linux32DuplicateAction,
    Linux32DuplicateSequence,
    Linux32MissingTermConfig,
    Linux32InvalidCoordinate,
		
    //
    // Utility Errors
    //
    
    UtilityNone         = ErrorBase::Utility,
    UtilityFailed,              // General utility failure
    UtilityNotFound,            // Named utility not known
    UtilityHelpNotAvailable,    // Help not available for this utility

    // Utility Build Errors
    UtilBuildNone       = ErrorBase::UtilBuild,
    UtilBuildFailed,            // BLD+1  Build Failed
    UtilBuildNotValid,          // BLD+2  Not a valid BEFE Build Machine
    UtilBuildUnknownPort,       // BLD+3  Unknown build platform
    UtilBuildGITNotExist,       // BLD+4  BEFE_GIT directory does not exist
    UtilBuildGITNotDir,         // BLD+5  BEFE_GIT is not a directory
    UtilBuildBuildNotSet,       // BLD+6  BEFE_Build environment variable is not set
    UtilBuildBuildNotExist,     // BLD+7  BEFE_Build directory does not exist
    UtilBuildBuildNotDir,       // BLD+8  BEFE_Build is not a directory
    UtilBuildTempNotSet,        // BLD+9  BEFE_Temp environment variable is not set
    UtilBuildTempNotPath,       // BLD+10 BEFE_Temp is not a valid path
    UtilBuildTempNotExist,      // BLD+11 BEFE_Temp directory does not exist
    UtilBuildTempNotDir,        // BLD+12 BEFE_Temp path is not a directory
    UtilBuildExecutableMissing, // BLD+13 Required external binary Executable missing
    UtilBuildUnknownTarget,     // BLD+14 Unknown Build Target
    UtilBuildBadArgument,       // BLD+15 Error in argument <1>:
    UtilBuildUnknownOption,     // BLD+16 Unknown Build Option

    //
    // Internal errors
    //

    InternalNone        = ErrorBase::Internal,
    Internal,                   // General internal error detected
    InternalIntegrity,          // Internal Integrity Check Failed
    InternalRecursive,          // Possible infinite recursive detected
    InternalNullPointer,        // Unexpected NULL Address Pointer
    InternalBaseCallInvalid,    // Invalid call to base class's method, use this class's extended ones instead
    InternalNegativeAlloc,      // Invalid negative allocation size
    InternalZeroLength,         // Expected non-zero Length
    InternalMutating,           // Data unexpectedly Mutating
    InternalOutOfRange,         // Index not in expected Range
    InternalNoTheBefe,          // No TheBefe
    InternalNoTheOS,            // No TheBefe->TheOS initialised
    InternalNoThePlatform,      // No TheBefe->ThePlatform initialised
    InternalNoTheProcess,       // No TheBefe->TheProcess initialised
    InternalNoTheProcessor,     // No TheBefe->TheProcessor initialised
    InternalNoTheNetwork,       // No TheBefe->TheNetwork initialised
    InternalBadFilePos,         // Unexpected current position in File/Stream
    
    //
    // Masks...
    //

    MaskType  = (Int)0xFFFFFF00,      // For Getting error base
    MaskCode  = 0x000000FF,           // For Getting error code within base

    //
    // "Exit"...
    //

    CommandBOM  = (Int)0xFFFFFFFE,    // Internal: For signaling when to skip initial BOM
    CommandExit = (Int)0xFFFFFFFF     // For telling CommandShell, or whoever, to just exit

  }; // ...Error Status Enumeration

} // ...Namespace Error

} // ...Namespace BEFE

#endif // ...ERROR_H
