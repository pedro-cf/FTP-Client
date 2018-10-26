#include "rcom_util.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int findChar(char *str, char c) {
	int i;

	for (i = 0; i < strlen(str); i++) {
		if (str[i] == c){
			return i;
		}
	}

	return -1;
}

int findLastChar(char *str, char c) {
	int i, r = -1;

	for (i = 0; i < strlen(str); i++)
		if (str[i] == c)
			r = i;


	return r;
}

int findChar_ignore_2(char *str, char c) {
	int i, r = -1, count = 0;

	for (i = 0; i < strlen(str); i++) {
		if (str[i] == c) {
			count++;
			r = i;
			if (count > 2)
				break;
		}
	}


	return r;
}

int findCharCount(char *str, char c) {
	int i, r = 0;;

	for (i = 0; i < strlen(str); i++) {
		if (str[i] == c){
			r++;
		}
	}

	return r;
}

int fileExists(char *fname) {
	FILE *file;
	if ((file = fopen(fname, "r")) == NULL) {

	    return 0;
	} else {
	  fclose(file);
	}
	return 1;
}
