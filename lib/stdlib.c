#include <stdlib.h>
#include <ctype.h>

int atoi(const char *s)
{
	int res = 0;
	char sign = ' ';

	while (isspace(*s)) {
		s++;
	}

	if (*s == '-' || *s == '+') {
		sign = *s;
		s++;
	}

	while ('0' <= *s && *s <= '9') {
		res = res * 10 + (*s - '0');
		s++;

	}

	if (sign == '-') {
		res = res * -1;
	}

	return res;
}

char* itoa(int i, char* buffer)
{
	int pos = 0;
	int rem = 0;
	char temp = 0;
	int negativeFlag = 0;
	if( i < 0){
		negativeFlag = 1;
		i = -i;
	}

	if(i == 0){
		buffer[pos++] = '0';
		buffer[pos] = '\0';
		return buffer;
	}

	while( i != 0)
	{
		rem = i % 10;
		buffer[pos++] = rem + '0';
		i /= 10;
	}

	if(negativeFlag)
	{
		buffer[pos++] = '-';
	}
	
	for(int j = 0; j < pos / 2; j++)
	{
		temp = buffer[j];
		buffer[j] = buffer[pos - 1 - j];
		buffer[pos - 1 - j] = temp;
	}
	buffer[pos] = '\0';
	return buffer;
}

int isdigit(char c){
	switch(c){
		case '0': 
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			return 1;
			break;
		default:
			return 0;
			break;
	}
}

