#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <string.h>
#include <errno.h>

#define CUSTOM_POS 4

size_t ascciToNumber_size(char number){
	char nums[10] = {
			 '0', '1', '2', '3', '4',
	       		 '5', '6', '7', '8', '9'
			};
	for(size_t i=0; i<10; i++){
		if(number == nums[i]){
			return i;
		}
	}
	printf("\n[!] An errer occured in ascciToNumber_size(), results may be wrong!");
	return -1;
}

off_t ascciToNumber_off(char number){
        char nums[10] = {
                         '0', '1', '2', '3', '4',
                         '5', '6', '7', '8', '9'
                        };
        for(off_t i=0; i<10; i++){
                if(number == nums[i]){
                        return i;
                }
        }
        printf("\n[!] An errer occured in ascciToNumber_off(), results may be wrong!");
        return -1;
}

off_t getOffset(char *_argv[]){/*cell 3*/
	off_t ret = 0;
	size_t digits = 0;

	/*
         * Get amount of digits for charArray
         */
        while(*_argv[3] != '\0'){
                digits++;
                ++_argv[3];
        }

	/* 
         * declair charArray and reset the position of _argv[3]
         */
        char charArray[digits];
        _argv[3] -= digits;

        /*
         * Fill charArray with digit values
         * obtained from char count of
         * _argv[3]
         */
        for(int i=0; i<digits; i++){
                charArray[i] = *_argv[3];
                ++_argv[3];
        }

        /* 
         * Convert chars into usable numbers 
         */
        off_t base = 1;
        for(int i=digits-1; i>-1; i--){
                ret += ascciToNumber_off(charArray[i]) * base;
                base = base * 10;
        }
        return ret;
}

size_t getBufSize(char *_argv[]){/*cell 2*/
	size_t ret = 0;
	size_t digits = 0;

	/* 
	 * Get amount of digits for charArray
	 */
	while(*_argv[2] != '\0'){
		digits++;
		++_argv[2];
	}

	/* 
	 * declair charArray and reset the position of _argv[2]
	 */
	char charArray[digits];
	_argv[2] -= digits;
	
	/*
	 * Fill charArray with digit values
	 * obtained from char count of
	 * _argv[2]
	 */	
	for(int i=0; i<digits; i++){
		charArray[i] = *_argv[2];
		++_argv[2];
	}		

	/* 
	 * Convert chars into usable numbers 
	 */
	size_t base = 1;
	for(int i=digits-1; i>-1; i--){
		ret += ascciToNumber_size(charArray[i]) * base;
		base = base * 10;
	}
	return ret;
}

void fillBuf(char *_buffer, size_t _bufSize){
	for(int i=0; i<_bufSize; i++)
		_buffer[i] = 'p';
}

int main(int argc, char *argv[]){
	if(argc < 3 || argc > 4 || argv[1] == "--help"){
		printf("Usage: %s, [File Name] [amount of bytes] [file position (optional)]\n", argv[0]);
		return EINVAL;
	}
	int fd;
	ssize_t status;
	struct stat st;
	off_t offset;

	/* 
	 * check if file exists, if not; make it 
	 */
	if((fd = open(argv[1], O_EXCL | O_CREAT | O_RDWR, S_IRUSR | S_IWUSR)) == -1){
		printf("\n[File %s exists]", argv[1]);
		if((fd = open(argv[1], O_RDWR)) == -1){
			printf("\nFailed to open file");
			return EBADF;
		}else{
			stat(argv[1], &st);
			offset = st.st_size;
		}
	}else{
		offset = 0;
	}

	/* 
	 * check if arguments have requested a write 
	 * relative to a specific position and change
	 * the offset to the custom offset.
	 */
	if(argc == CUSTOM_POS){
		offset = getOffset(argv);
		printf("custom offset: %ld", (long)offset);
	}	
	
	/* 
	 * fill buffer and write x amount 
	 * of 'p' chars to file 
	 */
	size_t bufSize = getBufSize(argv);
	char buffer[bufSize];

	fillBuf(buffer, bufSize); /*fills buffer with bufSize amounts of 'p'*/
	
	status = pwrite(fd, buffer, bufSize, offset);

	if(status != bufSize){
		printf("\nFailed to write some or all of the buffer!");
		close(fd);
		return EIO;
	}else{
		printf("Write succesful!");
		close(fd);
	}
	return EXIT_SUCCESS;
}
