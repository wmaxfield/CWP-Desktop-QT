#-------------------------------------------------
#
# Project created by QtCreator 2014-01-26T17:07:17
#  Qt5.x is the target
#/* ========================================
# *
# * Copyright Wade Maxfield, 2010-2020
# * Commercial license available
# *
# * This program is free software: you can redistribute it and/or modify
# * it under the terms of the GNU General Public License as published by
# * the Free Software Foundation, either version 1 of the License, or
# * (at your option) any later version.
# *
# * This program is distributed in the hope that it will be useful,
# * but WITHOUT ANY WARRANTY; without even the implied warranty of
# * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# * GNU General Public License for more details.
# *
# * You should have received a copy of the GNU General Public License
# * along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# * ========================================
#*/
#-------------------------------------------------
# CRASHES:
# Win32
#       If you use mt.exe to add manifest to CWP before debugging, then when debug process starts, gdborig.exe will crash, every time.
#       Solution: don't add manifest to executable when building debug version.
#
#-------------------------------------------------
# Build notes:
#  You will have to configure your project to get debug features at launch of CWP.
#  When you launch CWP with -debug command line option, it enables debug menus.
#  Build tested down to Qt 5.3.2 and up to Qt 5.14
#
# Windows: Need at least 1 sdk.  Microsoft Visual Studios is easiest to install.
#          You CAN build to local directory with source on remote server
#          even under shadow build and still debug!  (The one thing Windows got right)
#
# Mac: XCode is required to be installed. (It contains sdk).
#      You must follow shadow build rules for directory, which means the
#      directory shows up in your project folder and must be deleted before commits,
#      OR
#      you must set up your build directory to have the word "build" in it and set the
#      .gitignore file to have *build* on a line to make sure those are excluded
#
#      If you get an error "no file at "/opt/local/lib/mysql55/lib/libmysqlclient.18.dylib" THEN
#      locate libmysqlclient.18.dylib in your Qt Creator app and copy to "/opt/local/lib/mysql55/lib/"
#      directory that was referenced in error message. (may need to create it)
#      This is safe to do because libmysqlclient is not used, just asked for by the macdeployqt
#      but under certain versions of Qt it causes a build error
#
# Maximum Platform Deployment (Mac)
#      Later Mac/Qt Builds force some gaming library links that are not available on
#      10.8, 10.9, 10.10, etc.,
#      This requires you compile on an OSX verions *before* that library is required by Qt.
#
#      Build on VM containing at least OSX 10.10 in order to work on 10.8.5.
#      *MUST BUILD LOCALLY, NOT ON "shared" DRIVE. CLANG FAILS on shared drives*
#      THAT MEANS YOU CAN'T DEBUG ON OLDER MACS USING REMOTE SHARED DRIVE
#
#      I have been unable to get Qt version 5.6 to debug on Macintosh, so debugging is done on
#      Qt Version 5.3.2.
#
# Linux Built Note: Install QTCreator (Ubuntu: sudo apt-get install qtcreator)
#        See Macintosh Note on shadow build.
#        Very little time spent on Linux builds as main devl platform is Mac
#
#        LinuxDeployQt is included in helpers folder / Linux folder.
#        LinuxDeployQt is hard coded (by creators) to only run on Ubuntu 14
#        for now.  That may change in the future, keep a watch for that.
#        That hard coding allows running on multiple platforms from Ubuntu 14 forward.
#-------------------------------------------------
# QtWebkit history:
# tldr; could not use 3rd party library, fixed code for webkit compile for mingw (win), linux, mac
#
# Trying for only using webkit rather than webengine (was too much happening to migrate to webengine ),
# I picked up latest qwebkit from https://github.com/qtwebkit/qtwebkit/releases/tag/qtwebkit-tp5
# and hand installed into my C:\Qt\Qt5.12.5\5.12.5\mingw73_64 directory
# by moving files into matching directories.
#  compile  ok.
#
# QtWebKit DID NOT WORK.  (Not sure what I did wrong, but attempt failed.)
# So, adapted to qwebengine, BUT requires Visual Studio install to get msvc compiler for webengine
# Note: MINGW DOES NOT INCLUDE qwebengine!!!!! (but it does include qtwebkit)
#
# Updated: Code changed to allow either MinGW or MVSC compile, either webengine or webkit.
# webengine automatically selected for Qt version ABOVE 5.5, except linux
#-------------------------------------------------
#-------------------------------------------------
# QT CREATOR Windows:
# To get qtcreator larger font size for compile output, etc. use stylesheet. (google it)
# example startup for windows batch file or desktop launcher:
#C:\Qt\Tools\QtCreator\bin\qtcreator.exe -stylesheet C:\Qt\Tools\QtCreator\bin\qtcreatorstyle.css.txt
#-----
# qtcreatorstyle.css.txt contents:
#QWidget
#{
#font: 18pt "Courier New"; font-weight: bold;
#}
#
#TESTING
#
#-----------------------------------------------------------
# CWP Windows: Tested on XP, win 7, win 10.  With 1 or 2 CPU cores,
# navigation to exact verse can fail, fixed by repeating
# navigation calls.  After testing, can fail on
# older Macintosh, so made fix universal
#-----------------------------------------------------------
#-------------------------------------------------
# Linux Build Notes:
#
# Build on Ubuntu 14.  64 bit (for linuxdeployqt)
#
# linuxdeployqt:
#
# https://github.com/probonopd/linuxdeployqt
# binary found here
# https://github.com/probonopd/linuxdeployqt/releases
# I renamed it linuxdeployqt and copied to /bin (in system path)
# 
# Shadow build to home directory for release can be done, just make sure to soft link CWPfiles
# for the final packaging
# 
# WebKit: (qt 5.12 qwebengine not available on lubuntu)
# run the following install command:
# sudo apt-get install libqt5webkit5 libqt5webkit5-dev
#
#-------------------------------------------------------------------------------------
# All Builds
#-------------------------------------------------------------------------------------
# qHash Error, qt 5.12 .  need to change function declaration in .h file to following:
#Q_DECL_CONST_FUNCTION inline uint qHash(std::nullptr_t, uint seed = 0) Q_DECL_NOTHROW
#{
#    const void *ptr = nullptr; // work-around for MSVC's/CLANG's reinterpret_cast bug
#
#    return qHash(reinterpret_cast<quintptr>(ptr), seed);//<-- this line fixes errors
#   // return qHash(reinterpret_cast<quintptr>(nullptr), seed);<-- this line causes errors
#}
#

