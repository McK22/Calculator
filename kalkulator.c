/***************************************************
*												   *
*	Kalkulator na zaliczenie projektu              *
*	z przedmiotu Programowanie strukturalne        *
*                                                  *
*	Autor: Maciej Pluta                            *
*                                                  *
****************************************************/

#define _CRT_SECURE_NO_DEPRECATE
#define FILE_NAME_LENGTH 20
#define DIV_RESULT 0
#define DIV_REMAINDER 1
#define STEP 10000


#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

int powerDec(int a, int b)
{
	if (b == 0)
		return 1;
	if (b % 2 == 1)
		return a * powerDec(a, b - 1);
	int p = powerDec(a, b / 2);
	return p * p;
}

struct IntArr
{
	int* arr;
	int length;
};

void removeLeadingZeros(struct IntArr* n)
{
	int toRemove, newLength;
	int* newArr;
	if (n->length == 1)
		return;
	toRemove = 0;
	for (int i = n->length - 1; i >= 0; i--)
	{
		if (n->arr[i] == 0)
			toRemove++;
		else
			break;
	}
	if (toRemove == 0)
		return;
	if (toRemove == n->length)
		toRemove--;
	newLength = n->length - toRemove;
	newArr = calloc(newLength, sizeof(int));
	for (int i = 0; i < newLength; i++)
		newArr[i] = n->arr[i];
	free(n->arr);
	n->arr = newArr;
	n->length = newLength;
}

bool isFirstBigger(struct IntArr first, struct IntArr second)
{
	if (first.length > second.length)
		return true;
	else if (first.length < second.length)
		return false;
	for (int i = first.length - 1; i >= 0; i--)
	{
		if (first.arr[i] > second.arr[i])
			return true;
		else if (first.arr[i] < second.arr[i])
			return false;
	}
	return false;
}

bool isFirstBiggerOrEqual(struct IntArr first, struct IntArr second)
{
	if (first.length > second.length)
		return true;
	else if (first.length < second.length)
		return false;
	for (int i = first.length - 1; i >= 0; i--)
	{
		if (first.arr[i] > second.arr[i])
			return true;
		else if (first.arr[i] < second.arr[i])
			return false;
	}
	return true;
}

int toDec(struct IntArr n, int base)
{
	int result = 0;
	for (int i = 0; i < n.length; i++)
		result += n.arr[i] * powerDec(base, i);
	return result;
}

char* copystr(char* source, int length)
{
	char* result = calloc(length, sizeof(char));
	for (int i = 0; i < length; i++)
		result[i] = source[i];
	return result;
}

struct IntArr copyInt(struct IntArr n)
{
	struct IntArr result;
	result.length = n.length;
	result.arr = calloc(result.length, sizeof(int));
	for (int i = 0; i < result.length; i++)
		result.arr[i] = n.arr[i];
	return result;
}

char* append(char* first, char* second, int firstLength, int count, bool addZeroChar)
{
	char* result = calloc(firstLength + count + addZeroChar, sizeof(char));
	for (int i = 0; i < firstLength; i++)
		result[i] = first[i];
	for (int i = 0; i < count; i++)
		result[i + firstLength] = second[i];
	if (addZeroChar)
		result[firstLength + count] = '\0';
	return result;
}

char* readLine(FILE* fp)
{
	char s[STEP + 1];
	int currentLength = 0;
	int endOfLineIndex = -1;
	char* result = calloc(0, sizeof(char));

	if (fgets(s, STEP + 1, fp) == NULL) //end of file
		return NULL;
	for (int i = 0; i < STEP; i++)
	{
		if (s[i] == '\n')
		{
			endOfLineIndex = i;
			break;
		}
	}

	while (endOfLineIndex == -1)
	{
		char* buff = append(result, s, currentLength, STEP, false);
		free(result);
		currentLength += STEP;
		result = calloc(currentLength, sizeof(char));
		for (int i = 0; i < currentLength; i++)
			result[i] = buff[i];
		free(buff);

		if (fgets(s, STEP + 1, fp) == NULL)
			return NULL;

		for (int i = 0; i < STEP; i++)
		{
			if (s[i] == '\n')
			{
				endOfLineIndex = i;
				break;
			}
		}
	}
	char* buff = append(result, s, currentLength, endOfLineIndex, true);
	free(result);
	currentLength += endOfLineIndex + 1;
	result = calloc(currentLength, sizeof(char));
	for (int i = 0; i < currentLength; i++)
		result[i] = buff[i];
	free(buff);
	return result;
}

