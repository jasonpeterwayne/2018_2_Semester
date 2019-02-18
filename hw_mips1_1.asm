.data
digit:   .word 10, 12, 23, 28, 7, 39, 10, 11, 23, 12, 3, 4, 5, 1, 34, 17, 0, 5, 24
length:   .word 19 # the length of the digit list
integer: .word 0

str1: .asciiz "sum is : "
.text
main:
  la $s0, digit
  lw $t1, length
  lw $t2, int 

loop: beq $t2, $t1, exit # if(t2== length) go to Exit
      sll $t3, $t2, 2 # t3 = t2 * 4
      addu $t3, $t3, $s0 # t3 = digit[t2]
      lw $s1, 0($t1) # s1 = digit[int] 
      addu $s2, $s2, $s1 # s2 += s1
      addi $t2, $t2, 1 # t2 = t2 + 1
      j loop


exit: li $v0, 4
      la $a0, str1
      syscall

      li $v0, 1
      la $a0, 0($s2)
      syscall

         

# HERE, implement mips code
# to get the sum, max, and min of the ‘digit’ list above
# and to print the results (sum, max, and min)

# the printing format should be as follows:
# sum is xxx
# max is yyy
# min is zzz

.end

###########################################################

# In C
#
# int i, sum, max, min;
# sum = 0;
# max = digit[0];
# min = digit[0];
#
# for(int i=0; i<19; i++) {
#    sum = sum + digit[i];
#    if(max < digit[i+1]) max = digit[i+1];
#    if(min > digit[i+1]) min = digit[i+1];
# }
#
# print(sum = %d);
# print(max = %d);
# print(min = %d);

###########################################################