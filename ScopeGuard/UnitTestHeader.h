#pragma once

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
	std::cout << case_success << "/" << case_count << "passed" << endl; \
}

#define TEST_CASE case_count++;
#define CASE_DONE case_success++

size_t case_count = 0;
size_t case_success = 0;