struct IntArr convertToInt(char* s)
{
	struct IntArr result;
	result.length = strlen(s);
	result.arr = calloc(result.length, sizeof(int));

	for (int i = 0; i < result.length; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
			result.arr[result.length - i - 1] = s[i] - '0';
		else
			result.arr[result.length - i - 1] = s[i] - 'A' + 10;
	}
	return result;
}

bool isOperationCorrect(char* s)
{
	if (s[0] == '+' || s[0] == '*' || s[0] == '^' || s[0] == '/' || (s[0] >= '2' && s[0] <= '9'))
	{
		if (s[1] != ' ')
			return false;
		if (((s[2] >= '2' && s[2] <= '9') || (s[2] == '1' && s[3] >= '0' && s[3] <= '6')) && s[4] == '\0')
			return true;
		return false;
	}
	if (s[0] < '1' || s[0] > '9')
		return false;
	if (s[1] < '0' || s[1] > '6')
		return false;
	if (s[2] != ' ')
		return false;
	if (((s[3] >= '2' && s[3] <= '9') || (s[3] == '1' && s[4] >= '0' && s[4] <= '6')) && s[5] == '\0')
		return true;
	return false;
}

bool isNumberCorrect(char* s, int base)
{
	int length = strlen(s);
	if (length == 0)
		return false;
	for (int i = 0; i < length; i++)
	{
		if (s[i] >= '0' && s[i] <= '9')
		{
			if (s[i] - '0' >= base)
				return false;
		}else if (s[i] >= 'A' && s[i] <= 'F')
		{
			if (s[i] - 'A' + 10 >= base)
				return false;
		}else
			return false;
	}
	return true;
}

char* convertToString(struct IntArr n)
{
	char* result = calloc(n.length + 1, sizeof(char));

	for (int i = 0; i < n.length; i++)
	{
		if (n.arr[i] >= 0 && n.arr[i] <= 9)
			result[n.length - i - 1] = '0' + n.arr[i];
		else
			result[n.length - i - 1] = 'A' + n.arr[i] - 10;
	}
	result[n.length] = '\0';
	return result;
}

struct IntArr add(struct IntArr n1, struct IntArr n2, int base)
{
	struct IntArr result;
	int overflow = 0;
	result.length = max(n1.length, n2.length) + 1;
	result.arr = calloc(result.length + 1, sizeof(int));
	for (int i = 0; i < result.length; i++)
	{
		int sum = overflow;
		overflow = 0;
		if (i < n1.length)
			sum += n1.arr[i];
		if (i < n2.length)
			sum += n2.arr[i];
		if (sum >= base)
		{
			sum -= base;
			overflow++;
		}
		result.arr[i] = sum;
	}
	result.arr[result.length] = overflow;
	return result;
}

struct IntArr subtract(struct IntArr n1, struct IntArr n2, int base)
{
	struct IntArr result;
	result.length = n1.length;
	result.arr = calloc(result.length, sizeof(int));
	for (int i = 0; i < n1.length; i++)
	{
		if (i >= n2.length)
		{
			result.arr[i] = n1.arr[i];
			continue;
		}
		if (n2.arr[i] <= n1.arr[i])
			result.arr[i] = n1.arr[i] - n2.arr[i];
		else
		{
			for (int j = i + 1; j < n1.length; j++)
			{
				if (n1.arr[j] > 0)
				{
					n1.arr[j]--;
					break;
				} else
					n1.arr[j] = base - 1;
			}
			result.arr[i] = n1.arr[i] + base - n2.arr[i];
		}
	}
	return result;
}

struct IntArr multiply(struct IntArr n1, struct IntArr n2, int base)
{
	struct IntArr result;
	int overflow;

	result.length = n1.length + n2.length;
	result.arr = calloc(result.length, sizeof(int));

	for (int i = 0; i < n1.length; i++)	
		for (int j = 0; j < n2.length; j++)
		{
			result.arr[i + j] += (n1.arr[i] * n2.arr[j]) % base;
			result.arr[i + j + 1] += (n1.arr[i] * n2.arr[j]) / base;
		}
	
	overflow = 0;
	for (int i = 0; i < result.length; i++)
	{
		result.arr[i] += overflow;
		overflow = result.arr[i] / base;
		result.arr[i] %= base;
	}
	return result;
}

