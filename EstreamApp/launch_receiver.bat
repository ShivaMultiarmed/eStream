@echo off
chcp 65001 > nul

start "" cmd /k "title Encryptor_2 && "..\hc-128\x64\Debug\hc-128.exe" 9000"

start "" cmd /k "title Receiver && java -jar "..\..\Practice12\SecureClient\build\libs\estream-client.jar" listen"