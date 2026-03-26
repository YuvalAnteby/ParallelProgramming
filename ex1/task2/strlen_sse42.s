.section .data

.section .text
.globl strlen_sse42
.type strlen_sse42, @function

strlen_sse42:
    # Save the original pointer in %rax (used to iterate)
    mov %rdi, %rax
    # Clear %xmm0 to hold 16 null bytes using XOR
    pxor %xmm0, %xmm0        

.Lloop_sse42:
    # Compare 16 bytes of memory at %rax against %xmm0
    # $0x08: Equal Each, Unsigned Bytes
    pcmpistri $0x08, (%rax), %xmm0
    
    # The Zero Flag is on if a null byte was found in the loaded memory segment
    # The index of the first match (null byte) is placed in %rcx
    jz .Lfound_sse42

    # Advance pointer by 16 bytes
    add $16, %rax            
    # Go to next 16 byte chunk
    jmp .Lloop_sse42         

.Lfound_sse42:
    # Add the index of the null byte to the current pointer
    add %rcx, %rax           
    # Subtract the original base pointer to get the exact length
    sub %rdi, %rax           
    ret
   