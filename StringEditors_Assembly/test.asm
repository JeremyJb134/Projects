; Adrian Maldonado - Jeremy Bruce - CMSC 313 FA23 - PROJECT 3
; Description:
; Asks the user for a string that is longer than 8. Then ask for a number between 2 and the length of the string.
; There will be error/input correction.  This number will be where the string will be split. Then display the original string
; Then display the new edited string that was divided in 2 pieces by the user input
extern printNum
    section .data

request1:       db  "Please enter the text: ", 10
len_request1:   equ $-request1

request2:       db  "Enter a number between two and the total number of characters in the string: ", 10
len_request2:   equ $-request2

msg1:           db "Unedited String:", 10
len_msg1:       equ $-msg1

msg2:           db "Edited String:", 10
len_msg2:       equ $-msg2

newline:        db      10

    section .bss

origString:     resb    99
otherString:    resb    99
editString:     resb    8
origSize:       resb    1
cutOffInt:      resb    3
cutPoint:       resb    1



    section .text

    global main

main:

get_string:
;   Writes out the starting message that requests a string from user and reads it in
    mov rax, 1
    mov rdi, 1
    mov rsi, request1
    mov rdx, len_request1
    syscall

    mov rax, 0
    mov rdi, 0
    mov rsi, origString
    mov rdx, 99
    syscall

;   Gets counter ready and stores pointer to word in rax register
    xor r8, r8
    mov rax, origString

string_loop:
;   Goes through the string letter by letter and uses a counter to check length
    cmp byte[rax], 10
    je check_string
    inc r8b
    inc rax
    jmp string_loop

check_string:
;   Error checks the string read in from the user
    cmp r8b, 8
    jl  get_string
    mov [origSize], r8b 

get_integer:
;   Writes out the next message that requests an integer from user and reads in their input
    mov rax, 1
    mov rdi, 1
    mov rsi, request2
    mov rdx, len_request2
    syscall

    mov rax, 0
    mov rdi, 0
    mov rsi, cutOffInt
    mov rdx, 3
    syscall


;   Clear the registers for use
    xor r9, r9
    xor r10, r10
    xor r11, r11
    xor rax, rax
    xor rsi, rsi
    mov rsi, cutOffInt
    mov r9, 0
    mov r10, 0
    mov rax, 0

;   Reads in the first place digit
    mov r9b, byte[rsi]
    sub r9b, 48

;   Reads in the next place digit
    inc rsi
    mov r10b, byte[rsi]

;   Check if the number entered is 2 digits by seeing if it is a new line charater
    cmp r10b, 10
    je ones_check
    jmp tens_check
    
tens_check:
;   If the number is a tens digit
    sub r10b, 48
    imul r9, r9, 10
    add al, r9b
    add al, r10b
    mov r11, rax
    jmp integer_check

ones_check:
;   If the number is a ones digit
    add al, r9b
    mov r11, rax
    jmp integer_check

integer_check:
;   Check if the number is in the range
    cmp r11, 1
    jle get_integer
    cmp r11, r8
    ja get_integer
    mov [cutPoint], r11b
    jmp original_print

original_print:
;   Prints the unedited msg and the original word
    mov rax, 1
    mov rdi, 1
    mov rsi, msg1
    mov rdx, len_msg1
    syscall

    mov rax, 1
    mov rdi, 1
    mov rsi, origString
    mov rdx, r8
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
    mov r13, otherString
    mov [editString], r13
    mov rsi, origString
    mov r11, [cutPoint]
    dec r11

get_to_split:
;   Get to the split point in the string
    cmp r12, r11
    je add_r12
    inc rsi
    inc r12
    jmp get_to_split

add_r12:
;   Increase r12 to fix missing first letter
    inc r12

part_one:
;   Adds the first part of the string
    mov r10, [rsi]
    mov [r13], r10b
    dec r12
    dec rsi
    inc r13
    cmp r12, 0
    je part_two
    jmp part_one

part_two:
;   Gets to the second part of the string
    cmp r12, r8
    je part_three
    inc rsi
    inc r12
    jmp part_two

part_three:
;   Adds the second part of the string by going from back to the cut off point
    mov r10, [rsi]
    mov [r13], r10b
    dec r12
    dec rsi
    inc r13
    cmp r12, r11
    je final_print
    jmp part_three

final_print:
;   Prints the final output of the string reversed at the point

    mov rax, 1
    mov rdi, 1
    mov rsi, [editString]
    mov rdx, r8
    syscall

    mov rax, 1
    mov rdi, 1
    mov rsi, newline
    mov rdx, 1
    syscall

exit:
;   End of the program
    mov rax, 60
    xor rdi, rdi
    syscall

