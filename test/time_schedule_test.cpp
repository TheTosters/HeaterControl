#include <gtest/gtest.h>
#include "temperatureSheduler.h"

TEST(temperatureShedulerTest, checkDefaultAlias) {
  TemperatureSheduler sch;
  EXPECT_EQ(1, sch.getTemperatureAliasCount());
}

TEST(temperatureShedulerTest, removeDefaultAlias) {
  TemperatureSheduler sch;
  auto alias = sch.getTemperatureAlias(0);
  sch.removeTemperatureAlias(alias.name);
  EXPECT_EQ(1, sch.getTemperatureAliasCount());
}

TEST(temperatureShedulerTest, addAlias) {
  TemperatureSheduler sch;
  sch.addTemperatureAlias("alias-1", TemperatureC(7));

  //default, alias-1 => count:2
  EXPECT_EQ(2, sch.getTemperatureAliasCount());
}

TEST(temperatureShedulerTest, removeAlias) {
  TemperatureSheduler sch;
  const std::string ALIAS_NAME{"alias-1"};
  sch.addTemperatureAlias(ALIAS_NAME, TemperatureC(7));
  sch.removeTemperatureAlias(ALIAS_NAME);

  EXPECT_EQ(1, sch.getTemperatureAliasCount());
  auto al = sch.getTemperatureAlias(0);
  EXPECT_NE(ALIAS_NAME, al.name);
}

TEST(temperatureShedulerTest, iterateAliases) {
  TemperatureSheduler sch;

  const std::string ALIAS_1_NAME{"alias-1"};
  const TemperatureC ALIAS_1_TEMP{7};

  const std::string ALIAS_2_NAME{"alias-2"};
  const TemperatureC ALIAS_2_TEMP{21};

  sch.addTemperatureAlias(ALIAS_1_NAME, ALIAS_1_TEMP);
  sch.addTemperatureAlias(ALIAS_2_NAME, ALIAS_2_TEMP);

  EXPECT_EQ(3, sch.getTemperatureAliasCount());

  auto al1 = sch.getTemperatureAlias(1);
  EXPECT_EQ(ALIAS_1_NAME, al1.name);
  EXPECT_EQ(ALIAS_1_TEMP, al1.temperature);

  auto al2 = sch.getTemperatureAlias(2);
  EXPECT_EQ(ALIAS_2_NAME, al2.name);
  EXPECT_EQ(ALIAS_2_TEMP, al2.temperature);
}

TEST(temperatureShedulerTest, updateAlias) {
  TemperatureSheduler sch;

  const std::string ALIAS_1_NAME{"alias-1"};
  const TemperatureC ALIAS_1_TEMP{7};

  const std::string ALIAS_2_NAME{"alias-2"};
  const TemperatureC ALIAS_2_TEMP{21};

  sch.addTemperatureAlias(ALIAS_1_NAME, ALIAS_1_TEMP);
  sch.updateTemperatureAlias(ALIAS_1_NAME, ALIAS_2_NAME, ALIAS_2_TEMP);

  auto al2 = sch.getTemperatureAlias(1);
  EXPECT_EQ(ALIAS_2_NAME, al2.name);
  EXPECT_EQ(ALIAS_2_TEMP, al2.temperature);
}

TEST(temperatureShedulerTest, updateAlias_periods) {
  TemperatureSheduler sch;
  using namespace std::chrono;
  WeekTime start{WeekDay::SUNDAY, hours{12}, minutes{00}};
  WeekTime end{WeekDay::SUNDAY, hours{12}, minutes{40}};

  const std::string ALIAS_1_NAME{"alias-1"};
  const std::string ALIAS_2_NAME{"alias-2"};
  const TemperatureC ALIAS_1_TEMP{7};
  sch.addTemperatureAlias(ALIAS_1_NAME, ALIAS_1_TEMP);
  sch.setTemperaturePeriod(start, end, ALIAS_1_NAME);
  EXPECT_EQ(1, sch.getPeriodsCount());

  sch.updateTemperatureAlias(ALIAS_1_NAME, ALIAS_2_NAME, ALIAS_1_TEMP);
  auto period = sch.getPeriod(0);
  EXPECT_EQ(ALIAS_2_NAME, period.temperatureAlias);
}
