#include <stdexcept>
#include "gtest/gtest.h"
#include "util.h"
#include <vector>

class DistortionTest : public ::testing::Test {
 protected:
   void SetUp() override {
     trans["optical_distortion"]["transverse"]["x"] = {1};
     trans["optical_distortion"]["transverse"]["y"] = {2};

     radial["optical_distortion"]["radial"]["coefficients"] = {1, 2};

     kaguya["optical_distortion"]["kaguyalism"]["x"] = {1};
     kaguya["optical_distortion"]["kaguyalism"]["y"] = {2};
     kaguya["optical_distortion"]["kaguyalism"]["boresight_x"] = 1;
     kaguya["optical_distortion"]["kaguyalism"]["boresight_y"] = 2;

     dawn["optical_distortion"]["dawnfc"]["coefficients"] = {1, 2};

     lro["optical_distortion"]["lrolrocnac"]["coefficients"] = {1,2};

     fake["optical_distortion"]["foo"]["x"] = {1};
     fake["optical_distortion"]["foo"]["y"] = {2};
  }
  ale::json trans;
  ale::json radial;
  ale::json kaguya;
  ale::json dawn;
  ale::json lro;
  ale::json fake;
  ale::json noDist;
};

TEST(GetLogFile, LogFileExists)
{
  ale::json j;
  j["log_file"] = "fake/path";
  EXPECT_STREQ(ale::getLogFile(j).c_str(), "fake/path");
}

TEST(GetLogFile, NoLogFile)
{
  ale::json j;
  EXPECT_THROW(ale::getLogFile(j), std::runtime_error);
}

TEST_F(DistortionTest, TransverseType)
{
  //first 10 coefficients in output are x. next 10 are y.
  std::vector<double> coeffs = ale::getDistortionCoeffs(trans);
  EXPECT_EQ(ale::getDistortionModel(trans), ale::DistortionType::TRANSVERSE);
  EXPECT_DOUBLE_EQ(coeffs[0], 1);
  EXPECT_DOUBLE_EQ(coeffs[10], 2);
}

TEST_F(DistortionTest, RadialType)
{
  std::vector<double> coeffs = ale::getDistortionCoeffs(radial);
  EXPECT_EQ(ale::getDistortionModel(radial), ale::DistortionType::RADIAL);
  EXPECT_DOUBLE_EQ(coeffs[0], 1);
  EXPECT_DOUBLE_EQ(coeffs[1], 2);
}

TEST_F(DistortionTest, KaguyaLISMType)
{
  //first 4 coefficients in output are x. next 4 are y.
  std::vector<double> coeffs = ale::getDistortionCoeffs(kaguya);
  EXPECT_EQ(ale::getDistortionModel(kaguya), ale::DistortionType::KAGUYALISM);
  EXPECT_DOUBLE_EQ(coeffs[0], 1);
  EXPECT_DOUBLE_EQ(coeffs[1], 1);
  EXPECT_DOUBLE_EQ(coeffs[5], 2);
  EXPECT_DOUBLE_EQ(coeffs[6], 2);
}

TEST_F(DistortionTest, DawnFCType)
{
  std::vector<double> coeffs = ale::getDistortionCoeffs(dawn);
  EXPECT_EQ(ale::getDistortionModel(dawn), ale::DistortionType::DAWNFC);
  EXPECT_DOUBLE_EQ(coeffs[0], 1);
  EXPECT_DOUBLE_EQ(coeffs[1], 2);
}

TEST_F(DistortionTest, LroLrocNACType)
{
  std::vector<double> coeffs = ale::getDistortionCoeffs(lro);
  EXPECT_EQ(ale::getDistortionModel(lro), ale::DistortionType::LROLROCNAC);
  EXPECT_DOUBLE_EQ(coeffs[0], 1);
  EXPECT_DOUBLE_EQ(coeffs[1], 2);
}

TEST_F(DistortionTest, UnrecognizedType)
{
  EXPECT_EQ(ale::getDistortionModel(fake), ale::DistortionType::TRANSVERSE);
  EXPECT_THROW(ale::getDistortionCoeffs(fake), std::runtime_error);
}

TEST_F(DistortionTest, NoDistortion)
{
  EXPECT_THROW(ale::getDistortionModel(noDist), std::runtime_error);
  EXPECT_THROW(ale::getDistortionCoeffs(noDist), std::runtime_error);
}
