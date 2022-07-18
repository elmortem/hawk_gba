set CFADVTOOLS=e:\m.osokin\GBAdev\Demos\_gba_test0
mkdir out
cd out
%CFADVTOOLS%\tmx2incl.exe b ..\resources\sprites.bmp gr_sprites

%CFADVTOOLS%\tmx2incl.exe bct ..\resources\mainmenu.bmp ..\resources\mainmenu.tmx gr_mainmenu
%CFADVTOOLS%\tmx2incl.exe bct ..\resources\meta.bmp ..\resources\meta.tmx gr_meta
%CFADVTOOLS%\tmx2incl.exe bct ..\resources\dialog.bmp ..\resources\dialog.tmx gr_dialog
%CFADVTOOLS%\tmx2incl.exe bct ..\resources\end.bmp ..\resources\end.tmx gr_end

%CFADVTOOLS%\tmx2incl.exe bct ..\resources\tiles.bmp ..\resources\mission1.tmx gr_tiles
%CFADVTOOLS%\tmx2incl.exe bct ..\resources\tiles.bmp ..\resources\mission2.tmx gr_tiles
%CFADVTOOLS%\tmx2incl.exe bct ..\resources\tiles.bmp ..\resources\mission3.tmx gr_tiles
%CFADVTOOLS%\tmx2incl.exe bct ..\resources\tiles.bmp ..\resources\mission4.tmx gr_tiles
%CFADVTOOLS%\tmx2incl.exe bct ..\resources\tiles.bmp ..\resources\mission5.tmx gr_tiles

cd ..
@REM pause