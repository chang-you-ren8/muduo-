#include "Timestamp.h"
#include <iostream>
#include <string>
#include <unistd.h>
using std::cout;
using std::endl;
using std::string;


int main(void)
{
	cout << Timestamp::now().toDate(false) << endl;
	cout << Timestamp::now().toDate(true) << endl;

	sleep(1);

	cout << Timestamp::now().toDate(false) << endl;
	cout << Timestamp::now().toDate(true) << endl;

	return 0;
}
