; Script generated by the HM NIS Edit Script Wizard.

; HM NIS Edit Wizard helper defines
!define PRODUCT_NAME "MADARA"
!define PRODUCT_PUBLISHER "James Edmondson"
!define PRODUCT_WEB_SITE "http://madara.googlecode.com"
!define PRODUCT_DIR_REGKEY "Software\Microsoft\Windows\CurrentVersion\App Paths\kats_batch.exe"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"
!define PRODUCT_UNINST_ROOT_KEY "HKLM"
!define env_hklm 'HKLM "SYSTEM\CurrentControlSet\Control\Session Manager\Environment"'
!define env_hkcu 'HKCU "Environment"'

; MUI 1.67 compatible ------
!include "MUI.nsh"
!include "winmessages.nsh"
!include "WordFunc.nsh"

; MUI Settings
!define MUI_ABORTWARNING
!define MUI_ICON "${NSISDIR}\Contrib\Graphics\Icons\modern-install.ico"
!define MUI_UNICON "${NSISDIR}\Contrib\Graphics\Icons\modern-uninstall.ico"

; Welcome page
!insertmacro MUI_PAGE_WELCOME
; License page
!insertmacro MUI_PAGE_LICENSE "..\..\LICENSE.txt"
; Components page
!insertmacro MUI_PAGE_COMPONENTS
; Directory page
!insertmacro MUI_PAGE_DIRECTORY
; Instfiles page
!insertmacro MUI_PAGE_INSTFILES
; Finish page
!define MUI_FINISHPAGE_SHOWREADME "$INSTDIR\README.txt"
!insertmacro MUI_PAGE_FINISH

; Uninstaller pages
!insertmacro MUI_UNPAGE_INSTFILES

; Language files
!insertmacro MUI_LANGUAGE "English"
!insertmacro WordAdd
!insertmacro un.WordAdd
; MUI end ------

!system "get_version.exe"
!include "VERSION.txt"

Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "madara_${PRODUCT_VERSION}.exe"
InstallDir "C:\MADARA"
InstallDirRegKey HKLM "${PRODUCT_DIR_REGKEY}" ""
ShowInstDetails hide
ShowUnInstDetails hide

Section "debug-libs" SEC01
  SetOutPath "$INSTDIR\lib"
  SetOverwrite ifnewer
  File "..\..\lib\KATSd.dll"
  File "..\..\lib\MADARAd.dll"
  File "..\..\lib\KATSd.lib"
  File "..\..\lib\MADARAd.lib"
SectionEnd

Section "-release-libs" SEC02
  File "..\..\lib\KATS.dll"
  File "..\..\lib\MADARA.dll"
  File "..\..\lib\KATS.lib"
  File "..\..\lib\MADARA.lib"
  File "..\..\lib\maal.py"
  File "..\..\lib\maml.py"
SectionEnd

Section "docs" SEC03
  SetOutPath "$INSTDIR\docs"
  File "..\..\docs\Doxyfile_KaRL"
  File "..\..\docs\Doxyfile_KATS"
  File "..\..\docs\Doxyfile_MAML"
  File "..\..\docs\Doxyfile_MAAL"
  File "..\..\docs\Doxyfile_CID"
  File "..\..\docs\README.txt"

  SetOutPath "$INSTDIR\docs\cid"
  File /r ..\..\docs\cid\html\*.*

  SetOutPath "$INSTDIR\docs\kats"
  File /r ..\..\docs\kats\html\*.*

  SetOutPath "$INSTDIR\docs\karl"
  File /r ..\..\docs\karl\html\*.*
  
  SetOutPath "$INSTDIR\docs\maml"
  File /r ..\..\docs\maml\html\*.*

  SetOutPath "$INSTDIR\docs\maal"
  File /r ..\..\docs\maal\html\*.*

  CreateDirectory "$SMPROGRAMS\MADARA"
  CreateShortCut "$SMPROGRAMS\MADARA\README.lnk" "$INSTDIR\docs\README.txt"
SectionEnd

