##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=BEFE_Dev
ConfigurationName      :=Debug
WorkspacePath          := "C:\SVN\CodeLite\BEFE"
ProjectPath            := "C:\SVN\CodeLite\BEFE\BEFE_Dev"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=Bruce
Date                   :=11/26/14
CodeLitePath           :="C:\Program Files\CodeLite"
LinkerName             :=C:/MinGW-4.8.1/bin/g++.exe 
SharedObjectLinkerName :=C:/MinGW-4.8.1/bin/g++.exe -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/befe-dev
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="BEFE_Dev.txt"
PCHCompileFlags        :=
MakeDirCommand         :=makedir
RcCmpOptions           := 
RcCompilerName         :=C:/MinGW-4.8.1/bin/windres.exe 
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch)$(INC)/array $(IncludeSwitch)$(INC)/cache $(IncludeSwitch)$(INC)/command $(IncludeSwitch)$(INC)/core $(IncludeSwitch)$(INC)/crypt $(IncludeSwitch)$(INC)/datamodel $(IncludeSwitch)$(INC)/devices $(IncludeSwitch)$(INC)/format $(IncludeSwitch)$(INC)/lang $(IncludeSwitch)$(INC)/lex $(IncludeSwitch)$(INC)/message $(IncludeSwitch)$(INC)/number $(IncludeSwitch)$(INC)/persist $(IncludeSwitch)$(INC)/platform/win32 $(IncludeSwitch)$(INC)/seq $(IncludeSwitch)$(INC)/storage $(IncludeSwitch)$(INC)/test $(IncludeSwitch)$(INC)/various 
IncludePCH             := 
RcIncludePath          := 
Libs                   := 
ArLibs                 :=  
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := C:/MinGW-4.8.1/bin/ar.exe rcu
CXX      := C:/MinGW-4.8.1/bin/g++.exe 
CC       := C:/MinGW-4.8.1/bin/gcc.exe 
CXXFLAGS :=  -g -O0 -Wall -std=c++0x -fno-exceptions -finline-functions -nodefaultlibs -fno-rtti $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := C:/MinGW-4.8.1/bin/as.exe 


##
## User defined environment variables
##
CodeLiteDir:=C:\Program Files\CodeLite
UNIT_TEST_PP_SRC_DIR:=C:\UnitTest++-1.3
Objects0=$(IntermediateDirectory)/BEFE-Core_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/array__ArrayBase.cpp$(ObjectSuffix) $(IntermediateDirectory)/array__MapBase.cpp$(ObjectSuffix) $(IntermediateDirectory)/array__SparseBase.cpp$(ObjectSuffix) $(IntermediateDirectory)/array_Bytes.cpp$(ObjectSuffix) $(IntermediateDirectory)/array_Numbers.cpp$(ObjectSuffix) $(IntermediateDirectory)/array_Strings.cpp$(ObjectSuffix) $(IntermediateDirectory)/cache_Cache.cpp$(ObjectSuffix) $(IntermediateDirectory)/cache_CacheBlock.cpp$(ObjectSuffix) $(IntermediateDirectory)/cache_CacheBlockInfo.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/cache_CacheChain.cpp$(ObjectSuffix) $(IntermediateDirectory)/cache_CacheFile.cpp$(ObjectSuffix) $(IntermediateDirectory)/cache_CacheFileInfo.cpp$(ObjectSuffix) $(IntermediateDirectory)/cache_CacheFormat.cpp$(ObjectSuffix) $(IntermediateDirectory)/cache_CacheInfo.cpp$(ObjectSuffix) $(IntermediateDirectory)/command_cmdutils.cpp$(ObjectSuffix) $(IntermediateDirectory)/command_Command_Analyse.cpp$(ObjectSuffix) $(IntermediateDirectory)/command_Command_Build.cpp$(ObjectSuffix) $(IntermediateDirectory)/command_Command_Crypt.cpp$(ObjectSuffix) $(IntermediateDirectory)/command_CommandDecl.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/command_CommandLayer.cpp$(ObjectSuffix) $(IntermediateDirectory)/command_CommandLayer_BEFE.cpp$(ObjectSuffix) $(IntermediateDirectory)/command_CommandLayer_Lang.cpp$(ObjectSuffix) $(IntermediateDirectory)/command_CommandLayer_OS.cpp$(ObjectSuffix) $(IntermediateDirectory)/command_CommandShell.cpp$(ObjectSuffix) $(IntermediateDirectory)/command_Help.cpp$(ObjectSuffix) $(IntermediateDirectory)/core__SanityCheck.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_BEFE.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_BEFECallbacks.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_BEFEEnviron.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/core_BEFEWrappers.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_Buffer.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_BufferLow.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_Error.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_Globals.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_Network.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_OperatingSystem.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_Platform.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_Process.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_Processor.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/core_SBuffer.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_String.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringAppend.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringBuild.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringCompare.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringConstruct.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringFind.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringFolding.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringFormat.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringGeneral.cpp$(ObjectSuffix) \
	

Objects1=$(IntermediateDirectory)/core_StringGet.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringInsert.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringJoin.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringManip.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringOperator.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringPersist.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringQuery.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringRemove.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringReplace.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringReverse.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/core_StringSplit.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_StringSubscript.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_Time.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_UTF8.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_UTF8Fold.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_UUID.cpp$(ObjectSuffix) $(IntermediateDirectory)/core_VarTerm.cpp$(ObjectSuffix) $(IntermediateDirectory)/crypt_Crypt.cpp$(ObjectSuffix) $(IntermediateDirectory)/crypt_Crypt0.cpp$(ObjectSuffix) $(IntermediateDirectory)/crypt_CryptFileHead.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/crypt_CryptKey.cpp$(ObjectSuffix) $(IntermediateDirectory)/datamodel_Class.cpp$(ObjectSuffix) $(IntermediateDirectory)/datamodel_Ids.cpp$(ObjectSuffix) $(IntermediateDirectory)/datamodel_IdSpace.cpp$(ObjectSuffix) $(IntermediateDirectory)/datamodel_IdSpaceInfoExt.cpp$(ObjectSuffix) $(IntermediateDirectory)/datamodel_StringSpace.cpp$(ObjectSuffix) $(IntermediateDirectory)/devices_Console.cpp$(ObjectSuffix) $(IntermediateDirectory)/format_FormatOut.cpp$(ObjectSuffix) $(IntermediateDirectory)/format_LexFormatOut.cpp$(ObjectSuffix) $(IntermediateDirectory)/format_ToAscii.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/format_ToString.cpp$(ObjectSuffix) $(IntermediateDirectory)/format_ToStrings.cpp$(ObjectSuffix) $(IntermediateDirectory)/lang_Bcmd.cpp$(ObjectSuffix) $(IntermediateDirectory)/lang_BcmdArg.cpp$(ObjectSuffix) $(IntermediateDirectory)/lang_BcmdCommand.cpp$(ObjectSuffix) $(IntermediateDirectory)/lang_Blang.cpp$(ObjectSuffix) $(IntermediateDirectory)/lang_Bmsg.cpp$(ObjectSuffix) $(IntermediateDirectory)/lang_BmsgLine.cpp$(ObjectSuffix) $(IntermediateDirectory)/lang_BmsgToken.cpp$(ObjectSuffix) $(IntermediateDirectory)/lex_Lex.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/lex_LexAction.cpp$(ObjectSuffix) $(IntermediateDirectory)/lex_LexFuncs.cpp$(ObjectSuffix) $(IntermediateDirectory)/lex_LexLangActions.cpp$(ObjectSuffix) $(IntermediateDirectory)/lex_LexLangBefe.cpp$(ObjectSuffix) $(IntermediateDirectory)/lex_LexLangCpp.cpp$(ObjectSuffix) $(IntermediateDirectory)/lex_LexLanguage.cpp$(ObjectSuffix) $(IntermediateDirectory)/lex_LexRange.cpp$(ObjectSuffix) $(IntermediateDirectory)/lex_LexState.cpp$(ObjectSuffix) $(IntermediateDirectory)/lex_LexStream.cpp$(ObjectSuffix) $(IntermediateDirectory)/lex_LexToken.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/lex_Match.cpp$(ObjectSuffix) $(IntermediateDirectory)/lex_MatchNode.cpp$(ObjectSuffix) $(IntermediateDirectory)/lex_MatchNodes.cpp$(ObjectSuffix) $(IntermediateDirectory)/message_Message.cpp$(ObjectSuffix) $(IntermediateDirectory)/message_MessageMessages.cpp$(ObjectSuffix) $(IntermediateDirectory)/message_MessageRange.cpp$(ObjectSuffix) $(IntermediateDirectory)/message_MessageRanges.cpp$(ObjectSuffix) 

Objects2=$(IntermediateDirectory)/number_Char.cpp$(ObjectSuffix) $(IntermediateDirectory)/number_Crc32.cpp$(ObjectSuffix) $(IntermediateDirectory)/number_Integer.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/number_IntegerDigit.cpp$(ObjectSuffix) $(IntermediateDirectory)/number_IntegerFull.cpp$(ObjectSuffix) $(IntermediateDirectory)/number_IntegerOperator.cpp$(ObjectSuffix) $(IntermediateDirectory)/number_IntFuncs.cpp$(ObjectSuffix) $(IntermediateDirectory)/number_MD5.cpp$(ObjectSuffix) $(IntermediateDirectory)/number_MD5Calc.cpp$(ObjectSuffix) $(IntermediateDirectory)/number_Number.cpp$(ObjectSuffix) $(IntermediateDirectory)/number_OraNumber.cpp$(ObjectSuffix) $(IntermediateDirectory)/persist_LexWorkspace.cpp$(ObjectSuffix) $(IntermediateDirectory)/persist_LexWorkspaceBlob.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/persist_LexWorkspaceFile.cpp$(ObjectSuffix) $(IntermediateDirectory)/persist_LexWorkspaceIdEntry.cpp$(ObjectSuffix) $(IntermediateDirectory)/persist_LexWorkspaceString.cpp$(ObjectSuffix) $(IntermediateDirectory)/persist_LexWsArrayHead.cpp$(ObjectSuffix) $(IntermediateDirectory)/persist_LexWsBlob.cpp$(ObjectSuffix) $(IntermediateDirectory)/persist_LexWsBlockHead.cpp$(ObjectSuffix) $(IntermediateDirectory)/persist_LexWsFile.cpp$(ObjectSuffix) $(IntermediateDirectory)/persist_LexWsHead.cpp$(ObjectSuffix) $(IntermediateDirectory)/persist_LexWsString.cpp$(ObjectSuffix) $(IntermediateDirectory)/persist_Persist.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/persist_PersistAnon.cpp$(ObjectSuffix) $(IntermediateDirectory)/persist_PersistAtomic.cpp$(ObjectSuffix) $(IntermediateDirectory)/persist_PersistHead.cpp$(ObjectSuffix) $(IntermediateDirectory)/persist_PersistNamed.cpp$(ObjectSuffix) $(IntermediateDirectory)/seq_Alloc.cpp$(ObjectSuffix) $(IntermediateDirectory)/seq_Cycle.cpp$(ObjectSuffix) $(IntermediateDirectory)/seq_Range.cpp$(ObjectSuffix) $(IntermediateDirectory)/seq_Slice.cpp$(ObjectSuffix) $(IntermediateDirectory)/seq_SortBubble.cpp$(ObjectSuffix) $(IntermediateDirectory)/seq_SortInsertion.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/seq_SortMerge.cpp$(ObjectSuffix) $(IntermediateDirectory)/seq_SortSelection.cpp$(ObjectSuffix) $(IntermediateDirectory)/seq_Span.cpp$(ObjectSuffix) $(IntermediateDirectory)/seq_URange.cpp$(ObjectSuffix) $(IntermediateDirectory)/seq_USpan.cpp$(ObjectSuffix) $(IntermediateDirectory)/storage_BufferStream.cpp$(ObjectSuffix) $(IntermediateDirectory)/storage_File.cpp$(ObjectSuffix) $(IntermediateDirectory)/storage_FileInfo.cpp$(ObjectSuffix) $(IntermediateDirectory)/storage_FileStream.cpp$(ObjectSuffix) $(IntermediateDirectory)/storage_Lines.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/storage_MemoryStats.cpp$(ObjectSuffix) $(IntermediateDirectory)/storage_MemoryUtils.cpp$(ObjectSuffix) $(IntermediateDirectory)/storage_Stream.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_Test.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestArray.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestBcmd.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestCache.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestConsole.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestContains.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestFile.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/test_TestIdSpace.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestLexicon.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestMD5.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestMessage.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestMorphing.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestNull.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestNumber.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestSequence.cpp$(ObjectSuffix) 

Objects3=$(IntermediateDirectory)/test_TestStream.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestString.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/test_TestTime.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestUTF8.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestUUID.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_TestVarTerm.cpp$(ObjectSuffix) $(IntermediateDirectory)/test_trythis.cpp$(ObjectSuffix) $(IntermediateDirectory)/various_Build.cpp$(ObjectSuffix) $(IntermediateDirectory)/various_BuildDisplay.cpp$(ObjectSuffix) $(IntermediateDirectory)/various_BuildFiles.cpp$(ObjectSuffix) $(IntermediateDirectory)/various_BuildMisc.cpp$(ObjectSuffix) $(IntermediateDirectory)/various_BuildSettings.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/various_BuildZip.cpp$(ObjectSuffix) $(IntermediateDirectory)/various_Dictionary.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32__Iphlpapi.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32__Kernel32.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32__Ole32.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32__Psapi.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32_Win32.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32_Win32Console.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32_Win32File.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32_Win32Network.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/win32_Win32OperatingSystem.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32_Win32Platform.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32_Win32Process.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32_Win32Processor.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32_win32reg.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32_win32utils.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32_Win32WCHAR.cpp$(ObjectSuffix) $(IntermediateDirectory)/win32_Win32xterm.cpp$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) $(Objects2) $(Objects3) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	@echo $(Objects2) >> $(ObjectsFileList)
	@echo $(Objects3) >> $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

$(IntermediateDirectory)/.d:
	@$(MakeDirCommand) "./Debug"

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/BEFE-Core_main.cpp$(ObjectSuffix): ../../../BEFE-Core/main.cpp $(IntermediateDirectory)/BEFE-Core_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/BEFE-Core_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/BEFE-Core_main.cpp$(DependSuffix): ../../../BEFE-Core/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/BEFE-Core_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/BEFE-Core_main.cpp$(DependSuffix) -MM "../../../BEFE-Core/main.cpp"

$(IntermediateDirectory)/BEFE-Core_main.cpp$(PreprocessSuffix): ../../../BEFE-Core/main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/BEFE-Core_main.cpp$(PreprocessSuffix) "../../../BEFE-Core/main.cpp"

$(IntermediateDirectory)/array__ArrayBase.cpp$(ObjectSuffix): ../../../BEFE-Core/array/_ArrayBase.cpp $(IntermediateDirectory)/array__ArrayBase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/array/_ArrayBase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/array__ArrayBase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/array__ArrayBase.cpp$(DependSuffix): ../../../BEFE-Core/array/_ArrayBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/array__ArrayBase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/array__ArrayBase.cpp$(DependSuffix) -MM "../../../BEFE-Core/array/_ArrayBase.cpp"

$(IntermediateDirectory)/array__ArrayBase.cpp$(PreprocessSuffix): ../../../BEFE-Core/array/_ArrayBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/array__ArrayBase.cpp$(PreprocessSuffix) "../../../BEFE-Core/array/_ArrayBase.cpp"

$(IntermediateDirectory)/array__MapBase.cpp$(ObjectSuffix): ../../../BEFE-Core/array/_MapBase.cpp $(IntermediateDirectory)/array__MapBase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/array/_MapBase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/array__MapBase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/array__MapBase.cpp$(DependSuffix): ../../../BEFE-Core/array/_MapBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/array__MapBase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/array__MapBase.cpp$(DependSuffix) -MM "../../../BEFE-Core/array/_MapBase.cpp"

$(IntermediateDirectory)/array__MapBase.cpp$(PreprocessSuffix): ../../../BEFE-Core/array/_MapBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/array__MapBase.cpp$(PreprocessSuffix) "../../../BEFE-Core/array/_MapBase.cpp"

$(IntermediateDirectory)/array__SparseBase.cpp$(ObjectSuffix): ../../../BEFE-Core/array/_SparseBase.cpp $(IntermediateDirectory)/array__SparseBase.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/array/_SparseBase.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/array__SparseBase.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/array__SparseBase.cpp$(DependSuffix): ../../../BEFE-Core/array/_SparseBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/array__SparseBase.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/array__SparseBase.cpp$(DependSuffix) -MM "../../../BEFE-Core/array/_SparseBase.cpp"

$(IntermediateDirectory)/array__SparseBase.cpp$(PreprocessSuffix): ../../../BEFE-Core/array/_SparseBase.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/array__SparseBase.cpp$(PreprocessSuffix) "../../../BEFE-Core/array/_SparseBase.cpp"

$(IntermediateDirectory)/array_Bytes.cpp$(ObjectSuffix): ../../../BEFE-Core/array/Bytes.cpp $(IntermediateDirectory)/array_Bytes.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/array/Bytes.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/array_Bytes.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/array_Bytes.cpp$(DependSuffix): ../../../BEFE-Core/array/Bytes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/array_Bytes.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/array_Bytes.cpp$(DependSuffix) -MM "../../../BEFE-Core/array/Bytes.cpp"