struct IntArr power(struct IntArr n1, int second, int base)
{
	if (second == 0)
	{
		struct IntArr result;
		result.length = 1;
		result.arr = calloc(result.length, sizeof(int));
		result.arr[0] = 1;
		return result;
	}
	struct IntArr result, p;
	if (second % 2 == 1)
	{
		p = power(n1, second - 1, base);
		result = multiply(n1, p, base);
	}else
	{
		p = power(n1, second / 2, base);
		result = multiply(p, p, base);
	}
	free(p.arr);
	return result;
}

int howManyFits(struct IntArr first, struct IntArr second, int base)
{
	if (!isFirstBiggerOrEqual(first, second))
		return 0;
	int result = 0;
	struct IntArr current = copyInt(second);
	while (isFirstBiggerOrEqual(first, current))
	{
		result++;
		struct IntArr buff = add(current, second, base);
		free(current.arr);
		current = copyInt(buff);
		free(buff.arr);
		removeLeadingZeros(&current);
	}
	free(current.arr);
	return result;
}

struct IntArr* divide(struct IntArr n1, struct IntArr n2, int base)
{
	struct IntArr* result = calloc(2, sizeof(struct IntArr));
	int shift = n1.length - n2.length, used = 0;

	result[1] = copyInt(n1);
	if (isFirstBigger(n2, n1))
	{
		result[DIV_RESULT].length = 1;
		result[DIV_RESULT].arr = calloc(result[0].length, sizeof(int));
		result[DIV_RESULT].arr[0] = 0;
		return result;
	}
	result[DIV_RESULT].length = shift + 1;
	result[DIV_RESULT].arr = calloc(shift + 1, sizeof(int));

	while (shift >= 0)
	{
		struct IntArr first;
		int howMany;

		first.length = result[1].length - shift - used;
		first.arr = calloc(first.length, sizeof(int));
		for (int i = shift; i < shift + first.length; i++)
			first.arr[i - shift] = result[1].arr[i];
		removeLeadingZeros(&first);

		howMany = howManyFits(first, n2, base);
		if (howMany > 0)
		{
			struct IntArr hm, second, newPart;
			hm.length = 1;
			hm.arr = calloc(1, sizeof(int));
			hm.arr[0] = howMany;
			second = multiply(n2, hm, base);
			removeLeadingZeros(&second);
			newPart = subtract(first, second, base);
			for (int i = shift; i < shift + newPart.length; i++)
				result[DIV_REMAINDER].arr[i] = newPart.arr[i - shift];
			for (int i = newPart.length - 1; i >= 0; i--)
			{
				if (newPart.arr[i] == 0)
					used++;
				else
					break;
			}
			free(hm.arr);
			free(second.arr);
			free(newPart.arr);
		}
		result[DIV_RESULT].arr[shift] = howMany;
		shift--;
		free(first.arr);
	}
	return result;
}

struct IntArr convertDecNumber(int n, int base)
{
	struct IntArr result;
	int i;

	result.length = 5;
	result.arr = calloc(result.length, sizeof(int));
	for (int i = 0; i < result.length; i++)
		result.arr[i] = 0;
	i = 0;
	while (n > 0)
	{
		result.arr[i] = n % base;
		n /= base;
		i++;
	}
	return result;
}

struct IntArr convert(struct IntArr n, int oldBase, int newBase)
{
	struct IntArr result, zero, current, newBaseConverted;
	char* res;
	int i;

	result.length = n.length * 4;
	result.arr = calloc(result.length, sizeof(int));

	zero.arr = calloc(1, sizeof(int));
	zero.arr[0] = 0;
	zero.length = 1;

	current = copyInt(n);

	newBaseConverted = convertDecNumber(newBase, oldBase);
	removeLeadingZeros(&newBaseConverted);
	i = 0;
	while (isFirstBigger(current, zero))
	{
		struct IntArr* temp = divide(current, newBaseConverted, oldBase);
		int newDigit;
		removeLeadingZeros(&temp[DIV_RESULT]);
		removeLeadingZeros(&temp[DIV_REMAINDER]);
		newDigit = toDec(temp[DIV_REMAINDER], oldBase);
		result.arr[i] = newDigit;
		i++;

		free(current.arr);
		current = temp[DIV_RESULT];
		free(temp[DIV_REMAINDER].arr);
		free(temp);
	}
	free(zero.arr);
	free(newBaseConverted.arr);
	free(current.arr);

	result.length = i;
	return result;
}