QT       += core gui sql

# not available in qmake of 5.9 era -->versionAtLeast(QT_VERSION,5.6.0) {
linux {
        CONFIG += webkit #webkit build
        message("qwebkit build")
} else {
    # windows Qt 5.5.1 includes webkit on mingw, not on msvc
    greaterThan(QT_MAJOR_VERSION,4):greaterThan(QT_MINOR_VERSION,5) {
        #  for webengine (Qt 5.6 and above)
        CONFIG += qwebengine
        message("qwebengine build")
    }
}
#-----------------------------------------------------------------------
# if qwebengine is defined, then include correct qt
# under linux, ubuntu, "sudo apt-get install qtwebengine5-dev"
# under windows, qtwebengine only available on msvc install (as of 06-2020)
# under Mac, qtwebengine install is an option in the install package
#-----------------------------------------------------------------------
qwebengine {
    message("QWebEngine build")
    QT       += webengine webenginewidgets
    DEFINES  += WEBENGINE_USED=1
}else{
    #-----------------------------------------------------------------------
    # otherwise it is webkit, so include correct qt libs
    #-----------------------------------------------------------------------
    message("QWebKit build")
    DEFINES  += WEBKIT_USED=1
    QT += webkit webkitwidgets
}
#----------------------------------------------------------
# the following also works, can be used for qmake variables
#QMAKE_CWP_TITLE='CWP Teacher Bible Study Program'
#DEFINES += 'CWP_TITLE_STRING=\'\"$${QMAKE_CWP_TITLE}\"\''
#----------------------------------------------------------
# have to use this format:  \'\"txt\"\' to avoid clang command line quote swallowing
DEFINES += 'CWP_TITLE_STRING=\'\"CWP Bible Study Program\"\''

DEFINES += 'CWP_VERSION_NUMBER=\'\"1.0.7\"\''
#win32 {
    VERSION = 1.0.7
    QMAKE_TARGET_COMPANY = Crossword Project
    QMAKE_TARGET_PRODUCT = CWP
    QMAKE_TARGET_DESCRIPTION = Bible Study Program
    QMAKE_TARGET_COPYRIGHT = Wade Maxfield, 2020

#}
#versionAtLeast(QT_VERSION,5.12.5) { <---not available in earlier versions of qmake, so can't use

linux {
    DEPLOYQT_BIN = $$system(which linuxdeployqt)
        isEmpty(DEPLOYQT_BIN){
            message("linuxdeployqt not found")
        } else {
            message("linuxdeployqt found!")
        }
}
#-----------------------------------------------------------------------
# if is release mode, set release defines and config
#-----------------------------------------------------------------------
CONFIG(release, debug|release) {# release
    message( "release: working directory-->" $$PWD )
    CONFIG += bundle_bibles
    CONFIG += force_rebuild_of_important_files
    # set release version to 1 to hide menus and other items
    DEFINES += "RELEASE_VERSION=1"
    DEFINES += "ENABLE_EDIT=1"
    DEFINES += "ENABLE_MENUS=0" # not sure, but is needed for now
    DEFINES += "ENABLE_SPEECH=0"  # no speech for now
    exe_dir=release
}
#-----------------------------------------------------------------------
# if is debug mode, set debug defines and config
#-----------------------------------------------------------------------
CONFIG(debug, debug|release) {
    # this is debug build
    message( "debug: working directory-->" $$PWD )
    # don't try bundle_bibles on linux during debug
    macx|win32 {
        CONFIG += bundle_bibles
    }
    #CONFIG += force_rebuild_of_important_files
    DEFINES += "RELEASE_VERSION=0"
    DEFINES += "ENABLE_EDIT=1"
    DEFINES += "ENABLE_SPEECH=0"  #no speech for now
    exe_dir=debug
    DEFINES += "ENABLE_MENUS=0" # not sure, but is needed for now
}