$(IntermediateDirectory)/array_Bytes.cpp$(PreprocessSuffix): ../../../BEFE-Core/array/Bytes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/array_Bytes.cpp$(PreprocessSuffix) "../../../BEFE-Core/array/Bytes.cpp"

$(IntermediateDirectory)/array_Numbers.cpp$(ObjectSuffix): ../../../BEFE-Core/array/Numbers.cpp $(IntermediateDirectory)/array_Numbers.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/array/Numbers.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/array_Numbers.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/array_Numbers.cpp$(DependSuffix): ../../../BEFE-Core/array/Numbers.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/array_Numbers.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/array_Numbers.cpp$(DependSuffix) -MM "../../../BEFE-Core/array/Numbers.cpp"

$(IntermediateDirectory)/array_Numbers.cpp$(PreprocessSuffix): ../../../BEFE-Core/array/Numbers.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/array_Numbers.cpp$(PreprocessSuffix) "../../../BEFE-Core/array/Numbers.cpp"

$(IntermediateDirectory)/array_Strings.cpp$(ObjectSuffix): ../../../BEFE-Core/array/Strings.cpp $(IntermediateDirectory)/array_Strings.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/array/Strings.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/array_Strings.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/array_Strings.cpp$(DependSuffix): ../../../BEFE-Core/array/Strings.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/array_Strings.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/array_Strings.cpp$(DependSuffix) -MM "../../../BEFE-Core/array/Strings.cpp"

$(IntermediateDirectory)/array_Strings.cpp$(PreprocessSuffix): ../../../BEFE-Core/array/Strings.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/array_Strings.cpp$(PreprocessSuffix) "../../../BEFE-Core/array/Strings.cpp"

$(IntermediateDirectory)/cache_Cache.cpp$(ObjectSuffix): ../../../BEFE-Core/cache/Cache.cpp $(IntermediateDirectory)/cache_Cache.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/cache/Cache.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cache_Cache.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cache_Cache.cpp$(DependSuffix): ../../../BEFE-Core/cache/Cache.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cache_Cache.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/cache_Cache.cpp$(DependSuffix) -MM "../../../BEFE-Core/cache/Cache.cpp"

$(IntermediateDirectory)/cache_Cache.cpp$(PreprocessSuffix): ../../../BEFE-Core/cache/Cache.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cache_Cache.cpp$(PreprocessSuffix) "../../../BEFE-Core/cache/Cache.cpp"

$(IntermediateDirectory)/cache_CacheBlock.cpp$(ObjectSuffix): ../../../BEFE-Core/cache/CacheBlock.cpp $(IntermediateDirectory)/cache_CacheBlock.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/cache/CacheBlock.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cache_CacheBlock.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cache_CacheBlock.cpp$(DependSuffix): ../../../BEFE-Core/cache/CacheBlock.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cache_CacheBlock.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/cache_CacheBlock.cpp$(DependSuffix) -MM "../../../BEFE-Core/cache/CacheBlock.cpp"

$(IntermediateDirectory)/cache_CacheBlock.cpp$(PreprocessSuffix): ../../../BEFE-Core/cache/CacheBlock.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cache_CacheBlock.cpp$(PreprocessSuffix) "../../../BEFE-Core/cache/CacheBlock.cpp"

$(IntermediateDirectory)/cache_CacheBlockInfo.cpp$(ObjectSuffix): ../../../BEFE-Core/cache/CacheBlockInfo.cpp $(IntermediateDirectory)/cache_CacheBlockInfo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/cache/CacheBlockInfo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cache_CacheBlockInfo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cache_CacheBlockInfo.cpp$(DependSuffix): ../../../BEFE-Core/cache/CacheBlockInfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cache_CacheBlockInfo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/cache_CacheBlockInfo.cpp$(DependSuffix) -MM "../../../BEFE-Core/cache/CacheBlockInfo.cpp"

$(IntermediateDirectory)/cache_CacheBlockInfo.cpp$(PreprocessSuffix): ../../../BEFE-Core/cache/CacheBlockInfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cache_CacheBlockInfo.cpp$(PreprocessSuffix) "../../../BEFE-Core/cache/CacheBlockInfo.cpp"

$(IntermediateDirectory)/cache_CacheChain.cpp$(ObjectSuffix): ../../../BEFE-Core/cache/CacheChain.cpp $(IntermediateDirectory)/cache_CacheChain.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/cache/CacheChain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cache_CacheChain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cache_CacheChain.cpp$(DependSuffix): ../../../BEFE-Core/cache/CacheChain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cache_CacheChain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/cache_CacheChain.cpp$(DependSuffix) -MM "../../../BEFE-Core/cache/CacheChain.cpp"

$(IntermediateDirectory)/cache_CacheChain.cpp$(PreprocessSuffix): ../../../BEFE-Core/cache/CacheChain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cache_CacheChain.cpp$(PreprocessSuffix) "../../../BEFE-Core/cache/CacheChain.cpp"

$(IntermediateDirectory)/cache_CacheFile.cpp$(ObjectSuffix): ../../../BEFE-Core/cache/CacheFile.cpp $(IntermediateDirectory)/cache_CacheFile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/cache/CacheFile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cache_CacheFile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cache_CacheFile.cpp$(DependSuffix): ../../../BEFE-Core/cache/CacheFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cache_CacheFile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/cache_CacheFile.cpp$(DependSuffix) -MM "../../../BEFE-Core/cache/CacheFile.cpp"

$(IntermediateDirectory)/cache_CacheFile.cpp$(PreprocessSuffix): ../../../BEFE-Core/cache/CacheFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cache_CacheFile.cpp$(PreprocessSuffix) "../../../BEFE-Core/cache/CacheFile.cpp"

$(IntermediateDirectory)/cache_CacheFileInfo.cpp$(ObjectSuffix): ../../../BEFE-Core/cache/CacheFileInfo.cpp $(IntermediateDirectory)/cache_CacheFileInfo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/cache/CacheFileInfo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cache_CacheFileInfo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cache_CacheFileInfo.cpp$(DependSuffix): ../../../BEFE-Core/cache/CacheFileInfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cache_CacheFileInfo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/cache_CacheFileInfo.cpp$(DependSuffix) -MM "../../../BEFE-Core/cache/CacheFileInfo.cpp"

$(IntermediateDirectory)/cache_CacheFileInfo.cpp$(PreprocessSuffix): ../../../BEFE-Core/cache/CacheFileInfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cache_CacheFileInfo.cpp$(PreprocessSuffix) "../../../BEFE-Core/cache/CacheFileInfo.cpp"

$(IntermediateDirectory)/cache_CacheFormat.cpp$(ObjectSuffix): ../../../BEFE-Core/cache/CacheFormat.cpp $(IntermediateDirectory)/cache_CacheFormat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/cache/CacheFormat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cache_CacheFormat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cache_CacheFormat.cpp$(DependSuffix): ../../../BEFE-Core/cache/CacheFormat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cache_CacheFormat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/cache_CacheFormat.cpp$(DependSuffix) -MM "../../../BEFE-Core/cache/CacheFormat.cpp"

$(IntermediateDirectory)/cache_CacheFormat.cpp$(PreprocessSuffix): ../../../BEFE-Core/cache/CacheFormat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cache_CacheFormat.cpp$(PreprocessSuffix) "../../../BEFE-Core/cache/CacheFormat.cpp"

$(IntermediateDirectory)/cache_CacheInfo.cpp$(ObjectSuffix): ../../../BEFE-Core/cache/CacheInfo.cpp $(IntermediateDirectory)/cache_CacheInfo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/cache/CacheInfo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/cache_CacheInfo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/cache_CacheInfo.cpp$(DependSuffix): ../../../BEFE-Core/cache/CacheInfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/cache_CacheInfo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/cache_CacheInfo.cpp$(DependSuffix) -MM "../../../BEFE-Core/cache/CacheInfo.cpp"

$(IntermediateDirectory)/cache_CacheInfo.cpp$(PreprocessSuffix): ../../../BEFE-Core/cache/CacheInfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/cache_CacheInfo.cpp$(PreprocessSuffix) "../../../BEFE-Core/cache/CacheInfo.cpp"

$(IntermediateDirectory)/command_cmdutils.cpp$(ObjectSuffix): ../../../BEFE-Core/command/cmdutils.cpp $(IntermediateDirectory)/command_cmdutils.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/command/cmdutils.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/command_cmdutils.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/command_cmdutils.cpp$(DependSuffix): ../../../BEFE-Core/command/cmdutils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/command_cmdutils.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/command_cmdutils.cpp$(DependSuffix) -MM "../../../BEFE-Core/command/cmdutils.cpp"

$(IntermediateDirectory)/command_cmdutils.cpp$(PreprocessSuffix): ../../../BEFE-Core/command/cmdutils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/command_cmdutils.cpp$(PreprocessSuffix) "../../../BEFE-Core/command/cmdutils.cpp"

$(IntermediateDirectory)/command_Command_Analyse.cpp$(ObjectSuffix): ../../../BEFE-Core/command/Command_Analyse.cpp $(IntermediateDirectory)/command_Command_Analyse.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/command/Command_Analyse.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/command_Command_Analyse.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/command_Command_Analyse.cpp$(DependSuffix): ../../../BEFE-Core/command/Command_Analyse.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/command_Command_Analyse.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/command_Command_Analyse.cpp$(DependSuffix) -MM "../../../BEFE-Core/command/Command_Analyse.cpp"

$(IntermediateDirectory)/command_Command_Analyse.cpp$(PreprocessSuffix): ../../../BEFE-Core/command/Command_Analyse.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/command_Command_Analyse.cpp$(PreprocessSuffix) "../../../BEFE-Core/command/Command_Analyse.cpp"

$(IntermediateDirectory)/command_Command_Build.cpp$(ObjectSuffix): ../../../BEFE-Core/command/Command_Build.cpp $(IntermediateDirectory)/command_Command_Build.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/command/Command_Build.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/command_Command_Build.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/command_Command_Build.cpp$(DependSuffix): ../../../BEFE-Core/command/Command_Build.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/command_Command_Build.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/command_Command_Build.cpp$(DependSuffix) -MM "../../../BEFE-Core/command/Command_Build.cpp"

$(IntermediateDirectory)/command_Command_Build.cpp$(PreprocessSuffix): ../../../BEFE-Core/command/Command_Build.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/command_Command_Build.cpp$(PreprocessSuffix) "../../../BEFE-Core/command/Command_Build.cpp"

$(IntermediateDirectory)/command_Command_Crypt.cpp$(ObjectSuffix): ../../../BEFE-Core/command/Command_Crypt.cpp $(IntermediateDirectory)/command_Command_Crypt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/command/Command_Crypt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/command_Command_Crypt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/command_Command_Crypt.cpp$(DependSuffix): ../../../BEFE-Core/command/Command_Crypt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/command_Command_Crypt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/command_Command_Crypt.cpp$(DependSuffix) -MM "../../../BEFE-Core/command/Command_Crypt.cpp"

$(IntermediateDirectory)/command_Command_Crypt.cpp$(PreprocessSuffix): ../../../BEFE-Core/command/Command_Crypt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/command_Command_Crypt.cpp$(PreprocessSuffix) "../../../BEFE-Core/command/Command_Crypt.cpp"

$(IntermediateDirectory)/command_CommandDecl.cpp$(ObjectSuffix): ../../../BEFE-Core/command/CommandDecl.cpp $(IntermediateDirectory)/command_CommandDecl.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/command/CommandDecl.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/command_CommandDecl.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/command_CommandDecl.cpp$(DependSuffix): ../../../BEFE-Core/command/CommandDecl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/command_CommandDecl.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/command_CommandDecl.cpp$(DependSuffix) -MM "../../../BEFE-Core/command/CommandDecl.cpp"

$(IntermediateDirectory)/command_CommandDecl.cpp$(PreprocessSuffix): ../../../BEFE-Core/command/CommandDecl.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/command_CommandDecl.cpp$(PreprocessSuffix) "../../../BEFE-Core/command/CommandDecl.cpp"

$(IntermediateDirectory)/command_CommandLayer.cpp$(ObjectSuffix): ../../../BEFE-Core/command/CommandLayer.cpp $(IntermediateDirectory)/command_CommandLayer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/command/CommandLayer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/command_CommandLayer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/command_CommandLayer.cpp$(DependSuffix): ../../../BEFE-Core/command/CommandLayer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/command_CommandLayer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/command_CommandLayer.cpp$(DependSuffix) -MM "../../../BEFE-Core/command/CommandLayer.cpp"

$(IntermediateDirectory)/command_CommandLayer.cpp$(PreprocessSuffix): ../../../BEFE-Core/command/CommandLayer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/command_CommandLayer.cpp$(PreprocessSuffix) "../../../BEFE-Core/command/CommandLayer.cpp"

$(IntermediateDirectory)/command_CommandLayer_BEFE.cpp$(ObjectSuffix): ../../../BEFE-Core/command/CommandLayer_BEFE.cpp $(IntermediateDirectory)/command_CommandLayer_BEFE.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/command/CommandLayer_BEFE.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/command_CommandLayer_BEFE.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/command_CommandLayer_BEFE.cpp$(DependSuffix): ../../../BEFE-Core/command/CommandLayer_BEFE.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/command_CommandLayer_BEFE.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/command_CommandLayer_BEFE.cpp$(DependSuffix) -MM "../../../BEFE-Core/command/CommandLayer_BEFE.cpp"

$(IntermediateDirectory)/command_CommandLayer_BEFE.cpp$(PreprocessSuffix): ../../../BEFE-Core/command/CommandLayer_BEFE.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/command_CommandLayer_BEFE.cpp$(PreprocessSuffix) "../../../BEFE-Core/command/CommandLayer_BEFE.cpp"

$(IntermediateDirectory)/command_CommandLayer_Lang.cpp$(ObjectSuffix): ../../../BEFE-Core/command/CommandLayer_Lang.cpp $(IntermediateDirectory)/command_CommandLayer_Lang.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/command/CommandLayer_Lang.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/command_CommandLayer_Lang.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/command_CommandLayer_Lang.cpp$(DependSuffix): ../../../BEFE-Core/command/CommandLayer_Lang.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/command_CommandLayer_Lang.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/command_CommandLayer_Lang.cpp$(DependSuffix) -MM "../../../BEFE-Core/command/CommandLayer_Lang.cpp"

$(IntermediateDirectory)/command_CommandLayer_Lang.cpp$(PreprocessSuffix): ../../../BEFE-Core/command/CommandLayer_Lang.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/command_CommandLayer_Lang.cpp$(PreprocessSuffix) "../../../BEFE-Core/command/CommandLayer_Lang.cpp"

$(IntermediateDirectory)/command_CommandLayer_OS.cpp$(ObjectSuffix): ../../../BEFE-Core/command/CommandLayer_OS.cpp $(IntermediateDirectory)/command_CommandLayer_OS.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/command/CommandLayer_OS.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/command_CommandLayer_OS.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/command_CommandLayer_OS.cpp$(DependSuffix): ../../../BEFE-Core/command/CommandLayer_OS.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/command_CommandLayer_OS.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/command_CommandLayer_OS.cpp$(DependSuffix) -MM "../../../BEFE-Core/command/CommandLayer_OS.cpp"

$(IntermediateDirectory)/command_CommandLayer_OS.cpp$(PreprocessSuffix): ../../../BEFE-Core/command/CommandLayer_OS.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/command_CommandLayer_OS.cpp$(PreprocessSuffix) "../../../BEFE-Core/command/CommandLayer_OS.cpp"

$(IntermediateDirectory)/command_CommandShell.cpp$(ObjectSuffix): ../../../BEFE-Core/command/CommandShell.cpp $(IntermediateDirectory)/command_CommandShell.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/command/CommandShell.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/command_CommandShell.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/command_CommandShell.cpp$(DependSuffix): ../../../BEFE-Core/command/CommandShell.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/command_CommandShell.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/command_CommandShell.cpp$(DependSuffix) -MM "../../../BEFE-Core/command/CommandShell.cpp"

$(IntermediateDirectory)/command_CommandShell.cpp$(PreprocessSuffix): ../../../BEFE-Core/command/CommandShell.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/command_CommandShell.cpp$(PreprocessSuffix) "../../../BEFE-Core/command/CommandShell.cpp"

$(IntermediateDirectory)/command_Help.cpp$(ObjectSuffix): ../../../BEFE-Core/command/Help.cpp $(IntermediateDirectory)/command_Help.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/command/Help.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/command_Help.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/command_Help.cpp$(DependSuffix): ../../../BEFE-Core/command/Help.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/command_Help.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/command_Help.cpp$(DependSuffix) -MM "../../../BEFE-Core/command/Help.cpp"

$(IntermediateDirectory)/command_Help.cpp$(PreprocessSuffix): ../../../BEFE-Core/command/Help.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/command_Help.cpp$(PreprocessSuffix) "../../../BEFE-Core/command/Help.cpp"

$(IntermediateDirectory)/core__SanityCheck.cpp$(ObjectSuffix): ../../../BEFE-Core/core/_SanityCheck.cpp $(IntermediateDirectory)/core__SanityCheck.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/_SanityCheck.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core__SanityCheck.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core__SanityCheck.cpp$(DependSuffix): ../../../BEFE-Core/core/_SanityCheck.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core__SanityCheck.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core__SanityCheck.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/_SanityCheck.cpp"

