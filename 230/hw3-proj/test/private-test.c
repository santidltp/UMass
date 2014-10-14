#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
// Include the check header file:
#include <check.h>

// Include assignment header file:
#include "../hw3.h"

#define MAX_WORD_SIZE 46

void setup() {
	// Perform any setup operations.
}

void teardown() {
	// Perform any teardown operations.
}

// Example test.
//  For more details see http://check.sourceforge.net
START_TEST(test_TEST_NAME)
{

}
END_TEST


Suite* tester_suite() {
	// Create the test suite:
	Suite* s = suite_create("Public Assignment Tests");

	/**** BEGIN UNIT TESTS ****/
	TCase* tc_inc = tcase_create("Public Tests");
	// To use setup/teardown (otherwise skip):
	tcase_add_checked_fixture(tc_inc, setup, teardown);
	//tcase_add_test(tc_inc, test_TEST_NAME);
	// Add unit tests to test suite:
	suite_add_tcase(s, tc_inc);
	/**** END UNIT TESTS   ****/

	// Return the suite:
	return s;
}

int main(int argc, char* argv[]) {
	int number_failed;
	Suite* s = tester_suite();
	SRunner* sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
