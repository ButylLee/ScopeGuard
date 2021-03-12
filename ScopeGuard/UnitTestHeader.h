#pragma once
#include <Windows.h>

#define TEST_CAT_(L,R) L##R
#define TEST_CAT(L,R) TEST_CAT_(L,R)

#define TEST_METHOD(NAME) \
extern void TEST_CAT(TEST_METHOD_, NAME) (); \
class TEST_CAT(_TEST_CLASS_, NAME) { \
public: \
	TEST_CAT(_TEST_CLASS_, NAME)(){TEST_CAT(TEST_METHOD_, NAME)();} \
}TEST_CAT(TEST_CLASS_, NAME); \
void TEST_CAT(TEST_METHOD_, NAME)() \

#define TEST_MAIN \
int main() \
{ \
	HANDLE ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE); \
	CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo; \
	if (!GetConsoleScreenBufferInfo(ConsoleHandle, &ConsoleInfo)) \
		abort(); \
\
	if (case_success == case_count) \
		SetConsoleTextAttribute(ConsoleHandle, 0x0a); \
	else \
		SetConsoleTextAttribute(ConsoleHandle, 0x0c); \
	std::cout << case_success << "/" << case_count << " passed" << endl; \
	SetConsoleTextAttribute(ConsoleHandle, ConsoleInfo.wAttributes); \
}

#define TEST_CASE case_count++;
#define CASE_DONE case_success++

size_t case_count = 0;
size_t case_success = 0;