#####################################################################################################
# FORCE REBUILD FOR TIME AND DATE ON "About" DIALOG
#touch the settings selection screen .cpp file, which contains compile date and time, visible to user.
#####################################################################################################
force_rebuild_of_important_files {
    message("Updating about Dialog for time/date")
    extralib.target = extra
#----
macx {
#    message("Removing CWP.app so *exists* error will not occur")
#    /usr/bin/touch $$PWD/../dialogs/about/dialogabout.cpp $$escape_expand(\\n\\t)/usr/bin/touch $$OUT_PWD/$$TARGET"."$$TEMPLATE $$escape_expand(\\n\\t) /bin/rm -rf  $$OUT_PWD/$$TARGET"."$$TEMPLATE $$escape_expand(\\n\\t) \
    extralib.commands ="@echo '--------------------------';\
    echo 'forcing compile of dialogabout.cpp';$$escape_expand(\\n\\t) $$escape_expand(\\n\\t)\
    /usr/bin/touch $$PWD/../dialogs/about/dialogabout.cpp $$escape_expand(\\n\\t) \
    echo '--------------------------'"
}
#----
win32 {
   # the +,, is a windows for copy which essentially says "touch this file"
    cpcmd =  $$PWD/../dialogs/about/dialogabout.cpp +,,
    cpcmd ~= s,/,\\,g
    extralib.commands = copy /b $$cpcmd
}
    extralib.depends =

    QMAKE_EXTRA_TARGETS += extralib
    PRE_TARGETDEPS = extra
}
#####################################################################################################


#-----------------------------------------------------------------------
#-----------------------------------------------------------------------

#-----------------------------------------------------------------------
# include widgets if is 4.xx or greater
#-----------------------------------------------------------------------
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

#macx {
#   QMAKE_MAC_SDK = macosx10.12
#   QMAKE_MAC_SDK_PATH = "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk"
#

#-----------------------------------------------------------------------
# Always use sqlite
#-----------------------------------------------------------------------
QTPLUGIN += qsqlite

#-----------------------------------------------------------------------
# build CWP, and use APP
#-----------------------------------------------------------------------
TARGET = CWP
TEMPLATE = app

# macintosh
# MAC NOTE: the Shadow Build Directory MUST be
# at the same level as the project CWP directory,
# in order for the CWPIcon.icns file to be used
# and for the Info.Plist.app to be found by qmake.
ICON = icon/CWPIcon.icns

#----------------------------------------------------------------------------------------
# General Note:  The following causes shell command to be executed.  If not included, use ';'
# but at some point the following *must* be used
# $escape_expand(\n\t)
#----------------------------------------------------------------------------------------

