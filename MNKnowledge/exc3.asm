; [26-Mar-21 10:46]
;
; Юлето къса листенца на маргаритка. Пропускайки първото листенце и късайки второто, Юлето продължава да пропуска и къса по едно листенце, докато накрая не остане само едно.
; Напишете програма, която по зададен брой листенца на маргаритката извежда кое ще е последното останало. Направете това по възможно най-оптимален начин.
; Пример: Ако маргаритка има 9 листенца, Юлето ще откъсне листенцата в следния ред: 2, 4, 6, 8, 1, 5, 9, 7, оставяйки 3-тото листенце последно.
;

SECTION .bss
	sIn:	resb	255
	sOut:	resb	255

SECTION .text
global _start

_start:
	; Read 10 bytes from STDIN in sIn
	mov		edx, 10
	mov		ecx, sIn
	mov		ebx, 0
	mov		eax, 3
	int		80h
	
	; Transform input to int and store in eax
	mov		esi, sIn
	mov		eax, 0
	mov		edx, 0
getInDigits:
	mov		dl, [esi]
	cmp		dl, 48
	jl		getInDigits_end
	cmp		dl, 57
	jg		getInDigits_end
	
	sub		dl, 48
	imul	eax, 10
	add		eax, edx
	inc		esi
	jmp		getInDigits
getInDigits_end:
	
	; Halt if input is bad
	cmp		eax, 0
	je		quit
	
	; Find result as int and store in eax
	mov		ebx, eax		; ebx = n
	bsr		ecx, ebx		; ecx = BSR(n)
	mov		eax, 1			; eax = 1
	sal		eax, cl			; eax = 1 << BSR(n)
	xor		eax, ebx		; eax = n ^ (1 << BSR(n))
	add		eax, eax		; eax = (n ^ (1 << BSR(n))) * 2
	or		eax, 1			; eax = (n ^ (1 << BSR(n))) * 2 | 1
	
	; Store digits of result in sIn in reverse order 
	mov		esi, sIn
	mov		ecx, 0			 ; stores the number of digits
	mov		ebx, 10
getOutDigitsReverse:
	mov		edx, 0
	idiv	ebx
	add		dl, 48
	mov		[esi+ecx], dl
	inc		ecx
	cmp		eax, 0
	jg		getOutDigitsReverse
	
	; Store digits of result in sOut
	mov		edi, sOut
	mov		ebx, ecx
getOutDigits:
	dec		ebx
	mov		edx, [esi+ebx]
	mov		[edi], dl
	inc		edi
	cmp		ebx, 0
	jg		 getOutDigits
	
	; Print sOut to STDOUT
	mov		edx, ecx
	mov		ecx, sOut
	mov		ebx, 1
	mov		eax, 4
	int		80h
	
quit:
	mov		ebx, 0
	mov		eax, 1
	int		80h
	ret