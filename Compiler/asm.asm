.586
.model flat, stdcall
.stack 4096
option casemap : none
includelib "e:\msvcrt.lib"
printf PROTO C : ptr byte, : vararg
ExitProcess	PROTO, dwExitCode : DWORD
exit equ <INVOKE ExitProcess, 0>
chr$ MACRO any_text : VARARG
LOCAL txtname
.data
IFDEF __UNICODE__
WSTR txtname, any_text
align 4
.code
EXITM <OFFSET txtname>
ENDIF
txtname db any_text, 0
align 4
.code
EXITM <OFFSET txtname>
ENDM
.data
score dword 0
i dword 0
j dword 0
avg dword 0
temp dword 0
e dword 10 dup(0)
buff dword 44 dup(0)
.code
main proc
L0: mov eax,10
mov 0[buff],eax
L1: mov eax,60
mov 0[buff],eax
L2: mov eax,0[buff]
mov score,eax
L3: mov eax,0
mov 4[buff],eax
L4: mov eax,4[buff]
mov i,eax
L5: mov eax,0
mov 8[buff],eax
L6: mov eax,8[buff]
mov j,eax
L7: mov eax,i
mov 12[buff],eax
L8: mov eax,10
mov 16[buff],eax
L9: mov eax,12[buff]
mov ebx,16[buff]
cmp eax,ebx
jnl C9
mov 20[buff],1
jmp L10
C9: mov 20[buff],0
L10: mov eax,20[buff]
cmp eax,0
je L21
L11: mov eax,i
mov 24[buff],eax
L12: mov eax,score
mov 28[buff],eax
L13: mov ebx,28[buff]
mov eax,24[buff]
mov [e+eax*4],ebx
L14: mov eax,i
mov 32[buff],eax
L15: inc i
L16: mov eax,score
mov 36[buff],eax
L17: inc score
L18: mov eax,score
mov 40[buff],eax
L19: inc score
L20: jmp L7
L21: mov eax,0
mov 0[buff],eax
L22: mov eax,0[buff]
mov avg,eax
L23: mov eax,j
mov 4[buff],eax
L24: mov eax,10
mov 8[buff],eax
L25: mov eax,4[buff]
mov ebx,8[buff]
cmp eax,ebx
jnl C25
mov 12[buff],1
jmp L26
C25: mov 12[buff],0
L26: mov eax,12[buff]
cmp eax,0
je L36
L27: mov eax,j
mov 16[buff],eax
L28: mov eax,16[buff]
mov ebx,[e+eax*4]
mov temp,ebx
L29: mov eax,avg
mov 20[buff],eax
L30: mov eax,temp
mov 24[buff],eax
L31: mov eax,20[buff]
add eax,24[buff]
mov 28[buff],eax
L32: mov eax,28[buff]
mov avg,eax
L33: mov eax,j
mov 32[buff],eax
L34: inc j
L35: jmp L23
L36: mov eax,avg
mov 0[buff],eax
L37: invoke printf, chr$("total score is:%d", 0DH, 0AH),avg
L38: mov eax,avg
mov 0[buff],eax
L39: mov eax,10
mov 4[buff],eax
L40: mov eax,0[buff]
mov edx,0
mov ebx,4[buff]
div ebx
mov 8[buff],eax
L41: mov eax,8[buff]
mov avg,eax
L42: mov eax,avg
mov 12[buff],eax
L43: invoke printf, chr$("avg is :%d", 0DH, 0AH),avg
L44: mov eax,0
mov 0[buff],eax
exit
main ENDP
END	main