#========================================================================================================
# Macintoish settings, and copy files after build section
#========================================================================================================
macx {

    # if bundle_bibles
    bundle_bibles {
        message("copying bible files to CWP destination")
        QMAKE_POST_LINK += "cp -rf $$PWD/../../CWPfiles $$OUT_PWD/$$TARGET"."$$TEMPLATE/Contents/Resources/"   $$escape_expand(\\n\\t)
    }# bundle bibles

    # remove unused plugins, which also includes some private API bundling
    message("removing mysql, odbc, psql plugins after execution of macdeployqt")

    greaterThan(QT_MAJOR_VERSION,4):greaterThan(QT_MINOR_VERSION,5) {
        QMAKE_POST_LINK += "macdeployqt "$$TARGET"."$$TEMPLATE" -appstore-compliant " $$escape_expand(\\n\\t)
    }else{
        QMAKE_POST_LINK += "macdeployqt "$$TARGET"."$$TEMPLATE" " $$escape_expand(\\n\\t)
        #QMAKE_INFO_PLIST = "~/Qt5.3.1/5.3/clang_64/mkspecs/macx-clang/Info.plist.app"
        #export(QMAKE_INFO_PLIST)
    }

    QMAKE_POST_LINK += "rm -f $$TARGET"."$$TEMPLATE/Contents/PlugIns/sqldrivers/libqsqlmysql.dylib" $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += "rm -f $$TARGET"."$$TEMPLATE/Contents/PlugIns/sqldrivers/libqsqlodbc.dylib" $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += "rm -f $$TARGET"."$$TEMPLATE/Contents/PlugIns/sqldrivers/libqsqlpsql.dylib" $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += "open . " $$escape_expand(\\n\\t)


    # copy icon file and info.plist file for those shadow builds that don't fit the qt model
    message( "using icon file in " $$PWD "/icon/" )
    QMAKE_POST_LINK += "cp -f  $$PWD/icon/CWPIcon.icns $$OUT_PWD/$$TARGET"."$$TEMPLATE/Contents/Resources/"   $$escape_expand(\\n\\t)
    message( "using pre-made Info.plist in " $$PWD "/mac-info-plist-file" )
    QMAKE_POST_LINK += "cp -f  $$PWD/mac-info-plist-file/Info.plist $$OUT_PWD/$$TARGET"."$$TEMPLATE/Contents/"  $$escape_expand(\\n\\t)

    message( "exporting " $$QMAKE_POST_LINK )
    export(QMAKE_POST_LINK)

}#macx
#========================================================================================================
# Windows manipulate built files
#========================================================================================================
win32 {
      QMAKE_POST_LINK += $$escape_expand(\\n\\t)
      MKDIR_FILE = $$OUT_PWD/$$TARGET"."$$TEMPLATE
      MKDIR_FILE ~= s,/,\\,g
      exists($$MKDIR_FILE){
        message( $$MKDIR_FILE "does exist")
      } else {
        QMAKE_POST_LINK += mkdir $$MKDIR_FILE $$escape_expand(\\n\\t)
      }

      #remove default manifest if it exists
      CONFIG -= embed_manifest_exe
      QMAKE_POST_LINK += cd  $$escape_expand(\\n\\t)

      # copy format below worked.
      #MFST_FILE = $$PWD/manifest/cwp.exe.manifest
      #  win32:MFST_FILE ~= s,/,\\,g
      #QMAKE_POST_LINK += copy /y $$MFST_FILE  $$shell_quote($$exe_dir) $$escape_expand(\\n\\t)

    # manifest in debug mode causes dborig.exe to crash every time
    CONFIG(release, debug|release) {# release mode only
          #apply manifest in release mode only
          QMAKE_POST_LINK += cd $$shell_quote($$exe_dir) $$escape_expand(\\n\\t) cd $$escape_expand(\\n\\t)
          MT_DIR = $$PWD\\..\\..\\helpers\\windows\\mt
          QMAKE_POST_LINK += $$MT_DIR\\mt.exe -manifest $$PWD/manifest/cwp.exe.manifest -outputresource:"$$OUT_PWD/$$shell_quote($$exe_dir)/$$TARGET".exe;1  $$escape_expand(\\n\\t)
    }

    #========================================================================================================
    # Windows copy files into the application folder if release mode
    #CONFIG(release, debug|release) <-- this provides a "RELEASE" indicator
    #========================================================================================================
    # if bundle set, copy files over
    bundle_bibles {
        !isEmpty(target.path): INSTALLS += target

        # copies the given files to the destination directory
        defineTest(copyToDestDir) {
        message("copying additional files to CWP destination")
            files = $$1
            dir = $$2
            # replace slashes in destination path for Windows
            win32:dir ~= s,/,\\,g

            for(file, files) {
                # replace slashes in source path for Windows
                win32:file ~= s,/,\\,g

                QMAKE_POST_LINK += $$QMAKE_COPY_DIR /D $$shell_quote($$file) $$shell_quote($$dir) $$escape_expand(\\n\\t)
           }
            outpwd=$$OUT_PWD/$$TARGET"."$$TEMPLATE
            outpwd ~= s,/,\\,g
            QMAKE_POST_LINK += "cd" $$escape_expand(\n\t) "copy /y .\\"$$shell_quote($$exe_dir)\\$$TARGET".exe" $$outpwd

            greaterThan(QT_MAJOR_VERSION,4):greaterThan(QT_MINOR_VERSION,5) {
                QMAKE_POST_LINK +=$$escape_expand(\n\t)cd $$shell_quote($$outpwd) "&& windeployqt --no-system-d3d-compiler "$$TARGET".exe  " $$escape_expand(\n\t)cd .. $$escape_expand(\n\t)
            }else{
                QMAKE_POST_LINK +=$$escape_expand(\n\t)cd $$shell_quote($$outpwd) "&& windeployqt --no-system-d3d-compiler "$$TARGET".exe " $$escape_expand(\n\t)cd .. $$escape_expand(\n\t)
            }

            message("exporting " $$QMAKE_POST_LINK)
            export(QMAKE_POST_LINK)
        }


        MY_OTHER_FILES += $$PWD/../../CWPfiles
        CONFIG(debug, debug|release) { #debug mode
            QMAKE_POST_LINK += $$escape_expand(\n\t)
            copyToDestDir($$MY_OTHER_FILES, $$OUT_PWD/debug/CWPfiles)
        }else{# release mode
            copyToDestDir($$MY_OTHER_FILES, $$OUT_PWD/$$TARGET"."$$TEMPLATE/CWPfiles)
        }#else
    }#bundle_bibles
}
#========================================================================================================
# Linux, includes copy files after build section
#========================================================================================================
linux {


    APPDIR="AppDir"
    # create appdir
    QMAKE_POST_LINK += "mkdir -p $$OUT_PWD/$$TARGET"."$$APPDIR" $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += "mkdir -p $$OUT_PWD/$$TARGET"."$$APPDIR/usr/bin " $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += "mkdir -p $$OUT_PWD/$$TARGET"."$$APPDIR/usr/share/applications/" $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += "mkdir -p $$OUT_PWD/$$TARGET"."$$APPDIR/usr/share/icons/hicolor/256x256/" $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += "cp -f -v -p $$PWD/icon/CWPicon256x256.png $$OUT_PWD/$$TARGET"."$$APPDIR/usr/share/icons/hicolor/256x256/CWP.png" $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += "cp -f -v -p $$PWD/icon/CWPicon256x256.png $$OUT_PWD/$$TARGET"."$$APPDIR/CWP.png" $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += "cp -f -v -p $$PWD/templates/CWP.desktop $$OUT_PWD/$$TARGET"."$$APPDIR/usr/share/applications/CWP.desktop" $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += "cp -f -v -p $$PWD/templates/CWP.desktop $$OUT_PWD/$$TARGET"."$$APPDIR/CWP.desktop" $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += "cp -f -v -p $$PWD/templates/AppRun $$OUT_PWD/$$TARGET"."$$APPDIR/" $$escape_expand(\\n\\t)

   # QMAKE_POST_LINK += chmod a+x $$OUT_PWD/$$TARGET"."$$TEMPLATE/AppRun $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += "cp -f -v -p $$OUT_PWD/$$TARGET $$OUT_PWD/$$TARGET"."$$APPDIR/usr/bin/$$TARGET" $$escape_expand(\\n\\t)
    QMAKE_POST_LINK += "chmod a+x  $$OUT_PWD/$$TARGET"."$$APPDIR/usr/bin/$$TARGET" $$escape_expand(\\n\\t)


    #========================================================================================================
    # copy files into the app folder if release mode
    #CONFIG(release, debug|release){ ... } <-- this provides a "RELEASE" switch to do work at release
    #========================================================================================================
    # if bundle set, copy files over
    bundle_bibles {
        message("Bundling Bibles")
        !isEmpty(target.path): INSTALLS += target

        # copy the CWPFiles (bibles, dictionaries)
        MY_OTHER_FILES = $$PWD/../../CWPfiles

        QMAKE_POST_LINK += $$QMAKE_COPY_DIR $$shell_quote($$MY_OTHER_FILES) $$shell_quote($$OUT_PWD/$$TARGET"."$$APPDIR/usr/bin) $$escape_expand(\\n\\t)
# below failed, but works from command line.  go figure
# QMAKE_POST_LINK += chmod a-x $$shell_quote($$OUT_PWD/$$TARGET"."$$APPDIR/usr/bin/CWPfiles/Dictionaries/ismith/*.png) $$escape_expand(\\n\\t)

        QMAKE_POST_LINK += pwd $$escape_expand(\n\t) cp $$TARGET $$OUT_PWD/$$TARGET"."$$APPDIR/usr/bin $$escape_expand(\n\t)
        QMAKE_POST_LINK +=  chmod a+x $$OUT_PWD/$$TARGET"."$$APPDIR/usr/bin/$$TARGET $$escape_expand(\n\t)



        isEmpty(DEPLOYQT_BIN){
            message("linuxdeployqt not found")
        } else {
        message("linuxdeployqt used")

            # IF YOU GET AN EXECUTION ERROR WHEN RUNNING APPIMAGE FILE, MAKE SURE AppRun in the AppDir is chmod a+x so it can be executed
            #QMAKE_POST_LINK += "export ARCH=x86_64" $$escape_expand(\n\t)
#            QMAKE_POST_LINK += linuxdeployqt $$OUT_PWD/$$TARGET"."$$APPDIR/usr/bin/$$TARGET -appimage -extra-plugins=iconengines,platformthemes/libqgtk3.so -always-overwrite $$escape_expand(\n\t)
            QMAKE_POST_LINK += ARCH=x86_64 linuxdeployqt $$OUT_PWD/$$TARGET"."$$APPDIR/usr/bin/$$TARGET -appimage -extra-plugins=iconengines,platformthemes/libqgtk3.so -always-overwrite $$escape_expand(\n\t)
            #QMAKE_POST_LINK += ARCH=x86_64 /bin/appimagetool-x86_64.AppImage $$OUT_PWD/$$TARGET"."$$APPDIR/ $$escape_expand(\n\t)
        }

    QMAKE_POST_LINK += xdg-open $$OUT_PWD/
    message("exporting " $$QMAKE_POST_LINK)
    export(QMAKE_POST_LINK)

    }#bundle_bibles
}#linux