$(IntermediateDirectory)/core__SanityCheck.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/_SanityCheck.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core__SanityCheck.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/_SanityCheck.cpp"

$(IntermediateDirectory)/core_BEFE.cpp$(ObjectSuffix): ../../../BEFE-Core/core/BEFE.cpp $(IntermediateDirectory)/core_BEFE.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/BEFE.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_BEFE.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_BEFE.cpp$(DependSuffix): ../../../BEFE-Core/core/BEFE.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_BEFE.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_BEFE.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/BEFE.cpp"

$(IntermediateDirectory)/core_BEFE.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/BEFE.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_BEFE.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/BEFE.cpp"

$(IntermediateDirectory)/core_BEFECallbacks.cpp$(ObjectSuffix): ../../../BEFE-Core/core/BEFECallbacks.cpp $(IntermediateDirectory)/core_BEFECallbacks.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/BEFECallbacks.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_BEFECallbacks.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_BEFECallbacks.cpp$(DependSuffix): ../../../BEFE-Core/core/BEFECallbacks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_BEFECallbacks.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_BEFECallbacks.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/BEFECallbacks.cpp"

$(IntermediateDirectory)/core_BEFECallbacks.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/BEFECallbacks.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_BEFECallbacks.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/BEFECallbacks.cpp"

$(IntermediateDirectory)/core_BEFEEnviron.cpp$(ObjectSuffix): ../../../BEFE-Core/core/BEFEEnviron.cpp $(IntermediateDirectory)/core_BEFEEnviron.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/BEFEEnviron.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_BEFEEnviron.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_BEFEEnviron.cpp$(DependSuffix): ../../../BEFE-Core/core/BEFEEnviron.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_BEFEEnviron.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_BEFEEnviron.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/BEFEEnviron.cpp"

$(IntermediateDirectory)/core_BEFEEnviron.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/BEFEEnviron.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_BEFEEnviron.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/BEFEEnviron.cpp"

$(IntermediateDirectory)/core_BEFEWrappers.cpp$(ObjectSuffix): ../../../BEFE-Core/core/BEFEWrappers.cpp $(IntermediateDirectory)/core_BEFEWrappers.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/BEFEWrappers.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_BEFEWrappers.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_BEFEWrappers.cpp$(DependSuffix): ../../../BEFE-Core/core/BEFEWrappers.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_BEFEWrappers.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_BEFEWrappers.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/BEFEWrappers.cpp"

$(IntermediateDirectory)/core_BEFEWrappers.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/BEFEWrappers.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_BEFEWrappers.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/BEFEWrappers.cpp"

$(IntermediateDirectory)/core_Buffer.cpp$(ObjectSuffix): ../../../BEFE-Core/core/Buffer.cpp $(IntermediateDirectory)/core_Buffer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/Buffer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_Buffer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_Buffer.cpp$(DependSuffix): ../../../BEFE-Core/core/Buffer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_Buffer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_Buffer.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/Buffer.cpp"

$(IntermediateDirectory)/core_Buffer.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/Buffer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_Buffer.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/Buffer.cpp"

$(IntermediateDirectory)/core_BufferLow.cpp$(ObjectSuffix): ../../../BEFE-Core/core/BufferLow.cpp $(IntermediateDirectory)/core_BufferLow.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/BufferLow.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_BufferLow.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_BufferLow.cpp$(DependSuffix): ../../../BEFE-Core/core/BufferLow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_BufferLow.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_BufferLow.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/BufferLow.cpp"

$(IntermediateDirectory)/core_BufferLow.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/BufferLow.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_BufferLow.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/BufferLow.cpp"

$(IntermediateDirectory)/core_Error.cpp$(ObjectSuffix): ../../../BEFE-Core/core/Error.cpp $(IntermediateDirectory)/core_Error.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/Error.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_Error.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_Error.cpp$(DependSuffix): ../../../BEFE-Core/core/Error.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_Error.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_Error.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/Error.cpp"

$(IntermediateDirectory)/core_Error.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/Error.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_Error.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/Error.cpp"

$(IntermediateDirectory)/core_Globals.cpp$(ObjectSuffix): ../../../BEFE-Core/core/Globals.cpp $(IntermediateDirectory)/core_Globals.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/Globals.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_Globals.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_Globals.cpp$(DependSuffix): ../../../BEFE-Core/core/Globals.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_Globals.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_Globals.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/Globals.cpp"

$(IntermediateDirectory)/core_Globals.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/Globals.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_Globals.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/Globals.cpp"

$(IntermediateDirectory)/core_Network.cpp$(ObjectSuffix): ../../../BEFE-Core/core/Network.cpp $(IntermediateDirectory)/core_Network.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/Network.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_Network.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_Network.cpp$(DependSuffix): ../../../BEFE-Core/core/Network.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_Network.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_Network.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/Network.cpp"

$(IntermediateDirectory)/core_Network.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/Network.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_Network.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/Network.cpp"

$(IntermediateDirectory)/core_OperatingSystem.cpp$(ObjectSuffix): ../../../BEFE-Core/core/OperatingSystem.cpp $(IntermediateDirectory)/core_OperatingSystem.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/OperatingSystem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_OperatingSystem.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_OperatingSystem.cpp$(DependSuffix): ../../../BEFE-Core/core/OperatingSystem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_OperatingSystem.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_OperatingSystem.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/OperatingSystem.cpp"

$(IntermediateDirectory)/core_OperatingSystem.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/OperatingSystem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_OperatingSystem.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/OperatingSystem.cpp"

$(IntermediateDirectory)/core_Platform.cpp$(ObjectSuffix): ../../../BEFE-Core/core/Platform.cpp $(IntermediateDirectory)/core_Platform.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/Platform.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_Platform.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_Platform.cpp$(DependSuffix): ../../../BEFE-Core/core/Platform.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_Platform.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_Platform.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/Platform.cpp"

$(IntermediateDirectory)/core_Platform.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/Platform.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_Platform.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/Platform.cpp"

$(IntermediateDirectory)/core_Process.cpp$(ObjectSuffix): ../../../BEFE-Core/core/Process.cpp $(IntermediateDirectory)/core_Process.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/Process.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_Process.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_Process.cpp$(DependSuffix): ../../../BEFE-Core/core/Process.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_Process.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_Process.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/Process.cpp"

$(IntermediateDirectory)/core_Process.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/Process.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_Process.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/Process.cpp"

$(IntermediateDirectory)/core_Processor.cpp$(ObjectSuffix): ../../../BEFE-Core/core/Processor.cpp $(IntermediateDirectory)/core_Processor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/Processor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_Processor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_Processor.cpp$(DependSuffix): ../../../BEFE-Core/core/Processor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_Processor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_Processor.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/Processor.cpp"

$(IntermediateDirectory)/core_Processor.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/Processor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_Processor.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/Processor.cpp"

$(IntermediateDirectory)/core_SBuffer.cpp$(ObjectSuffix): ../../../BEFE-Core/core/SBuffer.cpp $(IntermediateDirectory)/core_SBuffer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/SBuffer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_SBuffer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_SBuffer.cpp$(DependSuffix): ../../../BEFE-Core/core/SBuffer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_SBuffer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_SBuffer.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/SBuffer.cpp"

$(IntermediateDirectory)/core_SBuffer.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/SBuffer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_SBuffer.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/SBuffer.cpp"

$(IntermediateDirectory)/core_String.cpp$(ObjectSuffix): ../../../BEFE-Core/core/String.cpp $(IntermediateDirectory)/core_String.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/String.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_String.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_String.cpp$(DependSuffix): ../../../BEFE-Core/core/String.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_String.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_String.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/String.cpp"

$(IntermediateDirectory)/core_String.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/String.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_String.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/String.cpp"

$(IntermediateDirectory)/core_StringAppend.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringAppend.cpp $(IntermediateDirectory)/core_StringAppend.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringAppend.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringAppend.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringAppend.cpp$(DependSuffix): ../../../BEFE-Core/core/StringAppend.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringAppend.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringAppend.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringAppend.cpp"

$(IntermediateDirectory)/core_StringAppend.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringAppend.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringAppend.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringAppend.cpp"

$(IntermediateDirectory)/core_StringBuild.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringBuild.cpp $(IntermediateDirectory)/core_StringBuild.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringBuild.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringBuild.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringBuild.cpp$(DependSuffix): ../../../BEFE-Core/core/StringBuild.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringBuild.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringBuild.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringBuild.cpp"

$(IntermediateDirectory)/core_StringBuild.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringBuild.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringBuild.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringBuild.cpp"

$(IntermediateDirectory)/core_StringCompare.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringCompare.cpp $(IntermediateDirectory)/core_StringCompare.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringCompare.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringCompare.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringCompare.cpp$(DependSuffix): ../../../BEFE-Core/core/StringCompare.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringCompare.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringCompare.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringCompare.cpp"

$(IntermediateDirectory)/core_StringCompare.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringCompare.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringCompare.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringCompare.cpp"

$(IntermediateDirectory)/core_StringConstruct.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringConstruct.cpp $(IntermediateDirectory)/core_StringConstruct.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringConstruct.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringConstruct.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringConstruct.cpp$(DependSuffix): ../../../BEFE-Core/core/StringConstruct.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringConstruct.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringConstruct.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringConstruct.cpp"

$(IntermediateDirectory)/core_StringConstruct.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringConstruct.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringConstruct.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringConstruct.cpp"

$(IntermediateDirectory)/core_StringFind.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringFind.cpp $(IntermediateDirectory)/core_StringFind.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringFind.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringFind.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringFind.cpp$(DependSuffix): ../../../BEFE-Core/core/StringFind.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringFind.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringFind.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringFind.cpp"

$(IntermediateDirectory)/core_StringFind.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringFind.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringFind.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringFind.cpp"

$(IntermediateDirectory)/core_StringFolding.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringFolding.cpp $(IntermediateDirectory)/core_StringFolding.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringFolding.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringFolding.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringFolding.cpp$(DependSuffix): ../../../BEFE-Core/core/StringFolding.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringFolding.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringFolding.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringFolding.cpp"

$(IntermediateDirectory)/core_StringFolding.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringFolding.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringFolding.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringFolding.cpp"

$(IntermediateDirectory)/core_StringFormat.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringFormat.cpp $(IntermediateDirectory)/core_StringFormat.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringFormat.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringFormat.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringFormat.cpp$(DependSuffix): ../../../BEFE-Core/core/StringFormat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringFormat.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringFormat.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringFormat.cpp"

$(IntermediateDirectory)/core_StringFormat.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringFormat.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringFormat.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringFormat.cpp"

$(IntermediateDirectory)/core_StringGeneral.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringGeneral.cpp $(IntermediateDirectory)/core_StringGeneral.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringGeneral.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringGeneral.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringGeneral.cpp$(DependSuffix): ../../../BEFE-Core/core/StringGeneral.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringGeneral.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringGeneral.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringGeneral.cpp"

$(IntermediateDirectory)/core_StringGeneral.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringGeneral.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringGeneral.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringGeneral.cpp"

$(IntermediateDirectory)/core_StringGet.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringGet.cpp $(IntermediateDirectory)/core_StringGet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringGet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringGet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringGet.cpp$(DependSuffix): ../../../BEFE-Core/core/StringGet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringGet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringGet.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringGet.cpp"

$(IntermediateDirectory)/core_StringGet.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringGet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringGet.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringGet.cpp"

$(IntermediateDirectory)/core_StringInsert.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringInsert.cpp $(IntermediateDirectory)/core_StringInsert.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringInsert.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringInsert.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringInsert.cpp$(DependSuffix): ../../../BEFE-Core/core/StringInsert.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringInsert.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringInsert.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringInsert.cpp"

$(IntermediateDirectory)/core_StringInsert.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringInsert.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringInsert.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringInsert.cpp"

$(IntermediateDirectory)/core_StringJoin.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringJoin.cpp $(IntermediateDirectory)/core_StringJoin.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringJoin.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringJoin.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringJoin.cpp$(DependSuffix): ../../../BEFE-Core/core/StringJoin.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringJoin.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringJoin.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringJoin.cpp"

$(IntermediateDirectory)/core_StringJoin.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringJoin.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringJoin.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringJoin.cpp"

$(IntermediateDirectory)/core_StringManip.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringManip.cpp $(IntermediateDirectory)/core_StringManip.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringManip.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringManip.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringManip.cpp$(DependSuffix): ../../../BEFE-Core/core/StringManip.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringManip.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringManip.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringManip.cpp"

$(IntermediateDirectory)/core_StringManip.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringManip.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringManip.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringManip.cpp"

$(IntermediateDirectory)/core_StringOperator.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringOperator.cpp $(IntermediateDirectory)/core_StringOperator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringOperator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringOperator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringOperator.cpp$(DependSuffix): ../../../BEFE-Core/core/StringOperator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringOperator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringOperator.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringOperator.cpp"

$(IntermediateDirectory)/core_StringOperator.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringOperator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringOperator.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringOperator.cpp"

$(IntermediateDirectory)/core_StringPersist.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringPersist.cpp $(IntermediateDirectory)/core_StringPersist.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringPersist.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringPersist.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringPersist.cpp$(DependSuffix): ../../../BEFE-Core/core/StringPersist.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringPersist.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringPersist.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringPersist.cpp"

$(IntermediateDirectory)/core_StringPersist.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringPersist.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringPersist.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringPersist.cpp"

$(IntermediateDirectory)/core_StringQuery.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringQuery.cpp $(IntermediateDirectory)/core_StringQuery.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringQuery.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringQuery.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringQuery.cpp$(DependSuffix): ../../../BEFE-Core/core/StringQuery.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringQuery.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringQuery.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringQuery.cpp"

$(IntermediateDirectory)/core_StringQuery.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringQuery.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringQuery.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringQuery.cpp"

$(IntermediateDirectory)/core_StringRemove.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringRemove.cpp $(IntermediateDirectory)/core_StringRemove.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringRemove.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringRemove.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringRemove.cpp$(DependSuffix): ../../../BEFE-Core/core/StringRemove.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringRemove.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringRemove.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringRemove.cpp"

$(IntermediateDirectory)/core_StringRemove.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringRemove.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringRemove.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringRemove.cpp"

$(IntermediateDirectory)/core_StringReplace.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringReplace.cpp $(IntermediateDirectory)/core_StringReplace.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringReplace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringReplace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringReplace.cpp$(DependSuffix): ../../../BEFE-Core/core/StringReplace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringReplace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringReplace.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringReplace.cpp"

$(IntermediateDirectory)/core_StringReplace.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringReplace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringReplace.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringReplace.cpp"

$(IntermediateDirectory)/core_StringReverse.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringReverse.cpp $(IntermediateDirectory)/core_StringReverse.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringReverse.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringReverse.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringReverse.cpp$(DependSuffix): ../../../BEFE-Core/core/StringReverse.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringReverse.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringReverse.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringReverse.cpp"

$(IntermediateDirectory)/core_StringReverse.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringReverse.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringReverse.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringReverse.cpp"

$(IntermediateDirectory)/core_StringSplit.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringSplit.cpp $(IntermediateDirectory)/core_StringSplit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringSplit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringSplit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringSplit.cpp$(DependSuffix): ../../../BEFE-Core/core/StringSplit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringSplit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringSplit.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringSplit.cpp"

$(IntermediateDirectory)/core_StringSplit.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringSplit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringSplit.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringSplit.cpp"

$(IntermediateDirectory)/core_StringSubscript.cpp$(ObjectSuffix): ../../../BEFE-Core/core/StringSubscript.cpp $(IntermediateDirectory)/core_StringSubscript.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/StringSubscript.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_StringSubscript.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_StringSubscript.cpp$(DependSuffix): ../../../BEFE-Core/core/StringSubscript.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_StringSubscript.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_StringSubscript.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/StringSubscript.cpp"

$(IntermediateDirectory)/core_StringSubscript.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/StringSubscript.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_StringSubscript.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/StringSubscript.cpp"

$(IntermediateDirectory)/core_Time.cpp$(ObjectSuffix): ../../../BEFE-Core/core/Time.cpp $(IntermediateDirectory)/core_Time.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/Time.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_Time.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_Time.cpp$(DependSuffix): ../../../BEFE-Core/core/Time.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_Time.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_Time.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/Time.cpp"

$(IntermediateDirectory)/core_Time.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/Time.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_Time.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/Time.cpp"

$(IntermediateDirectory)/core_UTF8.cpp$(ObjectSuffix): ../../../BEFE-Core/core/UTF8.cpp $(IntermediateDirectory)/core_UTF8.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/UTF8.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_UTF8.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_UTF8.cpp$(DependSuffix): ../../../BEFE-Core/core/UTF8.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_UTF8.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_UTF8.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/UTF8.cpp"

