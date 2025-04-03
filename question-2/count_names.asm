section .data
    filename db "names.txt", 0          ; File name to open
    err_msg db "Error opening file", 10 ; Error message
    err_len equ $ - err_msg
    buffer_size equ 1024                ; Buffer size for reading

section .bss
    buffer resb buffer_size            ; Buffer to store file contents
    fd resd 1                          ; File descriptor
    count resd 1                       ; Counter for names

section .text
    global _start

_start:
    ; Open the file
    mov eax, 5                         ; sys_open syscall
    mov ebx, filename                  ; Pointer to filename
    mov ecx, 0                         ; Read-only mode
    mov edx, 0                         ; No special permissions
    int 0x80                           ; Make syscall
    mov [fd], eax                      ; Store file descriptor

    ; Check for error
    cmp eax, 0
    jl error_exit

    ; Initialize counter
    mov dword [count], 0

read_loop:
    ; Read from file
    mov eax, 4                         ; sys_read syscall
    mov ebx, [fd]                      ; File descriptor
    mov ecx, buffer                    ; Buffer to read into
    mov edx, buffer_size               ; Number of bytes to read
    int 0x80                           ; Make syscall

    ; Check if we've reached EOF
    cmp eax, 0
    jle close_file

    ; Process the buffer
    mov esi, buffer                    ; Point to buffer start
    mov ecx, eax                       ; Bytes read
    xor ebx, ebx                       ; Clear ebx for char processing

count_names:
    lodsb                              ; Load byte from buffer
    cmp al, 10                         ; Check for newline
    jne next_char                      ; If not newline, continue
    inc dword [count]                  ; Increment name counter

next_char:
    loop count_names                   ; Continue until all bytes processed
    jmp read_loop                      ; Read next chunk

close_file:
    ; Close the file
    mov eax, 6                         ; sys_close syscall
    mov ebx, [fd]                      ; File descriptor
    int 0x80                           ; Make syscall

    ; Convert count to string
    mov eax, [count]                   ; Load count
    mov ebx, 10                        ; Base 10
    mov ecx, buffer + buffer_size - 1  ; Point to end of buffer
    mov byte [ecx], 10                 ; Add newline
    dec ecx

convert_loop:
    xor edx, edx                       ; Clear edx for division
    div ebx                            ; Divide by 10
    add dl, '0'                        ; Convert remainder to ASCII
    mov [ecx], dl                      ; Store digit
    dec ecx                            ; Move buffer pointer
    test eax, eax                      ; Check if quotient is 0
    jnz convert_loop                   ; Continue if not

    ; Write count to console
    mov eax, 4                         ; sys_write syscall
    mov ebx, 1                         ; stdout
    inc ecx                            ; Point to start of number
    mov edx, buffer + buffer_size      ; End of buffer
    sub edx, ecx                       ; Calculate length
    int 0x80                           ; Make syscall

exit:
    ; Exit program
    mov eax, 1                         ; sys_exit syscall
    xor ebx, ebx                       ; Return code 0
    int 0x80                           ; Make syscall

error_exit:
    ; Print error message
    mov eax, 4                         ; sys_write syscall
    mov ebx, 2                         ; stderr
    mov ecx, err_msg                   ; Error message
    mov edx, err_len                   ; Message length
    int 0x80                           ; Make syscall
    mov eax, 1                         ; sys_exit
    mov ebx, 1                         ; Return code 1
    int 0x80                           ; Make syscall
