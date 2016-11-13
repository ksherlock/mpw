#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "floating_point.h"

#include <cmath>
#include <cstring>

namespace fp = floating_point;



void bitdump(const void *vp, unsigned bytes) {
  const uint8_t *p = (const uint8_t *)vp;

  p += bytes;

  for (unsigned i = 0; i < bytes; ++i) {
    uint8_t c = *(--p);
    for (int j = 0x80; j; j >>= 1) {
      printf ("%d", c & j ? 1 : 0);

    }
      printf(" ");
  }
  printf("\n");
}


TEST_CASE( "1.0 is handled", "[floating point info]") {

  fp::info fpi;

  fpi = fp::info((float)1.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x8000000000000000);

  fpi = fp::info((double)1.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x8000000000000000);

  fpi = fp::info((long double)1.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x8000000000000000);
}


TEST_CASE( "-1.0 is handled", "[floating point info]") {

  fp::info fpi;

  fpi = fp::info((float)-1.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 1);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x8000000000000000);

  fpi = fp::info((double)-1.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 1);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x8000000000000000);

  fpi = fp::info((long double)-1.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 1);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x8000000000000000);
}


TEST_CASE( "2.0 is handled", "[floating point info]") {

  fp::info fpi;

  fpi = fp::info((float)2.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 1);
  REQUIRE(fpi.sig == 0x8000000000000000);

  fpi = fp::info((double)2.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 1);
  REQUIRE(fpi.sig == 0x8000000000000000);

  fpi = fp::info((long double)2.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 1);
  REQUIRE(fpi.sig == 0x8000000000000000);
}


TEST_CASE( "NaN("") is handled", "[floating point info]") {

  fp::info fpi;

  fpi = fp::info(nanf(""));
  REQUIRE(fpi.nan == true);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x0000000000000000);

  fpi = fp::info(nan(""));
  REQUIRE(fpi.nan == true);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x0000000000000000);

  fpi = fp::info(nanl(""));
  REQUIRE(fpi.nan == true);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x0000000000000000);
}


TEST_CASE( "NaN(255) is handled", "[floating point info]") {

  fp::info fpi;

  fpi = fp::info(nanf("255"));
  REQUIRE(fpi.nan == true);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 255);

  fpi = fp::info(nan("255"));
  REQUIRE(fpi.nan == true);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 255);

  fpi = fp::info(nanl("255"));
  REQUIRE(fpi.nan == true);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 255);
}


TEST_CASE( "Inf is handled", "[floating point info]") {

  fp::info fpi;

  fpi = fp::info((float)1.0/0.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == true);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0);

  fpi = fp::info((double)1.0/0.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == true);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0);

  fpi = fp::info((long double)1.0/0.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == true);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0);
}


TEST_CASE( "Re-cast 0.0", "[floating point info]") {

  float target_f = 0.0;
  double target_d = 0.0;
  double target_ld = 0.0;
  long double ld;
  double d;
  float f;

  fp::info fpi(0.0l);

  fpi.write(f);
  fpi.write(d);
  fpi.write(ld);


  REQUIRE(memcmp(&target_f, &f, sizeof(f)) == 0);
  REQUIRE(memcmp(&target_d, &d, sizeof(d)) == 0);
  REQUIRE(memcmp(&target_ld, &ld, sizeof(ld)) == 0);

}

TEST_CASE( "Re-cast 1.0", "[floating point info]") {

  float target_f = 1.0;
  double target_d = 1.0;
  double target_ld = 1.0;
  long double ld;
  double d;
  float f;

  fp::info fpi(1.0l);

  fpi.write(f);
  fpi.write(d);
  fpi.write(ld);


  REQUIRE(memcmp(&target_f, &f, sizeof(f)) == 0);
  REQUIRE(memcmp(&target_d, &d, sizeof(d)) == 0);
  REQUIRE(memcmp(&target_ld, &ld, sizeof(ld)) == 0);
}


TEST_CASE( "Re-cast -1.0", "[floating point info]") {

  float target_f = -1.0;
  double target_d = -1.0;
  double target_ld = -1.0;
  long double ld;
  double d;
  float f;

  fp::info fpi(-1.0l);

  fpi.write(f);
  fpi.write(d);
  fpi.write(ld);


  REQUIRE(memcmp(&target_f, &f, sizeof(f)) == 0);
  REQUIRE(memcmp(&target_d, &d, sizeof(d)) == 0);
  REQUIRE(memcmp(&target_ld, &ld, sizeof(ld)) == 0);

}


TEST_CASE( "Re-cast 1000.0", "[floating point info]") {

  float target_f = 1000.0;
  double target_d = 1000.0;
  double target_ld = 1000.0;
  long double ld;
  double d;
  float f;

  fp::info fpi(1000.0l);

  fpi.write(f);
  fpi.write(d);
  fpi.write(ld);


  REQUIRE(memcmp(&target_f, &f, sizeof(f)) == 0);
  REQUIRE(memcmp(&target_d, &d, sizeof(d)) == 0);
  REQUIRE(memcmp(&target_ld, &ld, sizeof(ld)) == 0);
}



TEST_CASE( "Re-cast Inf", "[floating point info]") {

  float target_f = 1.0/0.0;
  double target_d = 1.0/0.0;
  double target_ld = 1.0/0.0;
  long double ld;
  double d;
  float f;

  fp::info fpi(1.0/0.0l);

  fpi.write(f);
  fpi.write(d);
  fpi.write(ld);


  REQUIRE(memcmp(&target_f, &f, sizeof(f)) == 0);
  REQUIRE(memcmp(&target_d, &d, sizeof(d)) == 0);
  REQUIRE(memcmp(&target_ld, &ld, sizeof(ld)) == 0);
}


TEST_CASE( "Re-cast NaN", "[floating point info]") {

  float target_f = nanf("16");
  double target_d = nan("16");
  double target_ld = nanl("16");
  long double ld;
  double d;
  float f;

  fp::info fpi(nanl("16"));

  fpi.write(f);
  fpi.write(d);
  fpi.write(ld);


  REQUIRE(memcmp(&target_f, &f, sizeof(f)) == 0);
  REQUIRE(memcmp(&target_d, &d, sizeof(d)) == 0);
  REQUIRE(memcmp(&target_ld, &ld, sizeof(ld)) == 0);
}