$(IntermediateDirectory)/core_UTF8.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/UTF8.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_UTF8.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/UTF8.cpp"

$(IntermediateDirectory)/core_UTF8Fold.cpp$(ObjectSuffix): ../../../BEFE-Core/core/UTF8Fold.cpp $(IntermediateDirectory)/core_UTF8Fold.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/UTF8Fold.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_UTF8Fold.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_UTF8Fold.cpp$(DependSuffix): ../../../BEFE-Core/core/UTF8Fold.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_UTF8Fold.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_UTF8Fold.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/UTF8Fold.cpp"

$(IntermediateDirectory)/core_UTF8Fold.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/UTF8Fold.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_UTF8Fold.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/UTF8Fold.cpp"

$(IntermediateDirectory)/core_UUID.cpp$(ObjectSuffix): ../../../BEFE-Core/core/UUID.cpp $(IntermediateDirectory)/core_UUID.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/UUID.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_UUID.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_UUID.cpp$(DependSuffix): ../../../BEFE-Core/core/UUID.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_UUID.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_UUID.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/UUID.cpp"

$(IntermediateDirectory)/core_UUID.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/UUID.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_UUID.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/UUID.cpp"

$(IntermediateDirectory)/core_VarTerm.cpp$(ObjectSuffix): ../../../BEFE-Core/core/VarTerm.cpp $(IntermediateDirectory)/core_VarTerm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/core/VarTerm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/core_VarTerm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/core_VarTerm.cpp$(DependSuffix): ../../../BEFE-Core/core/VarTerm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/core_VarTerm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/core_VarTerm.cpp$(DependSuffix) -MM "../../../BEFE-Core/core/VarTerm.cpp"

$(IntermediateDirectory)/core_VarTerm.cpp$(PreprocessSuffix): ../../../BEFE-Core/core/VarTerm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/core_VarTerm.cpp$(PreprocessSuffix) "../../../BEFE-Core/core/VarTerm.cpp"

$(IntermediateDirectory)/crypt_Crypt.cpp$(ObjectSuffix): ../../../BEFE-Core/crypt/Crypt.cpp $(IntermediateDirectory)/crypt_Crypt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/crypt/Crypt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/crypt_Crypt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/crypt_Crypt.cpp$(DependSuffix): ../../../BEFE-Core/crypt/Crypt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/crypt_Crypt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/crypt_Crypt.cpp$(DependSuffix) -MM "../../../BEFE-Core/crypt/Crypt.cpp"

$(IntermediateDirectory)/crypt_Crypt.cpp$(PreprocessSuffix): ../../../BEFE-Core/crypt/Crypt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/crypt_Crypt.cpp$(PreprocessSuffix) "../../../BEFE-Core/crypt/Crypt.cpp"

$(IntermediateDirectory)/crypt_Crypt0.cpp$(ObjectSuffix): ../../../BEFE-Core/crypt/Crypt0.cpp $(IntermediateDirectory)/crypt_Crypt0.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/crypt/Crypt0.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/crypt_Crypt0.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/crypt_Crypt0.cpp$(DependSuffix): ../../../BEFE-Core/crypt/Crypt0.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/crypt_Crypt0.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/crypt_Crypt0.cpp$(DependSuffix) -MM "../../../BEFE-Core/crypt/Crypt0.cpp"

$(IntermediateDirectory)/crypt_Crypt0.cpp$(PreprocessSuffix): ../../../BEFE-Core/crypt/Crypt0.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/crypt_Crypt0.cpp$(PreprocessSuffix) "../../../BEFE-Core/crypt/Crypt0.cpp"

$(IntermediateDirectory)/crypt_CryptFileHead.cpp$(ObjectSuffix): ../../../BEFE-Core/crypt/CryptFileHead.cpp $(IntermediateDirectory)/crypt_CryptFileHead.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/crypt/CryptFileHead.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/crypt_CryptFileHead.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/crypt_CryptFileHead.cpp$(DependSuffix): ../../../BEFE-Core/crypt/CryptFileHead.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/crypt_CryptFileHead.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/crypt_CryptFileHead.cpp$(DependSuffix) -MM "../../../BEFE-Core/crypt/CryptFileHead.cpp"

$(IntermediateDirectory)/crypt_CryptFileHead.cpp$(PreprocessSuffix): ../../../BEFE-Core/crypt/CryptFileHead.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/crypt_CryptFileHead.cpp$(PreprocessSuffix) "../../../BEFE-Core/crypt/CryptFileHead.cpp"

$(IntermediateDirectory)/crypt_CryptKey.cpp$(ObjectSuffix): ../../../BEFE-Core/crypt/CryptKey.cpp $(IntermediateDirectory)/crypt_CryptKey.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/crypt/CryptKey.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/crypt_CryptKey.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/crypt_CryptKey.cpp$(DependSuffix): ../../../BEFE-Core/crypt/CryptKey.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/crypt_CryptKey.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/crypt_CryptKey.cpp$(DependSuffix) -MM "../../../BEFE-Core/crypt/CryptKey.cpp"

$(IntermediateDirectory)/crypt_CryptKey.cpp$(PreprocessSuffix): ../../../BEFE-Core/crypt/CryptKey.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/crypt_CryptKey.cpp$(PreprocessSuffix) "../../../BEFE-Core/crypt/CryptKey.cpp"

$(IntermediateDirectory)/datamodel_Class.cpp$(ObjectSuffix): ../../../BEFE-Core/datamodel/Class.cpp $(IntermediateDirectory)/datamodel_Class.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/datamodel/Class.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/datamodel_Class.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/datamodel_Class.cpp$(DependSuffix): ../../../BEFE-Core/datamodel/Class.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/datamodel_Class.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/datamodel_Class.cpp$(DependSuffix) -MM "../../../BEFE-Core/datamodel/Class.cpp"

$(IntermediateDirectory)/datamodel_Class.cpp$(PreprocessSuffix): ../../../BEFE-Core/datamodel/Class.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/datamodel_Class.cpp$(PreprocessSuffix) "../../../BEFE-Core/datamodel/Class.cpp"

$(IntermediateDirectory)/datamodel_Ids.cpp$(ObjectSuffix): ../../../BEFE-Core/datamodel/Ids.cpp $(IntermediateDirectory)/datamodel_Ids.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/datamodel/Ids.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/datamodel_Ids.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/datamodel_Ids.cpp$(DependSuffix): ../../../BEFE-Core/datamodel/Ids.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/datamodel_Ids.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/datamodel_Ids.cpp$(DependSuffix) -MM "../../../BEFE-Core/datamodel/Ids.cpp"

$(IntermediateDirectory)/datamodel_Ids.cpp$(PreprocessSuffix): ../../../BEFE-Core/datamodel/Ids.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/datamodel_Ids.cpp$(PreprocessSuffix) "../../../BEFE-Core/datamodel/Ids.cpp"

$(IntermediateDirectory)/datamodel_IdSpace.cpp$(ObjectSuffix): ../../../BEFE-Core/datamodel/IdSpace.cpp $(IntermediateDirectory)/datamodel_IdSpace.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/datamodel/IdSpace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/datamodel_IdSpace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/datamodel_IdSpace.cpp$(DependSuffix): ../../../BEFE-Core/datamodel/IdSpace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/datamodel_IdSpace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/datamodel_IdSpace.cpp$(DependSuffix) -MM "../../../BEFE-Core/datamodel/IdSpace.cpp"

$(IntermediateDirectory)/datamodel_IdSpace.cpp$(PreprocessSuffix): ../../../BEFE-Core/datamodel/IdSpace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/datamodel_IdSpace.cpp$(PreprocessSuffix) "../../../BEFE-Core/datamodel/IdSpace.cpp"

$(IntermediateDirectory)/datamodel_IdSpaceInfoExt.cpp$(ObjectSuffix): ../../../BEFE-Core/datamodel/IdSpaceInfoExt.cpp $(IntermediateDirectory)/datamodel_IdSpaceInfoExt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/datamodel/IdSpaceInfoExt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/datamodel_IdSpaceInfoExt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/datamodel_IdSpaceInfoExt.cpp$(DependSuffix): ../../../BEFE-Core/datamodel/IdSpaceInfoExt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/datamodel_IdSpaceInfoExt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/datamodel_IdSpaceInfoExt.cpp$(DependSuffix) -MM "../../../BEFE-Core/datamodel/IdSpaceInfoExt.cpp"

$(IntermediateDirectory)/datamodel_IdSpaceInfoExt.cpp$(PreprocessSuffix): ../../../BEFE-Core/datamodel/IdSpaceInfoExt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/datamodel_IdSpaceInfoExt.cpp$(PreprocessSuffix) "../../../BEFE-Core/datamodel/IdSpaceInfoExt.cpp"

$(IntermediateDirectory)/datamodel_StringSpace.cpp$(ObjectSuffix): ../../../BEFE-Core/datamodel/StringSpace.cpp $(IntermediateDirectory)/datamodel_StringSpace.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/datamodel/StringSpace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/datamodel_StringSpace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/datamodel_StringSpace.cpp$(DependSuffix): ../../../BEFE-Core/datamodel/StringSpace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/datamodel_StringSpace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/datamodel_StringSpace.cpp$(DependSuffix) -MM "../../../BEFE-Core/datamodel/StringSpace.cpp"

$(IntermediateDirectory)/datamodel_StringSpace.cpp$(PreprocessSuffix): ../../../BEFE-Core/datamodel/StringSpace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/datamodel_StringSpace.cpp$(PreprocessSuffix) "../../../BEFE-Core/datamodel/StringSpace.cpp"

$(IntermediateDirectory)/devices_Console.cpp$(ObjectSuffix): ../../../BEFE-Core/devices/Console.cpp $(IntermediateDirectory)/devices_Console.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/devices/Console.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/devices_Console.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/devices_Console.cpp$(DependSuffix): ../../../BEFE-Core/devices/Console.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/devices_Console.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/devices_Console.cpp$(DependSuffix) -MM "../../../BEFE-Core/devices/Console.cpp"

$(IntermediateDirectory)/devices_Console.cpp$(PreprocessSuffix): ../../../BEFE-Core/devices/Console.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/devices_Console.cpp$(PreprocessSuffix) "../../../BEFE-Core/devices/Console.cpp"

$(IntermediateDirectory)/format_FormatOut.cpp$(ObjectSuffix): ../../../BEFE-Core/format/FormatOut.cpp $(IntermediateDirectory)/format_FormatOut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/format/FormatOut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/format_FormatOut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/format_FormatOut.cpp$(DependSuffix): ../../../BEFE-Core/format/FormatOut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/format_FormatOut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/format_FormatOut.cpp$(DependSuffix) -MM "../../../BEFE-Core/format/FormatOut.cpp"

$(IntermediateDirectory)/format_FormatOut.cpp$(PreprocessSuffix): ../../../BEFE-Core/format/FormatOut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/format_FormatOut.cpp$(PreprocessSuffix) "../../../BEFE-Core/format/FormatOut.cpp"

$(IntermediateDirectory)/format_LexFormatOut.cpp$(ObjectSuffix): ../../../BEFE-Core/format/LexFormatOut.cpp $(IntermediateDirectory)/format_LexFormatOut.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/format/LexFormatOut.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/format_LexFormatOut.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/format_LexFormatOut.cpp$(DependSuffix): ../../../BEFE-Core/format/LexFormatOut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/format_LexFormatOut.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/format_LexFormatOut.cpp$(DependSuffix) -MM "../../../BEFE-Core/format/LexFormatOut.cpp"

$(IntermediateDirectory)/format_LexFormatOut.cpp$(PreprocessSuffix): ../../../BEFE-Core/format/LexFormatOut.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/format_LexFormatOut.cpp$(PreprocessSuffix) "../../../BEFE-Core/format/LexFormatOut.cpp"

$(IntermediateDirectory)/format_ToAscii.cpp$(ObjectSuffix): ../../../BEFE-Core/format/ToAscii.cpp $(IntermediateDirectory)/format_ToAscii.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/format/ToAscii.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/format_ToAscii.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/format_ToAscii.cpp$(DependSuffix): ../../../BEFE-Core/format/ToAscii.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/format_ToAscii.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/format_ToAscii.cpp$(DependSuffix) -MM "../../../BEFE-Core/format/ToAscii.cpp"

$(IntermediateDirectory)/format_ToAscii.cpp$(PreprocessSuffix): ../../../BEFE-Core/format/ToAscii.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/format_ToAscii.cpp$(PreprocessSuffix) "../../../BEFE-Core/format/ToAscii.cpp"

$(IntermediateDirectory)/format_ToString.cpp$(ObjectSuffix): ../../../BEFE-Core/format/ToString.cpp $(IntermediateDirectory)/format_ToString.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/format/ToString.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/format_ToString.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/format_ToString.cpp$(DependSuffix): ../../../BEFE-Core/format/ToString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/format_ToString.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/format_ToString.cpp$(DependSuffix) -MM "../../../BEFE-Core/format/ToString.cpp"

$(IntermediateDirectory)/format_ToString.cpp$(PreprocessSuffix): ../../../BEFE-Core/format/ToString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/format_ToString.cpp$(PreprocessSuffix) "../../../BEFE-Core/format/ToString.cpp"

$(IntermediateDirectory)/format_ToStrings.cpp$(ObjectSuffix): ../../../BEFE-Core/format/ToStrings.cpp $(IntermediateDirectory)/format_ToStrings.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/format/ToStrings.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/format_ToStrings.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/format_ToStrings.cpp$(DependSuffix): ../../../BEFE-Core/format/ToStrings.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/format_ToStrings.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/format_ToStrings.cpp$(DependSuffix) -MM "../../../BEFE-Core/format/ToStrings.cpp"

$(IntermediateDirectory)/format_ToStrings.cpp$(PreprocessSuffix): ../../../BEFE-Core/format/ToStrings.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/format_ToStrings.cpp$(PreprocessSuffix) "../../../BEFE-Core/format/ToStrings.cpp"

$(IntermediateDirectory)/lang_Bcmd.cpp$(ObjectSuffix): ../../../BEFE-Core/lang/Bcmd.cpp $(IntermediateDirectory)/lang_Bcmd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lang/Bcmd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lang_Bcmd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lang_Bcmd.cpp$(DependSuffix): ../../../BEFE-Core/lang/Bcmd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lang_Bcmd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lang_Bcmd.cpp$(DependSuffix) -MM "../../../BEFE-Core/lang/Bcmd.cpp"

$(IntermediateDirectory)/lang_Bcmd.cpp$(PreprocessSuffix): ../../../BEFE-Core/lang/Bcmd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lang_Bcmd.cpp$(PreprocessSuffix) "../../../BEFE-Core/lang/Bcmd.cpp"

$(IntermediateDirectory)/lang_BcmdArg.cpp$(ObjectSuffix): ../../../BEFE-Core/lang/BcmdArg.cpp $(IntermediateDirectory)/lang_BcmdArg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lang/BcmdArg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lang_BcmdArg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lang_BcmdArg.cpp$(DependSuffix): ../../../BEFE-Core/lang/BcmdArg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lang_BcmdArg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lang_BcmdArg.cpp$(DependSuffix) -MM "../../../BEFE-Core/lang/BcmdArg.cpp"

$(IntermediateDirectory)/lang_BcmdArg.cpp$(PreprocessSuffix): ../../../BEFE-Core/lang/BcmdArg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lang_BcmdArg.cpp$(PreprocessSuffix) "../../../BEFE-Core/lang/BcmdArg.cpp"

$(IntermediateDirectory)/lang_BcmdCommand.cpp$(ObjectSuffix): ../../../BEFE-Core/lang/BcmdCommand.cpp $(IntermediateDirectory)/lang_BcmdCommand.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lang/BcmdCommand.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lang_BcmdCommand.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lang_BcmdCommand.cpp$(DependSuffix): ../../../BEFE-Core/lang/BcmdCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lang_BcmdCommand.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lang_BcmdCommand.cpp$(DependSuffix) -MM "../../../BEFE-Core/lang/BcmdCommand.cpp"

$(IntermediateDirectory)/lang_BcmdCommand.cpp$(PreprocessSuffix): ../../../BEFE-Core/lang/BcmdCommand.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lang_BcmdCommand.cpp$(PreprocessSuffix) "../../../BEFE-Core/lang/BcmdCommand.cpp"

$(IntermediateDirectory)/lang_Blang.cpp$(ObjectSuffix): ../../../BEFE-Core/lang/Blang.cpp $(IntermediateDirectory)/lang_Blang.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lang/Blang.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lang_Blang.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lang_Blang.cpp$(DependSuffix): ../../../BEFE-Core/lang/Blang.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lang_Blang.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lang_Blang.cpp$(DependSuffix) -MM "../../../BEFE-Core/lang/Blang.cpp"

$(IntermediateDirectory)/lang_Blang.cpp$(PreprocessSuffix): ../../../BEFE-Core/lang/Blang.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lang_Blang.cpp$(PreprocessSuffix) "../../../BEFE-Core/lang/Blang.cpp"

$(IntermediateDirectory)/lang_Bmsg.cpp$(ObjectSuffix): ../../../BEFE-Core/lang/Bmsg.cpp $(IntermediateDirectory)/lang_Bmsg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lang/Bmsg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lang_Bmsg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lang_Bmsg.cpp$(DependSuffix): ../../../BEFE-Core/lang/Bmsg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lang_Bmsg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lang_Bmsg.cpp$(DependSuffix) -MM "../../../BEFE-Core/lang/Bmsg.cpp"

