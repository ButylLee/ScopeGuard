#include "UnitTestHeader.h"
#include "ScopeGuard.h"
#include <iostream>
#include <functional>
#define FUNC_OUTPUT 0

using namespace std;
using namespace sg;
TEST_MAIN

#if FUNC_OUTPUT
#define PRINTMSG \
printf_s("function: %s %s>line: %d\n", __func__, \
"----------------------------------------------" \
"------------------" + sizeof(__func__), __LINE__);
//printf_s("function: %-60s line: %d\n", __func__, __LINE__);
#else
#define PRINTMSG
#endif

namespace {
	constexpr auto message = "message in a bottle";
	constexpr int dump = 10;

	void expect_func() { PRINTMSG }
	void expect_func_nothrow() noexcept{ PRINTMSG }

	[[noreturn]] void throwing(){ PRINTMSG throw std::runtime_error(message); }
	int returning() { PRINTMSG return{}; }
	void para_int(int) { PRINTMSG }


	auto* pointer_expect_func = expect_func;
	auto* pointer_expect_func_nothrow = expect_func_nothrow;
	auto* pointer_throwing = throwing;
	auto* pointer_returning = returning;
	auto* pointer_para_int = para_int;


	struct expect_struct
	{
		void operator()() { PRINTMSG }
	}functor_expect;
	struct expect_struct_const
	{
		void operator()() const { PRINTMSG }
	}functor_expect_const;
	struct expect_struct_nothrow
	{
		void operator()() noexcept { PRINTMSG }
	}functor_expect_nothrow;

	struct throwing_struct
	{
		[[noreturn]] void operator()() { throw std::runtime_error(message); }
	}functor_throwing;
	struct returning_struct
	{
		int operator()() { PRINTMSG return{}; }
	}functor_returning;
	struct para_int_struct
	{
		void operator()(int) { PRINTMSG }
	}functor_para_int;

	struct test_rvalue_struct
	{
		void operator()(){ cout << this->x << endl; }
		test_rvalue_struct& setx(int x){ this->x = x; return *this; }
	private:
		int x = 0;
	}functor_test_rvalue;
	struct test_copy_move_struct
	{
		void operator()() { PRINTMSG }
		test_copy_move_struct() = default;
		test_copy_move_struct(const test_copy_move_struct&) { cout << "Copy Constructor" << endl; }
		test_copy_move_struct(test_copy_move_struct&&) noexcept { cout << "Move Constructor" << endl; }
	}functor_test_copy_move;


	auto lambda_expect = [] { PRINTMSG };
	auto lambda_expect_copy_cap = [=] { PRINTMSG };
	auto lambda_expect_ref_cap = [&] { PRINTMSG };


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
	ON_SCOPE_EXIT{
		PRINTMSG
	};

	ON_SCOPE_EXIT{
		expect_func();
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
	SCOPEGUARD(lambda_expect);
	SCOPEGUARD(functor_expect);
	SCOPEGUARD(expect_func);
	SCOPEGUARD(pointer_expect_func);

	SCOPEGUARD(expect_func_nothrow);

	//SCOPEGUARD(throwing);
	//SCOPEGUARD(returning);
	//SCOPEGUARD(para_int);
	//SCOPEGUARD(para_int);
	//SCOPEGUARD(pointer_throwing);
	//SCOPEGUARD(pointer_returning);
	//SCOPEGUARD(pointer_para_int);

	SCOPEGUARD(pointer_expect_func);
	SCOPEGUARD(pointer_expect_func_nothrow);


	SCOPEGUARD(functor_expect);
	SCOPEGUARD(functor_expect_const);
	SCOPEGUARD(functor_expect_nothrow);
	//SCOPEGUARD(functor_throwing);
	//SCOPEGUARD(functor_returning);
	//SCOPEGUARD(functor_para_int);

	SCOPEGUARD(lambda_expect);
	SCOPEGUARD(lambda_expect_copy_cap);
	SCOPEGUARD(lambda_expect_ref_cap);

	//SCOPEGUARD(stdfun_throwing);
	SCOPEGUARD(stdfun_expect);
	SCOPEGUARD(stdfun_expect_nothrow);
	//SCOPEGUARD(stdfun_returning);
	//SCOPEGUARD(stdfun_para_int);

	SCOPEGUARD(bind_expect_func);
	//SCOPEGUARD(bind_throwing);
	//SCOPEGUARD(bind_returning);
	//SCOPEGUARD(bind_para_int);

	// pass by rvalue
	SCOPEGUARD(&expect_func);
	SCOPEGUARD(expect_struct());
	SCOPEGUARD([]{ PRINTMSG });

	SCOPEGUARD(functor_test_copy_move);
	SCOPEGUARD(test_copy_move_struct());
}

// same as USAGE_SCOPEGUARD at large
TEST_METHOD(USAGE_MakeScopeGuard)
{
	auto sg = sg::MakeScopeGuard(expect_func);

	auto sg2 = sgFactory();
	auto sg3 = sgFactory([] { cout << "lambda from factory" << endl; });


}

TEST_METHOD(TEST_PASS_RVALUE)
{
	using fun_type = void(*)();
	fun_type f = []{cout << "origin" << endl; };
	SCOPEGUARD(*f);
	f = []{cout << "changed" << endl; };

	SCOPEGUARD(std::move(functor_test_rvalue));
	functor_test_rvalue.setx(1)();

}