#-----------------------------------------------------------------------
# Compiler settings
#-----------------------------------------------------------------------
# macintosh start
#-----------------------------------------------------------------------
macx {
    message("Mac build")
    CONFIG += c++11
    CLANG_VERSION = $$system("clang++ --version")

    contains(CLANG_VERSION,5.[0-9].[0-9]){
      message("Xcode 5.1 version")
    DEFINES += XCODE_5=1
    }else{
          contains(CLANG_VERSION,6.[0-9].[0-9]){
          message("Xcode 6")
            DEFINES += XCODE_5=1  # prevent warning on [[fallthroug]]
        }else{
          message("Later XCode")
            DEFINES += FALLTHROUGH_ALLOWED
            QMAKE_CXXFLAGS += -Wno-inconsistent-missing-override
        }
    }
}
#-----------------------------------------------------------------------
# macintosh end
#-----------------------------------------------------------------------
#-----------------------------------------------------------------------
# linux compiler start
#-----------------------------------------------------------------------
linux {
    message("Linux build")
    GPP_VERSION = $$system("g++ --version")

    contains(GPP_VERSION,4.[0-9].[0-9]){
      message("GCC 4 version")
        QMAKE_CXXFLAGS += -std=c++0x -Wno-old-style-cast -Wno-unused-function
        DEFINES += C0_COMPILE=1
    }else{
        message("!GCC 4")
        QMAKE_CXXFLAGS += -Wno-expansion-to-defined -Wno-old-style-cast -Wno-unused-function
            DEFINES += FALLTHROUGH_ALLOWED=1
            QMAKE_CXXFLAGS += -Wno-inconsistent-missing-override
        CONFIG += c++11
        }

}
#-----------------------------------------------------------------------
# linux end
#-----------------------------------------------------------------------


