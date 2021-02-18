#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <gmp.h>
#include <stdbool.h>
#ifdef _OPENMP
#include <omp.h>
#endif

#define DEBUG

/* printf for debuggin */
#ifdef DEBUG
#define debug_printf printf
#define debug_gmp_printf gmp_printf
#else
#define debug_printf 1 ? (void)0 : printf
#endif

void ShowResult(mpz_t testNum, bool b)
{
  if (b)
  {
    gmp_printf("Test Number:%Zd is Prime.\n", testNum);
  }
  else
  {
    gmp_printf("Test Number:%Zd is not Prime.\n", testNum);
  }
}
/******************************************************
 * Miller-Rabin 
 * input: Number 'testNum' to test a prime number
 * output: True when the number may be a prime number
 *        ,False when it is not a prime number.
 *
 * Reference: https://en.wikipedia.org/wiki/Miller%E2%80%93Rabin_primality_test
 * 
 *****************************************************/
bool MillerRabin(mpz_t testNum)
{
  mpz_t result;
  mpz_init(result);

  /* testNum == 2 */
  if (mpz_cmp_ui(testNum, 2) == 0)
    return true;

  /* result = testNum % 2 */
  mpz_mod_ui(result, testNum, 2);
  /*  2 < testNum && result == 0 */
  if (mpz_cmp_ui(testNum, 2) > 0 && mpz_cmp_ui(result, 0) == 0)
    return false;

  mpz_t op_s, op_t, op_u;
  mpz_init(op_s);
  mpz_init(op_t);
  mpz_init(op_u);

  /* op_s = 0 */
  mpz_set_ui(op_s, 0);
  /* op_t = testNum -1 */
  mpz_sub_ui(op_t, testNum, 1);
  mpz_sub_ui(op_u, testNum, 1);

  debug_gmp_printf("s:%Zd, t:%Zd\n", op_s, op_t);

  /* result = op_t % 2 */
  mpz_mod_ui(result, op_t, 2);
  while (mpz_cmp_ui(result, 0) == 0)
  {
    /* t = t >> 1 */
    mpz_tdiv_q_ui(op_t, op_t, 2);
    /* s++ */
    mpz_add_ui(op_s, op_s, 1);
    /* result = op_t % 2 */
    mpz_mod_ui(result, op_t, 2);
  }
  debug_gmp_printf("s:%Zd, t:%Zd\n", op_s, op_t);

  mpz_t op_a;
  mpz_init(op_a);
  // srand((unsigned)time(NULL));
  // unsigned long a = (unsigned long)rand() % (testNum - 1) + 1;

  /* op_a = 3 */
  mpz_set_ui(op_a, 3);

  /* Felmat Test */
  /* result = a^t % testNum */
  mpz_powm(result, op_a, op_t, testNum);
  debug_printf("\n************************\n");
  debug_gmp_printf("Felmat Test : a^t %% testnum == 1\na:%Zd\nresult:%Zd\n", op_a, result);
  debug_printf("************************\n\n");
  /* result == 1 */
  if (mpz_cmp_ui(result, 1) == 0)
    return true;

  /* while(i < s) */
  for (unsigned int i = 0; mpz_cmp_ui(op_s, (unsigned long)i) > 0 ; i++)
  {
    /* result = 2^i */
    mpz_ui_pow_ui(result, 2, (unsigned long)i);
    /* result = result * t */
    mpz_mul(result, result, op_t);
    /* result = a^result % testNum */
    mpz_powm(result, op_a, result, testNum);
    /* a^(2^i * t) % testNum == testNum - 1 */
    if (mpz_cmp(result, op_u) == 0)
      return true;
  }

  return false;
}

int main(int argc, char *argv[])
{
  int myID, rank;
  mpz_t testNum;
  bool result;

  mpz_init(testNum);

  printf("Test Number:");
  gmp_scanf("%Zd", testNum);

  debug_gmp_printf("Input Number : %Zd\n", testNum);

  result = MillerRabin(testNum);
  ShowResult(testNum, result);

  mpz_clear(testNum);

  return 0;
}