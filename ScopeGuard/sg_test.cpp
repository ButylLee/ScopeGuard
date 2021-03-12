#include "UnitTestHeader.h"
#include "ScopeGuard.h"
#include <iostream>
#include <functional>
#define FUNC_OUTPUT 1

using namespace std;
using namespace sg;
TEST_MAIN

#if FUNC_OUTPUT
#define PRINTMSG \
printf_s("function: %s %s>line: %d\n", __func__, \
"----------------------------------------------" \
"------------------" + sizeof(__func__), __LINE__)
//printf_s("function: %-60s line: %d\n", __func__, __LINE__)
#else
#define PRINTMSG
#endif

namespace {
	constexpr auto message = "message in a bottle";
	constexpr int dump = 10;

	void expect_func() { PRINTMSG; CASE_DONE; }
	void expect_func_nothrow() noexcept{ PRINTMSG; CASE_DONE; }

	[[noreturn]] void throwing() { PRINTMSG; CASE_DONE; throw std::runtime_error(message); }
	int returning() { PRINTMSG; CASE_DONE; return{}; }
	void para_int(int) { PRINTMSG; CASE_DONE; }


	auto* pointer_expect_func = expect_func;
	auto* pointer_expect_func_nothrow = expect_func_nothrow;
	auto* pointer_throwing = throwing;
	auto* pointer_returning = returning;
	auto* pointer_para_int = para_int;


	struct expect_struct
	{
		void operator()() { PRINTMSG; CASE_DONE; }
	}functor_expect;
	struct expect_struct_const
	{
		void operator()() const { PRINTMSG; CASE_DONE; }
	}functor_expect_const;
	struct expect_struct_nothrow
	{
		void operator()() noexcept { PRINTMSG; CASE_DONE; }
	}functor_expect_nothrow;

	struct throwing_struct
	{
		[[noreturn]] void operator()() { CASE_DONE; CASE_DONE; throw std::runtime_error(message); }
	}functor_throwing;
	struct returning_struct
	{
		int operator()() { PRINTMSG; CASE_DONE; return{}; }
	}functor_returning;
	struct para_int_struct
	{
		void operator()(int) { PRINTMSG; CASE_DONE; }
	}functor_para_int;

	struct test_rvalue_struct
	{
		void operator()() { cout << this->x << endl; }
		void setx(int x){ this->x = x; }
	private:
		int x = 0;
	}functor_test_rvalue;
	struct test_copy_move_struct
	{
		void operator()() { PRINTMSG; CASE_DONE; }
		test_copy_move_struct() = default;
		test_copy_move_struct(const test_copy_move_struct&) { cout << "Copy Constructor" << endl; }
		test_copy_move_struct(test_copy_move_struct&&) noexcept { cout << "Move Constructor" << endl; }
	}functor_test_copy_move;


	auto lambda_expect = [] { PRINTMSG; CASE_DONE; };
	auto lambda_expect_copy_cap = [=] { PRINTMSG; CASE_DONE; };
	auto lambda_expect_ref_cap = [&] { PRINTMSG; CASE_DONE; };


	using StdFun = std::function<void()>;
	StdFun stdfun_throwing = throwing;
	StdFun stdfun_expect = expect_func;
	StdFun stdfun_expect_nothrow = expect_func_nothrow;

	std::function<int()> stdfun_returning = returning;
	std::function<void(int)> stdfun_para_int = para_int;


	auto bind_expect_func = bind(expect_func);

	auto bind_throwing = bind(throwing);
	auto bind_returning = bind(returning);
	auto bind_para_int = bind(para_int);


	auto sgFactory()
	{
		auto sg = sg::MakeScopeGuard(expect_func);
		return sg;
	}

	template<typename Fun>
	decltype(auto) sgFactory(Fun&& fun)
	{
		auto sg = sg::MakeScopeGuard(std::forward<Fun>(fun));
		return sg;
	}
}


