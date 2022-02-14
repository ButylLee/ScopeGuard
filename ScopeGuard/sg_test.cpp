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
		finally{
			PRINTMSG;
			CASE_DONE;
		};
	

	TEST_CASE
		finally{
			expect_func();
		};
	
	TEST_CASE
		finally{
			expect_func_nothrow();
		};

	finally{
		//throw runtime_error(message); // could NOT throw here
	};
	finally{
		//return 0; // could NOT return value here
		return;
	};

	int mutable_v = 0;
	string mutable_str = "origin string";
	finally{
		mutable_v = 1;
		mutable_str = " changed string";
		cout << mutable_v << mutable_str << endl;
	};


}

TEST_METHOD(USAGE_SCOPEGUARD)
{
	// pass by lvalue
	TEST_CASE
		INVOKE_ON_EXIT(lambda_expect);
	TEST_CASE
		INVOKE_ON_EXIT(functor_expect);
	TEST_CASE
		INVOKE_ON_EXIT(expect_func);
	TEST_CASE
		INVOKE_ON_EXIT(pointer_expect_func);

	TEST_CASE
		INVOKE_ON_EXIT(expect_func_nothrow);

	//INVOKE_ON_EXIT(throwing);
	//INVOKE_ON_EXIT(returning);
	//INVOKE_ON_EXIT(para_int);
	//INVOKE_ON_EXIT(para_int);
	//INVOKE_ON_EXIT(pointer_throwing);
	//INVOKE_ON_EXIT(pointer_returning);
	//INVOKE_ON_EXIT(pointer_para_int);

	TEST_CASE
		INVOKE_ON_EXIT(pointer_expect_func);
	TEST_CASE
		INVOKE_ON_EXIT(pointer_expect_func_nothrow);


	TEST_CASE
		INVOKE_ON_EXIT(functor_expect);
	TEST_CASE
		INVOKE_ON_EXIT(functor_expect_const);
	TEST_CASE
		INVOKE_ON_EXIT(functor_expect_nothrow);
	//INVOKE_ON_EXIT(functor_throwing);
	//INVOKE_ON_EXIT(functor_returning);
	//INVOKE_ON_EXIT(functor_para_int);

	TEST_CASE
		INVOKE_ON_EXIT(lambda_expect);
	TEST_CASE
		INVOKE_ON_EXIT(lambda_expect_copy_cap);
	TEST_CASE
		INVOKE_ON_EXIT(lambda_expect_ref_cap);

	//INVOKE_ON_EXIT(stdfun_throwing);
	TEST_CASE
		INVOKE_ON_EXIT(stdfun_expect);
	TEST_CASE
		INVOKE_ON_EXIT(stdfun_expect_nothrow);
	//INVOKE_ON_EXIT(stdfun_returning);
	//INVOKE_ON_EXIT(stdfun_para_int);

	TEST_CASE
		INVOKE_ON_EXIT(bind_expect_func);
	//INVOKE_ON_EXIT(bind_throwing);
	//INVOKE_ON_EXIT(bind_returning);
	//INVOKE_ON_EXIT(bind_para_int);

	// pass by rvalue
	TEST_CASE
		INVOKE_ON_EXIT(&expect_func);
	TEST_CASE
		INVOKE_ON_EXIT(expect_struct());
	TEST_CASE
		INVOKE_ON_EXIT([] { PRINTMSG; CASE_DONE; });

	TEST_CASE
		INVOKE_ON_EXIT(functor_test_copy_move); // lvalue
	TEST_CASE
		INVOKE_ON_EXIT(test_copy_move_struct()); // rvalue
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

	TEST_CASE
		const auto sg4 = sg::MakeScopeGuard(expect_func);
}

// test case before merge branch std-function-like
TEST_METHOD(TEST_PASS_RVALUE)
{
	using fun_type = void(*)();
	fun_type f = []{cout << "origin" << endl; };
	INVOKE_ON_EXIT(f);
	INVOKE_ON_EXIT(*f);
	f = []{cout << "changed" << endl; };

	INVOKE_ON_EXIT(functor_test_rvalue);
	INVOKE_ON_EXIT(std::move(functor_test_rvalue));
	functor_test_rvalue.setx(1);

}