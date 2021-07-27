@echo off
cls
echo **********************************************
echo Making TestMain.java...

pushd C:\SVN\java

rm *.class 2>nul
rm BNF/*.class 2>nul
rm BEFE/*.class 2>nul

javac TestMain.java
rem javac TestMain.java 2>errors.txt
rem notepad++ errors.txt
rem javac BEFE/SyntaxError.java
rem javac BEFE/Span.java
rem javac BNF/FileLines.java
rem javac BNF/FileWords.java
rem javac BNF/Word.java
rem javac BNF/Rule.java
rem javac BNF/utils.java

popd