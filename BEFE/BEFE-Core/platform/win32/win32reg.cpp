//----------------------------------------------------------------------
// File: win32reg.cpp - Windows 32 Registry functions
//----------------------------------------------------------------------
//
// *** See technical documentation for a more detailed description ***
//
// The reason these functions exist is because I'm sick and bloody tired
// of the horrible way Windows presents it's registry.  The way their
// registry functions are called and how many people seem to accept it
// and happily just get on with it and continue to use Microsoft's
// "HOLY API" just makes me want to puke sometimes.
//
// I guess these people seem to figure that "Microsoft knows best" sort
// of like "Father Knows Best".  Come on, give me a break!  Microsoft
// never will be best as far as fitting innto "OUR environment".  So,
// taking that to heart, I need to write OUR environment.  And here
// it is...
//
// The way I see it, a Microsoft Windows registry can be thought of as
// a "structured" document.  Instead...
//
// Microsoft (in their infinite wisdom) decided to invent some off the
// wall and hokey way of accessing and storing arbitrairy data values.
// Fine, that's up to them (it IS their code), but as a consumer of
// their smart-ass technical decision I don't want to think of it the way
// they they want me to.  They have the following ideas when it comes to
// the registry...
//
// Palette: ─│┌┐└┘├┤┬┴┼▲►▼◄•○
//
//      Microsoft's take on it                     BEFE's take on it
//    ┌────────────────────────────────────────┐   ┌───────────────┐
//   M│ ○ Key type   ◄─┬─── The Registry key   │◄──┼─────────────┐ │
//   I│                │                       │   │             │ │
//   C│ ○ Key name   ◄─┘                       │   │             │ │
//   R│                                        │   │             │ │
//   O│ ○ Value type ◄─┬─── The Registry value │◄──┼───────┐     │ │
//   S│                │                       │   │       │     │ │
//   O│ ○ Value name ◄─┘                       │   │       │     │ │
//   F│                                        │   │       │     │ │
//   T│ ○ Value      ◄───── The actual "value" │◄──┼──┐    │     │ │
//    └────────────────────────────────────────┘   │  │    │     │ │
//                                                 └──┼────┼─────┼─┘
//      Contents or  ◄────────────────────────────────┘    │     │
//        Value                                            │     │
//                                                         │     │
//      File or      ◄─────────────────────────────────────┘     │
//        Attribute                                              │
//                                                               │
//      Path Name    ◄───────────────────────────────────────────┘
//
//  That's not exactly how Microsoft puts it...  They call the
//  key "type" a "handle" instead but, give me a break.  It might
//  as well be thought of as an immutable "type" instead.
//
//  Once again, Microsoft DEMANDS you can't have the '\' character
//  in key names.  Oh well, just another arbitrary MS demand.  Still
//  think it sucks!!!
//
//  The main conceptual difference between the registry and a file
//  system or a structured XML document is that, typically, files
//  "contain" LOTS of data whereas registry named values have smallish
//  values (I think Microsoft's limit is 64K for all the values for
//  a given key at but there may be a mechanism for associating
//  a file with a value content, who cares?)
//
//  The other differences are...
//
//    1) Registry values are strongly typed, whereas file systems
//       and XML documents don't, typically, enforce type restrictions.
//       Actually, you CAN "enforce" restrictions on files and XML
//       documents by running them through a validator but, typically,
//       what matters the most for both is that they're STRUCTURALLY
//       intact.
//
//    2) Registry paths are strongly typed (each "type" has a different
//       set of paths), whereas file systems don't typically enforce
//       type restrictions on paths.  On the other hand, on Microsoft
//       file systems, you could consider 'D:' a "type" just as you
//       can think of "HKEY_LOCAL_MACHINE" being a type, couldn't you?
//
//    3) File systems don't let you "open" a directory, because
//       directories only contain files.  But, the registry does.
//       So, if you think of being able to open "." and ".." and get
//       their contents, then it all makes sense.  Just as it makes
//       sense to think of the named values as attributes.  ;-)
//
//    4) File systems let you ask them how big a file is.  The
//       registry doesn't.  On the other hand, this means that, by
//       necessity, each "value" in the registry is not a huge blob
//       of binary data like "Pirates of the Carribean" on DVD.  So,
//       we can deduce the size by allocating a 64K buffer (that's
//       the published max of all the key values for a registry
//       key, see http://support.microsoft.com/kb/256986) and reading
//       the value into it.  This sucks but, that's how it is.
//
//  Anyway, what I've done here is to, intially, provide "read access"
//  to the Microsoft Windows registry, sort of like I would any
//  structured hierarchical document.  Each node in the document has
//  a name.  Each node may have zero or more "children" nodes.  And
//  each node may have zero or more named attributes.  Each attribute
//  (including the NULL attribute) has a value.
//
//  Each "value" has a "preferred" type (REG_SZ, etc.) but can be
//  retrieved in other formats.
//
//  We'll see how that goes.  It's going to be trickier when I start
//  that anyway?  Most agree the registry really sucks and we need to
//  get away from it as fast as we can.
//
//  Whew!  Now with that over...
//
//  These functions provide the low level atomic functions needed to
//  access the registry as if it were a "file system".
//
//  In fact, after going through that rant, all we've provided
//  for external use is...
//
//    Win32RegGetValueString - Return a registry value as a string
//    Win32RegGetValueInt    - Return a registry value as an integer
//
//----------------------------------------------------------------------
// Copyright (©) 2004-2017, Bruce Ferris (UK) Email: befe@bferris.co.uk
//----------------------------------------------------------------------
#include "BEFE.h"
#include "Win32.h"
#include "Win32WCHAR.h"

