/*
 * trim.c
 *
 *  Created on: 2014. 12. 8.
 *      Author: GoldSunda
 */

#include <stdio.h>
#include <ctype.h> // isspace()
#include <string.h> // strcpy_s() strlen()
#define MAX_STR_LEN 4000

// 문자열 우측 공백문자 삭제 함수
char *ltrim(char *s) {
	while (isspace(*s))
		s++;
	return s;
}

char *rtrim(char *s) {
	char* back;
	int len = strlen(s);

	if (len == 0)
		return (s);

	back = s + len;
	while (isspace(*--back))
		;
	*(back + 1) = '\0';
	return s;
}

char *trim(char *s) {
	return rtrim(ltrim(s));
}

