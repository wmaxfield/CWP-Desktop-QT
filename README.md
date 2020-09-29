# CWP-Desktop-QT</br>
</br>
Build using QtCreator.  Tested on Qt 5.32 through Qt 5.14</br>
</br>
<b>Crossword Project Bible Study Program for the desktop</b></br>
General Notes: (see CWP.pro for more details) </br>
<b>Windows Build:</b> </br>
Requires:</br>
1. QT development environment for MinGW 5.5.1 (or possibly later) with QWebKit </br>
or </br>
QT development environment for MSVC 5.5.1 (or possily later) either QWebKit or QWebEngine</br>
2. QTCreator (usually included with Qt development environment.  see download.qt.io)</br>
3. InnoSetup is a good tool for creating an Install package. Example scripts are included</br>
4. You need at least 1 windows SDK installed to use mt.exe.  <br/>
   (mt.exe applies administrator mode privilege to prevent Virtual Store from being used when CWP is executed.)<br/>
5. When you build, build a 32 bit version so you are compatile back to Windows XP<br/>
<br/>
<b>Mac Build:</b></br>
1. XCode is required to be installed (contains SDK), along with command line tools.<br/>  Read the Qt Documentation.  (It is a pain to get XCode installed properly.)</br>
2. To work on OSX 10.8.5, you must build on OSX 10.10 or earlier, as later versions of Mac/Qt combination require gaming library not available on older Mac OS's.</br>
3. CLANG will not compile over a network shared drive, output must be on local drive.  <br/>This makes debugging difficult without copying entire project to your Virtual Machine.</br>
</br>
<b>Linux Build:</b></br>
1. Install qtcreator (Ubuntu: sudo apt-get install qtcreator)</br>
2. Project will use linuxdeployqt if installed in path. (included in helpers directory) <br/> linuxdeployqt requires you to build on Ubuntu 14. (Their desire is to run on maximum number of systems.)</br>
</br>