// According to Microsoft
#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

namespace BEFE { // Namespace BEFE...

#define PATH_CHAR '/'  // Path split character
#define ATTR_CHAR ':'  // Path Attribute split character

//----------------------------------------------------------------------
//
// Declarations and globals
//

// Symbolic root key types...
namespace KeyType { enum {
   HKCR  // HKEY_CLASSES_ROOT
  ,HKCU  // HKEY_CURRENT_USER
  ,HKLM  // HKEY__LOCAL_MACHINE
  ,HKUS  // HKEY_USERS
  ,HKPD  // HKEY_PERFORMANCE_DATA
  ,HKCC  // HKEY_CURRENT_CONFIG
  ,HKDD  // HKEY_DYN_DATA
  ,NumKeyTypes
}; };

// Integer KeyType::<n> -> HKEY mapping
static HKEY KeyTypeMap[] = {
   HKEY_CLASSES_ROOT
  ,HKEY_CURRENT_USER
  ,HKEY_LOCAL_MACHINE
  ,HKEY_USERS
  ,HKEY_PERFORMANCE_DATA
  ,HKEY_CURRENT_CONFIG
  ,HKEY_DYN_DATA
  ,0
};

// Integer KeyType::<n> -> Name mapping
static const char *KeyTypeNames[] = {
   "HKEY_CLASSES_ROOT"
  ,"HKEY_CURRENT_USER"
  ,"HKEY_LOCAL_MACHINE"
  ,"HKEY_USERS"
  ,"HKEY_PERFORMANCE_DATA"
  ,"HKEY_CURRENT_CONFIG"
  ,"HKEY_DYN_DATA"
  ,0
};

//----------------------------------------------------------------------
//
//   Win32IsValidKeyType - "Is given integer a valid KeyType?"
//   Win32ParseKey       - Parse key string into constituent parts
//

Boolean Win32RegIsValidKeyType(Int keytype) {
  return (keytype >= 0 && keytype < KeyType::NumKeyTypes);
}

Status Win32RegParseKey(const char *key, Strings &pathnames,String &attrname) {

  Status status;
  Int    keylen;

  String partname;
  Byte  *partcp;
  Int    partlen;
  Int    attrlen;

  Int     i;

  // Clear the return array and string
  pathnames.SetEmpty();
  attrname.SetEmpty();

  // Parameter validation
  if (IsNull(key)) goto EMPTYKEY;
  keylen = Strlen(key);
  if (keylen <= 0) goto EMPTYKEY;

  // Walk from back looking for attribute separator
  for (i=keylen-1; i>= 0 && key[i] != ATTR_CHAR; i--);
  if (i >= 0) {
    attrlen = keylen-i-1;
    if (attrlen != 0) {
      status = attrname.Append((Byte *)&key[i+1],attrlen);
      if (status) goto SOMEERROR;
    }
  }
  keylen = i;

  // Now, for each part in path...
  partcp = (Byte *)key;
  partlen = 0;
  for (i=0;i<keylen;i++) {
    if (key[i] == PATH_CHAR) {
      partname.SetEmpty();
      if (partlen != 0) {
        status = partname.Append((Byte *)partcp,partlen);
        if (status) goto SOMEERROR;
      }
      status = pathnames.Append(partname);
      if (status) goto SOMEERROR;
      partcp = (Byte *)key + i + 1;
      partlen = 0;
    }
    else {
      partlen++;
    }
  }
  if (partlen != 0) {
    partname.SetEmpty();
    if (partlen != 0) {
      status = partname.Append((Byte *)partcp,partlen);
      if (status) goto SOMEERROR;
    }
    status = pathnames.Append(partname);
    if (status) goto SOMEERROR;
  }

  // Handle errors
  status = Error::None;
  while (false) {
    EMPTYKEY:  status = Error::Win32RegEmptyKey; break;
    SOMEERROR:                                   break;
  }

  return status;

}

Status Win32RegFindRoot(String &rootname, Int &rootno) {

  Status status;
  Byte   buf[20];
  UInt   buflen;

  // Get the name into our buffer
  status = rootname.ToBytes(buf,sizeof(buf)-1,buflen);
  if (status) goto UNKNOWNROOT;

  // Find it...
  for (rootno=0; !IsNull(KeyTypeNames[rootno]); rootno++)
    if (Strcmpi(buf,KeyTypeNames[rootno]) == 0) break;
  if (IsNull(KeyTypeNames[rootno])) goto UNKNOWNROOT;

  // Handle errors
  status = Error::None;
  while (false) {
    UNKNOWNROOT: status = Error::Win32RegKeyRootUnknown; break;
  }

  return status;

}

Status Win32RegGetValueString(const char *key, String &value) {

  Status  status;

  DWORD   valTyp;
  UShort  valBuf[MAX_PATH+1];
  DWORD   valSiz;
  UShort  expBuf[MAX_PATH+1];
  DWORD   expSiz;
  HKEY    hKey;
  Int     error;

  UShort  regKey[MAX_KEY_LENGTH+1];
  UInt    regKeyLen;
  UInt    regKeyOff;
  Strings parts;
  String  part;
  UInt    partLen;
  String  attrName;
  Int     root;
  HKEY    hRoot;
  Int     i;
  String  tString;

  // Initialise the key handle
  hKey = 0;

  // Pull apart the string
  status = Win32RegParseKey(key, parts, attrName);
  if (status) goto SOMEERROR;

  // Find the root
  if (parts.Length() <= 0) goto EMPTYKEY;
  status = parts.Get(0,part);
  if (status) goto SOMEERROR;
  status = Win32RegFindRoot(part, root);
  if (status) goto SOMEERROR;
  hRoot = KeyTypeMap[root];

  // Form the key like Windows wants it...
  regKeyOff = 0;
  for (i=1; i < (Int)parts.Length(); i++) {
    status = parts.Get(i,part);
    if (status) goto SOMEERROR;
    if (i != 1)
      regKey[regKeyOff++] = '\\';
    status = Win32W_FromString(part, &regKey[regKeyOff], MAX_KEY_LENGTH+1 - regKeyOff, partLen);
    if (status) goto KEYTOOLONG;
    if (partLen)
      partLen--;
    if (regKeyOff + partLen + 2 > sizeof(regKey))
      goto KEYTOOLONG;
    regKeyOff += partLen;
  }

  // Open the key handle
  error = RegOpenKeyExW(hRoot
                       ,(WCHAR *)regKey
                       ,0
                       ,KEY_READ
                       ,&hKey
                       );
  if (error != ERROR_SUCCESS) goto KEYNOTFOUND;

  // Set up the attribute value
  status = Win32W_FromString(attrName, regKey, MAX_KEY_LENGTH+1, regKeyLen);
  if (status) goto KEYTOOLONG;
  if (regKeyLen)
    regKeyLen--;
    
  // Query the value type and length
  error = RegQueryValueExW(hKey,  (WCHAR *)regKey, NULL, &valTyp, NULL, &valSiz);
  if (error != ERROR_SUCCESS) goto VALNOTFOUND;

  // Make sure it's a compatible type
  if (valSiz > sizeof(valBuf)-1)
    goto VALNOTCOMPAT;

  //cout << "Win32RegGetValue: valsize = " << valsiz << endl;
  //cout << "                  valtyp  = " << valtyp << endl;

  // Get the value
  error = RegQueryValueExW(hKey, (WCHAR *)regKey, NULL, &valTyp, (BYTE *)valBuf, &valSiz);
  if (error != ERROR_SUCCESS) goto VALNOTFOUND;

  //
  // Based on the value type...
  //

  value.SetEmpty();

  switch (valTyp) {
    case REG_SZ:
      // If last byte is NUL, ignore it
      if (valSiz && valBuf[valSiz-1] == 0)
        valSiz--;
      // Append the value
      tString = Win32W_ToString(valBuf);
      status = value.Append(tString.Consumable());
      break;
    case REG_EXPAND_SZ:
      expSiz = ExpandEnvironmentStringsW((WCHAR *)valBuf, (WCHAR *)expBuf, MAX_PATH-1);
      if (expSiz && expBuf[expSiz-1] == 0)
        expSiz--;
      tString = Win32W_ToString(expBuf);
      status = value.Append(tString.Consumable());
      break;
    case REG_DWORD:
      value << ToString(*(Int *)valBuf, 10);
      goto OK;
    default:
      break;
  }

  goto VALNOTCOMPAT;

  // Handle errors
  while (false) {
    EMPTYKEY:     status = Error::Win32RegEmptyKey;           break;
    KEYTOOLONG:   status = Error::Win32RegKeyTooLong;         break;
    SOMEERROR:                                                break;
    KEYNOTFOUND:  status = Error::Win32RegKeyNotFound;        break;
    VALNOTFOUND:  status = Error::Win32RegValueNotFound;      break;
    VALNOTCOMPAT: status = Error::Win32RegValueNotCompatible; break;
    OK:           status = Error::None;                       break;
  }

  if (hKey != 0)
    RegCloseKey(hKey);

  return status;
  
}

Int Win32RegGetValueInt(const char *key) {

  Int    val;
  String value;
  Status status;

  val = NaN;
  status = Win32RegGetValueString(key,value);
  if (!status)
    val = value;

  return val;

}

//----------------------------------------------------------------------
//
// Low level stuff sort of knicked from examples on the web...
//

Status QueryKey(HKEY hKey,Strings &keys) {

  TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
  DWORD    cbName;                   // size of name string
  TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name
  DWORD    cchClassName = MAX_PATH;  // size of class string
  DWORD    cSubKeys=0;               // number of subkeys
  DWORD    cbMaxSubKey;              // longest subkey size
  DWORD    cchMaxClass;              // longest class string
  DWORD    cValues;              // number of values for key
  DWORD    cchMaxValue;          // longest value name
  DWORD    cbMaxValueData;       // longest value data
  DWORD    cbSecurityDescriptor; // size of security descriptor
  FILETIME ftLastWriteTime;      // last write time

  DWORD i, retCode;

  // Get the class name and the value count.
  retCode = RegQueryInfoKey(hKey,                   // key handle
                            achClass,               // buffer for class name
                           &cchClassName,           // size of class string
                            NULL,                   // reserved
                           &cSubKeys,               // number of subkeys
                           &cbMaxSubKey,            // longest subkey size
                           &cchMaxClass,            // longest class string
                           &cValues,                // number of values for this key
                           &cchMaxValue,            // longest value name
                           &cbMaxValueData,         // longest value data
                           &cbSecurityDescriptor,   // security descriptor
                           &ftLastWriteTime);       // last write time

  // Enumerate the subkeys, until RegEnumKeyEx fails.
  keys.SetEmpty();
  if (cSubKeys) {
    for (i=0; i<cSubKeys; i++) {
      cbName = MAX_KEY_LENGTH;
      retCode = RegEnumKeyEx(hKey, i,
                     achKey,
                     &cbName,
                     NULL,
                     NULL,
                     NULL,
                     &ftLastWriteTime);
            if (retCode == ERROR_SUCCESS) {
              String s;
              Int    j;
              for (j=0;achKey[j] != 0 && (achKey[j] & 0x80) == 0;j++);
              if (achKey[j] == 0) {
                s += (char *)achKey;
                keys.Append(s);
              }
            }
    }
  }

  return Error::None;
}

//----------------------------------------------------------------------
//
// Registry walker stack
//
// Note: This may come in handy at some point...
//

class StackEntry {
  public: Strings subkeys;
  public: Int     curindex; // Current index in subkeys (-1 = "Not started")
  public: StackEntry();
  public: ~StackEntry();
};

StackEntry::StackEntry() {
  curindex = -1;
}

StackEntry::~StackEntry() {
}

class Stack {
  public:

