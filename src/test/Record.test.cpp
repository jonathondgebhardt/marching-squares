#include <Record.hpp>
#include <gtest/gtest.h>

TEST(Record, isValid_empty)
{
  Record r;

  // Records are invalid by default.
  EXPECT_FALSE(r.numElevationsMatch());
  EXPECT_FALSE(r.minElevationsMatch());
  EXPECT_FALSE(r.maxElevationsMatch());
  EXPECT_FALSE(r.isValid());
}

TEST(Record, isValid_invalidParsedNumberOfElevations)
{
  Record r;
  r.elevations.push_back(0.0);

  // Number of parsed elevations doesn't match header of record.
  EXPECT_FALSE(r.numElevationsMatch());
  EXPECT_FALSE(r.minElevationsMatch());
  EXPECT_FALSE(r.maxElevationsMatch());
  EXPECT_FALSE(r.isValid());
}

TEST(Record, isValid_invalidMinMaxElevation)
{
  Record r;
  r.elevations.push_back(0.0);
  r.numElevations = 1;

  ASSERT_TRUE(r.numElevationsMatch());

  // Didn't parse a minimum/maximum elevation from file.
  EXPECT_FALSE(r.minElevationsMatch());
  EXPECT_FALSE(r.maxElevationsMatch());
  EXPECT_FALSE(r.isValid());
}

TEST(Record, isValid_invalidMinimumElevation)
{
  Record r;
  r.elevations.push_back(0.0);
  r.numElevations = 1;
  r.minimumElevation = 0.0;

  ASSERT_TRUE(r.numElevationsMatch());
  ASSERT_TRUE(r.minElevationsMatch());

  // Didn't parse a maximum elevation from a file.
  EXPECT_FALSE(r.maxElevationsMatch());
  EXPECT_FALSE(r.isValid());
}

TEST(Record, isValid_invalidMaximumElevation)
{
  Record r;
  r.elevations.push_back(0.0);
  r.numElevations = 1;
  r.maximumElevation = 0.0;

  ASSERT_TRUE(r.numElevationsMatch());
  ASSERT_TRUE(r.maxElevationsMatch());

  // Didn't parse a minimum elevation from file.
  EXPECT_FALSE(r.minElevationsMatch());
  EXPECT_FALSE(r.isValid());
}

TEST(Record, isValid_valid)
{
  Record r;
  r.elevations.push_back(0.0);
  r.numElevations = 1;
  r.minimumElevation = 0.0;
  r.maximumElevation = 0.0;

  EXPECT_TRUE(r.numElevationsMatch());
  EXPECT_TRUE(r.minElevationsMatch());
  EXPECT_TRUE(r.maxElevationsMatch());
  EXPECT_TRUE(r.isValid());
}