$(IntermediateDirectory)/lang_Bmsg.cpp$(PreprocessSuffix): ../../../BEFE-Core/lang/Bmsg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lang_Bmsg.cpp$(PreprocessSuffix) "../../../BEFE-Core/lang/Bmsg.cpp"

$(IntermediateDirectory)/lang_BmsgLine.cpp$(ObjectSuffix): ../../../BEFE-Core/lang/BmsgLine.cpp $(IntermediateDirectory)/lang_BmsgLine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lang/BmsgLine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lang_BmsgLine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lang_BmsgLine.cpp$(DependSuffix): ../../../BEFE-Core/lang/BmsgLine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lang_BmsgLine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lang_BmsgLine.cpp$(DependSuffix) -MM "../../../BEFE-Core/lang/BmsgLine.cpp"

$(IntermediateDirectory)/lang_BmsgLine.cpp$(PreprocessSuffix): ../../../BEFE-Core/lang/BmsgLine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lang_BmsgLine.cpp$(PreprocessSuffix) "../../../BEFE-Core/lang/BmsgLine.cpp"

$(IntermediateDirectory)/lang_BmsgToken.cpp$(ObjectSuffix): ../../../BEFE-Core/lang/BmsgToken.cpp $(IntermediateDirectory)/lang_BmsgToken.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lang/BmsgToken.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lang_BmsgToken.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lang_BmsgToken.cpp$(DependSuffix): ../../../BEFE-Core/lang/BmsgToken.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lang_BmsgToken.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lang_BmsgToken.cpp$(DependSuffix) -MM "../../../BEFE-Core/lang/BmsgToken.cpp"

$(IntermediateDirectory)/lang_BmsgToken.cpp$(PreprocessSuffix): ../../../BEFE-Core/lang/BmsgToken.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lang_BmsgToken.cpp$(PreprocessSuffix) "../../../BEFE-Core/lang/BmsgToken.cpp"

$(IntermediateDirectory)/lex_Lex.cpp$(ObjectSuffix): ../../../BEFE-Core/lex/Lex.cpp $(IntermediateDirectory)/lex_Lex.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lex/Lex.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lex_Lex.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lex_Lex.cpp$(DependSuffix): ../../../BEFE-Core/lex/Lex.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lex_Lex.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lex_Lex.cpp$(DependSuffix) -MM "../../../BEFE-Core/lex/Lex.cpp"

$(IntermediateDirectory)/lex_Lex.cpp$(PreprocessSuffix): ../../../BEFE-Core/lex/Lex.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lex_Lex.cpp$(PreprocessSuffix) "../../../BEFE-Core/lex/Lex.cpp"

$(IntermediateDirectory)/lex_LexAction.cpp$(ObjectSuffix): ../../../BEFE-Core/lex/LexAction.cpp $(IntermediateDirectory)/lex_LexAction.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lex/LexAction.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lex_LexAction.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lex_LexAction.cpp$(DependSuffix): ../../../BEFE-Core/lex/LexAction.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lex_LexAction.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lex_LexAction.cpp$(DependSuffix) -MM "../../../BEFE-Core/lex/LexAction.cpp"

$(IntermediateDirectory)/lex_LexAction.cpp$(PreprocessSuffix): ../../../BEFE-Core/lex/LexAction.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lex_LexAction.cpp$(PreprocessSuffix) "../../../BEFE-Core/lex/LexAction.cpp"

$(IntermediateDirectory)/lex_LexFuncs.cpp$(ObjectSuffix): ../../../BEFE-Core/lex/LexFuncs.cpp $(IntermediateDirectory)/lex_LexFuncs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lex/LexFuncs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lex_LexFuncs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lex_LexFuncs.cpp$(DependSuffix): ../../../BEFE-Core/lex/LexFuncs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lex_LexFuncs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lex_LexFuncs.cpp$(DependSuffix) -MM "../../../BEFE-Core/lex/LexFuncs.cpp"

$(IntermediateDirectory)/lex_LexFuncs.cpp$(PreprocessSuffix): ../../../BEFE-Core/lex/LexFuncs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lex_LexFuncs.cpp$(PreprocessSuffix) "../../../BEFE-Core/lex/LexFuncs.cpp"

$(IntermediateDirectory)/lex_LexLangActions.cpp$(ObjectSuffix): ../../../BEFE-Core/lex/LexLangActions.cpp $(IntermediateDirectory)/lex_LexLangActions.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lex/LexLangActions.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lex_LexLangActions.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lex_LexLangActions.cpp$(DependSuffix): ../../../BEFE-Core/lex/LexLangActions.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lex_LexLangActions.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lex_LexLangActions.cpp$(DependSuffix) -MM "../../../BEFE-Core/lex/LexLangActions.cpp"

$(IntermediateDirectory)/lex_LexLangActions.cpp$(PreprocessSuffix): ../../../BEFE-Core/lex/LexLangActions.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lex_LexLangActions.cpp$(PreprocessSuffix) "../../../BEFE-Core/lex/LexLangActions.cpp"

$(IntermediateDirectory)/lex_LexLangBefe.cpp$(ObjectSuffix): ../../../BEFE-Core/lex/LexLangBefe.cpp $(IntermediateDirectory)/lex_LexLangBefe.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lex/LexLangBefe.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lex_LexLangBefe.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lex_LexLangBefe.cpp$(DependSuffix): ../../../BEFE-Core/lex/LexLangBefe.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lex_LexLangBefe.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lex_LexLangBefe.cpp$(DependSuffix) -MM "../../../BEFE-Core/lex/LexLangBefe.cpp"

$(IntermediateDirectory)/lex_LexLangBefe.cpp$(PreprocessSuffix): ../../../BEFE-Core/lex/LexLangBefe.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lex_LexLangBefe.cpp$(PreprocessSuffix) "../../../BEFE-Core/lex/LexLangBefe.cpp"

$(IntermediateDirectory)/lex_LexLangCpp.cpp$(ObjectSuffix): ../../../BEFE-Core/lex/LexLangCpp.cpp $(IntermediateDirectory)/lex_LexLangCpp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lex/LexLangCpp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lex_LexLangCpp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lex_LexLangCpp.cpp$(DependSuffix): ../../../BEFE-Core/lex/LexLangCpp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lex_LexLangCpp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lex_LexLangCpp.cpp$(DependSuffix) -MM "../../../BEFE-Core/lex/LexLangCpp.cpp"

$(IntermediateDirectory)/lex_LexLangCpp.cpp$(PreprocessSuffix): ../../../BEFE-Core/lex/LexLangCpp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lex_LexLangCpp.cpp$(PreprocessSuffix) "../../../BEFE-Core/lex/LexLangCpp.cpp"

$(IntermediateDirectory)/lex_LexLanguage.cpp$(ObjectSuffix): ../../../BEFE-Core/lex/LexLanguage.cpp $(IntermediateDirectory)/lex_LexLanguage.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lex/LexLanguage.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lex_LexLanguage.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lex_LexLanguage.cpp$(DependSuffix): ../../../BEFE-Core/lex/LexLanguage.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lex_LexLanguage.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lex_LexLanguage.cpp$(DependSuffix) -MM "../../../BEFE-Core/lex/LexLanguage.cpp"

$(IntermediateDirectory)/lex_LexLanguage.cpp$(PreprocessSuffix): ../../../BEFE-Core/lex/LexLanguage.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lex_LexLanguage.cpp$(PreprocessSuffix) "../../../BEFE-Core/lex/LexLanguage.cpp"

$(IntermediateDirectory)/lex_LexRange.cpp$(ObjectSuffix): ../../../BEFE-Core/lex/LexRange.cpp $(IntermediateDirectory)/lex_LexRange.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lex/LexRange.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lex_LexRange.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lex_LexRange.cpp$(DependSuffix): ../../../BEFE-Core/lex/LexRange.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lex_LexRange.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lex_LexRange.cpp$(DependSuffix) -MM "../../../BEFE-Core/lex/LexRange.cpp"

$(IntermediateDirectory)/lex_LexRange.cpp$(PreprocessSuffix): ../../../BEFE-Core/lex/LexRange.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lex_LexRange.cpp$(PreprocessSuffix) "../../../BEFE-Core/lex/LexRange.cpp"

$(IntermediateDirectory)/lex_LexState.cpp$(ObjectSuffix): ../../../BEFE-Core/lex/LexState.cpp $(IntermediateDirectory)/lex_LexState.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lex/LexState.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lex_LexState.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lex_LexState.cpp$(DependSuffix): ../../../BEFE-Core/lex/LexState.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lex_LexState.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lex_LexState.cpp$(DependSuffix) -MM "../../../BEFE-Core/lex/LexState.cpp"

$(IntermediateDirectory)/lex_LexState.cpp$(PreprocessSuffix): ../../../BEFE-Core/lex/LexState.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lex_LexState.cpp$(PreprocessSuffix) "../../../BEFE-Core/lex/LexState.cpp"

$(IntermediateDirectory)/lex_LexStream.cpp$(ObjectSuffix): ../../../BEFE-Core/lex/LexStream.cpp $(IntermediateDirectory)/lex_LexStream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lex/LexStream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lex_LexStream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lex_LexStream.cpp$(DependSuffix): ../../../BEFE-Core/lex/LexStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lex_LexStream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lex_LexStream.cpp$(DependSuffix) -MM "../../../BEFE-Core/lex/LexStream.cpp"

$(IntermediateDirectory)/lex_LexStream.cpp$(PreprocessSuffix): ../../../BEFE-Core/lex/LexStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lex_LexStream.cpp$(PreprocessSuffix) "../../../BEFE-Core/lex/LexStream.cpp"

$(IntermediateDirectory)/lex_LexToken.cpp$(ObjectSuffix): ../../../BEFE-Core/lex/LexToken.cpp $(IntermediateDirectory)/lex_LexToken.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lex/LexToken.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lex_LexToken.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lex_LexToken.cpp$(DependSuffix): ../../../BEFE-Core/lex/LexToken.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lex_LexToken.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lex_LexToken.cpp$(DependSuffix) -MM "../../../BEFE-Core/lex/LexToken.cpp"

$(IntermediateDirectory)/lex_LexToken.cpp$(PreprocessSuffix): ../../../BEFE-Core/lex/LexToken.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lex_LexToken.cpp$(PreprocessSuffix) "../../../BEFE-Core/lex/LexToken.cpp"

$(IntermediateDirectory)/lex_Match.cpp$(ObjectSuffix): ../../../BEFE-Core/lex/Match.cpp $(IntermediateDirectory)/lex_Match.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lex/Match.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lex_Match.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lex_Match.cpp$(DependSuffix): ../../../BEFE-Core/lex/Match.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lex_Match.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lex_Match.cpp$(DependSuffix) -MM "../../../BEFE-Core/lex/Match.cpp"

$(IntermediateDirectory)/lex_Match.cpp$(PreprocessSuffix): ../../../BEFE-Core/lex/Match.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lex_Match.cpp$(PreprocessSuffix) "../../../BEFE-Core/lex/Match.cpp"

$(IntermediateDirectory)/lex_MatchNode.cpp$(ObjectSuffix): ../../../BEFE-Core/lex/MatchNode.cpp $(IntermediateDirectory)/lex_MatchNode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lex/MatchNode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lex_MatchNode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lex_MatchNode.cpp$(DependSuffix): ../../../BEFE-Core/lex/MatchNode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lex_MatchNode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lex_MatchNode.cpp$(DependSuffix) -MM "../../../BEFE-Core/lex/MatchNode.cpp"

$(IntermediateDirectory)/lex_MatchNode.cpp$(PreprocessSuffix): ../../../BEFE-Core/lex/MatchNode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lex_MatchNode.cpp$(PreprocessSuffix) "../../../BEFE-Core/lex/MatchNode.cpp"

$(IntermediateDirectory)/lex_MatchNodes.cpp$(ObjectSuffix): ../../../BEFE-Core/lex/MatchNodes.cpp $(IntermediateDirectory)/lex_MatchNodes.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/lex/MatchNodes.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/lex_MatchNodes.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/lex_MatchNodes.cpp$(DependSuffix): ../../../BEFE-Core/lex/MatchNodes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/lex_MatchNodes.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/lex_MatchNodes.cpp$(DependSuffix) -MM "../../../BEFE-Core/lex/MatchNodes.cpp"

$(IntermediateDirectory)/lex_MatchNodes.cpp$(PreprocessSuffix): ../../../BEFE-Core/lex/MatchNodes.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/lex_MatchNodes.cpp$(PreprocessSuffix) "../../../BEFE-Core/lex/MatchNodes.cpp"

$(IntermediateDirectory)/message_Message.cpp$(ObjectSuffix): ../../../BEFE-Core/message/Message.cpp $(IntermediateDirectory)/message_Message.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/message/Message.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/message_Message.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/message_Message.cpp$(DependSuffix): ../../../BEFE-Core/message/Message.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/message_Message.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/message_Message.cpp$(DependSuffix) -MM "../../../BEFE-Core/message/Message.cpp"

$(IntermediateDirectory)/message_Message.cpp$(PreprocessSuffix): ../../../BEFE-Core/message/Message.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/message_Message.cpp$(PreprocessSuffix) "../../../BEFE-Core/message/Message.cpp"

$(IntermediateDirectory)/message_MessageMessages.cpp$(ObjectSuffix): ../../../BEFE-Core/message/MessageMessages.cpp $(IntermediateDirectory)/message_MessageMessages.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/message/MessageMessages.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/message_MessageMessages.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/message_MessageMessages.cpp$(DependSuffix): ../../../BEFE-Core/message/MessageMessages.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/message_MessageMessages.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/message_MessageMessages.cpp$(DependSuffix) -MM "../../../BEFE-Core/message/MessageMessages.cpp"

$(IntermediateDirectory)/message_MessageMessages.cpp$(PreprocessSuffix): ../../../BEFE-Core/message/MessageMessages.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/message_MessageMessages.cpp$(PreprocessSuffix) "../../../BEFE-Core/message/MessageMessages.cpp"

$(IntermediateDirectory)/message_MessageRange.cpp$(ObjectSuffix): ../../../BEFE-Core/message/MessageRange.cpp $(IntermediateDirectory)/message_MessageRange.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/message/MessageRange.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/message_MessageRange.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/message_MessageRange.cpp$(DependSuffix): ../../../BEFE-Core/message/MessageRange.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/message_MessageRange.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/message_MessageRange.cpp$(DependSuffix) -MM "../../../BEFE-Core/message/MessageRange.cpp"

$(IntermediateDirectory)/message_MessageRange.cpp$(PreprocessSuffix): ../../../BEFE-Core/message/MessageRange.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/message_MessageRange.cpp$(PreprocessSuffix) "../../../BEFE-Core/message/MessageRange.cpp"

$(IntermediateDirectory)/message_MessageRanges.cpp$(ObjectSuffix): ../../../BEFE-Core/message/MessageRanges.cpp $(IntermediateDirectory)/message_MessageRanges.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/message/MessageRanges.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/message_MessageRanges.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/message_MessageRanges.cpp$(DependSuffix): ../../../BEFE-Core/message/MessageRanges.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/message_MessageRanges.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/message_MessageRanges.cpp$(DependSuffix) -MM "../../../BEFE-Core/message/MessageRanges.cpp"

$(IntermediateDirectory)/message_MessageRanges.cpp$(PreprocessSuffix): ../../../BEFE-Core/message/MessageRanges.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/message_MessageRanges.cpp$(PreprocessSuffix) "../../../BEFE-Core/message/MessageRanges.cpp"

$(IntermediateDirectory)/number_Char.cpp$(ObjectSuffix): ../../../BEFE-Core/number/Char.cpp $(IntermediateDirectory)/number_Char.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/number/Char.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/number_Char.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/number_Char.cpp$(DependSuffix): ../../../BEFE-Core/number/Char.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/number_Char.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/number_Char.cpp$(DependSuffix) -MM "../../../BEFE-Core/number/Char.cpp"

$(IntermediateDirectory)/number_Char.cpp$(PreprocessSuffix): ../../../BEFE-Core/number/Char.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/number_Char.cpp$(PreprocessSuffix) "../../../BEFE-Core/number/Char.cpp"

$(IntermediateDirectory)/number_Crc32.cpp$(ObjectSuffix): ../../../BEFE-Core/number/Crc32.cpp $(IntermediateDirectory)/number_Crc32.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/number/Crc32.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/number_Crc32.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/number_Crc32.cpp$(DependSuffix): ../../../BEFE-Core/number/Crc32.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/number_Crc32.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/number_Crc32.cpp$(DependSuffix) -MM "../../../BEFE-Core/number/Crc32.cpp"

$(IntermediateDirectory)/number_Crc32.cpp$(PreprocessSuffix): ../../../BEFE-Core/number/Crc32.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/number_Crc32.cpp$(PreprocessSuffix) "../../../BEFE-Core/number/Crc32.cpp"

$(IntermediateDirectory)/number_Integer.cpp$(ObjectSuffix): ../../../BEFE-Core/number/Integer.cpp $(IntermediateDirectory)/number_Integer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/number/Integer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/number_Integer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/number_Integer.cpp$(DependSuffix): ../../../BEFE-Core/number/Integer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/number_Integer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/number_Integer.cpp$(DependSuffix) -MM "../../../BEFE-Core/number/Integer.cpp"

