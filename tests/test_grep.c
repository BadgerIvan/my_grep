#include <check.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void compare_grep_output(const char *filename, const char *flags) {
  char my_grep[256];
  char real_grep[256];

  snprintf(my_grep, sizeof(my_grep) * sizeof(char), "./my_grep %s %s", flags,
           filename);
  snprintf(real_grep, sizeof(real_grep) * sizeof(char), "grep %s %s", flags,
           filename);

  FILE *your_output = popen(my_grep, "r");
  FILE *real_output = popen(real_grep, "r");

  char my_line[1024], real_line[1024];
  while (fgets(my_line, sizeof(my_line), your_output) &&
         fgets(real_line, sizeof(real_line), real_output)) {
    ck_assert_str_eq(my_line, real_line);
  }

  pclose(your_output);
  pclose(real_output);
}

// simple
START_TEST(simple_1) {
  compare_grep_output("../tests/test_cases/test1", "L");
}
END_TEST;

START_TEST(simple_2) {
  compare_grep_output("../tests/test_cases/test2", "ERROR");
}
END_TEST;

START_TEST(simple_3) {
  compare_grep_output("../tests/test_cases/test3", "HEAP");
}
END_TEST;

// flag_e
START_TEST(flag_e_1) {
  compare_grep_output("../tests/test_cases/test1", "-e L -e T -e t");
}
END_TEST;

START_TEST(flag_e_2) {
  compare_grep_output("../tests/test_cases/test2", "-e ERROR -e DEBUG");
}
END_TEST;

START_TEST(flag_e_3) {
  compare_grep_output("../tests/test_cases/test3", "-e HEAP -e LEAK");
}
END_TEST;

// flag_i
START_TEST(flag_i_1) {
  compare_grep_output("../tests/test_cases/test1", "-i L");
}
END_TEST;

START_TEST(flag_i_2) {
  compare_grep_output("../tests/test_cases/test2", "-i ERROR");
}
END_TEST;

START_TEST(flag_i_3) {
  compare_grep_output("../tests/test_cases/test3", "-i HEAP");
}
END_TEST;

// flag_v
START_TEST(flag_v_1) {
  compare_grep_output("../tests/test_cases/test1", "-v L");
}
END_TEST;

START_TEST(flag_v_2) {
  compare_grep_output("../tests/test_cases/test2", "-v ERROR");
}
END_TEST;

START_TEST(flag_v_3) {
  compare_grep_output("../tests/test_cases/test3", "-v HEAP");
}
END_TEST;

// flag_c
START_TEST(flag_c_1) {
  compare_grep_output("../tests/test_cases/test1", "-c L");
}
END_TEST;

START_TEST(flag_c_2) {
  compare_grep_output("../tests/test_cases/test2", "-c ERROR");
}
END_TEST;

START_TEST(flag_c_3) {
  compare_grep_output("../tests/test_cases/test3", "-c HEAP");
}
END_TEST;

// flag_l
START_TEST(flag_l_1) {
  compare_grep_output("../tests/test_cases/test1", "-l L");
}
END_TEST;

START_TEST(flag_l_2) {
  compare_grep_output("../tests/test_cases/test2", "-l ERROR");
}
END_TEST;

START_TEST(flag_l_3) {
  compare_grep_output("../tests/test_cases/test3", "-l HEAP");
}
END_TEST;

// flag_n
START_TEST(flag_n_1) {
  compare_grep_output("../tests/test_cases/test1", "-n L");
}
END_TEST;

START_TEST(flag_n_2) {
  compare_grep_output("../tests/test_cases/test2", "-n ERROR");
}
END_TEST;

START_TEST(flag_n_3) {
  compare_grep_output("../tests/test_cases/test3", "-n HEAP");
}
END_TEST;

// shakes_flags
START_TEST(shakes_flags_1) {
  compare_grep_output("../tests/test_cases/test1 unknow_file",
                      "-n -e L -e t -l -l -n -s -o");
}
END_TEST;

START_TEST(shakes_flags_2) {
  compare_grep_output(
      "../tests/test_cases/test2 ../tests/test_cases/test3",
      "-n -e ERROR -c -h");
}
END_TEST;

START_TEST(shakes_flags_3) {
  compare_grep_output("../tests/test_cases/test3", "-c -l -v HEAP");
}
END_TEST;

// all_flags
START_TEST(all_flags_1) {
  compare_grep_output("../tests/test_cases/test1 unknow_file",
                      "-e L -l -i -v -c -l -n -h -s");
}
END_TEST;

START_TEST(all_flags_2) {
  compare_grep_output("../tests/test_cases/test2 unknow_file",
                      "-e ERROR -l -i -v -c -l -n -h -s");
}
END_TEST;

