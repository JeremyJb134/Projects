; String Manipulation in ASM and C - CMSC 313 - FA23 - Project 4 - Adrian Maldonado and Jeremy Bruce
;Jeremy Bruce, su18695
;Adrian Maldonado, fp93537
extern read
extern reverse
extern display
extern weave
extern printStats
extern freeList
extern printf          
extern scanf
extern printNum

section .data

menu:       db "Encryption menu options:", 10, "s - show current messages", 10, "r - read new message", 10, "e - transform", 10, "p - print stats", 10, "q - quit program", 10, "enter option letter -> ", 0
easterEgg:    db "/  イ             (((ヽ", 10
              db "(  ﾉ               ￣Ｙ＼", 10
              db "|  (\  (\    /)   |   )", 10
              db "ヽ ヽ  ( ͡° ͜ʖ ͡°) _ノ   /", 10
              db "  \ |    ⌒Ｙ⌒    /  /", 10
              db "  ｜ヽ    |     ﾉ |", 10
              db "    \ トー仝ーイ /", 10
              db "     |  ミ土彡  |", 10
              db "     ) \  °  / (", 10
              db "     (  \    / )", 10
              db "    /    /  \   \", 10
              db "   /  / /    \ \  \ ", 10
              db "  ( (  )      (  ) )", 10
              db "   (   )       (  ) ", 10
              db "   |   /       \  |", 10, 0
string1:   db "This is the original message.", 0
string2:   db "This is the original message.", 0
string3:   db "This is the original message.", 0
string4:   db "This is the original message.", 0
string5:   db "This is the original message.", 0
string6:   db "This is the original message.", 0
string7:   db "This is the original message.", 0
string8:   db "This is the original message.", 0
string9:   db "This is the original message.", 0
string10:   db "This is the original message.", 0
num1: dd 0
num2: dd 0
num3: dd 0
num4: dd 0
num5: dd 0
num6: dd 0
num7: dd 0
num8: dd 0
num9: dd 0
num10: dd 0

goodbye: db "Goodbye!", 10, 0

stringfmt:  db "%s", 0 
badInput:   db "Invalid option! Try again!", 10, 0

section .bss

arr         resq 10
memTracker  resq 10
input       resb 2
extraTrac   resb 4
tracker     resb 4

section .text

global main

main:
    xor r8, r8

init:
    ;intializes the string array and the memory tracker array with strings, and ints
    mov qword[arr+r8], string1
    mov qword[memTracker+r8], num1
    add r8, 8
    mov qword[arr+r8], string2
    mov qword[memTracker+r8], num2
    add r8, 8
    mov qword[arr+r8], string3
    mov qword[memTracker+r8], num3
    add r8, 8
    mov qword[arr+r8], string4
    mov qword[memTracker+r8], num4
    add r8, 8
    mov qword[arr+r8], string5
    mov qword[memTracker+r8], num5
    add r8, 8
    mov qword[arr+r8], string6
    mov qword[memTracker+r8], num6
    add r8, 8
    mov qword[arr+r8], string7
    mov qword[memTracker+r8], num7
    add r8, 8
    mov qword[arr+r8], string8
    mov qword[memTracker+r8], num8
    add r8, 8
    mov qword[arr+r8], string9
    mov qword[memTracker+r8], num9
    add r8, 8
    mov qword[arr+r8], string10
    mov qword[memTracker+r8], num10

    ;puts 0 in the tracker for encryption and tracker for easter egg
    mov dword[tracker], 0
    mov dword[extraTrac], 0

    xor rdi, rdi
    xor rsi, rsi

getInput:
    
    ;prints menu screen and call scanf to get input
    mov rdi, stringfmt
    mov rsi, menu
    xor rax, rax
    call printf
    
    push rbp
    mov rdi, stringfmt
    mov rsi, input
    mov rax, 0
    call scanf
    pop rbp

    xor r9, r9
    mov r9b, [input]

    ;if it equals z increment extra tracker, if not set extra tracker to 0
    cmp r9b, 'z'
    je easter

    cmp r9b, 'z'
    jne easterStop

continue:
    ; jumps to respective function based on the input
    cmp r9b, 's'
    je show

    cmp r9b, 'S'
    je show

    cmp r9b, 'r'
    je readInput

    cmp r9b, 'R'
    je readInput

    cmp r9b, 'e'
    je getRandom

    cmp r9b, 'E'
    je getRandom
    
    cmp r9b, 'p'
    je stats

    cmp r9b, 'P'
    je stats

    cmp r9b, 'q'
    je end
    cmp r9b, 'Q'
    je end

retry:
    ;if input was not one of the options, then go back to menu
    mov rdi, badInput
    mov rsi, stringfmt
    xor rax, rax
    call printf
    jmp getInput

show:
    ;puts arr in rdi and then calls print
    mov rdi, arr
    call display
    jmp getInput

readInput:
    ;passes string array and memory tracker array and calls read
    push rbp
    mov rdi, arr
    mov rsi, memTracker
    call read
    pop rbp
    jmp getInput

stats:
    ;passes array and then calls printStats
    push rbp
    mov rdi, arr
    call printStats
    pop rbp
    jmp getInput

getRandom:
    ;uses the clock tracker to get a random number that is stored in rax
    rdtsc
    ;shifts rax once to the right, if there is a carry then call weave, if not then call reverse
    shr rax, 1
    jnc weaver
    jmp reverser

weaver:
    ;passes tracker and array to weave function and then increments tracker 
    xor r8, r8
    xor r15, r15
    mov r8d, dword[tracker]
    mov r15d, dword[tracker]
    mov rdi, arr
    mov rsi, [tracker]
    call weave
    inc r15
    ;if tracker is 10, so end of string list, then set tracker to 0
    cmp r15, 10
    je setZero
    mov [tracker], r15d
    jmp getInput

reverser:   
    ;passes the string itself to reverse function, by incrementing the array based on the tracker
    xor r8, r8
    xor r15, r15
    mov r8d, dword[tracker]
    mov r15d, dword[tracker]
    imul r8, r8, 8
    mov rdi, qword[arr+r8]
    push rbp
    call reverse
    pop rbp
    ;if tracker is 10, so end of string list, then set tracker to 0
    inc r15
    cmp r15, 10
    je setZero
    mov [tracker], r15d
    jmp getInput

setZero:
    ;sets tracker to 0 after all strings have been encrypted
    xor r15, r15
    mov [tracker], r15
    mov r15, [tracker]
zeroSet:
    jmp getInput

easter:
    ;increments the z tracker by 1, and if the tracker is 4 then go to easterTime
    xor r14, r14
    mov r14d, dword[extraTrac]
    inc r14
    cmp r14, 4
    je easterTime
    mov [extraTrac], r14d
    jmp retry

easterStop:
    ;sets tracker to 0
    mov dword[extraTrac], 0
    jmp continue

easterTime:
    ;prints the easter message 
    mov rdi, stringfmt
    mov rsi, easterEgg
    xor rax, rax
    call printf
    jmp easterStop

end:
    ;moves array and memorytracker to free the memory if necessary
    mov rdi, arr
    mov rsi, memTracker
    call freeList

    mov rdi, goodbye
    mov rsi, stringfmt
    xor rax, rax
    call printf

    mov rax, 0
    ret
