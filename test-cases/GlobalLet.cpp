#include "runtime.h"

// Declarations
auto Main() -> int;

// Class Declarations

// Definitions
int const Result = 42;

auto Main() -> int
{
	return Result;
}

// Entry Point Adapter
int main(int argc, char const *const * argv)
{
	return Main();
}