#-----------------------------------------------------------------------
# Windows Compiler settings
#-----------------------------------------------------------------------
win32|win64 {
    message("Windows build")
#DEFINES += WINDOWS_FLASH_DEMO=1

  *-g++* {
   # mingw compiler is used
   message("MingGW")
   CONFIG+= mingw
   DEFINES += MINGW=1
   DEFINES += CPP11=1  # set to 1 if compiling with that flag
    #-Wno-inconsistent-missing-override -Wno-expansion-to-defined -Wno-missing-variable-declarations cant be used Mingw 4.92
   QMAKE_CXXFLAGS += -std=c++11 -Wno-implicit-fallthrough -Wno-attributes -Wno-old-style-cast -Wno-missing-declarations -Wno-zero-as-null-pointer-constant -Wno-missing-variable-declarations
   # change for later version of mingw
   DEFINES += FALLTHROUGH_ALLOWED=0
  }

 *-msvc* {
  message("MSVC")
  CONFIG += msvc
  # MSVC compiler is used
    #   disable old style cast warning C26493
    #   disable 'typedef' ignored on left of unsigned short when no variable is declared
    #  disable '%' unrecognized escape sequence
    QMAKE_CXXFLAGS += /std:c++17 /wd26493 /wd4091 /wd4129
  }
}
#-----------------------------------------------------------------------
#windows end
#-----------------------------------------------------------------------

#---------------------------------------------------
# DEPLOYMENT
#-----------------------------------------------------------------------
release_stuff {
    message("Release Build")
}

INCLUDEPATH += "../shared/"
INCLUDEPATH += "../dialogs/copyrightDialog/"
INCLUDEPATH += "../dialogs/notesWindow/"
INCLUDEPATH += ../dialogs/BookMarkDialog/ ../biblepicture/PictureInfoDatabase/
INCLUDEPATH += ../shared/TimerClass/ ../biblepicture/
INCLUDEPATH += ../shared/wheelWidget/ ../MediaModule/
INCLUDEPATH += ../shared/BookMarkDB/ ../BibleDictionary/
INCLUDEPATH += ../dialogs/AddTopic/ ../dialogs/SelectBookInBible/
INCLUDEPATH += ../shared/StrongsDialog/ ../dialogs/Search/
INCLUDEPATH += ../dialogs/EnglishDictionary/ ../shared/helpLibrary/
INCLUDEPATH += ../dialogs/Preferences/ ../shared/userhistory/
INCLUDEPATH += ../shared/Preferences/ ../dialogs/History/
INCLUDEPATH += ../shared/Notes/ ../dialogs/SelectChapterInBook/
INCLUDEPATH += ../dialogs/SelectVerse/ ../dialogs/FootNote/
INCLUDEPATH += ../dialogs/bookmarkviewer/ ../dialogs/bookmarklistviewer/

