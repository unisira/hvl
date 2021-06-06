.code

asm_vmwrite proc
    vmwrite rcx, rdx
    pushfq
    pop rax
    ret
asm_vmwrite endp

asm_vmread proc
    vmread [rdx], rcx
    pushfq
    pop rax
    ret
asm vmread endp

end
