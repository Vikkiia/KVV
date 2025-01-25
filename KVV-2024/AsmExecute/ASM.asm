.586
.model flat, stdcall
includelib libucrt.lib
includelib kernel32.lib
includelib ..\Debug\FuncLib.lib
ExitProcess PROTO:DWORD 
EXTRN random: PROC 
EXTRN toPow: PROC
EXTRN writestr: PROC
EXTRN writenum: PROC
EXTRN writenumline: PROC
EXTRN writestrline: PROC
EXTRN system_pause:PROC
.stack 4096
.const
ZEROMESSAGE  BYTE 'Ошибка:деление на ноль',0
OVERFLOWMESSAGE  BYTE 'Ошибка:переполнение типа',0
	countsumcalculating_sum BYTE 'calculating sum', 0
	coutsomethcalculating_combocalculating_combo BYTE 'calculating combo', 0
	main4 DWORD 4
	main5 DWORD 5
	main0x0 DWORD 0
	main10 DWORD 10
	maincounting_more_then_10 BYTE 'counting more then 10', 0
	maincounting_less_then_10 BYTE 'counting less then 10', 0
	mainl BYTE 'l', 0
	main0 DWORD 0
	main3 DWORD 3
	main2 DWORD 2
	main1 DWORD 1
	mainnew_calculating BYTE 'new calculating', 0
	main7 DWORD 7
	mainoperation_is_not_10 BYTE 'operation is not 10', 0
	mainoperation_is_not_equal BYTE 'operation is not equal', 0
	main6 DWORD 6
	mainoperation_less_6 BYTE 'operation less 6', 0
.data
	mainoperation1 DWORD ?
	mainhexvalue1 DWORD ?
	maincounter1 DWORD ?
	mainmessage1 DWORD ?

.code
countsum PROC countsumn1 :  DWORD , countsumm1 :  DWORD 
push OFFSET countsumcalculating_sum
pop eax
push eax
call writestrline
push countsumn1
push countsumm1
pop eax
pop ebx
add eax,ebx
push eax
jc OVERFLOW
pop eax
 
ret
ZEROERROR:
push OFFSET ZEROMESSAGE
call writestrline
push -1
	call		ExitProcess
OVERFLOW:
push OFFSET OVERFLOWMESSAGE
call writestrline
push -1
	call		ExitProcess
countsum ENDP

coutsomethingelse PROC coutsomethnn1 :  DWORD , coutsomethmm1 :  DWORD 
push OFFSET coutsomethcalculating_combocalculating_combo
pop eax
push eax
call writestrline
push coutsomethnn1
push coutsomethmm1
pop eax
pop ebx
add eax,ebx
push eax
jc OVERFLOW
push coutsomethnn1
push coutsomethmm1
call countsum
 push eax
pop eax
pop ebx
add eax,ebx
push eax
jc OVERFLOW
pop eax
 
ret
ZEROERROR:
push OFFSET ZEROMESSAGE
call writestrline
push -1
	call		ExitProcess
OVERFLOW:
push OFFSET OVERFLOWMESSAGE
call writestrline
push -1
	call		ExitProcess
coutsomethingelse ENDP

main PROC
push main4
push main5
call coutsomethingelse
 push eax
pop eax
push eax
pop mainoperation1
push mainoperation1
pop eax
push eax
call writenumline
push main0x0
pop eax
push eax
pop mainhexvalue1
push mainhexvalue1
pop eax
push eax
call writenumline
push mainoperation1
push main10
pop ebx
pop eax
cmp eax, ebx
jb SKIP19
push OFFSET maincounting_more_then_10
pop eax
push eax
call writestrline
jae SKIPELSE23
SKIP19:
push OFFSET maincounting_less_then_10
pop eax
push eax
call writestrline
push OFFSET mainl
pop eax
push eax
call writestrline
SKIPELSE23:
push main0
pop eax
push eax
pop maincounter1
push main5
call random
 pop ecx
 push eax
push main3
push main2
call toPow
 pop ecx
 push eax
pop ebx
pop eax
mul ebx
push eax
jc OVERFLOW
pop eax
push eax
pop mainoperation1
push mainoperation1
push main1
pop ebx
pop eax
sub eax,ebx
push eax
jc OVERFLOW
push main3

	pop ebx
	pop eax
test ebx, ebx
jz ZEROERROR
	cdq
	div ebx
	push eax
jc OVERFLOW
pop eax
push eax
pop mainoperation1
push mainoperation1
pop eax
push eax
call writenumline
push OFFSET mainnew_calculating
pop eax
push eax
pop mainmessage1
push mainoperation1
push main7

	pop ebx
	pop eax
test ebx, ebx
jz ZEROERROR
	cdq
	div ebx
	push edx
jc OVERFLOW
pop eax
push eax
pop mainoperation1
push main10
push main10
pop ebx
pop eax
cmp eax, ebx
jne SKIP38
push main10
pop eax
push eax
call writenumline
SKIP38:
push mainoperation1
push main0
pop ebx
pop eax
cmp eax, ebx
jbe SKIP42
push OFFSET mainoperation_is_not_10
pop eax
push eax
call writestrline
SKIP42:
push mainoperation1
push main5
pop ebx
pop eax
cmp eax, ebx
je SKIP46
push OFFSET mainoperation_is_not_equal
pop eax
push eax
call writestrline
SKIP46:
push mainoperation1
push main6
pop ebx
pop eax
cmp eax, ebx
jae SKIP50
push OFFSET mainoperation_less_6
pop eax
push eax
call writestrline
SKIP50:
push main0
pop eax
push eax
call	system_pause
 	call		ExitProcess
ZEROERROR:
push OFFSET ZEROMESSAGE
call writestrline
push -1
	call		ExitProcess
OVERFLOW:
push OFFSET OVERFLOWMESSAGE
call writestrline
push -1
	call		ExitProcess
 main ENDP
END main