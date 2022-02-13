#include <check.h>

#include "calc.h"

  char str3[10] = "4";
  int point2 = 0;

START_TEST(test_calc_1) {
  char givenStr[300] = "14+5";
  double result = 14 + 5;
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

// START_TEST(test_calc_2) {
//   char givenStr[] = "e+pi";
//   double result = M_E + M_PI;
//   double resultPN = result_PN(givenStr);
//   ck_assert_double_eq(resultPN, result);
// }
// END_TEST

START_TEST(test_calc_3) {
  char givenStr[300] = "sqrt(4.12)+asin(0)";
  double result = sqrt(4.12) + asin(0);
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

START_TEST(test_calc_4) {
  char givenStr[300] = "sin(3.14/2)*2.231^3-acos(0.123)";
  double result = sin(3.14/2.0)*pow(2.231, 3.0)-acos(0.123);
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

START_TEST(test_calc_5) {
  char givenStr[300] = "sin(3.14/2)*2.231^3-acos(0.123)";
  double result = sin(3.14/2) * pow(2.231, 3.0)-acos(0.123);
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

START_TEST(test_calc_6) {
  char givenStr[300] = "cos(3.14*3)+ln(0.0541)";
  double result = cos(3.14 * 3)+log(0.0541);
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

START_TEST(test_calc_7) {
  char givenStr[300] = "atan(0.5422/23.12)+tan(0.256)+log(2.21)";
  double result = atan(0.5422/23.12)+tan(0.256)+log10(2.21);
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

START_TEST(test_calc_8) {
  char givenStr[300] = "21.123%2";
  double result = fmod(21.123, 2);
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

START_TEST(test_calc_9) {
  char givenStr[300] = "(2)";
  double result = (2);
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

START_TEST(test_calc_10) {
  char givenStr[300] = "12+(cos(23.12+sin(12.12-cos(2.21/12))))";
  double result = 12 + (cos(23.12 + sin(12.12 - cos(2.21 / 12))));
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

START_TEST(test_calc_11) {
  char givenStr[300] = "-3-(-4)";
  double result = -3 - -4;
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

START_TEST(test_calc_12) {
  char givenStr[300] = ".";
  double result = 0;
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

START_TEST(test_calc_13) {
  char givenStr[300] = "+5+1.2";
  double result = +5 + 1.2;
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

START_TEST(test_calc_14) {
  char givenStr[300] = "()";
  double result = 0;
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

START_TEST(test_calc_15) {
  char givenStr[300] = "+";
  double result = 0;
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

START_TEST(test_calc_16) {
  char givenStr[300] = "sin()";
  double result = 0;
  double resultPN = run(givenStr, str3, &point2);
  ck_assert_double_eq(resultPN, result);
}
END_TEST

// START_TEST(test_credit_1) {
//   CreditSting creditString;
//   creditString.sumCredit = "5000000";
//   creditString.termCredit = "12";
//   creditString.monthOrYear = "year";
//   creditString.percentCredit = "8.21";
//   creditString.typeCredit = "annuity";
//   Credit credit;
//   credit = credit_work(&creditString);
//   char resultStr[256] = "";
//   snprintf(resultStr, sizeof(resultStr), "%.2f", credit.monthlyPayment);
//   ck_assert_str_eq(resultStr, "54700.01");
//   snprintf(resultStr, sizeof(resultStr), "%.2f", credit.overpayment);
//   ck_assert_str_eq(resultStr, "2876801.44");
//   snprintf(resultStr, sizeof(resultStr), "%.2f", credit.interestCharges);
//   ck_assert_str_eq(resultStr, "7876801.44");
// }
// END_TEST

// START_TEST(test_credit_2) {
//   CreditSting creditString;
//   creditString.sumCredit = "5000000";
//   creditString.termCredit = "12";
//   creditString.monthOrYear = "year";
//   creditString.percentCredit = "8.21";
//   creditString.typeCredit = "differentiated";
//   Credit credit;
//   credit = credit_work(&creditString);
//   char resultStr[256] = "";
//   snprintf(resultStr, sizeof(resultStr), "%.2f", credit.maxMonthlyPayment);
//   ck_assert_str_eq(resultStr, "68930.56");
//   snprintf(resultStr, sizeof(resultStr), "%.2f", credit.minMonthlyPayment);
//   ck_assert_str_eq(resultStr, "34959.78");
//   snprintf(resultStr, sizeof(resultStr), "%.2f", credit.overpayment);
//   ck_assert_str_eq(resultStr, "2480104.17");
//   snprintf(resultStr, sizeof(resultStr), "%.2f", credit.interestCharges);
//   ck_assert_str_eq(resultStr, "7480104.17");
// }
// END_TEST

// START_TEST(test_credit_3) {
//   CreditSting creditString;
//   creditString.sumCredit = "12312000";
//   creditString.termCredit = "44";
//   creditString.monthOrYear = "month";
//   creditString.percentCredit = "13";
//   creditString.typeCredit = "annuity";
//   Credit credit;
//   credit = credit_work(&creditString);
//   char resultStr[256] = "";
//   snprintf(resultStr, sizeof(resultStr), "%.2f", credit.monthlyPayment);
//   ck_assert_str_eq(resultStr, "353271.16");
//   snprintf(resultStr, sizeof(resultStr), "%.2f", credit.overpayment);
//   ck_assert_str_eq(resultStr, "3231931.04");
//   snprintf(resultStr, sizeof(resultStr), "%.2f", credit.interestCharges);
//   ck_assert_str_eq(resultStr, "15543931.04");
// }
// END_TEST

// START_TEST(test_credit_4) {
//   CreditSting creditString;
//   creditString.sumCredit = "12312000";
//   creditString.termCredit = "44";
//   creditString.monthOrYear = "month";
//   creditString.percentCredit = "13";
//   creditString.typeCredit = "differentiated";
//   Credit credit;
//   credit = credit_work(&creditString);
//   char resultStr[256] = "";
//   snprintf(resultStr, sizeof(resultStr), "%.2f", credit.maxMonthlyPayment);
//   ck_assert_str_eq(resultStr, "413198.18");
//   snprintf(resultStr, sizeof(resultStr), "%.2f", credit.minMonthlyPayment);
//   ck_assert_str_eq(resultStr, "282849.55");
//   snprintf(resultStr, sizeof(resultStr), "%.2f", credit.overpayment);
//   ck_assert_str_eq(resultStr, "3001050.00");
//   snprintf(resultStr, sizeof(resultStr), "%.2f", credit.interestCharges);
//   ck_assert_str_eq(resultStr, "15313050.00");
// }
// END_TEST

// START_TEST(test_credit_5) {
//   CreditSting creditString;
//   creditString.sumCredit = "-12312000";
//   creditString.termCredit = "65";
//   creditString.monthOrYear = "month";
//   creditString.percentCredit = "-13";
//   creditString.typeCredit = "differentiated";
//   Credit credit;
//   credit = credit_work(&creditString);
//   ck_assert_int_eq(credit.check, FAILURE);
// }
// END_TEST

int main(void) {
  Suite *s1 = suite_create("fpeEGg");
  SRunner *sr = srunner_create(s1);
  int nf;

  TCase *tc1_1 = tcase_create("fpeEGg");
  suite_add_tcase(s1, tc1_1);
  tcase_add_test(tc1_1, test_calc_1);
  // tcase_add_test(tc1_1, test_calc_2);
  tcase_add_test(tc1_1, test_calc_3);
  tcase_add_test(tc1_1, test_calc_4);
  tcase_add_test(tc1_1, test_calc_5);
  tcase_add_test(tc1_1, test_calc_6);
  tcase_add_test(tc1_1, test_calc_7);
  tcase_add_test(tc1_1, test_calc_8);
  tcase_add_test(tc1_1, test_calc_9);
  tcase_add_test(tc1_1, test_calc_10);
  tcase_add_test(tc1_1, test_calc_11);
  tcase_add_test(tc1_1, test_calc_12);
  tcase_add_test(tc1_1, test_calc_13);
  tcase_add_test(tc1_1, test_calc_14);
  tcase_add_test(tc1_1, test_calc_15);
  tcase_add_test(tc1_1, test_calc_16);
  // tcase_add_test(tc1_1, test_calc_17);
  // tcase_add_test(tc1_1, test_credit_1);
  // tcase_add_test(tc1_1, test_credit_2);
  // tcase_add_test(tc1_1, test_credit_3);
  // tcase_add_test(tc1_1, test_credit_4);
  // tcase_add_test(tc1_1, test_credit_5);

  srunner_run_all(sr, CK_ENV);
  nf = srunner_ntests_failed(sr);
  srunner_free(sr);

  return nf == 0 ? 0 : 1;
}
