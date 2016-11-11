#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include "../toolbox/floating_point.h"

#include <cmath>
#include <cstring>

TEST_CASE( "1.0 is handled", "[floating point info]") {

  fpinfo fpi;

  fpi = fpinfo((float)1.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x8000000000000000);

  fpi = fpinfo((double)1.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x8000000000000000);

  fpi = fpinfo((long double)1.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x8000000000000000);
}


TEST_CASE( "-1.0 is handled", "[floating point info]") {

  fpinfo fpi;

  fpi = fpinfo((float)-1.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 1);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x8000000000000000);

  fpi = fpinfo((double)-1.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 1);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x8000000000000000);

  fpi = fpinfo((long double)-1.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 1);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x8000000000000000);
}


TEST_CASE( "2.0 is handled", "[floating point info]") {

  fpinfo fpi;

  fpi = fpinfo((float)2.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 1);
  REQUIRE(fpi.sig == 0x8000000000000000);

  fpi = fpinfo((double)2.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 1);
  REQUIRE(fpi.sig == 0x8000000000000000);

  fpi = fpinfo((long double)2.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 1);
  REQUIRE(fpi.sig == 0x8000000000000000);
}


TEST_CASE( "NaN("") is handled", "[floating point info]") {

  fpinfo fpi;

  fpi = fpinfo(nanf(""));
  REQUIRE(fpi.nan == true);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x0000000000000000);

  fpi = fpinfo(nan(""));
  REQUIRE(fpi.nan == true);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x0000000000000000);

  fpi = fpinfo(nanl(""));
  REQUIRE(fpi.nan == true);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0x0000000000000000);
}


TEST_CASE( "NaN(255) is handled", "[floating point info]") {

  fpinfo fpi;

  fpi = fpinfo(nanf("255"));
  REQUIRE(fpi.nan == true);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 255);

  fpi = fpinfo(nan("255"));
  REQUIRE(fpi.nan == true);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 255);

  fpi = fpinfo(nanl("255"));
  REQUIRE(fpi.nan == true);
  REQUIRE(fpi.inf == false);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 255);
}


TEST_CASE( "Inf is handled", "[floating point info]") {

  fpinfo fpi;

  fpi = fpinfo((float)1.0/0.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == true);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0);

  fpi = fpinfo((double)1.0/0.0);
  REQUIRE(fpi.nan == false);
  REQUIRE(fpi.inf == true);
  REQUIRE(fpi.sign == 0);
  REQUIRE(fpi.exp == 0);
  REQUIRE(fpi.sig == 0);

  fpi = fpinfo((long double)1.0/0.0);
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
  double d;
  float f;

  fpinfo fpi(0.0l);

  fpi.write(f);
  fpi.write(d);


  REQUIRE(memcmp(&target_f, &f, sizeof(f)) == 0);
  REQUIRE(memcmp(&target_d, &d, sizeof(d)) == 0);

}

TEST_CASE( "Re-cast 1.0", "[floating point info]") {

  float target_f = 1.0;
  double target_d = 1.0;
  double target_ld = 1.0;
  double d;
  float f;

  fpinfo fpi(1.0l);

  fpi.write(f);
  fpi.write(d);


  REQUIRE(memcmp(&target_f, &f, sizeof(f)) == 0);
  REQUIRE(memcmp(&target_d, &d, sizeof(d)) == 0);
}


TEST_CASE( "Re-cast -1.0", "[floating point info]") {

  float target_f = -1.0;
  double target_d = -1.0;
  double target_ld = -1.0;
  double d;
  float f;

  fpinfo fpi(-1.0l);

  fpi.write(f);
  fpi.write(d);


  REQUIRE(memcmp(&target_f, &f, sizeof(f)) == 0);
  REQUIRE(memcmp(&target_d, &d, sizeof(d)) == 0);

}


TEST_CASE( "Re-cast 1000.0", "[floating point info]") {

  float target_f = 1000.0;
  double target_d = 1000.0;
  double target_ld = 1000.0;
  double d;
  float f;

  fpinfo fpi(1000.0l);

  fpi.write(f);
  fpi.write(d);


  REQUIRE(memcmp(&target_f, &f, sizeof(f)) == 0);
  REQUIRE(memcmp(&target_d, &d, sizeof(d)) == 0);
}



TEST_CASE( "Re-cast Inf", "[floating point info]") {

  float target_f = 1.0/0.0;
  double target_d = 1.0/0.0;
  double target_ld = 1.0/0.0;
  double d;
  float f;

  fpinfo fpi(1.0/0.0l);

  fpi.write(f);
  fpi.write(d);


  REQUIRE(memcmp(&target_f, &f, sizeof(f)) == 0);
  REQUIRE(memcmp(&target_d, &d, sizeof(d)) == 0);
}


TEST_CASE( "Re-cast NaN", "[floating point info]") {

  float target_f = nanf("16");
  double target_d = nan("16");
  double target_ld = nanl("16");
  double d;
  float f;

  fpinfo fpi(nanl("16"));

  fpi.write(f);
  fpi.write(d);


  REQUIRE(memcmp(&target_f, &f, sizeof(f)) == 0);
  REQUIRE(memcmp(&target_d, &d, sizeof(d)) == 0);
}
