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


	auto* expect_func_pointer = expect_func;
	auto* expect_func_nothrow_pointer = expect_func_nothrow;
	auto* throwing_pointer = throwing;
	auto* returning_pointer = returning;
	auto* para_int_pointer = para_int;


	struct expect_struct
	{
		void operator()() { PRINTMSG }
	}expect_functor;
	struct expect_struct_const
	{
		void operator()() const { PRINTMSG }
	}expect_functor_const;
	struct expect_struct_nothrow
	{
		void operator()() noexcept { PRINTMSG }
	}expect_functor_nothrow;

	struct throwing_struct
	{
		[[noreturn]] void operator()() { throw std::runtime_error(message); }
	}throwing_functor;
	struct returning_struct
	{
		int operator()() { PRINTMSG return{}; }
	}returning_functor;
	struct para_int_struct
	{
		void operator()(int) { PRINTMSG }
	}para_int_functor;


	auto expect_lambda = [] { PRINTMSG };
	auto expect_lambda_copy_cap = [=] { PRINTMSG };
	auto expect_lambda_ref_cap = [&] { PRINTMSG };


	using StdFun = std::function<void()>;
	StdFun throwing_stdfun = throwing;
	StdFun expect_stdfun = expect_func;
	StdFun expect_stdfun_nothrow = expect_func_nothrow;

	std::function<int()> returning_stdfun = returning;
	std::function<void(int)> para_int_stdfun = para_int;


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
	SCOPEGUARD(expect_lambda);
	SCOPEGUARD(expect_functor);
	SCOPEGUARD(expect_func);
	SCOPEGUARD(expect_func_pointer);
	//SCOPEGUARD(expect_stdfun);

	// pass by rvalue
	SCOPEGUARD(&expect_func);
	SCOPEGUARD(expect_struct());
	SCOPEGUARD([]{ PRINTMSG });


}

// same as USAGE_SCOPEGUARD at large
TEST_METHOD(USAGE_MakeScopeGuard)
{
	auto sg = sg::MakeScopeGuard(expect_func);


	auto sg2 = sgFactory();
	auto sg3 = sgFactory(expect_func);
	auto sg4 = sgFactory([]{});

}