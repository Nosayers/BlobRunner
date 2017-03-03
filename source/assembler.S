#   MIPS assembly
#   delay function written according to instructions in Lab 1
#   Lab 1 skeleton file was written by F Lundevall 

	.data
	.align 2
	.text

# As per instruction, should enable interrupts globally 
.global enable_interrupts
enable_interrupts:
    ei
    jr  $ra
    nop

# delay(int ms)
.global delay
delay:
	li $t5, 0xaa67	#CONSTANT:  used in for loop to get the delay. It was requested easy to change, so put at top
	
	#t0 starts at 0 and counts until it reaches a0, the argument of the function
	li $t0, 0x0
	mainloop:
	bne $a0,$t0,mainloopbody	#branch if t0 hasnt reached a0 yet to the while loop content
	nop

	#after while loop is done, go back to return adress
	jr $ra
	nop
	
		mainloopbody:
		#ms = ms - 1
		addi $t0,$t0,1	#increase loop counter t0
	
			#for loop
			li $t1, 0x0	#initialize counter t1
			forloop:
			bne $t1,$t5,forloopbody #always branch to forloopbody until t1 reaches t5 (4711)
			nop
			#after for loop is done, jump back to main "while" loop
			j mainloop
			nop
			
				forloopbody:
				addi $t1,$t1, 1	#increase counter
				j forloop	#return to loop
				nop