Section "tests" SEC04
  SetOutPath "$INSTDIR\bin"
  File "..\..\bin\test_basic_reasoning.exe"
  File "..\..\bin\test_cid.exe"
  File "..\..\bin\test_cid_disjoint.exe"
  File "..\..\bin\test_cid_linked.exe"
  File "..\..\bin\test_cid_read_deployment.exe"
  File "..\..\bin\test_dissemination.exe"
  File "..\..\bin\test_distributed_finisher.exe"
  File "..\..\bin\test_distributed_finisher_with_set.exe"
  File "..\..\bin\test_files.exe"
  File "..\..\bin\test_kats_barriers.exe"
  File "..\..\bin\test_kats_syncs.exe"
  File "..\..\bin\test_key_expansion.exe"
  File "..\..\bin\test_knowledge_domains.exe"
  File "..\..\bin\test_latency.exe"
  File "..\..\bin\test_print_statement.exe"
  File "..\..\bin\test_quality.exe"
  File "..\..\bin\test_reasoning_throughput.exe"
  File "..\..\bin\test_synchronization.exe"
  File "..\..\bin\test_synchronization_three_state.exe"
  File "..\..\bin\test_timed_wait.exe"
  File "..\..\bin\test_wait_with_ace.exe"
SectionEnd

Section "-exes" SEC09
  SetOutPath "$INSTDIR\bin"
  File "..\..\bin\kats_batch.exe"
  File "..\..\bin\kats_observer.exe"
  File "..\..\bin\kats_process.exe"
  File "..\..\bin\kats_sleep.exe"
SectionEnd

Section "gme" SEC05
  SetOutPath "$INSTDIR\gme"
  File "..\..\modeling\KATS.mta"
  File "..\..\modeling\KATS.xmp"
  File "..\..\modeling\interpreters\GenerateXML\Release\GenerateXML.dll"
  SetOutPath "$INSTDIR\gme\Icons"
  File "..\..\modeling\Icons\barrier.gif"
  File "..\..\modeling\Icons\Host.gif"
  File "..\..\modeling\Icons\kill.gif"
  File "..\..\modeling\Icons\binoculars.jpg"
  File "..\..\modeling\Icons\blue_play.jpg"
  File "..\..\modeling\Icons\blue_round_play_button_4180.jpg"
  File "..\..\modeling\Icons\buttongroup.jpg"
  File "..\..\modeling\Icons\domain.jpg"
  File "..\..\modeling\Icons\host.jpg"
  File "..\..\modeling\Icons\sleep.jpg"
  File "..\..\modeling\Icons\XMLGenerator.png"
  File "..\..\modeling\Icons\sources.txt"
SectionEnd

Section "configs" SEC06
  SetOutPath "$INSTDIR\configs\cid\deployments\test_cid"
  File "..\..\configs\cid\deployments\test_cid\1_even_fans_disjoint.template"
  File "..\..\configs\cid\deployments\test_cid\2_even_fans_disjoint.template"
  File "..\..\configs\cid\deployments\test_cid\3_deep_tree.template"
  File "..\..\configs\cid\deployments\test_cid\3_even_fans_disjoint.template"
  File "..\..\configs\cid\deployments\test_cid\3waytree.template"
  File "..\..\configs\cid\deployments\test_cid\3waytree_strict.template"
  File "..\..\configs\cid\deployments\test_cid\4_even_fans_disjoint.template"
  File "..\..\configs\cid\deployments\test_cid\mwaytree.template"
  File "..\..\configs\cid\deployments\test_cid\specialized.template"
  SetOutPath "$INSTDIR\configs\kats"
  File "..\..\configs\kats\FourSleeps.xml"
  File "..\..\configs\kats\Generics.xml"
  File "..\..\configs\kats\ospl_start.xml"
  File "..\..\configs\kats\Timeout.xml"
  File "..\..\configs\kats\TwoSleeps.xml"
  File /r "..\..\configs\kats\transports"
SectionEnd

