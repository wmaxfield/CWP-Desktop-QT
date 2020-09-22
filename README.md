# CWP-Desktop-QT</br>
</br>
Build using QtCreator.  Tested on Qt 5.51 through Qt 5.14</br>
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
4. You need at least 1 windows SDK downloaded for mt.exe</br>
5. When you build, build a 32 bit version so you are compatile back to Windows XP</br>
</br>
<b>Mac Build:</b></br>
1. XCode is required to be installed (contains SDK)</br>
2. To work on OSX 10.8.5, you must build on OSX 10.10 or earlier, as later versions of Mac/Qt combo require gaming library not available on older Mac OS's.</br>
3. CLANG will not compile over a VMWare shared drive, must be local files.</br>
</br>
<b>Linux Build:</b></br>
1. Install qtcreator (sudo apt-get install qtcreator)</br>
2. Project will use linuxdeployqt if installed in path.  linuxdeployqt requires you to build on Ubuntu 14. (Their desire is to run on maximum number of systems.)</br>
</br>