$(IntermediateDirectory)/number_Integer.cpp$(PreprocessSuffix): ../../../BEFE-Core/number/Integer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/number_Integer.cpp$(PreprocessSuffix) "../../../BEFE-Core/number/Integer.cpp"

$(IntermediateDirectory)/number_IntegerDigit.cpp$(ObjectSuffix): ../../../BEFE-Core/number/IntegerDigit.cpp $(IntermediateDirectory)/number_IntegerDigit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/number/IntegerDigit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/number_IntegerDigit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/number_IntegerDigit.cpp$(DependSuffix): ../../../BEFE-Core/number/IntegerDigit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/number_IntegerDigit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/number_IntegerDigit.cpp$(DependSuffix) -MM "../../../BEFE-Core/number/IntegerDigit.cpp"

$(IntermediateDirectory)/number_IntegerDigit.cpp$(PreprocessSuffix): ../../../BEFE-Core/number/IntegerDigit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/number_IntegerDigit.cpp$(PreprocessSuffix) "../../../BEFE-Core/number/IntegerDigit.cpp"

$(IntermediateDirectory)/number_IntegerFull.cpp$(ObjectSuffix): ../../../BEFE-Core/number/IntegerFull.cpp $(IntermediateDirectory)/number_IntegerFull.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/number/IntegerFull.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/number_IntegerFull.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/number_IntegerFull.cpp$(DependSuffix): ../../../BEFE-Core/number/IntegerFull.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/number_IntegerFull.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/number_IntegerFull.cpp$(DependSuffix) -MM "../../../BEFE-Core/number/IntegerFull.cpp"

$(IntermediateDirectory)/number_IntegerFull.cpp$(PreprocessSuffix): ../../../BEFE-Core/number/IntegerFull.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/number_IntegerFull.cpp$(PreprocessSuffix) "../../../BEFE-Core/number/IntegerFull.cpp"

$(IntermediateDirectory)/number_IntegerOperator.cpp$(ObjectSuffix): ../../../BEFE-Core/number/IntegerOperator.cpp $(IntermediateDirectory)/number_IntegerOperator.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/number/IntegerOperator.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/number_IntegerOperator.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/number_IntegerOperator.cpp$(DependSuffix): ../../../BEFE-Core/number/IntegerOperator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/number_IntegerOperator.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/number_IntegerOperator.cpp$(DependSuffix) -MM "../../../BEFE-Core/number/IntegerOperator.cpp"

$(IntermediateDirectory)/number_IntegerOperator.cpp$(PreprocessSuffix): ../../../BEFE-Core/number/IntegerOperator.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/number_IntegerOperator.cpp$(PreprocessSuffix) "../../../BEFE-Core/number/IntegerOperator.cpp"

$(IntermediateDirectory)/number_IntFuncs.cpp$(ObjectSuffix): ../../../BEFE-Core/number/IntFuncs.cpp $(IntermediateDirectory)/number_IntFuncs.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/number/IntFuncs.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/number_IntFuncs.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/number_IntFuncs.cpp$(DependSuffix): ../../../BEFE-Core/number/IntFuncs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/number_IntFuncs.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/number_IntFuncs.cpp$(DependSuffix) -MM "../../../BEFE-Core/number/IntFuncs.cpp"

$(IntermediateDirectory)/number_IntFuncs.cpp$(PreprocessSuffix): ../../../BEFE-Core/number/IntFuncs.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/number_IntFuncs.cpp$(PreprocessSuffix) "../../../BEFE-Core/number/IntFuncs.cpp"

$(IntermediateDirectory)/number_MD5.cpp$(ObjectSuffix): ../../../BEFE-Core/number/MD5.cpp $(IntermediateDirectory)/number_MD5.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/number/MD5.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/number_MD5.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/number_MD5.cpp$(DependSuffix): ../../../BEFE-Core/number/MD5.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/number_MD5.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/number_MD5.cpp$(DependSuffix) -MM "../../../BEFE-Core/number/MD5.cpp"

$(IntermediateDirectory)/number_MD5.cpp$(PreprocessSuffix): ../../../BEFE-Core/number/MD5.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/number_MD5.cpp$(PreprocessSuffix) "../../../BEFE-Core/number/MD5.cpp"

$(IntermediateDirectory)/number_MD5Calc.cpp$(ObjectSuffix): ../../../BEFE-Core/number/MD5Calc.cpp $(IntermediateDirectory)/number_MD5Calc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/number/MD5Calc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/number_MD5Calc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/number_MD5Calc.cpp$(DependSuffix): ../../../BEFE-Core/number/MD5Calc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/number_MD5Calc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/number_MD5Calc.cpp$(DependSuffix) -MM "../../../BEFE-Core/number/MD5Calc.cpp"

$(IntermediateDirectory)/number_MD5Calc.cpp$(PreprocessSuffix): ../../../BEFE-Core/number/MD5Calc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/number_MD5Calc.cpp$(PreprocessSuffix) "../../../BEFE-Core/number/MD5Calc.cpp"

$(IntermediateDirectory)/number_Number.cpp$(ObjectSuffix): ../../../BEFE-Core/number/Number.cpp $(IntermediateDirectory)/number_Number.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/number/Number.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/number_Number.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/number_Number.cpp$(DependSuffix): ../../../BEFE-Core/number/Number.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/number_Number.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/number_Number.cpp$(DependSuffix) -MM "../../../BEFE-Core/number/Number.cpp"

$(IntermediateDirectory)/number_Number.cpp$(PreprocessSuffix): ../../../BEFE-Core/number/Number.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/number_Number.cpp$(PreprocessSuffix) "../../../BEFE-Core/number/Number.cpp"

$(IntermediateDirectory)/number_OraNumber.cpp$(ObjectSuffix): ../../../BEFE-Core/number/OraNumber.cpp $(IntermediateDirectory)/number_OraNumber.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/number/OraNumber.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/number_OraNumber.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/number_OraNumber.cpp$(DependSuffix): ../../../BEFE-Core/number/OraNumber.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/number_OraNumber.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/number_OraNumber.cpp$(DependSuffix) -MM "../../../BEFE-Core/number/OraNumber.cpp"

$(IntermediateDirectory)/number_OraNumber.cpp$(PreprocessSuffix): ../../../BEFE-Core/number/OraNumber.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/number_OraNumber.cpp$(PreprocessSuffix) "../../../BEFE-Core/number/OraNumber.cpp"

$(IntermediateDirectory)/persist_LexWorkspace.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/LexWorkspace.cpp $(IntermediateDirectory)/persist_LexWorkspace.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/LexWorkspace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_LexWorkspace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_LexWorkspace.cpp$(DependSuffix): ../../../BEFE-Core/persist/LexWorkspace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_LexWorkspace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_LexWorkspace.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/LexWorkspace.cpp"

$(IntermediateDirectory)/persist_LexWorkspace.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/LexWorkspace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_LexWorkspace.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/LexWorkspace.cpp"

$(IntermediateDirectory)/persist_LexWorkspaceBlob.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/LexWorkspaceBlob.cpp $(IntermediateDirectory)/persist_LexWorkspaceBlob.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/LexWorkspaceBlob.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_LexWorkspaceBlob.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_LexWorkspaceBlob.cpp$(DependSuffix): ../../../BEFE-Core/persist/LexWorkspaceBlob.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_LexWorkspaceBlob.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_LexWorkspaceBlob.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/LexWorkspaceBlob.cpp"

$(IntermediateDirectory)/persist_LexWorkspaceBlob.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/LexWorkspaceBlob.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_LexWorkspaceBlob.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/LexWorkspaceBlob.cpp"

$(IntermediateDirectory)/persist_LexWorkspaceFile.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/LexWorkspaceFile.cpp $(IntermediateDirectory)/persist_LexWorkspaceFile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/LexWorkspaceFile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_LexWorkspaceFile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_LexWorkspaceFile.cpp$(DependSuffix): ../../../BEFE-Core/persist/LexWorkspaceFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_LexWorkspaceFile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_LexWorkspaceFile.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/LexWorkspaceFile.cpp"

$(IntermediateDirectory)/persist_LexWorkspaceFile.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/LexWorkspaceFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_LexWorkspaceFile.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/LexWorkspaceFile.cpp"

$(IntermediateDirectory)/persist_LexWorkspaceIdEntry.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/LexWorkspaceIdEntry.cpp $(IntermediateDirectory)/persist_LexWorkspaceIdEntry.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/LexWorkspaceIdEntry.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_LexWorkspaceIdEntry.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_LexWorkspaceIdEntry.cpp$(DependSuffix): ../../../BEFE-Core/persist/LexWorkspaceIdEntry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_LexWorkspaceIdEntry.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_LexWorkspaceIdEntry.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/LexWorkspaceIdEntry.cpp"

$(IntermediateDirectory)/persist_LexWorkspaceIdEntry.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/LexWorkspaceIdEntry.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_LexWorkspaceIdEntry.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/LexWorkspaceIdEntry.cpp"

$(IntermediateDirectory)/persist_LexWorkspaceString.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/LexWorkspaceString.cpp $(IntermediateDirectory)/persist_LexWorkspaceString.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/LexWorkspaceString.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_LexWorkspaceString.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_LexWorkspaceString.cpp$(DependSuffix): ../../../BEFE-Core/persist/LexWorkspaceString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_LexWorkspaceString.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_LexWorkspaceString.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/LexWorkspaceString.cpp"

$(IntermediateDirectory)/persist_LexWorkspaceString.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/LexWorkspaceString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_LexWorkspaceString.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/LexWorkspaceString.cpp"

$(IntermediateDirectory)/persist_LexWsArrayHead.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/LexWsArrayHead.cpp $(IntermediateDirectory)/persist_LexWsArrayHead.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/LexWsArrayHead.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_LexWsArrayHead.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_LexWsArrayHead.cpp$(DependSuffix): ../../../BEFE-Core/persist/LexWsArrayHead.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_LexWsArrayHead.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_LexWsArrayHead.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/LexWsArrayHead.cpp"

$(IntermediateDirectory)/persist_LexWsArrayHead.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/LexWsArrayHead.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_LexWsArrayHead.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/LexWsArrayHead.cpp"

$(IntermediateDirectory)/persist_LexWsBlob.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/LexWsBlob.cpp $(IntermediateDirectory)/persist_LexWsBlob.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/LexWsBlob.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_LexWsBlob.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_LexWsBlob.cpp$(DependSuffix): ../../../BEFE-Core/persist/LexWsBlob.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_LexWsBlob.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_LexWsBlob.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/LexWsBlob.cpp"

$(IntermediateDirectory)/persist_LexWsBlob.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/LexWsBlob.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_LexWsBlob.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/LexWsBlob.cpp"

$(IntermediateDirectory)/persist_LexWsBlockHead.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/LexWsBlockHead.cpp $(IntermediateDirectory)/persist_LexWsBlockHead.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/LexWsBlockHead.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_LexWsBlockHead.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_LexWsBlockHead.cpp$(DependSuffix): ../../../BEFE-Core/persist/LexWsBlockHead.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_LexWsBlockHead.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_LexWsBlockHead.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/LexWsBlockHead.cpp"

$(IntermediateDirectory)/persist_LexWsBlockHead.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/LexWsBlockHead.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_LexWsBlockHead.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/LexWsBlockHead.cpp"

$(IntermediateDirectory)/persist_LexWsFile.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/LexWsFile.cpp $(IntermediateDirectory)/persist_LexWsFile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/LexWsFile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_LexWsFile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_LexWsFile.cpp$(DependSuffix): ../../../BEFE-Core/persist/LexWsFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_LexWsFile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_LexWsFile.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/LexWsFile.cpp"

$(IntermediateDirectory)/persist_LexWsFile.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/LexWsFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_LexWsFile.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/LexWsFile.cpp"

$(IntermediateDirectory)/persist_LexWsHead.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/LexWsHead.cpp $(IntermediateDirectory)/persist_LexWsHead.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/LexWsHead.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_LexWsHead.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_LexWsHead.cpp$(DependSuffix): ../../../BEFE-Core/persist/LexWsHead.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_LexWsHead.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_LexWsHead.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/LexWsHead.cpp"

$(IntermediateDirectory)/persist_LexWsHead.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/LexWsHead.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_LexWsHead.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/LexWsHead.cpp"

$(IntermediateDirectory)/persist_LexWsString.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/LexWsString.cpp $(IntermediateDirectory)/persist_LexWsString.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/LexWsString.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_LexWsString.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_LexWsString.cpp$(DependSuffix): ../../../BEFE-Core/persist/LexWsString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_LexWsString.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_LexWsString.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/LexWsString.cpp"

$(IntermediateDirectory)/persist_LexWsString.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/LexWsString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_LexWsString.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/LexWsString.cpp"

$(IntermediateDirectory)/persist_Persist.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/Persist.cpp $(IntermediateDirectory)/persist_Persist.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/Persist.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_Persist.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_Persist.cpp$(DependSuffix): ../../../BEFE-Core/persist/Persist.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_Persist.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_Persist.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/Persist.cpp"

$(IntermediateDirectory)/persist_Persist.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/Persist.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_Persist.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/Persist.cpp"

$(IntermediateDirectory)/persist_PersistAnon.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/PersistAnon.cpp $(IntermediateDirectory)/persist_PersistAnon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/PersistAnon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_PersistAnon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_PersistAnon.cpp$(DependSuffix): ../../../BEFE-Core/persist/PersistAnon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_PersistAnon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_PersistAnon.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/PersistAnon.cpp"

$(IntermediateDirectory)/persist_PersistAnon.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/PersistAnon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_PersistAnon.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/PersistAnon.cpp"

$(IntermediateDirectory)/persist_PersistAtomic.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/PersistAtomic.cpp $(IntermediateDirectory)/persist_PersistAtomic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/PersistAtomic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_PersistAtomic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_PersistAtomic.cpp$(DependSuffix): ../../../BEFE-Core/persist/PersistAtomic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_PersistAtomic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_PersistAtomic.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/PersistAtomic.cpp"

$(IntermediateDirectory)/persist_PersistAtomic.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/PersistAtomic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_PersistAtomic.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/PersistAtomic.cpp"

$(IntermediateDirectory)/persist_PersistHead.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/PersistHead.cpp $(IntermediateDirectory)/persist_PersistHead.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/PersistHead.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_PersistHead.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_PersistHead.cpp$(DependSuffix): ../../../BEFE-Core/persist/PersistHead.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_PersistHead.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_PersistHead.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/PersistHead.cpp"

$(IntermediateDirectory)/persist_PersistHead.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/PersistHead.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_PersistHead.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/PersistHead.cpp"

$(IntermediateDirectory)/persist_PersistNamed.cpp$(ObjectSuffix): ../../../BEFE-Core/persist/PersistNamed.cpp $(IntermediateDirectory)/persist_PersistNamed.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/persist/PersistNamed.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/persist_PersistNamed.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/persist_PersistNamed.cpp$(DependSuffix): ../../../BEFE-Core/persist/PersistNamed.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/persist_PersistNamed.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/persist_PersistNamed.cpp$(DependSuffix) -MM "../../../BEFE-Core/persist/PersistNamed.cpp"

$(IntermediateDirectory)/persist_PersistNamed.cpp$(PreprocessSuffix): ../../../BEFE-Core/persist/PersistNamed.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/persist_PersistNamed.cpp$(PreprocessSuffix) "../../../BEFE-Core/persist/PersistNamed.cpp"

$(IntermediateDirectory)/seq_Alloc.cpp$(ObjectSuffix): ../../../BEFE-Core/seq/Alloc.cpp $(IntermediateDirectory)/seq_Alloc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/seq/Alloc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/seq_Alloc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/seq_Alloc.cpp$(DependSuffix): ../../../BEFE-Core/seq/Alloc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/seq_Alloc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/seq_Alloc.cpp$(DependSuffix) -MM "../../../BEFE-Core/seq/Alloc.cpp"

$(IntermediateDirectory)/seq_Alloc.cpp$(PreprocessSuffix): ../../../BEFE-Core/seq/Alloc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/seq_Alloc.cpp$(PreprocessSuffix) "../../../BEFE-Core/seq/Alloc.cpp"

$(IntermediateDirectory)/seq_Cycle.cpp$(ObjectSuffix): ../../../BEFE-Core/seq/Cycle.cpp $(IntermediateDirectory)/seq_Cycle.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/seq/Cycle.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/seq_Cycle.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/seq_Cycle.cpp$(DependSuffix): ../../../BEFE-Core/seq/Cycle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/seq_Cycle.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/seq_Cycle.cpp$(DependSuffix) -MM "../../../BEFE-Core/seq/Cycle.cpp"

$(IntermediateDirectory)/seq_Cycle.cpp$(PreprocessSuffix): ../../../BEFE-Core/seq/Cycle.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/seq_Cycle.cpp$(PreprocessSuffix) "../../../BEFE-Core/seq/Cycle.cpp"

$(IntermediateDirectory)/seq_Range.cpp$(ObjectSuffix): ../../../BEFE-Core/seq/Range.cpp $(IntermediateDirectory)/seq_Range.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/seq/Range.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/seq_Range.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/seq_Range.cpp$(DependSuffix): ../../../BEFE-Core/seq/Range.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/seq_Range.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/seq_Range.cpp$(DependSuffix) -MM "../../../BEFE-Core/seq/Range.cpp"