START_TEST(all_flags_3) {
  compare_grep_output("../tests/test_cases/test3 unknow_file",
                      "-e HEAP -l -i -v -c -l -n -h -s");
}
END_TEST;

// flag_h
START_TEST(flag_h_1) {
  compare_grep_output(
      "../tests/test_cases/test1 ../tests/test_cases/test2 "
      "../tests/test_cases/test3 ../tests/test_cases/test4",
      "-e L -e ERROR -e HEAP -h");
}
END_TEST;

START_TEST(flag_h_2) {
  compare_grep_output("../tests/test_cases/test2", "-h ERROR");
}
END_TEST;

START_TEST(flag_h_3) {
  compare_grep_output(
      "../tests/test_cases/test3 ../tests/test_cases/test4",
      "-h -e ERROR -e HEAP");
}
END_TEST;

// flag_s
START_TEST(flag_s_1) {
  compare_grep_output(
      "../tests/test_cases/test1 ../tests/test_cases/test2 "
      "../tests/test_cases/test3 unknow_file",
      "-e L -e ERROR -s");
}
END_TEST;

START_TEST(flag_s_2) { compare_grep_output("unknow_file", "-s ERROR"); }
END_TEST;

START_TEST(flag_s_3) {
  compare_grep_output("unknow_file ../tests/test_cases/test4",
                      "-s -e ERROR -e HEAP");
}
END_TEST;

// hard
START_TEST(hard_1) {
  compare_grep_output(
      "../tests/test_cases/test1 ../tests/test_cases/test2 "
      "../tests/test_cases/test3",
      "-e L -e ERROR -e HEAP -l -i -v -c -l -n");
}
END_TEST;

START_TEST(hard_2) {
  compare_grep_output("../tests/test_cases/test4",
                      "-e L -e ERROR -e HEAP -c -n");
}
END_TEST;

START_TEST(hard_3) {
  compare_grep_output("../tests/test_cases/test5",
                      "-f ../tests/test_cases/patterns2");
}
END_TEST;

// flag_f
START_TEST(flag_f_1) {
  compare_grep_output(
      "../tests/test_cases/test1 ../tests/test_cases/test2 "
      "../tests/test_cases/test3",
      "-f ../tests/test_cases/patterns1");
}
END_TEST;

START_TEST(flag_f_2) {
  compare_grep_output(
      "../tests/test_cases/test1 ../tests/test_cases/test2 "
      "../tests/test_cases/test3",
      "-f ../tests/test_cases/patterns_empty");
}
END_TEST;

START_TEST(flag_f_3) {
  compare_grep_output(
      "../tests/test_cases/test1 ../tests/test_cases/test2 "
      "../tests/test_cases/test4",
      "-f ../tests/test_cases/patterns1 -f "
      "../tests/test_cases/patterns_empty");
}
END_TEST;

// flag_o
START_TEST(flag_o_1) {
  compare_grep_output(
      "../tests/test_cases/test1 ../tests/test_cases/test2 "
      "../tests/test_cases/test3",
      "-f ../tests/test_cases/patterns1 -o");
}
END_TEST;

START_TEST(flag_o_2) {
  compare_grep_output(
      "../tests/test_cases/test1 ../tests/test_cases/test2 "
      "../tests/test_cases/test3",
      "HEAP -o");
}
END_TEST;

START_TEST(flag_o_3) {
  compare_grep_output(
      "../tests/test_cases/test1 ../tests/test_cases/test2 "
      "../tests/test_cases/test5",
      "-f ../tests/test_cases/patterns2 -o");
}
END_TEST;

// double flags
START_TEST(flag_iv) {
  compare_grep_output("../tests/test_cases/test5", "-iv город-сад");
}
END_TEST;

START_TEST(flag_ic) {
  compare_grep_output("../tests/test_cases/test5", "-ic город-сад");
}
END_TEST;

START_TEST(flag_il) {
  compare_grep_output("../tests/test_cases/test5", "-il город-сад");
}
END_TEST;

START_TEST(flag_in) {
  compare_grep_output("../tests/test_cases/test5", "-in город-сад");
}
END_TEST;

START_TEST(flag_ih) {
  compare_grep_output("../tests/test_cases/test5", "-ih город-сад");
}
END_TEST;

START_TEST(flag_is) {
  compare_grep_output("../tests/test_cases/test5", "-is город-сад");
}
END_TEST;

START_TEST(flag_io) {
  compare_grep_output("../tests/test_cases/test5", "-io город-сад");
}
END_TEST;

START_TEST(flag_vc) {
  compare_grep_output("../tests/test_cases/test5", "-vc город-сад");
}
END_TEST;

START_TEST(flag_vl) {
  compare_grep_output("../tests/test_cases/test5", "-vl город-сад");
}
END_TEST;

