.data
digit:   .word 10, 12, 23, 28, 7, 39, 10, 11, 23, 12, 3, 4, 5, 1, 34, 17, 0, 5, 24
length:   .word 19 # the length of the digit list

.text
.global main
main:
  la $s1, digit
  lw $t0, length 
  li $s2, $zero # int i = 0
  li $s3, $zero # int sum = 0
  lw $s4, 0($s1) # int max = digit[0]
  lw $s5, 0($s1) # int min = digit[0]

Loop: sll $t1, $s2, 2 # t1 = i * 4
      beq $t1, $t0, Exit # if(t1== length) go to Exit
      lw $t1, ($t1) # t1 = digit[t1] 
      sw $t1, ($s3) # sum = t1
      addi $s2, $s2, 1 # t1 = t1 + 1
      j Loop


Exit: li $v0, 1
      lw $a0, 0($s3)
      syscall

      li $v0, 4
      la $a0, str
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