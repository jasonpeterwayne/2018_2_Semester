.data
digit:   .word 10, 12, 23, 28, 7, 39, 10, 11, 23, 12, 3, 4, 5, 1, 34, 17, 0, 5, 24
length:   .word 19 # the length of the digit list
integer:   .word 1

str0: .asciiz "\n"
str1: .asciiz "sum is : "
str2: .asciiz "max is : "
str3: .asciiz "min is : "

.text
main:
  la $s0, digit
  lw $t1, integer # digit[0] + (sum of digit[1~18])
  lw $t2, length
  lbu $s2, 0($s0) # max, min = digit[0]


Loop: beq $t1, $t2, Exit # if(integer == length) go to Exit
      sll $t3, $t1, 2 # t3 = t1 * 4
      addu $t3, $t3, $s0 # t3 = digit[t3]
      lw $t4, 0($t3) # t4 = digit[int]
      addu $s1, $s1, $t4 # s1 += t4
      addi $t1, $t1, 1 # t1 = t1 + 1
      
      sltu $t5, $s2, $t4 # max case(s2<digit[i]) t5=1(max<digit[i])
      bne $t5, $zero, Max # if(t5!=0) go to max
      sltu $t6, $t3, $s2 # min case(digit[i]<s2) t5=1(min>digit[i])
      bne $t6, $zero, Min # if(t5!=0) go to min
      j Loop

Max: addu $s3, $t4, $zero
     j Loop

Min: addu $s4, $t3, $zero
     j Loop

Exit: 
      li $v0, 4
      la $a0, str1 #print "sum is"
      syscall

      li $v0, 1
      add $a0, $s1, $s2 #sum
      syscall
      
      li $v0, 4
      la $a0, str0 #change line "\n"
      syscall

      li $v0, 4
      la $a0, str2 #print "max is"
      syscall

      li $v0, 1
      la $a0, ($s3) #max
      syscall

      li $v0, 4
      la $a0, str0 #change line "\n"
      syscall

      li $v0, 4
      la $a0, str3 #print "min is"
      syscall

      li $v0, 1
      la $a0, ($s4) #min
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
# for(int i=1; i<19; i++) {
#    sum = sum + digit[i];
#    if(max < digit[i+1]) max = digit[i+1];
#    if(min > digit[i+1]) min = digit[i+1];
# }
#
# print(sum = %d);
# print(max = %d);
# print(min = %d);

###########################################################