#include "runtime.h"

// Declarations
auto Main() -> int;

// Class Declarations

// Definitions
auto Main() -> int
{
	int result;
	result = 42;
	result += 1;
	result -= 1;
	return result;
}

// Entry Point Adapter
int main(int argc, char const *const * argv)
{
	return Main();
}
