set CFADVTOOLS=e:\m.osokin\GBAdev\Demos\_gba_test0
mkdir out
cd out
%CFADVTOOLS%\wav2incl.exe ..\sounds\lobby.wav snd_bank.inc snd_lobby
%CFADVTOOLS%\wav2incl.exe ..\sounds\gameplay.wav +snd_bank.inc snd_gameplay
%CFADVTOOLS%\wav2incl.exe ..\sounds\click.wav +snd_bank.inc snd_click
%CFADVTOOLS%\wav2incl.exe ..\sounds\touch.wav +snd_bank.inc snd_touch
%CFADVTOOLS%\wav2incl.exe ..\sounds\bonus.wav +snd_bank.inc snd_bonus
%CFADVTOOLS%\wav2incl.exe ..\sounds\cow.wav +snd_bank.inc snd_cow
%CFADVTOOLS%\wav2incl.exe ..\sounds\shoot.wav +snd_bank.inc snd_shoot
%CFADVTOOLS%\wav2incl.exe ..\sounds\shoot2.wav +snd_bank.inc snd_shoot_power
%CFADVTOOLS%\wav2incl.exe ..\sounds\player_shoot.wav +snd_bank.inc snd_player_shoot
%CFADVTOOLS%\wav2incl.exe ..\sounds\expl.wav +snd_bank.inc snd_expl
%CFADVTOOLS%\wav2incl.exe ..\sounds\buy.wav +snd_bank.inc snd_buy
%CFADVTOOLS%\wav2incl.exe ..\sounds\levelup.wav +snd_bank.inc snd_levelup
%CFADVTOOLS%\wav2incl.exe ..\sounds\fail.wav +snd_bank.inc snd_fail
%CFADVTOOLS%\wav2incl.exe ..\sounds\state.wav +snd_bank.inc snd_state
%CFADVTOOLS%\wav2incl.exe ..\sounds\dialog.wav +snd_bank.inc snd_dialog

cd ..
@REM pause