# ScopeGuard
A header-only, modern C++ ScopeGuard facility that provide multiple usages which are simple and easy to use.

**NOTE:** It's NOT finished yet but basically usable.

### Simple examples

There're 3 ways to use ScopeGuard:

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

```C++
...
{
    ...
    auto guard = sg::MakeScopeGuard(my_callback);
    ...
} // my_callback is invoked at this point
```

You could also see the comments in file ScopeGuard.h.












