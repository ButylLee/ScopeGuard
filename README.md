# ScopeGuard
A header-only, modern C++ and fast facility ScopeGuard that provide multiple usages which are simple and easy to use.

C++14 or later standard is required.

### Simple examples

There're 3 ways to use ScopeGuard:

1. Put the code executing when leaving current scope in code block **ON_SCOPE_EXIT{ }**.
```C++
#include "ScopeGuard.h"
...
{
    ...
    ON_SCOPE_EXIT{
        // callback statments
        // such as releasing resources
    };
    ...
} // callback statments are executed at this point
```

2. put callable in **SCOPEGUARD( )**

```C++
...
{
    void my_callback();
    ...
    SCOPEGUARD(my_callback);
    ...
    SCOPEGUARD([&] { // releasing resources });
    ...
} // lambda and my_callback are invoked at this point
```

3. using the make function **MakeScopeGuard** manually.

```C++
...
{
    ...
    auto guard = sg::MakeScopeGuard(my_callback);
    ...
} // my_callback is invoked at this point
```

You could see the comments in [ScopeGuard.h](ScopeGuard/ScopeGuard.h) for more.

### Tutorial

See tutorial [here](http://origought.cn/2021/cpp/88/) and also [here](http://origought.cn/2021/cpp/99/) (Chinese).










