; Adrian Maldonado - Jeremy Bruce - CMSC 313 FA23 - PROJECT 3
; Description:
; Asks the user for a string that is longer than 8. Then ask for a number between 2 and the length of the string.
; There will be error/input correction.  This number will be where the string will be split. Then display the original string
; Then display the new edited string that was divided in 2 pieces by the user input

extern scanf
extern printf
extern printNum

    section .data

request1:       db  "Please enter the text: ", 10
len_request1:   equ $-request1

request2:       db  "Enter a location value: ", 0
len_request2:   equ $-request2

msg1:           db "Unedited String:", 10, 0
len_msg1:       equ $-msg1

msg2:           db "Edited String:", 10
len_msg2:       equ $-msg2

newline:        db      10

format:         db "%d", 0
stringfmt:      db "%s", 0 

sample:           db "This is the original message.", 0

    section .bss

origString:     resq    1
origSize:       resq    1
cutPoint:       resb    8



    section .text

    global reverse

reverse:

get_string:
    ;puts the pointer to the string into origString
    xor r8, r8
    mov rax, rdi
    mov [origString], rax
string_loop:
;   Goes through the string letter by letter and uses a counter to check length
    cmp byte[rax], 0
    je check_string
    inc r8
    inc rax
    jmp string_loop

check_string:
;   Error checks the string read in from the user
    cmp r8, 8
    jl  get_string
    mov [origSize], r8

get_integer:
;   Writes out the next message that requests an integer from user and reads in their input
    mov rax, 1
    mov rdi, 1
    mov rsi, request2
    mov rdx, len_request2
    syscall

    push rbp
    mov rdi, format
    mov rsi, cutPoint
    mov rax, 0
    call scanf
    pop rbp

original_print:
;   Prints the unedited msg and the original word
    mov rax, 1
    mov rdi, 1
    mov rsi, msg1
    mov rdx, len_msg1
    syscall

    mov rax, 1
    mov rdi, 1
    mov rsi, [origString]
    mov rdx, [origSize]
    syscall

    mov rax, 1
    mov rdi, 1
    mov rsi, newline
    mov rdx, 1
    syscall

split_word:
;   Now that everything is verified we can begin to reverse the string at that point
    mov rax, 1
    mov rdi, 1
    mov rsi, msg2
    mov rdx, len_msg2
    syscall

;   r8 holds the total number of characters and r11 holds the number we will split at
    xor r12, r12
    xor r13, r13
    xor r10, r10
    mov r12, 0
    mov r13, 0
    mov r10, 0
    mov r13, [origString]
    mov rsi, [origString]
    mov r11, [cutPoint]
    mov r8, [origSize]
    dec r11

get_to_split:
;   Get to the split point in the string
    cmp r12, r11
    je add_r12
    inc rsi
    inc r12
    jmp get_to_split

add_r12:
    inc r12

part_one:
;   swaps the location of the split point with the beginning of the string
;   if r13 is greater then rsi, then jump to next part to avoid swapping back locations
    cmp r13, rsi
    jge moveIndexes
    mov r10b, byte[rsi]
    mov al, byte[r13]
    mov [r13], r10b
    mov [rsi], al
    dec rsi
    dec r12
    inc r13
    jmp part_one

moveIndexes:
;   gets rsi to the beginning of the string, and r13 to the split point
    dec rsi
    dec r12
    inc r13
    cmp r12, 0
    je part_two
    jmp moveIndexes


part_two:
;   gets r12 to the end of the string
    cmp r12, r8
    je part_three
    inc rsi
    inc r12
    jmp part_two


part_three:
;   swaps the end of the string with the split point and stops when r13 passes rsi to avoid double swapping
    cmp r13, rsi
    jge final_print
    mov r10b, byte[rsi]
    mov al, byte[r13]
    mov [r13], r10b
    mov [rsi], al
    dec rsi
    inc r13 
    jmp part_three

final_print:
;   Prints the final output of the string reversed at the point
    mov rax, 1
    mov rdi, 1
    mov rsi, [origString]
    mov rdx, [origSize]
    syscall

    mov rax, 1
    mov rdi, 1
    mov rsi, newline
    mov rdx, 1
    syscall

    jmp end

stop:
    mov rax, 1
    mov rdi, 1
    mov rsi, msg1
    mov rdx, len_msg1
    syscall

end:
    mov rax, 0
    ret