START_TEST(flag_vn) {
  compare_grep_output("../tests/test_cases/test5", "-vn город-сад");
}
END_TEST;

START_TEST(flag_vh) {
  compare_grep_output("../tests/test_cases/test5", "-vh город-сад");
}
END_TEST;

START_TEST(flag_vs) {
  compare_grep_output("../tests/test_cases/test5", "-vs город-сад");
}
END_TEST;

START_TEST(flag_vo) {
  compare_grep_output("../tests/test_cases/test5", "-vo город-сад");
}
END_TEST;

START_TEST(flag_cl) {
  compare_grep_output("../tests/test_cases/test5", "-cl город-сад");
}
END_TEST;

START_TEST(flag_cn) {
  compare_grep_output("../tests/test_cases/test5", "-cn город-сад");
}
END_TEST;

START_TEST(flag_ch) {
  compare_grep_output("../tests/test_cases/test5", "-ch город-сад");
}
END_TEST;

START_TEST(flag_cs) {
  compare_grep_output("../tests/test_cases/test5", "-cs город-сад");
}
END_TEST;

START_TEST(flag_co) {
  compare_grep_output("../tests/test_cases/test5", "-co город-сад");
}
END_TEST;

START_TEST(flag_ln) {
  compare_grep_output("../tests/test_cases/test5", "-ln город-сад");
}
END_TEST;

START_TEST(flag_lh) {
  compare_grep_output("../tests/test_cases/test5", "-lh город-сад");
}
END_TEST;

START_TEST(flag_ls) {
  compare_grep_output("../tests/test_cases/test5", "-ls город-сад");
}
END_TEST;

START_TEST(flag_lo) {
  compare_grep_output("../tests/test_cases/test5", "-lo город-сад");
}
END_TEST;

START_TEST(flag_nh) {
  compare_grep_output("../tests/test_cases/test5", "-nh город-сад");
}
END_TEST;

START_TEST(flag_ns) {
  compare_grep_output("../tests/test_cases/test5", "-ns город-сад");
}
END_TEST;

START_TEST(flag_no) {
  compare_grep_output("../tests/test_cases/test5", "-no город-сад");
}
END_TEST;

START_TEST(flag_hs) {
  compare_grep_output("../tests/test_cases/test5", "-hs город-сад");
}
END_TEST;

START_TEST(flag_ho) {
  compare_grep_output("../tests/test_cases/test5", "-ho город-сад");
}
END_TEST;

START_TEST(flag_so) {
  compare_grep_output("../tests/test_cases/test5", "-so город-сад");
}
END_TEST;

Suite *first_suite() {
  Suite *suite = suite_create("first suite");
  TCase *simple = tcase_create("simple");
  tcase_add_test(simple, simple_1);
  tcase_add_test(simple, simple_2);
  tcase_add_test(simple, simple_3);
  suite_add_tcase(suite, simple);
  TCase *flag_e = tcase_create("flag_e");
  tcase_add_test(flag_e, flag_e_1);
  tcase_add_test(flag_e, flag_e_2);
  tcase_add_test(flag_e, flag_e_3);
  suite_add_tcase(suite, flag_e);
  TCase *flag_i = tcase_create("flag_i");
  tcase_add_test(flag_i, flag_i_1);
  tcase_add_test(flag_i, flag_i_2);
  tcase_add_test(flag_i, flag_i_3);
  suite_add_tcase(suite, flag_i);
  TCase *flag_v = tcase_create("flag_v");
  tcase_add_test(flag_v, flag_v_1);
  tcase_add_test(flag_v, flag_v_2);
  tcase_add_test(flag_v, flag_v_3);
  suite_add_tcase(suite, flag_v);
  TCase *flag_c = tcase_create("flag_c");
  tcase_add_test(flag_c, flag_c_1);
  tcase_add_test(flag_c, flag_c_2);
  tcase_add_test(flag_c, flag_c_3);
  suite_add_tcase(suite, flag_c);
  TCase *flag_l = tcase_create("flag_l");
  tcase_add_test(flag_l, flag_l_1);
  tcase_add_test(flag_l, flag_l_2);
  tcase_add_test(flag_l, flag_l_3);
  suite_add_tcase(suite, flag_l);
  TCase *flag_n = tcase_create("flag_n");
  tcase_add_test(flag_n, flag_n_1);
  tcase_add_test(flag_n, flag_n_2);
  tcase_add_test(flag_n, flag_n_3);
  suite_add_tcase(suite, flag_n);
  TCase *shakes_flags = tcase_create("shakes_flags");
  tcase_add_test(shakes_flags, shakes_flags_1);
  tcase_add_test(shakes_flags, shakes_flags_2);
  tcase_add_test(shakes_flags, shakes_flags_3);
  suite_add_tcase(suite, shakes_flags);
  TCase *all_flags = tcase_create("all_flags");
  tcase_add_test(all_flags, all_flags_1);
  tcase_add_test(all_flags, all_flags_2);
  tcase_add_test(all_flags, all_flags_3);
  suite_add_tcase(suite, all_flags);
  return suite;
}

