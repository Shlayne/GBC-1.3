call vendor\premake\premake5.exe vs2022
call vendor\premake\GBCFindAppend.exe GBC\GBC.vcxproj "</LanguageStandard>" "<UseStandardPreprocessor>true</UseStandardPreprocessor>"
call vendor\premake\GBCFindAppend.exe GBC\GBC.vcxproj "<UseStandardPreprocessor>" true
call vendor\premake\GBCFindAppend.exe Editor\Editor.vcxproj "</LanguageStandard>" "<UseStandardPreprocessor>true</UseStandardPreprocessor>"
call vendor\premake\GBCFindAppend.exe Editor\Editor.vcxproj "<UseStandardPreprocessor>" true
pause
