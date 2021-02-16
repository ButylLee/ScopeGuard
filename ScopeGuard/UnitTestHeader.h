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
{}