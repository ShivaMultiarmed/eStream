@echo off
chcp 65001 > nul

start "" cmd /k "title Encryptor_1 && "..\hc-128\x64\Debug\hc-128.exe" 8000"

start "" cmd /k "title Sender && java -jar "..\..\Practice12\SecureClient\build\libs\estream-client.jar" transfer"