@echo off
chcp 65001 > nul

REM Start hc128.exe with absolute paths
start "" cmd /k "title Encryptor_1 && "..\hc-128\x64\Debug\hc-128.exe" 8000"
start "" cmd /k "title Encryptor_2 && "..\hc-128\x64\Debug\hc-128.exe" 9000"
timeout /t 2 > nul

REM Start the Java server with absolute path
start "" cmd /k "title Server && java -jar "..\..\Practice12\SecureServer\build\libs\estream-server.jar""
timeout /t 2 > nul

REM Start the Java receiver with absolute path
start "" cmd /k "title Receiver && java -jar "..\..\Practice12\SecureClient\build\libs\estream-client.jar" listen"
timeout /t 2 > nul

REM Start the Java sender with absolute path
start "" cmd /k "title Sender && java -jar "..\..\Practice12\SecureClient\build\libs\estream-client.jar" transfer"