#include "runtime.cpp"

void Main()
{
	bool x;
	x = true && false;
	x = true || false;
}

// Entry Point Adapter
int main(int argc, const char * argv[])
{
	Main();
	return 0;
}