Section "ace" SEC10

  SetOutPath "$INSTDIR"
  
  File /oname=$INSTDIR\ACE-LICENSE.txt "$%ACE_ROOT%\COPYING"
  CreateShortCut "$SMPROGRAMS\MADARA\ACE-LICENSE.lnk" "$INSTDIR\ACE-LICENSE.txt"

  SetOutPath "$INSTDIR\lib"
  File "$%ACE_ROOT%\lib\ACE.dll"
  File "$%ACE_ROOT%\lib\ACEd.dll"
  File "$%ACE_ROOT%\lib\ACE.lib"
  File "$%ACE_ROOT%\lib\ACEd.lib"
  
  SetOutPath "$INSTDIR\include"

  File /r $%ACE_ROOT%\ace

SectionEnd

Section "opensplice" SEC11

  SetOutPath "$INSTDIR"

  File /oname=$INSTDIR\SPLICE-LICENSE.txt "$%OSPL_HOME%\LICENSE"
  CreateShortCut "$SMPROGRAMS\MADARA\SPLICE-LICENSE.lnk" "$INSTDIR\SPLICE-LICENSE.txt"

  SetOutPath "$INSTDIR\lib"

  ; copy OSPL dlls
  File "$%OSPL_HOME%\lib\cmxml.dll"
  File "$%OSPL_HOME%\lib\commonserv.dll"
  File "$%OSPL_HOME%\lib\dcpsccpp.dll"
  File "$%OSPL_HOME%\lib\dcpsgapi.dll"
  File "$%OSPL_HOME%\lib\dcpsjni.dll"
  File "$%OSPL_HOME%\lib\dcpssac.dll"
  File "$%OSPL_HOME%\lib\dcpssacpp.dll"
  File "$%OSPL_HOME%\lib\dcpssaj.dll"
  File "$%OSPL_HOME%\lib\ddsconf.dll"
  File "$%OSPL_HOME%\lib\ddsconfparser.dll"
  File "$%OSPL_HOME%\lib\ddsdatabase.dll"
  File "$%OSPL_HOME%\lib\ddskernel.dll"
  File "$%OSPL_HOME%\lib\ddsos.dll"
  File "$%OSPL_HOME%\lib\ddsosnet.dll"
  File "$%OSPL_HOME%\lib\ddsserialization.dll"
  File "$%OSPL_HOME%\lib\ddsuser.dll"
  File "$%OSPL_HOME%\lib\ddsutil.dll"

  ; copy OSPL libs
  File "$%OSPL_HOME%\lib\cmxml.lib"
  File "$%OSPL_HOME%\lib\commonserv.lib"
  File "$%OSPL_HOME%\lib\dcpsccpp.lib"
  File "$%OSPL_HOME%\lib\dcpsgapi.lib"
  File "$%OSPL_HOME%\lib\dcpsjni.lib"
  File "$%OSPL_HOME%\lib\dcpssac.lib"
  File "$%OSPL_HOME%\lib\dcpssacpp.lib"
  File "$%OSPL_HOME%\lib\dcpssaj.lib"
  File "$%OSPL_HOME%\lib\ddsconf.lib"
  File "$%OSPL_HOME%\lib\ddsconfparser.lib"
  File "$%OSPL_HOME%\lib\ddsdatabase.lib"
  File "$%OSPL_HOME%\lib\ddskernel.lib"
  File "$%OSPL_HOME%\lib\ddsos.lib"
  File "$%OSPL_HOME%\lib\ddsosnet.lib"
  File "$%OSPL_HOME%\lib\ddsserialization.lib"
  File "$%OSPL_HOME%\lib\ddsuser.lib"
  File "$%OSPL_HOME%\lib\ddsutil.lib"
  
  ; copy OSPL bin

  SetOutPath "$INSTDIR\bin"

  File "$%OSPL_HOME%\bin\cppgen.exe"
  File "$%OSPL_HOME%\bin\ddsi.exe"
  File "$%OSPL_HOME%\bin\durability.exe"
  File "$%OSPL_HOME%\bin\idlpp.exe"
  File "$%OSPL_HOME%\bin\mmstat.exe"
  File "$%OSPL_HOME%\bin\networking.exe"
  File "$%OSPL_HOME%\bin\ospl.exe"
  File "$%OSPL_HOME%\bin\shmdump.exe"
  File "$%OSPL_HOME%\bin\spliced.exe"
  
  SetOutPath "$INSTDIR\include"
  File /r $%OSPL_HOME%\include\*.*
  