$(IntermediateDirectory)/seq_Range.cpp$(PreprocessSuffix): ../../../BEFE-Core/seq/Range.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/seq_Range.cpp$(PreprocessSuffix) "../../../BEFE-Core/seq/Range.cpp"

$(IntermediateDirectory)/seq_Slice.cpp$(ObjectSuffix): ../../../BEFE-Core/seq/Slice.cpp $(IntermediateDirectory)/seq_Slice.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/seq/Slice.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/seq_Slice.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/seq_Slice.cpp$(DependSuffix): ../../../BEFE-Core/seq/Slice.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/seq_Slice.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/seq_Slice.cpp$(DependSuffix) -MM "../../../BEFE-Core/seq/Slice.cpp"

$(IntermediateDirectory)/seq_Slice.cpp$(PreprocessSuffix): ../../../BEFE-Core/seq/Slice.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/seq_Slice.cpp$(PreprocessSuffix) "../../../BEFE-Core/seq/Slice.cpp"

$(IntermediateDirectory)/seq_SortBubble.cpp$(ObjectSuffix): ../../../BEFE-Core/seq/SortBubble.cpp $(IntermediateDirectory)/seq_SortBubble.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/seq/SortBubble.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/seq_SortBubble.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/seq_SortBubble.cpp$(DependSuffix): ../../../BEFE-Core/seq/SortBubble.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/seq_SortBubble.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/seq_SortBubble.cpp$(DependSuffix) -MM "../../../BEFE-Core/seq/SortBubble.cpp"

$(IntermediateDirectory)/seq_SortBubble.cpp$(PreprocessSuffix): ../../../BEFE-Core/seq/SortBubble.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/seq_SortBubble.cpp$(PreprocessSuffix) "../../../BEFE-Core/seq/SortBubble.cpp"

$(IntermediateDirectory)/seq_SortInsertion.cpp$(ObjectSuffix): ../../../BEFE-Core/seq/SortInsertion.cpp $(IntermediateDirectory)/seq_SortInsertion.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/seq/SortInsertion.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/seq_SortInsertion.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/seq_SortInsertion.cpp$(DependSuffix): ../../../BEFE-Core/seq/SortInsertion.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/seq_SortInsertion.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/seq_SortInsertion.cpp$(DependSuffix) -MM "../../../BEFE-Core/seq/SortInsertion.cpp"

$(IntermediateDirectory)/seq_SortInsertion.cpp$(PreprocessSuffix): ../../../BEFE-Core/seq/SortInsertion.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/seq_SortInsertion.cpp$(PreprocessSuffix) "../../../BEFE-Core/seq/SortInsertion.cpp"

$(IntermediateDirectory)/seq_SortMerge.cpp$(ObjectSuffix): ../../../BEFE-Core/seq/SortMerge.cpp $(IntermediateDirectory)/seq_SortMerge.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/seq/SortMerge.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/seq_SortMerge.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/seq_SortMerge.cpp$(DependSuffix): ../../../BEFE-Core/seq/SortMerge.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/seq_SortMerge.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/seq_SortMerge.cpp$(DependSuffix) -MM "../../../BEFE-Core/seq/SortMerge.cpp"

$(IntermediateDirectory)/seq_SortMerge.cpp$(PreprocessSuffix): ../../../BEFE-Core/seq/SortMerge.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/seq_SortMerge.cpp$(PreprocessSuffix) "../../../BEFE-Core/seq/SortMerge.cpp"

$(IntermediateDirectory)/seq_SortSelection.cpp$(ObjectSuffix): ../../../BEFE-Core/seq/SortSelection.cpp $(IntermediateDirectory)/seq_SortSelection.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/seq/SortSelection.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/seq_SortSelection.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/seq_SortSelection.cpp$(DependSuffix): ../../../BEFE-Core/seq/SortSelection.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/seq_SortSelection.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/seq_SortSelection.cpp$(DependSuffix) -MM "../../../BEFE-Core/seq/SortSelection.cpp"

$(IntermediateDirectory)/seq_SortSelection.cpp$(PreprocessSuffix): ../../../BEFE-Core/seq/SortSelection.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/seq_SortSelection.cpp$(PreprocessSuffix) "../../../BEFE-Core/seq/SortSelection.cpp"

$(IntermediateDirectory)/seq_Span.cpp$(ObjectSuffix): ../../../BEFE-Core/seq/Span.cpp $(IntermediateDirectory)/seq_Span.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/seq/Span.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/seq_Span.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/seq_Span.cpp$(DependSuffix): ../../../BEFE-Core/seq/Span.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/seq_Span.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/seq_Span.cpp$(DependSuffix) -MM "../../../BEFE-Core/seq/Span.cpp"

$(IntermediateDirectory)/seq_Span.cpp$(PreprocessSuffix): ../../../BEFE-Core/seq/Span.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/seq_Span.cpp$(PreprocessSuffix) "../../../BEFE-Core/seq/Span.cpp"

$(IntermediateDirectory)/seq_URange.cpp$(ObjectSuffix): ../../../BEFE-Core/seq/URange.cpp $(IntermediateDirectory)/seq_URange.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/seq/URange.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/seq_URange.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/seq_URange.cpp$(DependSuffix): ../../../BEFE-Core/seq/URange.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/seq_URange.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/seq_URange.cpp$(DependSuffix) -MM "../../../BEFE-Core/seq/URange.cpp"

$(IntermediateDirectory)/seq_URange.cpp$(PreprocessSuffix): ../../../BEFE-Core/seq/URange.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/seq_URange.cpp$(PreprocessSuffix) "../../../BEFE-Core/seq/URange.cpp"

$(IntermediateDirectory)/seq_USpan.cpp$(ObjectSuffix): ../../../BEFE-Core/seq/USpan.cpp $(IntermediateDirectory)/seq_USpan.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/seq/USpan.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/seq_USpan.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/seq_USpan.cpp$(DependSuffix): ../../../BEFE-Core/seq/USpan.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/seq_USpan.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/seq_USpan.cpp$(DependSuffix) -MM "../../../BEFE-Core/seq/USpan.cpp"

$(IntermediateDirectory)/seq_USpan.cpp$(PreprocessSuffix): ../../../BEFE-Core/seq/USpan.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/seq_USpan.cpp$(PreprocessSuffix) "../../../BEFE-Core/seq/USpan.cpp"

$(IntermediateDirectory)/storage_BufferStream.cpp$(ObjectSuffix): ../../../BEFE-Core/storage/BufferStream.cpp $(IntermediateDirectory)/storage_BufferStream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/storage/BufferStream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/storage_BufferStream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/storage_BufferStream.cpp$(DependSuffix): ../../../BEFE-Core/storage/BufferStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/storage_BufferStream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/storage_BufferStream.cpp$(DependSuffix) -MM "../../../BEFE-Core/storage/BufferStream.cpp"

$(IntermediateDirectory)/storage_BufferStream.cpp$(PreprocessSuffix): ../../../BEFE-Core/storage/BufferStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/storage_BufferStream.cpp$(PreprocessSuffix) "../../../BEFE-Core/storage/BufferStream.cpp"

$(IntermediateDirectory)/storage_File.cpp$(ObjectSuffix): ../../../BEFE-Core/storage/File.cpp $(IntermediateDirectory)/storage_File.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/storage/File.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/storage_File.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/storage_File.cpp$(DependSuffix): ../../../BEFE-Core/storage/File.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/storage_File.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/storage_File.cpp$(DependSuffix) -MM "../../../BEFE-Core/storage/File.cpp"

$(IntermediateDirectory)/storage_File.cpp$(PreprocessSuffix): ../../../BEFE-Core/storage/File.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/storage_File.cpp$(PreprocessSuffix) "../../../BEFE-Core/storage/File.cpp"

$(IntermediateDirectory)/storage_FileInfo.cpp$(ObjectSuffix): ../../../BEFE-Core/storage/FileInfo.cpp $(IntermediateDirectory)/storage_FileInfo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/storage/FileInfo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/storage_FileInfo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/storage_FileInfo.cpp$(DependSuffix): ../../../BEFE-Core/storage/FileInfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/storage_FileInfo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/storage_FileInfo.cpp$(DependSuffix) -MM "../../../BEFE-Core/storage/FileInfo.cpp"

$(IntermediateDirectory)/storage_FileInfo.cpp$(PreprocessSuffix): ../../../BEFE-Core/storage/FileInfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/storage_FileInfo.cpp$(PreprocessSuffix) "../../../BEFE-Core/storage/FileInfo.cpp"

$(IntermediateDirectory)/storage_FileStream.cpp$(ObjectSuffix): ../../../BEFE-Core/storage/FileStream.cpp $(IntermediateDirectory)/storage_FileStream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/storage/FileStream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/storage_FileStream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/storage_FileStream.cpp$(DependSuffix): ../../../BEFE-Core/storage/FileStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/storage_FileStream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/storage_FileStream.cpp$(DependSuffix) -MM "../../../BEFE-Core/storage/FileStream.cpp"

$(IntermediateDirectory)/storage_FileStream.cpp$(PreprocessSuffix): ../../../BEFE-Core/storage/FileStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/storage_FileStream.cpp$(PreprocessSuffix) "../../../BEFE-Core/storage/FileStream.cpp"

$(IntermediateDirectory)/storage_Lines.cpp$(ObjectSuffix): ../../../BEFE-Core/storage/Lines.cpp $(IntermediateDirectory)/storage_Lines.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/storage/Lines.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/storage_Lines.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/storage_Lines.cpp$(DependSuffix): ../../../BEFE-Core/storage/Lines.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/storage_Lines.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/storage_Lines.cpp$(DependSuffix) -MM "../../../BEFE-Core/storage/Lines.cpp"

$(IntermediateDirectory)/storage_Lines.cpp$(PreprocessSuffix): ../../../BEFE-Core/storage/Lines.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/storage_Lines.cpp$(PreprocessSuffix) "../../../BEFE-Core/storage/Lines.cpp"

$(IntermediateDirectory)/storage_MemoryStats.cpp$(ObjectSuffix): ../../../BEFE-Core/storage/MemoryStats.cpp $(IntermediateDirectory)/storage_MemoryStats.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/storage/MemoryStats.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/storage_MemoryStats.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/storage_MemoryStats.cpp$(DependSuffix): ../../../BEFE-Core/storage/MemoryStats.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/storage_MemoryStats.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/storage_MemoryStats.cpp$(DependSuffix) -MM "../../../BEFE-Core/storage/MemoryStats.cpp"

$(IntermediateDirectory)/storage_MemoryStats.cpp$(PreprocessSuffix): ../../../BEFE-Core/storage/MemoryStats.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/storage_MemoryStats.cpp$(PreprocessSuffix) "../../../BEFE-Core/storage/MemoryStats.cpp"

$(IntermediateDirectory)/storage_MemoryUtils.cpp$(ObjectSuffix): ../../../BEFE-Core/storage/MemoryUtils.cpp $(IntermediateDirectory)/storage_MemoryUtils.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/storage/MemoryUtils.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/storage_MemoryUtils.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/storage_MemoryUtils.cpp$(DependSuffix): ../../../BEFE-Core/storage/MemoryUtils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/storage_MemoryUtils.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/storage_MemoryUtils.cpp$(DependSuffix) -MM "../../../BEFE-Core/storage/MemoryUtils.cpp"

$(IntermediateDirectory)/storage_MemoryUtils.cpp$(PreprocessSuffix): ../../../BEFE-Core/storage/MemoryUtils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/storage_MemoryUtils.cpp$(PreprocessSuffix) "../../../BEFE-Core/storage/MemoryUtils.cpp"

$(IntermediateDirectory)/storage_Stream.cpp$(ObjectSuffix): ../../../BEFE-Core/storage/Stream.cpp $(IntermediateDirectory)/storage_Stream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/storage/Stream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/storage_Stream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/storage_Stream.cpp$(DependSuffix): ../../../BEFE-Core/storage/Stream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/storage_Stream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/storage_Stream.cpp$(DependSuffix) -MM "../../../BEFE-Core/storage/Stream.cpp"

$(IntermediateDirectory)/storage_Stream.cpp$(PreprocessSuffix): ../../../BEFE-Core/storage/Stream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/storage_Stream.cpp$(PreprocessSuffix) "../../../BEFE-Core/storage/Stream.cpp"

$(IntermediateDirectory)/test_Test.cpp$(ObjectSuffix): ../../../BEFE-Core/test/Test.cpp $(IntermediateDirectory)/test_Test.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/Test.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_Test.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_Test.cpp$(DependSuffix): ../../../BEFE-Core/test/Test.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_Test.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_Test.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/Test.cpp"

$(IntermediateDirectory)/test_Test.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/Test.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_Test.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/Test.cpp"

$(IntermediateDirectory)/test_TestArray.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestArray.cpp $(IntermediateDirectory)/test_TestArray.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestArray.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestArray.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestArray.cpp$(DependSuffix): ../../../BEFE-Core/test/TestArray.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestArray.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestArray.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestArray.cpp"

$(IntermediateDirectory)/test_TestArray.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestArray.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestArray.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestArray.cpp"

$(IntermediateDirectory)/test_TestBcmd.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestBcmd.cpp $(IntermediateDirectory)/test_TestBcmd.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestBcmd.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestBcmd.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestBcmd.cpp$(DependSuffix): ../../../BEFE-Core/test/TestBcmd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestBcmd.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestBcmd.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestBcmd.cpp"

$(IntermediateDirectory)/test_TestBcmd.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestBcmd.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestBcmd.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestBcmd.cpp"

$(IntermediateDirectory)/test_TestCache.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestCache.cpp $(IntermediateDirectory)/test_TestCache.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestCache.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestCache.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestCache.cpp$(DependSuffix): ../../../BEFE-Core/test/TestCache.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestCache.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestCache.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestCache.cpp"

$(IntermediateDirectory)/test_TestCache.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestCache.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestCache.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestCache.cpp"

$(IntermediateDirectory)/test_TestConsole.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestConsole.cpp $(IntermediateDirectory)/test_TestConsole.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestConsole.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestConsole.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestConsole.cpp$(DependSuffix): ../../../BEFE-Core/test/TestConsole.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestConsole.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestConsole.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestConsole.cpp"

$(IntermediateDirectory)/test_TestConsole.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestConsole.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestConsole.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestConsole.cpp"

$(IntermediateDirectory)/test_TestContains.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestContains.cpp $(IntermediateDirectory)/test_TestContains.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestContains.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestContains.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestContains.cpp$(DependSuffix): ../../../BEFE-Core/test/TestContains.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestContains.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestContains.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestContains.cpp"

$(IntermediateDirectory)/test_TestContains.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestContains.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestContains.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestContains.cpp"

$(IntermediateDirectory)/test_TestFile.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestFile.cpp $(IntermediateDirectory)/test_TestFile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestFile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestFile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestFile.cpp$(DependSuffix): ../../../BEFE-Core/test/TestFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestFile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestFile.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestFile.cpp"

$(IntermediateDirectory)/test_TestFile.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestFile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestFile.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestFile.cpp"

$(IntermediateDirectory)/test_TestIdSpace.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestIdSpace.cpp $(IntermediateDirectory)/test_TestIdSpace.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestIdSpace.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestIdSpace.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestIdSpace.cpp$(DependSuffix): ../../../BEFE-Core/test/TestIdSpace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestIdSpace.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestIdSpace.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestIdSpace.cpp"

$(IntermediateDirectory)/test_TestIdSpace.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestIdSpace.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestIdSpace.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestIdSpace.cpp"

$(IntermediateDirectory)/test_TestLexicon.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestLexicon.cpp $(IntermediateDirectory)/test_TestLexicon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestLexicon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestLexicon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestLexicon.cpp$(DependSuffix): ../../../BEFE-Core/test/TestLexicon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestLexicon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestLexicon.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestLexicon.cpp"

$(IntermediateDirectory)/test_TestLexicon.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestLexicon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestLexicon.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestLexicon.cpp"

$(IntermediateDirectory)/test_TestMD5.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestMD5.cpp $(IntermediateDirectory)/test_TestMD5.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestMD5.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestMD5.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestMD5.cpp$(DependSuffix): ../../../BEFE-Core/test/TestMD5.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestMD5.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestMD5.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestMD5.cpp"

$(IntermediateDirectory)/test_TestMD5.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestMD5.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestMD5.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestMD5.cpp"

$(IntermediateDirectory)/test_TestMessage.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestMessage.cpp $(IntermediateDirectory)/test_TestMessage.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestMessage.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestMessage.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestMessage.cpp$(DependSuffix): ../../../BEFE-Core/test/TestMessage.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestMessage.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestMessage.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestMessage.cpp"

$(IntermediateDirectory)/test_TestMessage.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestMessage.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestMessage.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestMessage.cpp"

$(IntermediateDirectory)/test_TestMorphing.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestMorphing.cpp $(IntermediateDirectory)/test_TestMorphing.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestMorphing.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestMorphing.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestMorphing.cpp$(DependSuffix): ../../../BEFE-Core/test/TestMorphing.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestMorphing.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestMorphing.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestMorphing.cpp"

$(IntermediateDirectory)/test_TestMorphing.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestMorphing.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestMorphing.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestMorphing.cpp"