  typedef Status (*Walker)(void *context);

  // Members
  HKEY root;
  StackEntry *entries;
  Int         maxentries;
  Int         depth;

  Stack();                           // <--- Need to write copy constructor and operator=
  ~Stack();

  Status Clear();
  Status SetRoot(HKEY newroot);
  Status Walk(Walker thewalker,void *context);
  Status Down(); // Can be called from walker

};

Stack::Stack() {
  root = HKEY_LOCAL_MACHINE;
  maxentries = 32;
  entries = new StackEntry[this->maxentries];
  Clear();
}

Stack::~Stack() {
  Clear();
  if (entries)
    delete[] entries;
}

Status Stack::Clear() {

  Int i;

  //root = NULL;

  if (!IsNull(entries)) for (i=0;i<maxentries;i++)
    entries[i].subkeys.SetEmpty();

  depth = 0;

  return Error::None;
}

Status Stack::SetRoot(HKEY root) {
  Clear();
  this->root = root;
  return Error::None;
}

Status Stack::Walk(Walker thewalker,void *context) {

  Status status;

  status = Clear();
  if (status) goto SOMEERROR;

  // Validate walker
  if (IsNull((void *)thewalker)) goto BADWALKER;

  // Clear initial entry
  Clear();
  status = Down();
  if (status) goto SOMEERROR;

  // Until finished, walk...
  while (depth > 0) {
    if (++entries[depth-1].curindex >= (Int)entries[depth-1].subkeys.Length()) {
      entries[depth-1].subkeys.SetEmpty();
      --depth;
      continue;
    }
    else {
      status = (*thewalker)(context);
      if (status) goto SOMEERROR;
    }
  }

  // Handle errors...
  status = Error::None;
  while (false) {
    BADWALKER: status = Error::Win32RegNoWalker; break;
    SOMEERROR:                                   break;
  }

  return status;

}

Status Stack::Down() {

  Status status;
  String fullname;
  String entryname;
  Int    i;
  Byte   namebuf[256];
  UInt   usedlen;
  HKEY   downkey;

  if (IsNull(root)) goto NOROOT;
  if (depth >= maxentries || IsNull(entries)) goto FULL;

  // TEMP...
  if (depth > 4) goto OK;

  // Form the full name up to TOS...
  fullname.SetEmpty();
  for (i=0;i<depth;i++) if (i != 0) {
    entryname.SetEmpty();
    status = entries[i].subkeys.Get(entries[i].curindex,entryname);
    if (status) goto BAD;
    if (fullname.Length() != 0)
      fullname += "\\";
    fullname += entryname;
  }

  // Initialise entry and bump
  entries[depth].curindex = -1;

  // Get the list of subkeys
  status = fullname.ToBytes(namebuf,sizeof(namebuf),usedlen);
  if (status) goto BAD;
  //cout << "Depth(): Key = " << namebuf << endl;
  if( RegOpenKeyEx( root,
      (const char *)namebuf,
      0,
      KEY_READ,
      &downkey) == ERROR_SUCCESS) {
     status = QueryKey(downkey,entries[depth].subkeys);
     RegCloseKey(downkey);
     if (status) goto BAD;
  }

  // Bump the depth
  ++depth;

  // Handle errors...
  status = Error::None;
  while (false) {
    NOROOT: status = Error::Win32RegNoRoot;  break;
    FULL:   status = Error::Win32RegTooDeep; break;
    OK:     status = Error::None;            break;
    BAD:                                     break;
  }

  return status;
}

Status sidewalker(void *context) {

  Status status;
  Stack *stack;

  static int counter;

  stack = (Stack *)context;
  ++counter;
  status = stack->Down();

  return status;

}

} // ...Namespace BEFE