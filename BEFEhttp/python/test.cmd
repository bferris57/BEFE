@echo off
cls
rem python BNF.py ../befe.bnf/BEFE.bnf
rem python BEFE_FileWords.py ../befe.befe/BEFE.befe
rem python BEFE_WordList.py c:/SVN/befe.befe/befe.py
rem python BEFE_Tree.py
rem python BEFE_ParseTree.py
rem python BEFE_Namespace.py ../befe.befe/BEFE.befe
rem python BEFE_VarTerm.py
rem python BEFE_MinExpDict.py
rem python t.py
rem python33 BEFE_MinExp.py n
rem befe +vv test minexp;x
rem befe +vv test trythis;x
rem befe +vvv lang;curlang .befe;lex c:/svn/befe.befe/BEFE.primitives.befe;x
rem befe +v lang;curlang .befe;lex t2.befe;x
rem befe lang;curlang .befe;lex c:/svn/testfiles/befe.compiler.befe;x
rem python33 sportr.py <sportrin.txt

rem befe +pyout;lang;curlang .befe;cd "../befe.befe";lex "BEFE.primitives.befe";x >__dict.py
rem python BEFE_WordList.py __dict.py

echo "Creating __dict.py..."
rem befe +vv;lang;lex +pyout c:/SVN/befe.befe/BEFE.primitives.befe;x >__dict.py
befe lang;lex +pyout c:/SVN/python/t.befe;x >__dict.py
rem efe +vv;lang;lex +pyout c:/SVN/python/BEFE_Word.py;x >__dict.py
rem befe +vv;lang;lex +pyout c:\svn\befe-core\_home\BEFE_ErrorMessages.bmsg;x >__dict.py
echo "Testing BEFE_Parse.py..."
python BEFE_Parse.py __dict.py

:END
