# Atomica
This program uses pwrite() to write to a file relative to a user-specified offset. It tries to utilize atomicity to append data to a file.

The data that it appends is hard-coded to be 'p'. You can change this program in anyway you want as long as it helps you better your understanding of computer programming :)


Known bugs:
1) When you specify an offset greater than the amount of data already available in the file, there will be a weird data hole. The planned fix involves accounting for file size and then appending 0's where the hole may be. 
