## Benchmarking the LSFR for TK2

### Comparison

The LSFR for TK2 was implemented in 3 ways. The first one is the most logical one, it expresses the LSFR on 16 4-bit
cells placed one after another packed in a uint64. The result is then the LSFR applied to all packed 4-bit cells. The
second and third one perform the LSFR where the TK2 state is bit sliced. This removes the need for dependent logical
operations that need to wait until the previous 'gate' is completed, because all equally-significant bits of every slices
are now packed in the same 16-bit raw. So now we only need to perform 1 rol and 1 xor instruction. This significant
gain can primarily be seen in the second row's implementation, where we need only 4 dependant instructions, instead of
10!

<table>

<tr>
<th>64-bit x86</th>
<th>Source Code</th>
</tr>

<tr>
<td>

```assembly
0000000000001770 <_Z16sequential_abcdem>:
    1770:	f3 0f 1e fa          	endbr64 
    1774:	48 ba ee ee ee ee ee 	movabs rdx,0xeeeeeeeeeeeeeeee
    177b:	ee ee ee 
    177e:	48 8d 04 3f          	lea    rax,[rdi+rdi*1]
    1782:	48 21 d0             	and    rax,rdx
    1785:	48 89 fa             	mov    rdx,rdi
    1788:	48 c1 ef 02          	shr    rdi,0x2
    178c:	48 c1 ea 03          	shr    rdx,0x3
    1790:	48 31 d7             	xor    rdi,rdx
    1793:	48 ba 11 11 11 11 11 	movabs rdx,0x1111111111111111
    179a:	11 11 11 
    179d:	48 21 d7             	and    rdi,rdx
    17a0:	48 09 f8             	or     rax,rdi
    17a3:	c3                   	ret    
    17a4:	66 66 2e 0f 1f 84 00 	data16 nop WORD PTR cs:[rax+rax*1+0x0]
    17ab:	00 00 00 00 
    17af:	90                   	nop
```

</td>
<td>

```cpp
u64 sequential_abcde(u64 state) {
	state = ((state << 1) & 0xEEEEEEEEEEEEEEEE)
	        ^
	        (((state >> 3) ^ (state >> 2)) & 0x1111111111111111);
	return state;
}
```

</td>

</tr>
<tr>
<td>

```assembly
00000000000017b0 <_Z11sliced_fghim>:
    17b0:	f3 0f 1e fa          	endbr64 
    17b4:	48 c1 c7 10          	rol    rdi,0x10
    17b8:	48 89 f8             	mov    rax,rdi
    17bb:	48 c1 e8 30          	shr    rax,0x30
    17bf:	48 31 f8             	xor    rax,rdi
    17c2:	c3                   	ret    
    17c3:	66 66 2e 0f 1f 84 00 	data16 nop WORD PTR cs:[rax+rax*1+0x0]
    17ca:	00 00 00 00 
    17ce:	66 90                	xchg   ax,ax
```

</td>
<td>

```cpp
u64 sliced_fghi(u64 state) {
	state = _lrotl(state, 0x10);
	state ^= (state & 0xFFFF000000000000) >> 0x30;
	return state;
}
```

</td>

</tr>
<tr>
<td>

```assembly
00000000000017d0 <_Z16sliced_simd_jklm18State64Sliced_16_t>:
    17d0:	f3 0f 1e fa          	endbr64 
    17d4:	48 83 ec 38          	sub    rsp,0x38
    17d8:	48 89 7c 24 08       	mov    QWORD PTR [rsp+0x8],rdi
    17dd:	0f 70 44 24 08 93    	pshufw mm0,QWORD PTR [rsp+0x8],0x93
    17e3:	64 48 8b 04 25 28 00 	mov    rax,QWORD PTR fs:0x28
    17ea:	00 00 
    17ec:	48 89 44 24 28       	mov    QWORD PTR [rsp+0x28],rax
    17f1:	31 c0                	xor    eax,eax
    17f3:	48 0f 7e c0          	movq   rax,mm0
    17f7:	89 c2                	mov    edx,eax
    17f9:	48 c1 e8 30          	shr    rax,0x30
    17fd:	49 0f 7e c0          	movq   r8,mm0
    1801:	31 d0                	xor    eax,edx
    1803:	0f 7f 44 24 18       	movq   QWORD PTR [rsp+0x18],mm0
    1808:	0f 7f 44 24 10       	movq   QWORD PTR [rsp+0x10],mm0
    180d:	66 41 89 c0          	mov    r8w,ax
    1811:	48 8b 44 24 28       	mov    rax,QWORD PTR [rsp+0x28]
    1816:	64 48 33 04 25 28 00 	xor    rax,QWORD PTR fs:0x28
    181d:	00 00 
    181f:	75 08                	jne    1829 <_Z16sliced_simd_jklm18State64Sliced_16_t+0x59>
    1821:	4c 89 c0             	mov    rax,r8
    1824:	48 83 c4 38          	add    rsp,0x38
    1828:	c3                   	ret    
    1829:	e8 12 f9 ff ff       	call   1140 <__stack_chk_fail@plt>
    182e:	66 90                	xchg   ax,ax
```

</td>
<td>

```cpp
State64Sliced_16_t sliced_simd_jklm(State64Sliced_16_t state) {
	state.m64state = _mm_shuffle_pi16(state.m64state, 0b10010011);
	state.slices[0] ^= state.slices[3];
	return state;
}
```

</td>

</tr>
</table>