SOURCES += main.cpp\
        mainwindow.cpp \
    ../shared/chapterversestructclass.cpp \
    ../shared/misclibrary.cpp \
    biblemodule.cpp \
    ../shared/commentarymodule.cpp \
    ../shared/commentarydatabaseclass.cpp \
    ../shared/bibleinfo.cpp \
    dictionarydb.cpp \
    searchlibrary.cpp \
    ../shared/bookhandlerclass.cpp \
    ../shared/strongsdatabaseclass.cpp \
    ../shared/strongsmodule.cpp \
    initialization.cpp \
    ../shared/bookmanipulationlibrary.cpp \
    ../shared/bookmanipulationclass.cpp \
    ../shared/bookpreferencesdbclass.cpp \
    ../shared/bookdbclass.cpp \
    ../shared/HTMLModule.cpp \
    ../dialogs/SelectBibles/selectbiblesdialog.cpp \
    ../shared/settings/myqsettings.cpp \
    ../shared/ActivityIndicator/activityindicatorclass.cpp \
    ../shared/busyIndicator/busyindicator.cpp \
    ../shared/helpLibrary/helplibrary.cpp \
    ../shared/TimedSearch/timedsearchroutineclass.cpp \
    ../shared/Database/database.cpp \
    ../shared/RecordSet/recordset.cpp \
    ../shared/morphology/morphology.cpp \
    ../dialogs/copyrightDialog/dialogcopyright.cpp \
    ../dialogs/notesWindow/noteswindow.cpp \
    ../dialogs/BookMarkDialog/bookmarkhandler.cpp \
    ../shared/TimerClass/timerclass.cpp \
    ../shared/wheelWidget/wheelwidget.cpp \
    ../shared/BookMarkDB/bookmarkdb.cpp \
    ../dialogs/AddTopic/dialogaddtopic.cpp \
    ../dialogs/SelectBookInBible/dialogselectbookinbible.cpp \
    ../shared/StrongsDialog/dialogshowstrongs.cpp \
    ../dialogs/EnglishDictionary/dialogenglishdictionary.cpp \
    ../dialogs/Preferences/dialogpreferences.cpp \
    ../shared/Preferences/preferenceslibrary.cpp \
    ../shared/Preferences/preferencesmodule.cpp \
    ../shared/Notes/notesdbclass.cpp \
    ../shared/Notes/noteslibrary.cpp \
    ../dialogs/SelectVerse/dialogselectverse.cpp \
    ../dialogs/FootNote/dialogfootnote.cpp \
    ../dialogs/History/dialoghistory.cpp \
    ../dialogs/SelectChapterInBook/dialogselectchapterinbook.cpp \
    ../shared/userhistory/userhistoryclass.cpp \
    ../dialogs/Search/dialogsearch.cpp \
    ../dialogs/bookmarkviewer/dialogbookmarkviewer.cpp \
    ../dialogs/bookmarklistviewer/dialogbookmarklistviewer.cpp \
    ../BibleDictionary/bibledictionarydialog.cpp \
    ../MediaModule/MediaModuleDatabase.cpp \
    ../biblepicture/dialogbiblepicture.cpp \
    ../biblepicture/PictureInfoDatabase/PIctureInfoDatabase.cpp \
    ../tagalog/tagalogdialog.cpp \
    ../editDB/editdb.cpp \
    ../dialogs/about/dialogabout.cpp \
    ../translationtextfile/translationtextfiledialog.cpp \
    ../dialogs/CreateBible/createbibledialog.cpp \
    ../shared/BibleDatabaseClass/bibledatabaseclass.cpp \
    ../dialogs/CopyDialog/copydialog.cpp \
    dialogEditVerse/dialogeditverse.cpp

