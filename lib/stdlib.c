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

int htoi(const char *hex) {
    if (hex[0] == '\0') {
        // Handle invalid input
        return 0;
    }

    int result = 0;
    int i = 0;

    // Skip any leading "0x" or "0X"
    if (hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X')) {
        i += 2;
    }

    while (hex[i] != '\0') {
        char currentChar = hex[i];

        // Convert hexadecimal character to integer
        int digit;
        if (currentChar >= '0' && currentChar <= '9') {
            digit = currentChar - '0';
        } else if (currentChar >= 'a' && currentChar <= 'f') {
            digit = 10 + (currentChar - 'a');
        } else if (currentChar >= 'A' && currentChar <= 'F') {
            digit = 10 + (currentChar - 'A');
        } else {
            // Invalid character encountered
            return 0;
        }

        // Update the result
        result = result * 16 + digit;

        // Move to the next character
        i++;
    }

    return result;
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

int toBCD(int num) {
	int tens_digit = num / 10;
	int ones_digit = num % 10;
	int bcd = (tens_digit << 4) | ones_digit;
	return bcd;
}

int fromBCD(int bcd) {
	int tens_digit = ((bcd & 0x70) >> 4) * 10;
	int ones_digit = bcd & 0x0F;
	int num = tens_digit + ones_digit;
	return num;
}