SectionEnd

Section "-include" SEC07
  SetOutPath "$INSTDIR\include\madara"
  File "..\..\include\madara\MADARA_export.h"
  File "..\..\include\madara\Thread_Pool.h"
  File "..\..\include\madara\Thread_Pool.cpp"
  
  ; copy the madara directories
  File /r "..\..\include\madara\utility"
  File /r "..\..\include\madara\transport"
  File /r "..\..\include\madara\knowledge_engine"
  File /r "..\..\include\madara\kats"
  File /r "..\..\include\madara\expression_tree"
  File /r "..\..\include\madara\cid"
  File /r "..\..\include\madara\maml"
  File /r "..\..\include\madara\maal"
  
SectionEnd

Section "-tests"
  SetOutPath "$INSTDIR"
  File /r "..\..\tests"
SectionEnd

Section "-basic" SEC08
  SetOutPath "$INSTDIR"
  File "..\..\VERSION.txt"
  CreateShortCut "$SMPROGRAMS\MADARA\VERSION.lnk" "$INSTDIR\VERSION.txt"
  File "..\..\LICENSE.txt"
  CreateShortCut "$SMPROGRAMS\MADARA\LICENSE.lnk" "$INSTDIR\LICENSE.txt"
  File "..\..\README.txt"
  CreateShortCut "$SMPROGRAMS\MADARA\README.lnk" "$INSTDIR\README.txt"
SectionEnd

Section -AdditionalIcons
  WriteIniStr "$INSTDIR\${PRODUCT_NAME}.url" "InternetShortcut" "URL" "${PRODUCT_WEB_SITE}"
  CreateShortCut "$SMPROGRAMS\MADARA\Website.lnk" "$INSTDIR\${PRODUCT_NAME}.url"
  CreateShortCut "$SMPROGRAMS\MADARA\Uninstall.lnk" "$INSTDIR\uninst.exe"
SectionEnd

Section -Post
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_DIR_REGKEY}" "" "$INSTDIR\bin\kats_batch.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayName" "$(^Name)"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\bin\kats_batch.exe"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "Publisher" "${PRODUCT_PUBLISHER}"

  WriteRegStr ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "OldMadaraRoot" "$%MADARA_ROOT%"

; set environment variables
   ; include for some of the windows messages defines
   ; HKLM (all users) vs HKCU (current user) defines
   ; set variable
   WriteRegExpandStr ${env_hklm} MADARA_ROOT $INSTDIR
   
   ; read the path variable
   ReadRegStr $1 ${env_hklm} PATH

   ; remove references to MADARA in the PATH variable
   ${WordAdd} $1 ";" "+%MADARA_ROOT%\lib;%MADARA_ROOT%\bin" $2
   
   ; now add them back
   WriteRegExpandStr ${env_hklm} PATH $2

   ; make sure windows knows about the change
   SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000

SectionEnd

; Section descriptions
!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC01} "MADARA libraries compiled in debug mode (optional)"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC02} "Should always be included"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC03} "Help files and API documentation"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC04} "Tests for KaRL, KATS, etc."
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC05} "GME paradigm and interpreters for KATS"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC06} "Example configuration files"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC07} "Include files for MADARA projects"
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC08} "Version, License, etc."
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC09} "MADARA exes (KATS)."
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC10} "Install ACE v6.0.1 (required if not already installed)."
  !insertmacro MUI_DESCRIPTION_TEXT ${SEC11} "Install OpenSplice v5.4.1 (required if not already installed)."
!insertmacro MUI_FUNCTION_DESCRIPTION_END


Function un.onUninstSuccess
  HideWindow
  MessageBox MB_ICONINFORMATION|MB_OK "MADARA was successfully removed from your computer."