TEST_METHOD(USAGE_ON_SCOPE_EXIT)
{
	TEST_CASE
		ON_SCOPE_EXIT{
			PRINTMSG;
			CASE_DONE;
		};
	

	TEST_CASE
		ON_SCOPE_EXIT{
			expect_func();
		};
	
	TEST_CASE
		ON_SCOPE_EXIT{
			expect_func_nothrow();
		};

	ON_SCOPE_EXIT{
		//throw runtime_error(message); // could NOT throw here
	};
	ON_SCOPE_EXIT{
		//return 0; // could NOT return value here
		return;
	};

	int mutable_v = 0;
	string mutable_str = "origin string";
	ON_SCOPE_EXIT{
		mutable_v = 1;
		mutable_str = " changed string";
		cout << mutable_v << mutable_str << endl;
	};


}

TEST_METHOD(USAGE_SCOPEGUARD)
{
	// pass by lvalue
	TEST_CASE
		SCOPEGUARD(lambda_expect);
	TEST_CASE
		SCOPEGUARD(functor_expect);
	TEST_CASE
		SCOPEGUARD(expect_func);
	TEST_CASE
		SCOPEGUARD(pointer_expect_func);

	TEST_CASE
		SCOPEGUARD(expect_func_nothrow);

	//SCOPEGUARD(throwing);
	//SCOPEGUARD(returning);
	//SCOPEGUARD(para_int);
	//SCOPEGUARD(para_int);
	//SCOPEGUARD(pointer_throwing);
	//SCOPEGUARD(pointer_returning);
	//SCOPEGUARD(pointer_para_int);

	TEST_CASE
		SCOPEGUARD(pointer_expect_func);
	TEST_CASE
		SCOPEGUARD(pointer_expect_func_nothrow);


	TEST_CASE
		SCOPEGUARD(functor_expect);
	TEST_CASE
		SCOPEGUARD(functor_expect_const);
	TEST_CASE
		SCOPEGUARD(functor_expect_nothrow);
	//SCOPEGUARD(functor_throwing);
	//SCOPEGUARD(functor_returning);
	//SCOPEGUARD(functor_para_int);

	TEST_CASE
		SCOPEGUARD(lambda_expect);
	TEST_CASE
		SCOPEGUARD(lambda_expect_copy_cap);
	TEST_CASE
		SCOPEGUARD(lambda_expect_ref_cap);

	//SCOPEGUARD(stdfun_throwing);
	TEST_CASE
		SCOPEGUARD(stdfun_expect);
	TEST_CASE
		SCOPEGUARD(stdfun_expect_nothrow);
	//SCOPEGUARD(stdfun_returning);
	//SCOPEGUARD(stdfun_para_int);

	TEST_CASE
		SCOPEGUARD(bind_expect_func);
	//SCOPEGUARD(bind_throwing);
	//SCOPEGUARD(bind_returning);
	//SCOPEGUARD(bind_para_int);

	// pass by rvalue
	TEST_CASE
		SCOPEGUARD(&expect_func);
	TEST_CASE
		SCOPEGUARD(expect_struct());
	TEST_CASE
		SCOPEGUARD([] { PRINTMSG; CASE_DONE; });

	TEST_CASE
		SCOPEGUARD(functor_test_copy_move); // lvalue
	TEST_CASE
		SCOPEGUARD(test_copy_move_struct()); // rvalue
}

// same as USAGE_SCOPEGUARD at large
TEST_METHOD(USAGE_MakeScopeGuard)
{
	TEST_CASE
		auto sg = sg::MakeScopeGuard(expect_func);

	TEST_CASE
		auto sg2 = sgFactory();
	TEST_CASE
		auto sg3 = sgFactory([] { cout << "lambda from factory" << endl; CASE_DONE; });


}

// test case before merge branch std-function-like
TEST_METHOD(TEST_PASS_RVALUE)
{
	using fun_type = void(*)();
	fun_type f = []{cout << "origin" << endl; };
	SCOPEGUARD(f);
	SCOPEGUARD(*f);
	f = []{cout << "changed" << endl; };

	SCOPEGUARD(functor_test_rvalue);
	SCOPEGUARD(std::move(functor_test_rvalue));
	functor_test_rvalue.setx(1);

}