//returns false if the input is wrong
bool perform(FILE* fin, FILE* fout, char* operation)
{
	if (!isOperationCorrect(operation))
		return false;

	struct IntArr result;
	char* res;
	free(readLine(fin));
	if (operation[0] >= '0' && operation[0] <= '9')
	{
		struct IntArr n;
		char* s1;
		int whereSpace;
		if (operation[1] == ' ')
			whereSpace = 1;
		else
			whereSpace = 2;
		int oldBase = operation[0] - '0';
		if (whereSpace == 2)
		{
			oldBase *= 10;
			oldBase += operation[1] - '0';
		}

		int newBase = operation[whereSpace + 1] - '0';
		if (strlen(operation) > whereSpace + 2)
		{
			newBase *= 10;
			newBase += operation[whereSpace + 2] - '0';
		}

		s1 = readLine(fin);
		if (!isNumberCorrect(s1, oldBase))
		{
			free(s1);
			return false;
		}
		n = convertToInt(s1);
		fputs(operation, fout);
		fputs("\n\n", fout);
		fputs(s1, fout);
		fputs("\n\n", fout);
		result = convert(n, oldBase, newBase);
		free(n.arr);
		free(s1);
	}else
	{
		struct IntArr n1, n2;
		char* s1;
		char* s2;
		int base;
		if (strlen(operation) == 3)
			base = operation[2] - '0';
		else
		{
			struct IntArr b;
			b.length = 2;
			b.arr = calloc(2, sizeof(int));
			b.arr[0] = operation[3] - '0';
			b.arr[1] = operation[2] - '0';
			base = toDec(b, 10);
			free(b.arr);
		}

		s1 = readLine(fin);
		if (!isNumberCorrect(s1, base))
		{
			free(s1);
			return false;
		}
		n1 = convertToInt(s1);

		free(readLine(fin));

		s2 = readLine(fin);
		if (!isNumberCorrect(s2, base))
		{
			free(s1);
			free(s2);
			return false;
		}
		n2 = convertToInt(s2);

		if (operation[0] == '/')
		{
			struct IntArr* buff = divide(n1, n2, base);
			result = copyInt(buff[DIV_RESULT]);
			free(buff[DIV_RESULT].arr);
			free(buff[DIV_REMAINDER].arr);
			free(buff);
		}else switch (operation[0])
		{
		case '+':
			result = add(n1, n2, base);
			break;
		case '*':
			result = multiply(n1, n2, base);
			break;
		case '^':
			result = power(n1, toDec(n2, base), base);
			break;
		}

		fputs(operation, fout);
		fputs("\n\n", fout);
		fputs(s1, fout);
		fputs("\n\n", fout);
		fputs(s2, fout);
		fputs("\n\n", fout);
		free(n1.arr);
		free(n2.arr);
		free(s1);
		free(s2);
	}

	removeLeadingZeros(&result);
	res = convertToString(result);
	fputs(res, fout);
	fputs("\n\n", fout);
	free(res);
	free(result.arr);
	return true;
}

int main(int argc, char** argv)
{
	if (argc < 2)
	{
		printf("Brak nazwy pliku wejsciowego!\n");
		return 1;
	}
	FILE* fin;
	if (!(fin = fopen(argv[1], "r")))
	{
		printf("Brak pliku o podanej nazwie: %s\n", argv[1]);
		return 2;
	}
	int i = 1;
	bool flag = true;

	if (argc > 2 && strcmp(argv[2], "manyFiles") == 0)
	{
		do
		{
			char* operation;
			char fileName[FILE_NAME_LENGTH];
			FILE* fout;
			while (true) //searching for a non-empty line
			{
				operation = readLine(fin);
				if (operation == NULL) //end of file reached
				{
					free(operation);
					flag = false;
					break;
				}
				if (strlen(operation) > 0)
					break;
			}
			if (!flag)
				break;

			sprintf(fileName, "out%d.txt", i);
			fout = fopen(fileName, "w");

			if (perform(fin, fout, operation))
			{
				i++;
				fclose(fout);
			}else
			{
				fclose(fout);
				remove(fileName);
				printf("Error at %d\n", i);
			}	

			free(operation);
		} while (flag);
	}else
	{
		FILE* fout = fopen("out.txt", "w");
		do
		{
			char* operation;
			while (true) //searching for a non-empty line
			{
				operation = readLine(fin);
				if (operation == NULL) //end of file reached
				{
					free(operation);
					flag = false;
					break;
				}
				if (strlen(operation) > 0)
					break;
			}
			if (!flag)
				break;

			if (perform(fin, fout, operation))
				i++;
			else
				printf("Error at %d\n", i);

			free(operation);
		} while (flag);

		fclose(fout);
	}
	fclose(fin);
	printf("Done\n");
	return 0;
}