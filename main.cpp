// Beczi Eliezer
// beim1697
// 511

#include "bigint.h"

using namespace std;

int main(int argc, char const *argv[])
{
	bigint num(1);

	for(unsigned i = 2; i <= 1000; ++i) {
		num *= i;
	}

	cout << num;

	return 0;
}
