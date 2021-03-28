; [26-Mar-21 10:46]
;
; Юлето къса листенца на маргаритка. Пропускайки първото листенце и късайки второто, Юлето продължава да пропуска и къса по едно листенце, докато накрая не остане само едно.
; Напишете програма, която по зададен брой листенца на маргаритката извежда кое ще е последното останало. Направете това по възможно най-оптимален начин.
; Пример: Ако маргаритка има 9 листенца, Юлето ще откъсне листенцата в следния ред: 2, 4, 6, 8, 1, 5, 9, 7, оставяйки 3-тото листенце последно.
;

SECTION .bss
	s: resb 255

SECTION .text
global _start

_start:
	; Read 10 bytes from STDIN in s
	mov edx, 10
	mov ecx, s
	mov ebx, 0
	mov eax, 3
	int 80h
	
	; Transform input to int and store in eax
	mov esi, s
	mov eax, 0
	mov edx, 0
getInDigits:
	mov dl, [esi]
	cmp dl, 48
	jl getInDigits_end
	cmp dl, 57
	jg getInDigits_end
	sub dl, 48
	imul eax, 10
	add eax, edx
	inc esi
	jmp getInDigits
getInDigits_end:
	
	; Halt if input is bad
	cmp eax, 0
	je quit
	
	; Find result as int and store in eax
	mov ebx, eax
	bsr ecx, ebx
	mov eax, 1
	sal eax, cl
	xor eax, ebx
	add eax, eax
	or eax, 1
	
	; Count number of digits in result [Can be improved with LUT]
	mov ecx, 0
	mov ebx, 1
countDigits:
	inc ecx
	imul ebx, 10
	cmp eax, ebx
	jge countDigits
	
	; Store digits of result in s
	mov esi, s
	mov edi, 10
	mov ebx, ecx
getOutDigits:
	mov edx, 0
	idiv edi
	add dl, 48
	dec ebx
	mov [esi+ebx], dl
	cmp eax, 0
	jg getOutDigits
	
	; Print s to STDOUT
	mov edx, ecx
	mov ecx, s
	mov ebx, 1
	mov eax, 4
	int 80h
	
quit:
	mov ebx, 0
	mov eax, 1
	int 80h
	ret
