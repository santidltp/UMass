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
START_TEST(test_words_next_word) // points=5
{
	int p[2];
	// Create a pipe:
	pipe(p);

	// Close normal stdout:
	close(1);
	// Make stdout same as p[1]:
	dup(p[1]);

	// Close normal stdin:
	close(0);
	// Make stdin same as p[0]:
	dup(p[0]);

	// Write a string to "stdout"
	printf("this,,,$$$is     some text!!EXCELLENT\n");
	char buf[MAX_WORD_SIZE];
        char  *word = buf;
	//int nbytes;

	// Test word 1:
	word = words_next_word();
	ck_assert_msg(word != NULL, "words_next_word() is returning NULL!");
	ck_assert_str_eq("this", word);
	ck_assert_int_eq(strlen(word), 4);
	// Test word 2:
	word = words_next_word();
	ck_assert_msg(word != NULL, "words_next_word() is returning NULL!");
	ck_assert_str_eq("is", word);
	ck_assert_int_eq(strlen(word), 2);
	// Test word 3:
	word = words_next_word();
	ck_assert_msg(word != NULL, "words_next_word() is returning NULL!");
	ck_assert_str_eq("some", word);
	ck_assert_int_eq(strlen(word), 4);
	// Test word 4:
	word = words_next_word();
	ck_assert_msg(word != NULL, "words_next_word() is returning NULL!");
	ck_assert_str_eq("text", word);
	ck_assert_int_eq(strlen(word), 4);
	// Test word 5:
	word = words_next_word();
	ck_assert_msg(word != NULL, "words_next_word() is returning NULL!");
	ck_assert_str_eq("EXCELLENT", word);
	ck_assert_int_eq(strlen(word), 9);
}
END_TEST

// An example of a test that fails - so you can see the output!
START_TEST(test_wset_new)
{
	WordSet* wset = wset_new();
	ck_assert_msg(wset != NULL, "The wset_new function is returning NULL!");
}
END_TEST

START_TEST(test_wset_free)
{
	WordSet* wset = wset_new();
	ck_assert_msg(wset != NULL, "The wset_new function is returning NULL!");
	wset_free(wset);
}
END_TEST

START_TEST(test_wset_add)
{
	WordSet* wset = wset_new();
	ck_assert_msg(wset != NULL, "The wset_new function is returning NULL!");
	wset_add(wset, "this");
	ck_assert_int_eq(wset_size(wset), 1);
	wset_add(wset, "is");
	ck_assert_int_eq(wset_size(wset), 2);
	wset_add(wset, "some");
	ck_assert_int_eq(wset_size(wset), 3);
	wset_add(wset, "text");
	ck_assert_int_eq(wset_size(wset), 4);
	wset_add(wset, "santiago");
	ck_assert_int_eq(wset_size(wset), 5);//additional test
}
END_TEST

START_TEST(test_wset_fast_add)
{
	WordSet* wset = wset_new();
	ck_assert_msg(wset != NULL, "The wset_new function is returning NULL!");
	wset_fast_add(wset, "this");
	ck_assert_int_eq(wset_size(wset), 1);
	wset_fast_add(wset, "is");
	ck_assert_int_eq(wset_size(wset), 2);
	wset_fast_add(wset, "some");
	ck_assert_int_eq(wset_size(wset), 3);
	wset_fast_add(wset, "text");
	ck_assert_int_eq(wset_size(wset), 4);
		wset_fast_add(wset, "this");
	ck_assert_int_eq(wset_size(wset), 5);
//wset_print(wset);
}
END_TEST

START_TEST(test_wset_exists)
{
	WordSet* wset1 = wset_new();
	// wset_print(wset1);
	ck_assert_msg(wset1 != NULL, "The wset_new function is returning NULL!");
	wset_add(wset1, "this");
	wset_add(wset1, "is");
	wset_add(wset1, "some");
	wset_add(wset1, "text");
	wset_add(wset1,"text");
	// wset_print(wset1);
	ck_assert_int_eq(wset_size(wset1), 4);
	
	ck_assert_int_eq(wset_exists(wset1, "this"), 1);
	ck_assert_int_eq(wset_exists(wset1, "is"), 1);
	ck_assert_int_eq(wset_exists(wset1, "some"), 1);
	ck_assert_int_eq(wset_exists(wset1, "text"), 1);
	WordSet* wset2 = wset_new();
	ck_assert_msg(wset2 != NULL, "The wset_new function is returning NULL!");
	wset_fast_add(wset2, "this");
	wset_fast_add(wset2, "is");
	wset_fast_add(wset2, "some");
	wset_fast_add(wset2, "text");
	//wset_print(wset2);
	ck_assert_int_eq(wset_exists(wset2, "this"), 1);
	ck_assert_int_eq(wset_exists(wset2, "is"), 1);
	ck_assert_int_eq(wset_exists(wset2, "some"), 1);
	ck_assert_int_eq(wset_exists(wset2, "text"), 1);
}
END_TEST

START_TEST(test_wset_remove)
{
	WordSet* wset2 = wset_new();
	ck_assert_msg(wset2 != NULL, "The wset_new function is returning NULL!");
	wset_fast_add(wset2, "this");
	wset_fast_add(wset2, "is");
	wset_fast_add(wset2, "some");
	wset_fast_add(wset2, "text");

	// wset_add(wset2, "this");
	//  wset_add(wset2, "is");
	//  wset_add(wset2, "some");
	//  wset_add(wset2, "text");

	  // wset_print(wset2);
	ck_assert_int_eq(wset_size(wset2),4 );

   // wset_remove(wset2, "santiago");
	wset_remove(wset2, "text");

	wset_remove(wset2, "this");
 
	wset_remove(wset2, "is");

	  wset_print(wset2);
	//wset_print(wset2);
ck_assert_int_eq(wset_size(wset2), 1);
	
	  wset_remove(wset2, "santiago");
	// // wset_remove(wset2, "santiago");
	// // // wset_print(wset2);

	 // wset_remove(wset2, "some");
	// //  	wset_print(wset2);
	// // // wset_print(wset2);
	  ck_assert_int_eq(wset_size(wset2), 1);

}
END_TEST

START_TEST(test_wset_freq)
{
	WordSet* wset2 = wset_new();
	ck_assert_msg(wset2 != NULL, "The wset_new function is returning NULL!");
	wset_fast_add(wset2, "this");
	wset_fast_add(wset2, "this");
	wset_fast_add(wset2, "this");
	wset_fast_add(wset2, "this");
	ck_assert_int_eq(wset_freq(wset2, "this"), 4);
}
END_TEST

Suite* tester_suite() {
	// Create the test suite:
	Suite* s = suite_create("Public Assignment Tests");

	/**** BEGIN UNIT TESTS ****/
	TCase* tc_inc = tcase_create("Public Tests");
	// To use setup/teardown (otherwise skip):
	tcase_add_checked_fixture(tc_inc, setup, teardown);
	tcase_add_test(tc_inc, test_words_next_word);
	tcase_add_test(tc_inc, test_wset_new);
	tcase_add_test(tc_inc, test_wset_free);
	tcase_add_test(tc_inc, test_wset_add);
	tcase_add_test(tc_inc, test_wset_fast_add);
	tcase_add_test(tc_inc, test_wset_remove);
	tcase_add_test(tc_inc, test_wset_exists);
	tcase_add_test(tc_inc, test_wset_freq);
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