Suite *second_suite() {
  Suite *suite = suite_create("second suite");
  TCase *hard = tcase_create("hard");
  tcase_add_test(hard, hard_1);
  tcase_add_test(hard, hard_2);
  tcase_add_test(hard, hard_3);
  suite_add_tcase(suite, hard);
  TCase *flag_h = tcase_create("flag_h");
  tcase_add_test(flag_h, flag_h_1);
  tcase_add_test(flag_h, flag_h_2);
  tcase_add_test(flag_h, flag_h_3);
  suite_add_tcase(suite, flag_h);
  TCase *flag_s = tcase_create("flag_s");
  tcase_add_test(flag_s, flag_s_1);
  tcase_add_test(flag_s, flag_s_2);
  tcase_add_test(flag_s, flag_s_3);
  suite_add_tcase(suite, flag_s);
  TCase *flag_f = tcase_create("flag_f");
  tcase_add_test(flag_f, flag_f_1);
  tcase_add_test(flag_f, flag_f_2);
  tcase_add_test(flag_f, flag_f_3);
  suite_add_tcase(suite, flag_f);
  TCase *flag_o = tcase_create("flag_o");
  tcase_add_test(flag_o, flag_o_1);
  tcase_add_test(flag_o, flag_o_2);
  tcase_add_test(flag_o, flag_o_3);
  suite_add_tcase(suite, flag_o);
  return suite;
}

Suite *third_suite() {
  Suite *suite = suite_create("third_suite");
  TCase *flag_i_pairs = tcase_create("flag_i_combinations");
  tcase_add_test(flag_i_pairs, flag_iv);
  tcase_add_test(flag_i_pairs, flag_ic);
  tcase_add_test(flag_i_pairs, flag_il);
  tcase_add_test(flag_i_pairs, flag_in);
  tcase_add_test(flag_i_pairs, flag_ih);
  tcase_add_test(flag_i_pairs, flag_is);
  tcase_add_test(flag_i_pairs, flag_io);
  suite_add_tcase(suite, flag_i_pairs);
  TCase *flag_v_pairs = tcase_create("flag_v_combinations");
  tcase_add_test(flag_v_pairs, flag_vc);
  tcase_add_test(flag_v_pairs, flag_vl);
  tcase_add_test(flag_v_pairs, flag_vn);
  tcase_add_test(flag_v_pairs, flag_vh);
  tcase_add_test(flag_v_pairs, flag_vs);
  tcase_add_test(flag_v_pairs, flag_vo);
  suite_add_tcase(suite, flag_v_pairs);
  TCase *flag_c_pairs = tcase_create("flag_c_combinations");
  tcase_add_test(flag_c_pairs, flag_cl);
  tcase_add_test(flag_c_pairs, flag_cn);
  tcase_add_test(flag_c_pairs, flag_ch);
  tcase_add_test(flag_c_pairs, flag_cs);
  tcase_add_test(flag_c_pairs, flag_co);
  suite_add_tcase(suite, flag_c_pairs);
  TCase *flag_l_pairs = tcase_create("flag_l_combinations");
  tcase_add_test(flag_l_pairs, flag_ln);
  tcase_add_test(flag_l_pairs, flag_lh);
  tcase_add_test(flag_l_pairs, flag_ls);
  tcase_add_test(flag_l_pairs, flag_lo);
  suite_add_tcase(suite, flag_l_pairs);
  TCase *flag_n_pairs = tcase_create("flag_n_combinations");
  tcase_add_test(flag_n_pairs, flag_nh);
  tcase_add_test(flag_n_pairs, flag_ns);
  tcase_add_test(flag_n_pairs, flag_no);
  suite_add_tcase(suite, flag_n_pairs);
  TCase *flag_h_pairs = tcase_create("flag_h_combinations");
  tcase_add_test(flag_h_pairs, flag_hs);
  tcase_add_test(flag_h_pairs, flag_ho);
  suite_add_tcase(suite, flag_h_pairs);
  TCase *flag_s_pairs = tcase_create("flag_s_combinations");
  tcase_add_test(flag_s_pairs, flag_so);
  suite_add_tcase(suite, flag_s_pairs);
  return suite;
}

int main() {
  SRunner *sr = srunner_create(NULL);
  srunner_add_suite(sr, first_suite());
  srunner_add_suite(sr, second_suite());
  srunner_add_suite(sr, third_suite());
  srunner_run_all(sr, CK_NORMAL);
  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}