FunctionEnd

Function un.onInit
  MessageBox MB_ICONQUESTION|MB_YESNO|MB_DEFBUTTON2 "Are you sure you want to completely remove MADARA and all of its components?" IDYES +2
  Abort
FunctionEnd

Section Uninstall
  Delete "$INSTDIR\${PRODUCT_NAME}.url"
  Delete "$INSTDIR\uninst.exe"
  Delete "$INSTDIR\LICENSE.txt"
  Delete "$INSTDIR\VERSION.txt"
  Delete "$INSTDIR\include\madara\transport\ndds\Ndds_Knowledge_Update.idl"
  Delete "$INSTDIR\include\madara\transport\ndds\Ndds_Knowledge_UpdateSupport.cxx"
  Delete "$INSTDIR\include\madara\transport\ndds\Ndds_Knowledge_UpdatePlugin.cxx"
  Delete "$INSTDIR\include\madara\transport\ndds\Ndds_Knowledge_Update.cxx"
  Delete "$INSTDIR\include\madara\transport\ndds\NDDS_Transport_Read_Thread.cpp"
  Delete "$INSTDIR\include\madara\transport\ndds\NDDS_Transport.cpp"
  Delete "$INSTDIR\include\madara\transport\ndds\NDDS_Listener.cpp"
  Delete "$INSTDIR\include\madara\transport\ndds\NDDS_Transport_Read_Thread.h"
  Delete "$INSTDIR\include\madara\transport\ndds\NDDS_Transport_export.h"
  Delete "$INSTDIR\include\madara\transport\ndds\NDDS_Transport.h"
  Delete "$INSTDIR\include\madara\transport\ndds\NDDS_Listener.h"
  Delete "$INSTDIR\include\madara\transport\ndds\Ndds_Knowledge_UpdateSupport.h"
  Delete "$INSTDIR\include\madara\transport\ndds\Ndds_Knowledge_UpdatePlugin.h"
  Delete "$INSTDIR\include\madara\transport\ndds\Ndds_Knowledge_Update.h"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Mutex_Message.idl"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Knowledge_Update.idl"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Transport_Read_Thread.cpp"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Subscriber_Listener.cpp"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Knowledge_UpdateSplDcps.cpp"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Knowledge_UpdateDcps_impl.cpp"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Knowledge_UpdateDcps.cpp"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Knowledge_Update.cpp"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_DDS_Transport.cpp"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_DataReader_Listener.cpp"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Transport_Read_Thread.h"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Subscriber_Listener.h"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Mutex_export.h"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Knowledge_UpdateSplDcps.h"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Knowledge_UpdateDcps_impl.h"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Knowledge_UpdateDcps.h"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_Knowledge_Update.h"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_DDS_Transport.h"
  Delete "$INSTDIR\include\madara\transport\splice\Splice_DataReader_Listener.h"
  Delete "$INSTDIR\include\madara\transport\splice\OSPL_Transport_export.h"
  Delete "$INSTDIR\include\madara\transport\splice\ccpp_Splice_Knowledge_Update.h"
  Delete "$INSTDIR\include\madara\transport\tcp\TCP_Transport_Read_Thread.cpp"
  Delete "$INSTDIR\include\madara\transport\tcp\TCP_Transport.cpp"
  Delete "$INSTDIR\include\madara\transport\tcp\TCP_Transport_Read_Thread.h"
  Delete "$INSTDIR\include\madara\transport\tcp\TCP_Transport.h"
  Delete "$INSTDIR\include\madara\transport\Transport.h"
  Delete "$INSTDIR\include\madara\utility\Utility.cpp"
  Delete "$INSTDIR\include\madara\utility\Refcounter.cpp"
  Delete "$INSTDIR\include\madara\utility\LStack.cpp"
  Delete "$INSTDIR\include\madara\utility\LQueue.cpp"
  Delete "$INSTDIR\include\madara\utility\Log_Macros.cpp"
  Delete "$INSTDIR\include\madara\utility\Utility.h"
  Delete "$INSTDIR\include\madara\utility\Refcounter.h"
  Delete "$INSTDIR\include\madara\utility\MADARA_Logger_export.h"
  Delete "$INSTDIR\include\madara\utility\LStack.h"
  Delete "$INSTDIR\include\madara\utility\LQueue.h"
  Delete "$INSTDIR\include\madara\utility\Log_Macros.h"
  Delete "$INSTDIR\include\madara\Thread_Pool.cpp"
  Delete "$INSTDIR\include\madara\Thread_Pool.h"
  Delete "$INSTDIR\include\madara\MADARA_export.h"
  Delete "$INSTDIR\include\madara\Globals.h"
  Delete "$INSTDIR\configs\xml\transports\Splice.xml"
  Delete "$INSTDIR\configs\xml\transports\NoTransport.xml"
  Delete "$INSTDIR\configs\xml\transports\NDDS.xml"
  Delete "$INSTDIR\configs\xml\TwoSleeps.xml"
  Delete "$INSTDIR\configs\xml\Timeout.xml"
  Delete "$INSTDIR\configs\xml\ospl_start.xml"
  Delete "$INSTDIR\configs\xml\Generics.xml"
  Delete "$INSTDIR\configs\xml\FourSleeps.xml"
  Delete "$INSTDIR\configs\cid\deployments\test_cid\specialized.template"
  Delete "$INSTDIR\configs\cid\deployments\test_cid\mwaytree.template"
  Delete "$INSTDIR\configs\cid\deployments\test_cid\4_even_fans_disjoint.template"
  Delete "$INSTDIR\configs\cid\deployments\test_cid\3waytree_strict.template"
  Delete "$INSTDIR\configs\cid\deployments\test_cid\3waytree.template"
  Delete "$INSTDIR\configs\cid\deployments\test_cid\3_even_fans_disjoint.template"
  Delete "$INSTDIR\configs\cid\deployments\test_cid\3_deep_tree.template"
  Delete "$INSTDIR\configs\cid\deployments\test_cid\2_even_fans_disjoint.template"
  Delete "$INSTDIR\configs\cid\deployments\test_cid\1_even_fans_disjoint.template"
  Delete "$INSTDIR\gme\Icons\sources.txt"
  Delete "$INSTDIR\gme\Icons\XMLGenerator.png"
  Delete "$INSTDIR\gme\Icons\sleep.jpg"
  Delete "$INSTDIR\gme\Icons\host.jpg"
  Delete "$INSTDIR\gme\Icons\domain.jpg"
  Delete "$INSTDIR\gme\Icons\buttongroup.jpg"
  Delete "$INSTDIR\gme\Icons\blue_round_play_button_4180.jpg"
  Delete "$INSTDIR\gme\Icons\blue_play.jpg"
  Delete "$INSTDIR\gme\Icons\binoculars.jpg"
  Delete "$INSTDIR\gme\Icons\kill.gif"
  Delete "$INSTDIR\gme\Icons\Host.gif"
  Delete "$INSTDIR\gme\Icons\barrier.gif"
  Delete "$INSTDIR\gme\GenerateXML.dll"
  Delete "$INSTDIR\gme\KATS.xmp"
  Delete "$INSTDIR\gme\KATS.mta"
  Delete "$INSTDIR\bin\test_wait_with_ace.exe"
  Delete "$INSTDIR\bin\test_timed_wait.exe"
  Delete "$INSTDIR\bin\test_synchronization_three_state.exe"
  Delete "$INSTDIR\bin\test_synchronization.exe"
  Delete "$INSTDIR\bin\test_reasoning_throughput.exe"
  Delete "$INSTDIR\bin\test_quality.exe"
  Delete "$INSTDIR\bin\test_print_statement.exe"
  Delete "$INSTDIR\bin\test_latency.exe"
  Delete "$INSTDIR\bin\test_knowledge_domains.exe"
  Delete "$INSTDIR\bin\test_key_expansion.exe"
  Delete "$INSTDIR\bin\test_kats_syncs.exe"
  Delete "$INSTDIR\bin\test_kats_barriers.exe"
  Delete "$INSTDIR\bin\test_files.exe"
  Delete "$INSTDIR\bin\test_distributed_finisher_with_set.exe"
  Delete "$INSTDIR\bin\test_distributed_finisher.exe"
  Delete "$INSTDIR\bin\test_dissemination.exe"
  Delete "$INSTDIR\bin\test_cid_read_deployment.exe"
  Delete "$INSTDIR\bin\test_cid_linked.exe"
  Delete "$INSTDIR\bin\test_cid_disjoint.exe"
  Delete "$INSTDIR\bin\test_cid.exe"
  Delete "$INSTDIR\bin\test_basic_reasoning.exe"
  Delete "$INSTDIR\bin\kats_sleep.exe"
  Delete "$INSTDIR\bin\kats_process.exe"
  Delete "$INSTDIR\bin\kats_observer.exe"
  Delete "$INSTDIR\bin\kats_batch.exe"
  Delete "$INSTDIR\docs\README.txt"
  Delete "$INSTDIR\docs\Doxyfile_MAML"
  Delete "$INSTDIR\docs\Doxyfile_KATS"
  Delete "$INSTDIR\docs\Doxyfile_KaRL"
  Delete "$INSTDIR\lib\maml.py"
  Delete "$INSTDIR\lib\maal.py"
  Delete "$INSTDIR\lib\Madara.lib"
  Delete "$INSTDIR\lib\Kats.lib"
  Delete "$INSTDIR\lib\Madara.dll"
  Delete "$INSTDIR\lib\Kats.dll"
  Delete "$INSTDIR\lib\maml.py"
  Delete "$INSTDIR\lib\maal.py"
  Delete "$INSTDIR\lib\Madarad.lib"
  Delete "$INSTDIR\lib\Katsd.lib"
  Delete "$INSTDIR\lib\Madarad.dll"
  Delete "$INSTDIR\lib\Katsd.dll"

  Delete "$SMPROGRAMS\MADARA\Uninstall.lnk"
  Delete "$SMPROGRAMS\MADARA\Website.lnk"
  Delete "$SMPROGRAMS\MADARA\LICENSE.lnk"
  Delete "$SMPROGRAMS\MADARA\VERSION.lnk"
  Delete "$SMPROGRAMS\MADARA\README.lnk"

  RMDir "$SMPROGRAMS\MADARA"
  RMDir /r "$INSTDIR\lib"
  RMDir /r "$INSTDIR\include"
  RMDir /r "$INSTDIR\gme"
  RMDir /r "$INSTDIR\docs"
  RMDir /r "$INSTDIR\configs"
  RMDir /r "$INSTDIR\modeling"
  RMDir /r "$INSTDIR\tests"
  RMDir /r "$INSTDIR\bin"
  RMDir /r "$INSTDIR"

  ReadRegStr $0 ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}" "OldMadaraRoot"

  ;ReadRegStr $0 HKLM "SOFTWARE\Microsoft" "Bob"

  StrCmp $0 '' 0 revert
   ; delete variable
   DeleteRegValue ${env_hklm} MADARA_ROOT
   ReadRegStr $1 ${env_hklm} PATH
   
   ; remove references to MADARA in the PATH variable
   ${WordAdd} $1 ";" "-%MADARA_ROOT%\lib;%MADARA_ROOT%\bin" $2

   WriteRegExpandStr ${env_hklm} PATH $2
   
   Goto done
   ; revert to the old MADARA variable
   revert:
   WriteRegExpandStr ${env_hklm} MADARA_ROOT $0
   ; make sure windows knows about the change

   SendMessage ${HWND_BROADCAST} ${WM_WININICHANGE} 0 "STR:Environment" /TIMEOUT=5000
   done:
  DeleteRegKey ${PRODUCT_UNINST_ROOT_KEY} "${PRODUCT_UNINST_KEY}"
  DeleteRegKey HKLM "${PRODUCT_DIR_REGKEY}"
  SetAutoClose true
SectionEnd