$(IntermediateDirectory)/test_TestNull.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestNull.cpp $(IntermediateDirectory)/test_TestNull.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestNull.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestNull.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestNull.cpp$(DependSuffix): ../../../BEFE-Core/test/TestNull.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestNull.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestNull.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestNull.cpp"

$(IntermediateDirectory)/test_TestNull.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestNull.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestNull.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestNull.cpp"

$(IntermediateDirectory)/test_TestNumber.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestNumber.cpp $(IntermediateDirectory)/test_TestNumber.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestNumber.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestNumber.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestNumber.cpp$(DependSuffix): ../../../BEFE-Core/test/TestNumber.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestNumber.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestNumber.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestNumber.cpp"

$(IntermediateDirectory)/test_TestNumber.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestNumber.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestNumber.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestNumber.cpp"

$(IntermediateDirectory)/test_TestSequence.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestSequence.cpp $(IntermediateDirectory)/test_TestSequence.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestSequence.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestSequence.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestSequence.cpp$(DependSuffix): ../../../BEFE-Core/test/TestSequence.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestSequence.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestSequence.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestSequence.cpp"

$(IntermediateDirectory)/test_TestSequence.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestSequence.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestSequence.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestSequence.cpp"

$(IntermediateDirectory)/test_TestStream.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestStream.cpp $(IntermediateDirectory)/test_TestStream.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestStream.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestStream.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestStream.cpp$(DependSuffix): ../../../BEFE-Core/test/TestStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestStream.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestStream.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestStream.cpp"

$(IntermediateDirectory)/test_TestStream.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestStream.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestStream.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestStream.cpp"

$(IntermediateDirectory)/test_TestString.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestString.cpp $(IntermediateDirectory)/test_TestString.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestString.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestString.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestString.cpp$(DependSuffix): ../../../BEFE-Core/test/TestString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestString.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestString.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestString.cpp"

$(IntermediateDirectory)/test_TestString.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestString.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestString.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestString.cpp"

$(IntermediateDirectory)/test_TestTime.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestTime.cpp $(IntermediateDirectory)/test_TestTime.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestTime.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestTime.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestTime.cpp$(DependSuffix): ../../../BEFE-Core/test/TestTime.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestTime.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestTime.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestTime.cpp"

$(IntermediateDirectory)/test_TestTime.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestTime.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestTime.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestTime.cpp"

$(IntermediateDirectory)/test_TestUTF8.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestUTF8.cpp $(IntermediateDirectory)/test_TestUTF8.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestUTF8.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestUTF8.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestUTF8.cpp$(DependSuffix): ../../../BEFE-Core/test/TestUTF8.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestUTF8.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestUTF8.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestUTF8.cpp"

$(IntermediateDirectory)/test_TestUTF8.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestUTF8.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestUTF8.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestUTF8.cpp"

$(IntermediateDirectory)/test_TestUUID.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestUUID.cpp $(IntermediateDirectory)/test_TestUUID.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestUUID.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestUUID.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestUUID.cpp$(DependSuffix): ../../../BEFE-Core/test/TestUUID.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestUUID.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestUUID.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestUUID.cpp"

$(IntermediateDirectory)/test_TestUUID.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestUUID.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestUUID.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestUUID.cpp"

$(IntermediateDirectory)/test_TestVarTerm.cpp$(ObjectSuffix): ../../../BEFE-Core/test/TestVarTerm.cpp $(IntermediateDirectory)/test_TestVarTerm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/TestVarTerm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_TestVarTerm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_TestVarTerm.cpp$(DependSuffix): ../../../BEFE-Core/test/TestVarTerm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_TestVarTerm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_TestVarTerm.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/TestVarTerm.cpp"

$(IntermediateDirectory)/test_TestVarTerm.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/TestVarTerm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_TestVarTerm.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/TestVarTerm.cpp"

$(IntermediateDirectory)/test_trythis.cpp$(ObjectSuffix): ../../../BEFE-Core/test/trythis.cpp $(IntermediateDirectory)/test_trythis.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/test/trythis.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/test_trythis.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/test_trythis.cpp$(DependSuffix): ../../../BEFE-Core/test/trythis.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/test_trythis.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/test_trythis.cpp$(DependSuffix) -MM "../../../BEFE-Core/test/trythis.cpp"

$(IntermediateDirectory)/test_trythis.cpp$(PreprocessSuffix): ../../../BEFE-Core/test/trythis.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/test_trythis.cpp$(PreprocessSuffix) "../../../BEFE-Core/test/trythis.cpp"

$(IntermediateDirectory)/various_Build.cpp$(ObjectSuffix): ../../../BEFE-Core/various/Build.cpp $(IntermediateDirectory)/various_Build.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/various/Build.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/various_Build.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/various_Build.cpp$(DependSuffix): ../../../BEFE-Core/various/Build.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/various_Build.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/various_Build.cpp$(DependSuffix) -MM "../../../BEFE-Core/various/Build.cpp"

$(IntermediateDirectory)/various_Build.cpp$(PreprocessSuffix): ../../../BEFE-Core/various/Build.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/various_Build.cpp$(PreprocessSuffix) "../../../BEFE-Core/various/Build.cpp"

$(IntermediateDirectory)/various_BuildDisplay.cpp$(ObjectSuffix): ../../../BEFE-Core/various/BuildDisplay.cpp $(IntermediateDirectory)/various_BuildDisplay.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/various/BuildDisplay.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/various_BuildDisplay.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/various_BuildDisplay.cpp$(DependSuffix): ../../../BEFE-Core/various/BuildDisplay.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/various_BuildDisplay.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/various_BuildDisplay.cpp$(DependSuffix) -MM "../../../BEFE-Core/various/BuildDisplay.cpp"

$(IntermediateDirectory)/various_BuildDisplay.cpp$(PreprocessSuffix): ../../../BEFE-Core/various/BuildDisplay.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/various_BuildDisplay.cpp$(PreprocessSuffix) "../../../BEFE-Core/various/BuildDisplay.cpp"

$(IntermediateDirectory)/various_BuildFiles.cpp$(ObjectSuffix): ../../../BEFE-Core/various/BuildFiles.cpp $(IntermediateDirectory)/various_BuildFiles.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/various/BuildFiles.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/various_BuildFiles.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/various_BuildFiles.cpp$(DependSuffix): ../../../BEFE-Core/various/BuildFiles.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/various_BuildFiles.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/various_BuildFiles.cpp$(DependSuffix) -MM "../../../BEFE-Core/various/BuildFiles.cpp"

$(IntermediateDirectory)/various_BuildFiles.cpp$(PreprocessSuffix): ../../../BEFE-Core/various/BuildFiles.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/various_BuildFiles.cpp$(PreprocessSuffix) "../../../BEFE-Core/various/BuildFiles.cpp"

$(IntermediateDirectory)/various_BuildMisc.cpp$(ObjectSuffix): ../../../BEFE-Core/various/BuildMisc.cpp $(IntermediateDirectory)/various_BuildMisc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/various/BuildMisc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/various_BuildMisc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/various_BuildMisc.cpp$(DependSuffix): ../../../BEFE-Core/various/BuildMisc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/various_BuildMisc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/various_BuildMisc.cpp$(DependSuffix) -MM "../../../BEFE-Core/various/BuildMisc.cpp"

$(IntermediateDirectory)/various_BuildMisc.cpp$(PreprocessSuffix): ../../../BEFE-Core/various/BuildMisc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/various_BuildMisc.cpp$(PreprocessSuffix) "../../../BEFE-Core/various/BuildMisc.cpp"

$(IntermediateDirectory)/various_BuildSettings.cpp$(ObjectSuffix): ../../../BEFE-Core/various/BuildSettings.cpp $(IntermediateDirectory)/various_BuildSettings.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/various/BuildSettings.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/various_BuildSettings.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/various_BuildSettings.cpp$(DependSuffix): ../../../BEFE-Core/various/BuildSettings.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/various_BuildSettings.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/various_BuildSettings.cpp$(DependSuffix) -MM "../../../BEFE-Core/various/BuildSettings.cpp"

$(IntermediateDirectory)/various_BuildSettings.cpp$(PreprocessSuffix): ../../../BEFE-Core/various/BuildSettings.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/various_BuildSettings.cpp$(PreprocessSuffix) "../../../BEFE-Core/various/BuildSettings.cpp"

$(IntermediateDirectory)/various_BuildZip.cpp$(ObjectSuffix): ../../../BEFE-Core/various/BuildZip.cpp $(IntermediateDirectory)/various_BuildZip.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/various/BuildZip.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/various_BuildZip.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/various_BuildZip.cpp$(DependSuffix): ../../../BEFE-Core/various/BuildZip.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/various_BuildZip.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/various_BuildZip.cpp$(DependSuffix) -MM "../../../BEFE-Core/various/BuildZip.cpp"

$(IntermediateDirectory)/various_BuildZip.cpp$(PreprocessSuffix): ../../../BEFE-Core/various/BuildZip.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/various_BuildZip.cpp$(PreprocessSuffix) "../../../BEFE-Core/various/BuildZip.cpp"

$(IntermediateDirectory)/various_Dictionary.cpp$(ObjectSuffix): ../../../BEFE-Core/various/Dictionary.cpp $(IntermediateDirectory)/various_Dictionary.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/various/Dictionary.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/various_Dictionary.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/various_Dictionary.cpp$(DependSuffix): ../../../BEFE-Core/various/Dictionary.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/various_Dictionary.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/various_Dictionary.cpp$(DependSuffix) -MM "../../../BEFE-Core/various/Dictionary.cpp"

$(IntermediateDirectory)/various_Dictionary.cpp$(PreprocessSuffix): ../../../BEFE-Core/various/Dictionary.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/various_Dictionary.cpp$(PreprocessSuffix) "../../../BEFE-Core/various/Dictionary.cpp"

$(IntermediateDirectory)/win32__Iphlpapi.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/_Iphlpapi.cpp $(IntermediateDirectory)/win32__Iphlpapi.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/_Iphlpapi.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32__Iphlpapi.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32__Iphlpapi.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/_Iphlpapi.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32__Iphlpapi.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32__Iphlpapi.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/_Iphlpapi.cpp"

$(IntermediateDirectory)/win32__Iphlpapi.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/_Iphlpapi.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32__Iphlpapi.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/_Iphlpapi.cpp"

$(IntermediateDirectory)/win32__Kernel32.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/_Kernel32.cpp $(IntermediateDirectory)/win32__Kernel32.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/_Kernel32.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32__Kernel32.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32__Kernel32.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/_Kernel32.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32__Kernel32.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32__Kernel32.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/_Kernel32.cpp"

$(IntermediateDirectory)/win32__Kernel32.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/_Kernel32.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32__Kernel32.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/_Kernel32.cpp"

$(IntermediateDirectory)/win32__Ole32.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/_Ole32.cpp $(IntermediateDirectory)/win32__Ole32.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/_Ole32.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32__Ole32.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32__Ole32.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/_Ole32.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32__Ole32.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32__Ole32.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/_Ole32.cpp"

$(IntermediateDirectory)/win32__Ole32.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/_Ole32.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32__Ole32.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/_Ole32.cpp"

$(IntermediateDirectory)/win32__Psapi.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/_Psapi.cpp $(IntermediateDirectory)/win32__Psapi.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/_Psapi.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32__Psapi.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32__Psapi.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/_Psapi.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32__Psapi.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32__Psapi.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/_Psapi.cpp"

$(IntermediateDirectory)/win32__Psapi.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/_Psapi.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32__Psapi.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/_Psapi.cpp"

$(IntermediateDirectory)/win32_Win32.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/Win32.cpp $(IntermediateDirectory)/win32_Win32.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/Win32.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32_Win32.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32_Win32.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/Win32.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32_Win32.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32_Win32.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/Win32.cpp"

$(IntermediateDirectory)/win32_Win32.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/Win32.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32_Win32.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/Win32.cpp"

$(IntermediateDirectory)/win32_Win32Console.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/Win32Console.cpp $(IntermediateDirectory)/win32_Win32Console.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/Win32Console.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32_Win32Console.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32_Win32Console.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/Win32Console.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32_Win32Console.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32_Win32Console.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/Win32Console.cpp"

$(IntermediateDirectory)/win32_Win32Console.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/Win32Console.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32_Win32Console.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/Win32Console.cpp"

$(IntermediateDirectory)/win32_Win32File.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/Win32File.cpp $(IntermediateDirectory)/win32_Win32File.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/Win32File.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32_Win32File.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32_Win32File.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/Win32File.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32_Win32File.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32_Win32File.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/Win32File.cpp"

$(IntermediateDirectory)/win32_Win32File.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/Win32File.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32_Win32File.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/Win32File.cpp"

$(IntermediateDirectory)/win32_Win32Network.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/Win32Network.cpp $(IntermediateDirectory)/win32_Win32Network.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/Win32Network.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32_Win32Network.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32_Win32Network.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/Win32Network.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32_Win32Network.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32_Win32Network.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/Win32Network.cpp"

$(IntermediateDirectory)/win32_Win32Network.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/Win32Network.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32_Win32Network.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/Win32Network.cpp"

$(IntermediateDirectory)/win32_Win32OperatingSystem.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/Win32OperatingSystem.cpp $(IntermediateDirectory)/win32_Win32OperatingSystem.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/Win32OperatingSystem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32_Win32OperatingSystem.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32_Win32OperatingSystem.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/Win32OperatingSystem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32_Win32OperatingSystem.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32_Win32OperatingSystem.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/Win32OperatingSystem.cpp"

$(IntermediateDirectory)/win32_Win32OperatingSystem.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/Win32OperatingSystem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32_Win32OperatingSystem.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/Win32OperatingSystem.cpp"

$(IntermediateDirectory)/win32_Win32Platform.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/Win32Platform.cpp $(IntermediateDirectory)/win32_Win32Platform.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/Win32Platform.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32_Win32Platform.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32_Win32Platform.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/Win32Platform.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32_Win32Platform.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32_Win32Platform.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/Win32Platform.cpp"

$(IntermediateDirectory)/win32_Win32Platform.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/Win32Platform.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32_Win32Platform.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/Win32Platform.cpp"

$(IntermediateDirectory)/win32_Win32Process.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/Win32Process.cpp $(IntermediateDirectory)/win32_Win32Process.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/Win32Process.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32_Win32Process.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32_Win32Process.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/Win32Process.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32_Win32Process.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32_Win32Process.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/Win32Process.cpp"

$(IntermediateDirectory)/win32_Win32Process.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/Win32Process.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32_Win32Process.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/Win32Process.cpp"

$(IntermediateDirectory)/win32_Win32Processor.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/Win32Processor.cpp $(IntermediateDirectory)/win32_Win32Processor.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/Win32Processor.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32_Win32Processor.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32_Win32Processor.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/Win32Processor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32_Win32Processor.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32_Win32Processor.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/Win32Processor.cpp"

$(IntermediateDirectory)/win32_Win32Processor.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/Win32Processor.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32_Win32Processor.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/Win32Processor.cpp"

$(IntermediateDirectory)/win32_win32reg.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/win32reg.cpp $(IntermediateDirectory)/win32_win32reg.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/win32reg.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32_win32reg.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32_win32reg.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/win32reg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32_win32reg.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32_win32reg.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/win32reg.cpp"

$(IntermediateDirectory)/win32_win32reg.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/win32reg.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32_win32reg.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/win32reg.cpp"

$(IntermediateDirectory)/win32_win32utils.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/win32utils.cpp $(IntermediateDirectory)/win32_win32utils.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/win32utils.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32_win32utils.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32_win32utils.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/win32utils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32_win32utils.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32_win32utils.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/win32utils.cpp"

$(IntermediateDirectory)/win32_win32utils.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/win32utils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32_win32utils.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/win32utils.cpp"

$(IntermediateDirectory)/win32_Win32WCHAR.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/Win32WCHAR.cpp $(IntermediateDirectory)/win32_Win32WCHAR.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/Win32WCHAR.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32_Win32WCHAR.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32_Win32WCHAR.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/Win32WCHAR.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32_Win32WCHAR.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32_Win32WCHAR.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/Win32WCHAR.cpp"

$(IntermediateDirectory)/win32_Win32WCHAR.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/Win32WCHAR.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32_Win32WCHAR.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/Win32WCHAR.cpp"

$(IntermediateDirectory)/win32_Win32xterm.cpp$(ObjectSuffix): ../../../BEFE-Core/platform/win32/Win32xterm.cpp $(IntermediateDirectory)/win32_Win32xterm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "C:/SVN/BEFE-Core/platform/win32/Win32xterm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/win32_Win32xterm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/win32_Win32xterm.cpp$(DependSuffix): ../../../BEFE-Core/platform/win32/Win32xterm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/win32_Win32xterm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/win32_Win32xterm.cpp$(DependSuffix) -MM "../../../BEFE-Core/platform/win32/Win32xterm.cpp"

$(IntermediateDirectory)/win32_Win32xterm.cpp$(PreprocessSuffix): ../../../BEFE-Core/platform/win32/Win32xterm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/win32_Win32xterm.cpp$(PreprocessSuffix) "../../../BEFE-Core/platform/win32/Win32xterm.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) ./Debug/*$(ObjectSuffix)
	$(RM) ./Debug/*$(DependSuffix)
	$(RM) $(OutputFile)
	$(RM) $(OutputFile).exe
	$(RM) "../.build-debug/BEFE_Dev"


