#pragma once

/*
 *         <ScopeGuard>  By James Taylor(ButylLee)
 *
 * ScopeGuard is a so-called Universal Resource Management
 * Class that employs RAII pattern. It provides a common way
 * of automatically releasing the resource acquired by the
 * users(programmer) via executing a provided callback when
 * leaving scope. Unlike smart pointer, ScopeGuard is not
 * taking charge of acquisition but release, referring to the
 * resources for which are too various to write control class
 * respectively, and users are supposed to acquire resource
 * first then bind it to ScopeGuard. It would operate when
 * exiting current scope by function return, exception throw, etc.
 *
 * NOTICE: You are NOT supposed to throw an exception or invoke
 * a throwing callable within ScopeGuard, if you did so, which
 * won't be prevented, the abort() will be called when sg operating.
 *
 * There are three ways to use ScopeGuard:
 *
 * (1) The recommended usage's as follows
 *
 *     -*-*-*-*-*-*-*-*-*-*-*-*-
 *     // acquire resource here
 *     ON_SCOPE_EXIT{
 *         // release statment here
 *     };
 *     -*-*-*-*-*-*-*-*-*-*-*-*-
 *     You can put statments inside { } as in normal function
 *     body, they will be execute when exiting current scope.
 *
 * (2) Using this when you want to write lambda or pass function by yourself
 *
 *     -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *     // acquire resource here
 *     SCOPEGUARD([&] { // release statment });
 *                ^^^^lambda, function or executable object
 *     -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *
 * (3) Using function MakeScopeGuard to create named variables when you
 *     have got to postpone or bring forward the release of resource.
 *
 *     -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 *     // acquire resource here
 *     auto sg = sg::MakeScopeGuard([&] { // release statment });
 *                                  ^^^^lambda, function or executable object
 *
 *     // release ahead of time vvv
 *     // manual release statment
 *     sg.dismiss();
 *
 *     // postpone release vvv
 *     auto foo(){
 *         // acquire resource here
 *         auto sg = sg::MakeScopeGuard([&] { // release statment });
 *         return sg;
 *     }
 *     -*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-
 */

#ifndef SCOPEGUARD_HEADER
#define SCOPEGUARD_HEADER

// sizeof ScopeGuard with template: 8 in x86 and 16 in x64(aligned)
// sizeof ScopeGuard with std::function: 48 in x86 and 72 in x64(aligned)
// just use template version :D
#define SG_USING_TEMPLATE 1

#define SG_BEGIN namespace sg {
#define SG_END   }
#define LINE_NAME_CAT(name,line) name##line
#define LINE_NAME(name,line) LINE_NAME_CAT(name,line)


#define ON_SCOPE_EXIT \
		auto LINE_NAME(OnScopeExit_Block_,__LINE__) = \
		sg::detail::eOnScopeExit() + [&]() noexcept ->void
#define SCOPEGUARD(callback) \
		auto LINE_NAME(SCOPEGUARD_,__LINE__) = sg::MakeScopeGuard(callback)

#if SG_USING_TEMPLATE
#include <type_traits>

SG_BEGIN
namespace detail {
	template<typename T>
	constexpr bool is_proper_callback_v
		= std::is_same_v<void, decltype(std::declval<T&&>()())>;

	template<typename TCallback, typename =
		std::enable_if_t<is_proper_callback_v<TCallback>>>
	class ScopeGuard;

	enum class eOnScopeExit {}; // dummy

	template<typename TCallback>
	constexpr ScopeGuard<TCallback> operator+(eOnScopeExit, TCallback&& callback)
	{
		return ScopeGuard<TCallback>(std::forward<TCallback>(callback));
	}

	template<typename TCallback>
	constexpr ScopeGuard<TCallback> MakeScopeGuard(TCallback&& callback)
	{
		return ScopeGuard<TCallback>(std::forward<TCallback>(callback));
	}

	template<typename TCallback>
	class ScopeGuard<TCallback> final
	{
		using Callback = std::decay_t<TCallback>;

		friend constexpr ScopeGuard<TCallback> operator+<TCallback>(eOnScopeExit, TCallback&&);
		friend constexpr ScopeGuard<TCallback> MakeScopeGuard<TCallback>(TCallback&&);
	private:
		explicit ScopeGuard(Callback callback)
			:m_callback(std::move(callback))
		{}
	public:
		~ScopeGuard() noexcept
		{
			if (m_active)
				m_callback();
		}

		ScopeGuard(ScopeGuard&& other)
			noexcept(std::is_nothrow_move_constructible_v<Callback>)
			: m_callback(std::move(other.m_callback))
			, m_active(other.m_active)
		{
			other.dismiss();
		}
		/* No move assign function */

		void dismiss() noexcept
		{
			m_active = false;
		}

		ScopeGuard(const ScopeGuard&) = delete;
		ScopeGuard& operator=(const ScopeGuard&) = delete;
	private:
		Callback m_callback;
		bool m_active = true;
	};
} // namespace detail

using detail::MakeScopeGuard;
SG_END
#else // ^^^ SG_USING_TEMPLATE / !SG_USING_TEMPLATE vvv
#include <functional>

SG_BEGIN
namespace detail{
	enum class eOnScopeExit {}; // dummy

	class ScopeGuard final
	{
		friend ScopeGuard operator+(eOnScopeExit, std::function<void()>);
		friend ScopeGuard MakeScopeGuard(std::function<void()>);
	private:
		explicit ScopeGuard(std::function<void()> callback) :m_callback(callback) {}
	public:
		~ScopeGuard() noexcept
		{
			if (m_active)
				m_callback();
		}

		ScopeGuard(ScopeGuard&& other)
			noexcept(std::is_nothrow_move_constructible_v<std::function<void()>>)
			: m_callback(std::move(other.m_callback))
			, m_active(other.m_active)
		{
			other.dismiss();
		}
		/* No move assign function */

		void dismiss() noexcept
		{
			m_active = false;
		}

		ScopeGuard(const ScopeGuard&) = delete;
		ScopeGuard& operator=(const ScopeGuard&) = delete;
	private:
		std::function<void()> m_callback;
		bool m_active = true;
	};

	inline ScopeGuard operator+(eOnScopeExit, std::function<void()> callback)
	{
		return ScopeGuard(callback);
	}

	inline ScopeGuard MakeScopeGuard(std::function<void()> callback)
	{
		return ScopeGuard(callback);
	}
} // namespace detail

using detail::MakeScopeGuard;
SG_END
#endif // ^^^ !SG_USING_TEMPLATE ^^^

#undef SG_USING_TEMPLATE
#undef SG_BEGIN
#undef SG_END

#endif // SCOPEGUARD_HEADER