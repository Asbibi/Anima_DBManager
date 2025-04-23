# Anima_DBManager
 Qt C++ Application to manage the database of my Anima Unreal game project.
 Versions used :
 Qt - 6.6.3 (MSVC 2019, x86_64)
 Qt Creator (IDE) - 13.0.1

# Deploy
1. on release mode, hit the build button, il will create an exe file under build/..
2. copy the exe file onto the wanted location
3. In order to work, the exe file requires few dll files :
- libgcc_s_seh-1.dll
- libstdc++-6.dll
- libwinpthread-1.dll
- Qt6Core.dll
- Qt6Gui.dll
- Qt6Multimedia.dll
- Qt6Network.dll
- QtWidgets.dll
Those can be found in the Qt installation folder, under ...\Qt\6.7.0\mingw_64\bin\
4. The exe file also requires platform specific (windows) to operate. Copy paste the "platforms" folder found under ...\Qt\6.7.0\mingw_64\plugins\ where the exe file is located. It should contains 4 dll files.
5. In the same way, to have the proper style copy paste the "styles" folder also present in ...\Qt\6.7.0\mingw_64\plugins\ at the same location. This folder should only contain one dll file

The deployment is now complete !
The exe file can be pinned to the startup menu, to the task bar, and shortcut can be associated with it.

It can also be selected in the Open with... option for .uadb files, which will start the application directly with the given file opened.