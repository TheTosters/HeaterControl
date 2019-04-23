#include "temperatureSheduler.h"
#include "decoded_time.h"

#include <gtest/gtest.h>
#include <math.h>
#include <chrono>
#include <iostream>

//uncomment this if you want to see ranges in tests
#define SHOW_RANGES

TEST(temperatureShedulerTest, checkIfDefaultAliasIsCreated) {
  TemperatureSheduler sch;
  EXPECT_EQ(1, sch.getTemperatureAliasCount());
}

TEST(temperatureShedulerTest, checkIfRemoveDefaultAliasIsImpossible) {
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

TEST(temperatureShedulerTest, removePeriodsWhenAliasIsRemoved) {
  TemperatureSheduler sch;
  const std::string ALIAS_NAME{"alias-1"};
  sch.addTemperatureAlias(ALIAS_NAME, TemperatureC(7));

  using namespace std::chrono;
  WeekTime start{WeekDay::SUNDAY, hours{12}, minutes{00}};
  WeekTime end{WeekDay::SUNDAY, hours{12}, minutes{40}};
  sch.setTemperaturePeriod(start, end, ALIAS_NAME);

  sch.removeTemperatureAlias(ALIAS_NAME);
  EXPECT_EQ(0, sch.getPeriodsCount());
}

TEST(temperatureShedulerTest, removePeriodsWhenAliasIsRemoved2) {
  TemperatureSheduler sch;
  const std::string ALIAS_1_NAME{"alias-1"};
  const std::string ALIAS_2_NAME{"alias-2"};
  sch.addTemperatureAlias(ALIAS_1_NAME, TemperatureC(7));
  sch.addTemperatureAlias(ALIAS_2_NAME, TemperatureC(27));

  using namespace std::chrono;
  sch.setTemperaturePeriod(
      {WeekDay::SUNDAY, hours{12}, minutes{00}},
      {WeekDay::SUNDAY, hours{12}, minutes{40}},
      ALIAS_1_NAME);
  sch.setTemperaturePeriod(
      {WeekDay::SUNDAY, hours{13}, minutes{00}},
      {WeekDay::SUNDAY, hours{13}, minutes{40}},
      ALIAS_2_NAME);
  sch.setTemperaturePeriod(
      {WeekDay::SUNDAY, hours{14}, minutes{00}},
      {WeekDay::SUNDAY, hours{14}, minutes{40}},
      ALIAS_1_NAME);

  sch.removeTemperatureAlias(ALIAS_1_NAME);
  EXPECT_EQ(1, sch.getPeriodsCount());
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

TEST(temperatureShedulerTest, updateAliasesInPeriodsWhenAliasIsUpdated) {
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

TEST(temperatureShedulerTest, removePeriod) {
  TemperatureSheduler sch;
  const std::string ALIAS_1_NAME{"alias-1"};
  const std::string ALIAS_2_NAME{"alias-2"};
  sch.addTemperatureAlias(ALIAS_1_NAME, TemperatureC(7));
  sch.addTemperatureAlias(ALIAS_2_NAME, TemperatureC(27));

  using namespace std::chrono;
  sch.setTemperaturePeriod(
      {WeekDay::SUNDAY, hours{12}, minutes{00}},
      {WeekDay::SUNDAY, hours{12}, minutes{40}},
      ALIAS_1_NAME);
  sch.setTemperaturePeriod(
      {WeekDay::SUNDAY, hours{13}, minutes{00}},
      {WeekDay::SUNDAY, hours{13}, minutes{40}},
      ALIAS_2_NAME);
  sch.setTemperaturePeriod(
      {WeekDay::SUNDAY, hours{14}, minutes{00}},
      {WeekDay::SUNDAY, hours{14}, minutes{40}},
      ALIAS_1_NAME);

  sch.removeTemperaturePeriod({WeekDay::SUNDAY, hours{13}, minutes{00}});
  EXPECT_EQ(2, sch.getPeriodsCount());

  sch.removeTemperaturePeriod({WeekDay::SUNDAY, hours{12}, minutes{00}});
  sch.removeTemperaturePeriod({WeekDay::SUNDAY, hours{14}, minutes{00}});
  EXPECT_EQ(0, sch.getPeriodsCount());
}

struct TimeRange {
  WeekTime start;
  WeekTime end;

  TimeRange(const WeekTime& start, const WeekTime& end)
  : start(start), end(end)
  {}

  void validate(TemperatureSheduler& sch, const TemperatureC& temp) {
    using namespace std::chrono;
    const std::chrono::seconds delta{60};
    for(WeekTime t = start; t < end; t = t + delta) {
      EXPECT_EQ(temp, sch.getTemperature(t));
      if (::testing::Test::HasFailure()) {
        break;
      }
    }
  }

  bool inRange(WeekTime t) const {
    return (start <= t) and (t < end);
  }
};

struct temperatureShedulerTestFixture : public testing::Test {
  TemperatureSheduler sch;
  const std::string ALIAS_1_NAME{"alias-1"};
  const TemperatureC ALIAS_1_TEMP{7};

  const std::string ALIAS_2_NAME{"alias-2"};
  const TemperatureC ALIAS_2_TEMP{17};

  const std::string ALIAS_3_NAME{"alias-3"};
  const TemperatureC ALIAS_3_TEMP{27};

  const std::string ALIAS_4_NAME{"alias-4"};
  const TemperatureC ALIAS_4_TEMP{11};

  void SetUp() {
    sch.addTemperatureAlias(ALIAS_1_NAME, ALIAS_1_TEMP);
    sch.addTemperatureAlias(ALIAS_2_NAME, ALIAS_2_TEMP);
    sch.addTemperatureAlias(ALIAS_3_NAME, ALIAS_3_TEMP);
    sch.addTemperatureAlias(ALIAS_4_NAME, ALIAS_4_TEMP);
  }

  void TearDown() {
  }

  void printScenarioRanges(const std::vector<TimeRange> ranges,
      TimeRange toInsert) {
    if (::testing::Test::HasFailure()) {
      std::cout << "Ranges for this scenario: \n";
      TimeRange limits = findLimits(ranges, toInsert);
      printRanges(limits, ranges);
      char c = 'A' + static_cast<char>(ranges.size());
      printRange(limits, toInsert, c);
      dumpPeriods();
    }
  }

  void dumpPeriods() {
    using namespace std::chrono;
    for(int t = 0; t < sch.getPeriodsCount(); t++) {
      auto period = sch.getPeriod(t);
      seconds secStart = static_cast<seconds>(period.startTime);
      seconds secEnd = static_cast<seconds>(period.endTime);

      DecodedTime dStart{secStart};
      DecodedTime dEnd{secEnd};

      std::cout << t << ": [" << period.temperatureAlias << "] " <<
          static_cast<std::string>(dStart) << " - " <<
          static_cast<std::string>(dEnd) << '\n';
    }
  }

  void printRange(TimeRange timeSpace, const TimeRange& range, char c) {
    std::chrono::seconds delta {15 * 60};
    for(auto t = timeSpace.start; t <= timeSpace.end; t = t + delta) {
      if (range.inRange(t)) {
        std::cout << c;

      } else {
        std::cout << ' ';
      }
    }
    std::cout << '\n';
  }

  void printRanges(const TimeRange& timeSpace,
      const std::vector<TimeRange>& ranges) {

    std::chrono::seconds delta {15 * 60};
    for(auto t = timeSpace.start; t <= timeSpace.end; t = t + delta) {
      auto it = std::find_if(ranges.begin(), ranges.end(), [=](auto& r){
        return r.inRange(t);
      });

      if (it != ranges.end()) {
        char dist = 'A' + static_cast<char>(std::distance(ranges.begin(), it));
        std::cout << dist;

      } else {
        std::cout << ' ';
      }
    }
    std::cout << '\n';
  }

  TimeRange findLimits(const std::vector<TimeRange>& ranges,
      const TimeRange& r) {
    using namespace std::chrono;
    TimeRange timeSpace {
      WeekTime{WeekDay::SATURDAY, hours{23}, minutes{59}},
      WeekTime{WeekDay::SUNDAY, hours{0}, minutes{0}}
    };

    for(const TimeRange& t : ranges) {
      timeSpace.start = std::min(t.start, timeSpace.start);
      timeSpace.end = std::max(t.end, timeSpace.end);
    }
    timeSpace.start = std::min(r.start, timeSpace.start);
    timeSpace.end = std::max(r.end, timeSpace.end);
    return timeSpace;
  }
};

TEST_F(temperatureShedulerTestFixture, setOverlapingPeriod) {
  //situation to test
  // | -- PERIOD 1 --      |        -- PERIOD 2 --|
  // | -- PERIOD 1 -- | -- NEW PER -- | PERIOD 2--|

  using namespace std::chrono;
  TimeRange P1 {
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange P2 {
    {WeekDay::SUNDAY, hours{13}, minutes{00}},
    {WeekDay::SUNDAY, hours{14}, minutes{00}}
  };
  TimeRange PNEW {
    {WeekDay::SUNDAY, hours{12}, minutes{30}},
    {WeekDay::SUNDAY, hours{13}, minutes{30}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  sch.setTemperaturePeriod(P2.start, P2.end, ALIAS_2_NAME);
  EXPECT_EQ(2, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_3_NAME);

  EXPECT_EQ(3, sch.getPeriodsCount());
  EXPECT_EQ(ALIAS_1_TEMP, sch.getTemperature(
      {WeekDay::SUNDAY, hours{12}, minutes{29}}));
  EXPECT_EQ(ALIAS_2_TEMP, sch.getTemperature(
      {WeekDay::SUNDAY, hours{13}, minutes{31}}));
  PNEW.validate(sch, ALIAS_3_TEMP);
  printScenarioRanges({P1, P2}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setHeadOverlapingPeriod) {
  //situation to test
  // | PERIOD 1  | <free> |    PERIOD 2        |
  // | PERIOD 1  | <free> | NEW PER | PERIOD 2 |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange P2{
    {WeekDay::SUNDAY, hours{14}, minutes{00}},
    {WeekDay::SUNDAY, hours{15}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{14}, minutes{00}},
    {WeekDay::SUNDAY, hours{14}, minutes{30}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  sch.setTemperaturePeriod(P2.start, P2.end, ALIAS_2_NAME);

  EXPECT_EQ(2, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_3_NAME);

  EXPECT_EQ(3, sch.getPeriodsCount());
  EXPECT_EQ(ALIAS_1_TEMP, sch.getTemperature(P1.start));
  EXPECT_EQ(ALIAS_2_TEMP, sch.getTemperature(P2.start));
  PNEW.validate(sch, ALIAS_3_TEMP);
  printScenarioRanges({P1, P2}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setHeadOverlapingPeriod2) {
  //situation to test
  // | PERIOD 1  | <free>      |    PERIOD 2        |
  // | PERIOD 1  | <free> | NEW PER | PERIOD 2 |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange P2{
    {WeekDay::SUNDAY, hours{14}, minutes{00}},
    {WeekDay::SUNDAY, hours{15}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{13}, minutes{30}},
    {WeekDay::SUNDAY, hours{14}, minutes{30}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  sch.setTemperaturePeriod(P2.start, P2.end, ALIAS_2_NAME);

  EXPECT_EQ(2, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_3_NAME);

  EXPECT_EQ(3, sch.getPeriodsCount());
  EXPECT_EQ(ALIAS_1_TEMP, sch.getTemperature(P1.start));
  EXPECT_EQ(ALIAS_2_TEMP, sch.getTemperature(P2.start));
  PNEW.validate(sch, ALIAS_3_TEMP);
  printScenarioRanges({P1, P2}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setTailOverlapingPeriod) {
  //situation to test
  // | PERIOD 1       | <free>      | PERIOD 2 |
  // | PERIOD 1 | NEW PER  | <free> | PERIOD 2 |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange P2{
    {WeekDay::SUNDAY, hours{14}, minutes{00}},
    {WeekDay::SUNDAY, hours{15}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{12}, minutes{30}},
    {WeekDay::SUNDAY, hours{13}, minutes{30}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  sch.setTemperaturePeriod(P2.start, P2.end, ALIAS_2_NAME);

  EXPECT_EQ(2, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_3_NAME);

  EXPECT_EQ(3, sch.getPeriodsCount());
  EXPECT_EQ(ALIAS_1_TEMP, sch.getTemperature(P1.start));
  EXPECT_EQ(ALIAS_2_TEMP, sch.getTemperature(P2.start));
  PNEW.validate(sch, ALIAS_3_TEMP);
  printScenarioRanges({P1, P2}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setTailOverlapingPeriod2) {
  //situation to test
  // | PERIOD 1            | <free> | PERIOD 2 |
  // | PERIOD 1 | NEW PER  | <free> | PERIOD 2 |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange P2{
    {WeekDay::SUNDAY, hours{14}, minutes{00}},
    {WeekDay::SUNDAY, hours{15}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{12}, minutes{30}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  sch.setTemperaturePeriod(P2.start, P2.end, ALIAS_2_NAME);

  EXPECT_EQ(2, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_3_NAME);

  EXPECT_EQ(3, sch.getPeriodsCount());
  EXPECT_EQ(ALIAS_1_TEMP, sch.getTemperature(P1.start));
  EXPECT_EQ(ALIAS_2_TEMP, sch.getTemperature(P2.start));
  PNEW.validate(sch, ALIAS_3_TEMP);
  printScenarioRanges({P1, P2}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setTailTouchPeriod) {
  //situation to test
  // | PERIOD 1 | <free>            | PERIOD 2 |
  // | PERIOD 1 | NEW PER  | <free> | PERIOD 2 |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange P2{
    {WeekDay::SUNDAY, hours{14}, minutes{00}},
    {WeekDay::SUNDAY, hours{15}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{13}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{30}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  sch.setTemperaturePeriod(P2.start, P2.end, ALIAS_2_NAME);

  EXPECT_EQ(2, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_3_NAME);

  EXPECT_EQ(3, sch.getPeriodsCount());
  EXPECT_EQ(ALIAS_1_TEMP, sch.getTemperature(P1.start));
  EXPECT_EQ(ALIAS_2_TEMP, sch.getTemperature(P2.start));
  PNEW.validate(sch, ALIAS_3_TEMP);
  printScenarioRanges({P1, P2}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setHeadTouchPeriod) {
  //situation to test
  // | PERIOD 1 | <free>           | PERIOD 2 |
  // | PERIOD 1 | <free> | NEW PER |PERIOD 2 |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange P2{
    {WeekDay::SUNDAY, hours{14}, minutes{00}},
    {WeekDay::SUNDAY, hours{15}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{13}, minutes{30}},
    {WeekDay::SUNDAY, hours{14}, minutes{00}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  sch.setTemperaturePeriod(P2.start, P2.end, ALIAS_2_NAME);

  EXPECT_EQ(2, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_3_NAME);

  EXPECT_EQ(3, sch.getPeriodsCount());
  EXPECT_EQ(ALIAS_1_TEMP, sch.getTemperature(P1.start));
  EXPECT_EQ(ALIAS_2_TEMP, sch.getTemperature(P2.start));
  PNEW.validate(sch, ALIAS_3_TEMP);
  printScenarioRanges({P1, P2}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setOverwriteFirstPeriod) {
  //situation to test
  // | PERIOD 1 | PERIOD 2 |
  // | NEW PER  | PERIOD 2 |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange P2{
    {WeekDay::SUNDAY, hours{13}, minutes{00}},
    {WeekDay::SUNDAY, hours{14}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  sch.setTemperaturePeriod(P2.start, P2.end, ALIAS_2_NAME);

  EXPECT_EQ(2, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_3_NAME);

  EXPECT_EQ(2, sch.getPeriodsCount());
  EXPECT_EQ(ALIAS_2_TEMP, sch.getTemperature(P2.start));
  PNEW.validate(sch, ALIAS_3_TEMP);
  printScenarioRanges({P1, P2}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setOverwriteSecondPeriod) {
  //situation to test
  // | PERIOD 1 | PERIOD 2 |
  // | NEW PER  | PERIOD 2 |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange P2{
    {WeekDay::SUNDAY, hours{13}, minutes{00}},
    {WeekDay::SUNDAY, hours{14}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{13}, minutes{00}},
    {WeekDay::SUNDAY, hours{14}, minutes{00}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  sch.setTemperaturePeriod(P2.start, P2.end, ALIAS_2_NAME);

  EXPECT_EQ(2, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_3_NAME);

  EXPECT_EQ(2, sch.getPeriodsCount());
  EXPECT_EQ(ALIAS_2_TEMP, sch.getTemperature(P2.start));
  PNEW.validate(sch, ALIAS_3_TEMP);
  printScenarioRanges({P1, P2}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setOverwriteTwoPeriods) {
  //situation to test
  // | PERIOD 1 | PERIOD 2 |
  // | NEW PER             |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange P2{
    {WeekDay::SUNDAY, hours{13}, minutes{00}},
    {WeekDay::SUNDAY, hours{14}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{14}, minutes{00}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  sch.setTemperaturePeriod(P2.start, P2.end, ALIAS_2_NAME);

  EXPECT_EQ(2, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_3_NAME);

  EXPECT_EQ(1, sch.getPeriodsCount());
  PNEW.validate(sch, ALIAS_3_TEMP);
  printScenarioRanges({P1, P2}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setSingleHeadOverlapPeriod) {
  //situation to test
  // | <free>  | PERIOD 1     |
  // | NEW PER     | PERIOD 1 |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{11}, minutes{00}},
    {WeekDay::SUNDAY, hours{12}, minutes{30}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  EXPECT_EQ(1, sch.getPeriodsCount());

  //TODO:segmentation fault
  //sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_3_NAME);

  EXPECT_EQ(2, sch.getPeriodsCount());
  PNEW.validate(sch, ALIAS_3_TEMP);
  printScenarioRanges({P1}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setSingleTailOverlapPeriod) {
  //situation to test
  // | PERIOD 1     | <free>        |
  // | PERIOD 1 | NEW PER  | <free> |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{12}, minutes{30}},
    {WeekDay::SUNDAY, hours{13}, minutes{30}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  EXPECT_EQ(1, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_3_NAME);

  EXPECT_EQ(2, sch.getPeriodsCount());
  PNEW.validate(sch, ALIAS_3_TEMP);
  printScenarioRanges({P1}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setSplittingPeriod) {
  //situation to test
  // | PERIOD 1                       |
  // | PERIOD 1 | NEW PER  | PERIOD 1`|

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{15}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{13}, minutes{00}},
    {WeekDay::SUNDAY, hours{14}, minutes{00}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  EXPECT_EQ(1, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_3_NAME);

  EXPECT_EQ(3, sch.getPeriodsCount());
  EXPECT_EQ(ALIAS_1_TEMP, sch.getTemperature(P1.start));
  EXPECT_EQ(ALIAS_1_TEMP, sch.getTemperature(PNEW.end));
  PNEW.validate(sch, ALIAS_3_TEMP);
  printScenarioRanges({P1}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setSwallowAndTruncatePeriod) {
  //situation to test
  // | PERIOD 1    | <Free> | Period 2 | <Free>  |    PERIOD 3 |
  // | PERIOD 1 |   NEW PERIOD                      | PERIOD 3 |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange P2{
    {WeekDay::SUNDAY, hours{13}, minutes{30}},
    {WeekDay::SUNDAY, hours{14}, minutes{00}}
  };
  TimeRange P3{
    {WeekDay::SUNDAY, hours{15}, minutes{00}},
    {WeekDay::SUNDAY, hours{16}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{12}, minutes{30}},
    {WeekDay::SUNDAY, hours{15}, minutes{30}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  sch.setTemperaturePeriod(P2.start, P2.end, ALIAS_2_NAME);
  sch.setTemperaturePeriod(P3.start, P3.end, ALIAS_3_NAME);
  EXPECT_EQ(3, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_4_NAME);

  EXPECT_EQ(3, sch.getPeriodsCount());
  EXPECT_EQ(ALIAS_1_TEMP, sch.getTemperature(P1.start));
  EXPECT_EQ(ALIAS_3_TEMP, sch.getTemperature(P3.end));
  PNEW.validate(sch, ALIAS_4_TEMP);
  printScenarioRanges({P1, P2, P3}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setSwallowPeriod) {
  //situation to test
  // | PERIOD 1    | <Free>   | Period 2 | <Free>   | PERIOD 3 |
  // | PERIOD 1    | <Free> | NEW PERIOD   | <Free> | PERIOD 3 |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange P2{
    {WeekDay::SUNDAY, hours{14}, minutes{00}},
    {WeekDay::SUNDAY, hours{15}, minutes{00}}
  };
  TimeRange P3{
    {WeekDay::SUNDAY, hours{16}, minutes{00}},
    {WeekDay::SUNDAY, hours{17}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{13}, minutes{30}},
    {WeekDay::SUNDAY, hours{15}, minutes{30}}
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  sch.setTemperaturePeriod(P2.start, P2.end, ALIAS_2_NAME);
  sch.setTemperaturePeriod(P3.start, P3.end, ALIAS_3_NAME);
  EXPECT_EQ(3, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_4_NAME);

  EXPECT_EQ(3, sch.getPeriodsCount());
  EXPECT_EQ(ALIAS_1_TEMP, sch.getTemperature(P1.start));
  EXPECT_EQ(ALIAS_3_TEMP, sch.getTemperature(P3.end));
  PNEW.validate(sch, ALIAS_4_TEMP);
  printScenarioRanges({P1, P2, P3}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setMergeToPeriodTail) {
  //situation to test
  // | PERIOD 1    |
  // | PERIOD 1           |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{13}, minutes{00}},
    {WeekDay::SUNDAY, hours{14}, minutes{00}}
  };
  TimeRange MERGED{
    P1.start,
    PNEW.end
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  EXPECT_EQ(1, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_1_NAME);

  EXPECT_EQ(1, sch.getPeriodsCount());
  MERGED.validate(sch, ALIAS_1_TEMP);
  printScenarioRanges({P1}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setMergeToPeriodTail2) {
  //situation to test
  // | PERIOD 1    |
  // | PERIOD 1           |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{12}, minutes{30}},
    {WeekDay::SUNDAY, hours{14}, minutes{00}}
  };
  TimeRange MERGED{
    P1.start,
    PNEW.end
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  EXPECT_EQ(1, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_1_NAME);

  EXPECT_EQ(1, sch.getPeriodsCount());
  MERGED.validate(sch, ALIAS_1_TEMP);
  printScenarioRanges({P1}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setMergeToPeriodHead) {
  //situation to test
  // |<free>  | PERIOD 1    |
  // | PERIOD 1             |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{11}, minutes{00}},
    {WeekDay::SUNDAY, hours{12}, minutes{00}}
  };
  TimeRange MERGED{
    PNEW.start,
    P1.end
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  EXPECT_EQ(1, sch.getPeriodsCount());

  //TODO: core dump sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_1_NAME);

  EXPECT_EQ(1, sch.getPeriodsCount());
  MERGED.validate(sch, ALIAS_1_TEMP);
  printScenarioRanges({P1}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setMergeToPeriodHead2) {
  //situation to test
  // |<free>  | PERIOD 1    |
  // | PERIOD 1             |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{11}, minutes{00}},
    {WeekDay::SUNDAY, hours{12}, minutes{30}}
  };
  TimeRange MERGED{
    PNEW.start,
    P1.end
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  EXPECT_EQ(1, sch.getPeriodsCount());

  //TODO: Core dump sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_1_NAME);

  EXPECT_EQ(1, sch.getPeriodsCount());
  MERGED.validate(sch, ALIAS_1_TEMP);
  printScenarioRanges({P1}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setMergeTwoPeriod) {
  //situation to test
  // | PERIOD 1 | <free> | PERIOD 1 |
  // | PERIOD 1                     |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange P2{
    {WeekDay::SUNDAY, hours{14}, minutes{00}},
    {WeekDay::SUNDAY, hours{15}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{13}, minutes{00}},
    {WeekDay::SUNDAY, hours{14}, minutes{00}}
  };
  TimeRange MERGED{
    P1.start,
    P2.end
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  sch.setTemperaturePeriod(P2.start, P2.end, ALIAS_1_NAME);
  EXPECT_EQ(2, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_1_NAME);

  EXPECT_EQ(1, sch.getPeriodsCount());
  MERGED.validate(sch, ALIAS_1_TEMP);
  printScenarioRanges({P1, P2}, PNEW);
}

TEST_F(temperatureShedulerTestFixture, setMergeTwoPeriod2) {
  //situation to test
  // | PERIOD 1 | <free> | PERIOD 1 |
  // | PERIOD 1                     |

  using namespace std::chrono;
  TimeRange P1{
    {WeekDay::SUNDAY, hours{12}, minutes{00}},
    {WeekDay::SUNDAY, hours{13}, minutes{00}}
  };
  TimeRange P2{
    {WeekDay::SUNDAY, hours{14}, minutes{00}},
    {WeekDay::SUNDAY, hours{15}, minutes{00}}
  };
  TimeRange PNEW{
    {WeekDay::SUNDAY, hours{12}, minutes{30}},
    {WeekDay::SUNDAY, hours{14}, minutes{30}}
  };
  TimeRange MERGED{
    P1.start,
    P2.end
  };

  sch.setTemperaturePeriod(P1.start, P1.end, ALIAS_1_NAME);
  sch.setTemperaturePeriod(P2.start, P2.end, ALIAS_1_NAME);
  EXPECT_EQ(2, sch.getPeriodsCount());

  sch.setTemperaturePeriod(PNEW.start, PNEW.end, ALIAS_1_NAME);

  EXPECT_EQ(1, sch.getPeriodsCount());
  MERGED.validate(sch, ALIAS_1_TEMP);
  printScenarioRanges({P1, P2}, PNEW);
}