HEADERS  += mainwindow.h \
    ../shared/chapterversestructclass.h \
    ../shared/structs.h \
    WebEngineViewWebPage.h \
    cwpdefines.h \
    ../shared/misclibrary.h \
    ../shared/BibleDatabaseClass/bibledatabaseclass.h \
    ../shared/WebDefinitionModule.h \
    ../shared/HTMLModule.h \
    ../shared/commentarymodule.h \
    ../shared/commentarydatabaseclass.h \
    ../shared/bibleinfo.h \
    dictionarydb.h \
    searchlibrary.h \
    ../shared/bookhandlerclass.h \
    ../shared/strongsdatabaseclass.h \
    ../shared/strongsmodule.h \
    ../shared/BookMarkDB/bookmarkdb.h \
    initialization.h \
    ../shared/bookmanipulationlibrary.h \
    ../shared/bookmanipulationclass.h \
    ../shared/bookpreferencesdbclass.h \
    ../shared/bookdbclass.h \
    ../dialogs/SelectBibles/selectbiblesdialog.h \
    ../shared/settings/myqsettings.h \
    ../shared/ActivityIndicator/activityindicatorclass.h \
    ../shared/busyIndicator/busyindicator.h \
    ../shared/helpLibrary/helplibrary.h \
    ../shared/TimedSearch/timedsearchroutineclass.h \
    indexpath.h \
    ../shared/Database/database.h \
    ../shared/RecordSet/recordset.h \
    ../shared/morphology/morphology.h \
    ../dialogs/copyrightDialog/dialogcopyright.h \
    ../dialogs/notesWindow/noteswindow.h \
    ../dialogs/BookMarkDialog/bookmarkhandler.h \
    ../shared/TimerClass/timerclass.h \
    ../shared/wheelWidget/wheelwidget.h \
    ../shared/BookMarkDB/BookMarkDB.h \
    ../dialogs/AddTopic/dialogaddtopic.h \
    ../dialogs/SelectBookInBible/dialogselectbookinbible.h \
    ../shared/StrongsDialog/dialogshowstrongs.h \
    ../dialogs/EnglishDictionary/dialogenglishdictionary.h \
    ../dialogs/Preferences/dialogpreferences.h \
    ../shared/Preferences/preferenceslibrary.h \
    ../shared/Preferences/preferencesmodule.h \
    ../shared/Notes/notesdbclass.h \
    ../shared/Notes/notesLibrary.h \
    ../dialogs/SelectVerse/dialogselectverse.h \
    ../dialogs/FootNote/dialogfootnote.h \
    ../dialogs/SelectChapterInBook/dialogselectchapterinbook.h \
    ../dialogs/History/dialoghistory.h \
    ../shared/userhistory/userhistoryclass.h \
    ../dialogs/Search/dialogsearch.h \
    ../dialogs/bookmarkviewer/dialogbookmarkviewer.h \
    ../dialogs/bookmarklistviewer/dialogbookmarklistviewer.h \
    ../BibleDictionary/bibledictionarydialog.h \
    ../MediaModule/MediaModuleDatabase.h \
    ../biblepicture/dialogbiblepicture.h \
    ../biblepicture/PictureInfoDatabase/PIctureInfoDatabase.h \
    ../tagalog/tagalogdialog.h \
    ../editDB/editdb.h \
    ../dialogs/about/dialogabout.h \
    ../translationtextfile/translationtextfiledialog.h \
    ../dialogs/CreateBible/createbibledialog.h \
    biblemodule.h \
    cwpdefines.h \
    dictionarydb.h \
    indexpath.h \
    initialization.h \
    searchlibrary.h \
    ../dialogs/CopyDialog/copydialog.h \
    dialogEditVerse/dialogeditverse.h \
    biblemaxvalues.h

FORMS    += mainwindow.ui \
    ../dialogs/SelectBibles/selectbiblesdialog.ui \
    ../dialogs/copyrightDialog/dialogcopyright.ui \
    ../dialogs/notesWindow/noteswindow.ui \
    ../dialogs/BookMarkDialog/bookmarkhandler.ui \
    ../dialogs/AddTopic/dialogaddtopic.ui \
    ../dialogs/SelectBookInBible/dialogselectbookinbible.ui \
    ../shared/StrongsDialog/dialogshowstrongs.ui \
    ../dialogs/EnglishDictionary/dialogenglishdictionary.ui \
    ../dialogs/Preferences/dialogpreferences.ui \
    ../dialogs/SelectVerse/dialogselectverse.ui \
    ../dialogs/FootNote/dialogfootnote.ui \
    ../dialogs/History/dialoghistory.ui \
    ../dialogs/SelectChapterInBook/dialogselectchapterinbook.ui \
    ../dialogs/Search/dialogsearch.ui \
    ../dialogs/bookmarkviewer/dialogbookmarkviewer.ui \
    ../dialogs/bookmarklistviewer/dialogbookmarklistviewer.ui \
    ../BibleDictionary/bibledictionarydialog.ui \
    ../biblepicture/dialogbiblepicture.ui \
    ../tagalog/tagalogdialog.ui \
    ../dialogs/about/dialogabout.ui \
    ../translationtextfile/translationtextfiledialog.ui \
    ../dialogs/CreateBible/createbibledialog.ui \
    ../dialogs/CopyDialog/copydialog.ui \
    dialogEditVerse/dialogeditverse.ui

RESOURCES += \
    pictureResources.qrc

#windows
win32 {
# did not work
#OTHER_FILES += \
#   icon/icon_mwG_icon.ico \
#   icon/cwp.rc

# use https://icoconvert.com to convert main png to multipl image ico file
    RC_ICONS = icon/icon_mwG_icon.ico
# did not work
#RC_FILE  = icon/cwp.rc
}

# these files are "other files" which allow them to be in the Git network, but not
# in the QMAKE "must do something to these files" network
DISTFILES += \
    ../../Docs/CrosswordBibleDatabaseFormat.doc \
    ../../Docs/PuttingBiblesOniPad.doc \
    ../../sql/bbl_schema2.sql
