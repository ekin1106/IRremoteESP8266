// Copyright 2017 David Conran

#include "IRsend.h"
#include "IRsend_test.h"
#include "ir_LG.h"
#include "gtest/gtest.h"

// Tests for calcLGChecksum()
TEST(TestCalcLGChecksum, General) {
  EXPECT_EQ(0x0, calcLGChecksum(0x0));
  EXPECT_EQ(0x1, calcLGChecksum(0x1));
  EXPECT_EQ(0xF, calcLGChecksum(0xF));
  EXPECT_EQ(0x4, calcLGChecksum(0x1111));
  EXPECT_EQ(0x8, calcLGChecksum(0x2222));
  EXPECT_EQ(0x0, calcLGChecksum(0x4444));
  EXPECT_EQ(0xA, calcLGChecksum(0x1234));
  EXPECT_EQ(0xA, calcLGChecksum(0x4321));
  EXPECT_EQ(0xE, calcLGChecksum(0xABCD));
  EXPECT_EQ(0x1, calcLGChecksum(0x4AE5));
  EXPECT_EQ(0xC, calcLGChecksum(0xFFFF));
}

// Tests for sendLG().

// Test sending typical data only.
TEST(TestSendLG, SendDataOnly) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendLG(0x4B4AE51);
  EXPECT_EQ(
      "m8500s4250"
      "m550s550m550s1600m550s550m550s550"
      "m550s1600m550s550m550s1600m550s1600m550s550m550s1600m550s550m550s550"
      "m550s1600m550s550m550s1600m550s550m550s1600m550s1600m550s1600m550s550"
      "m550s550m550s1600m550s550m550s1600m550s550m550s550m550s550m550s1600"
      "m550s50300", irsend.outputStr());

  irsend.reset();
  irsend.sendLG(0xB4B4AE51, kLg32Bits);
  EXPECT_EQ(
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s1680m560s560m560s1680m560s560m560s560"
      "m560s1680m560s560m560s1680m560s1680m560s560m560s1680m560s560m560s560"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s1680m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s560m560s560m560s1680"
      "m560s44800"
      "m8950s2250m550s96300", irsend.outputStr());
}

// Test sending with different repeats.
TEST(TestSendLG, SendWithRepeats) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendLG(0x4B4AE51, kLgBits, 1);
  EXPECT_EQ(
      "m8500s4250"
      "m550s550m550s1600m550s550m550s550"
      "m550s1600m550s550m550s1600m550s1600m550s550m550s1600m550s550m550s550"
      "m550s1600m550s550m550s1600m550s550m550s1600m550s1600m550s1600m550s550"
      "m550s550m550s1600m550s550m550s1600m550s550m550s550m550s550m550s1600"
      "m550s50300"
      "m8500s2250m550s96750", irsend.outputStr());

  irsend.reset();
  irsend.sendLG(0xB4B4AE51, kLg32Bits, 1);
  EXPECT_EQ(
      "m4480s4480"
      "m560s1680m560s560m560s1680m560s1680m560s560m560s1680m560s560m560s560"
      "m560s1680m560s560m560s1680m560s1680m560s560m560s1680m560s560m560s560"
      "m560s1680m560s560m560s1680m560s560m560s1680m560s1680m560s1680m560s560"
      "m560s560m560s1680m560s560m560s1680m560s560m560s560m560s560m560s1680"
      "m560s44800"
      "m8950s2250m550s96300"
      "m8950s2250m550s96300", irsend.outputStr());
}

// Test sending an atypical data size.
TEST(TestSendLG, SendUnusualSize) {
  IRsendTest irsend(4);
  irsend.begin();

  irsend.reset();
  irsend.sendLG(0x0, 31);
  EXPECT_EQ(
      "m8500s4250"
      "m550s550m550s550m550s550m550s550m550s550m550s550m550s550m550s550"
      "m550s550m550s550m550s550m550s550m550s550m550s550m550s550m550s550"
      "m550s550m550s550m550s550m550s550m550s550m550s550m550s550m550s550"
      "m550s550m550s550m550s550m550s550m550s550m550s550m550s550"
      "m550s60650", irsend.outputStr());

  irsend.reset();
  irsend.sendLG(0x0, 64);
  EXPECT_EQ(
      "m4480s4480"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s560m560s560m560s560m560s560m560s560m560s560m560s560m560s560"
      "m560s26880"
      "m8950s2250m550s96300", irsend.outputStr());
}

// Tests for encodeLG().

TEST(TestEncodeLG, NormalEncoding) {
  IRsendTest irsend(4);
  EXPECT_EQ(0x0, irsend.encodeLG(0, 0));
  EXPECT_EQ(0x100011, irsend.encodeLG(1, 1));
  EXPECT_EQ(0x100022, irsend.encodeLG(1, 2));
  EXPECT_EQ(0x43001DE, irsend.encodeLG(0x43, 0x1D));
  EXPECT_EQ(0xB4B4AE51, irsend.encodeLG(0xB4B, 0x4AE5));
  EXPECT_EQ(0xAA0055A, irsend.encodeLG(0xAA, 0x55));
  EXPECT_EQ(0xFFFFFFFC, irsend.encodeLG(0xFFFF, 0xFFFF));
}

// Tests for decodeLG().

// Decode normal LG messages.
TEST(TestDecodeLG, NormalDecodeWithStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal LG 28-bit message.
  irsend.reset();
  irsend.sendLG(0x4B4AE51, kLgBits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kLgBits, true));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLgBits, irsend.capture.bits);
  EXPECT_EQ(0x4B4AE51, irsend.capture.value);
  EXPECT_EQ(0x4B, irsend.capture.address);
  EXPECT_EQ(0x4AE5, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal LG 32-bit message.
  irsend.reset();
  irsend.sendLG(0xB4B4AE51, kLg32Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kLg32Bits, false));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLg32Bits, irsend.capture.bits);
  EXPECT_EQ(0xB4B4AE51, irsend.capture.value);
  EXPECT_EQ(0xB4B, irsend.capture.address);
  EXPECT_EQ(0x4AE5, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal LG 28-bit message.
  irsend.reset();
  irsend.sendLG(irsend.encodeLG(0x07, 0x99));
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kLgBits, true));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLgBits, irsend.capture.bits);
  EXPECT_EQ(0x700992, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Synthesised Normal LG 32-bit message.
  irsend.reset();
  irsend.sendLG(irsend.encodeLG(0x800, 0x8000), kLg32Bits);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kLg32Bits, true));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLg32Bits, irsend.capture.bits);
  EXPECT_EQ(0x80080008, irsend.capture.value);
  EXPECT_EQ(0x800, irsend.capture.address);
  EXPECT_EQ(0x8000, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode normal repeated LG messages.
TEST(TestDecodeLG, NormalDecodeWithRepeatAndStrict) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Normal LG 28-bit message with 2 repeats.
  irsend.reset();
  irsend.sendLG(irsend.encodeLG(0x07, 0x99), kLgBits, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kLgBits, true));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLgBits, irsend.capture.bits);
  EXPECT_EQ(0x700992, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  // Normal LG 32-bit message with 2 repeats.
  irsend.reset();
  irsend.sendLG(irsend.encodeLG(0x07, 0x99), kLg32Bits, 2);
  irsend.makeDecodeResult();
  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kLg32Bits, true));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLg32Bits, irsend.capture.bits);
  EXPECT_EQ(0x700992, irsend.capture.value);
  EXPECT_EQ(0x07, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode unsupported LG message values.
TEST(TestDecodeLG, DecodeWithNonStrictValues) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Illegal values should be rejected when strict is on.
  // Illegal LG 28-bit message value.
  irsend.reset();
  irsend.sendLG(0x1);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLgBits, true));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLg32Bits, true));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLg32Bits, false));
  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kLgBits, false));

  // Illegal LG 32-bit message value.
  irsend.reset();
  irsend.sendLG(0x1111111, kLg32Bits);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLg32Bits, true));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLgBits, true));

  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kLg32Bits, false));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLg32Bits, irsend.capture.bits);
  EXPECT_EQ(0x1111111, irsend.capture.value);
  EXPECT_EQ(0x11, irsend.capture.address);
  EXPECT_EQ(0x1111, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);

  irsend.reset();
  irsend.sendLG(0x1111111, kLg32Bits);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLgBits, false));
}

// Decode unsupported LG message sizes.
TEST(TestDecodeLG, DecodeWithNonStrictSizes) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  // Illegal sizes should be rejected when strict is on.
  // Illegal LG 16-bit message size.
  irsend.reset();
  irsend.sendLG(irsend.encodeLG(0x07, 0x99), 16);
  irsend.makeDecodeResult();
  // Should fail when unexpected against different bit sizes.
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLgBits, true));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLg32Bits, true));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLgBits, false));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLg32Bits, false));

  // Should pass if strict off.
  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, 16, false));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(16, irsend.capture.bits);
  EXPECT_EQ(0x992, irsend.capture.value);
  EXPECT_EQ(0x0, irsend.capture.address);
  EXPECT_EQ(0x99, irsend.capture.command);

  // Illegal LG 36-bit message size.
  irsend.reset();
  irsend.sendLG(0x123456789, 36);  // Illegal value LG 36-bit message.
  irsend.makeDecodeResult();
  // Should fail when unexpected against different bit sizes.
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLgBits, true));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLgBits, false));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLg32Bits, true));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLg32Bits, false));

  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, 36, false));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(36, irsend.capture.bits);
  EXPECT_EQ(0x123456789, irsend.capture.value);
  EXPECT_EQ(0x1234, irsend.capture.address);
  EXPECT_EQ(0x5678, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Decode (non-standard) 64-bit messages.
TEST(TestDecodeLG, Decode64BitMessages) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Illegal value & size LG 64-bit message.
  irsend.sendLG(0xFFFFFFFFFFFFFFFF, 64);
  irsend.makeDecodeResult();
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, 64, true));
  // Should work with a 'normal' match (not strict)
  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, 64, false));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(64, irsend.capture.bits);
  EXPECT_EQ(0xFFFFFFFFFFFFFFFF, irsend.capture.value);
  EXPECT_EQ(0xFFFFFFFF, irsend.capture.address);
  EXPECT_EQ(0xFFFF, irsend.capture.command);
}

// Decode a 'real' example via GlobalCache
TEST(TestDecodeLG, DecodeGlobalCacheExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

// TODO(anyone): Find a Global Cache example of the LG 28-bit message.
  irsend.reset();
  // LG (32-bit) code from Global Cache.
  uint16_t gc_test[75] = {38000, 1, 69, 341, 170, 21, 64, 21, 21, 21, 64,
                          21, 64, 21, 21, 21, 64, 21, 21, 21, 21, 21, 64,
                          21, 21, 21, 64, 21, 64, 21, 21, 21, 64, 21, 21,
                          21, 21, 21, 64, 21, 21, 21, 64, 21, 21, 21, 64,
                          21, 64, 21, 64, 21, 21, 21, 21, 21, 64, 21, 21,
                          21, 64, 21, 21, 21, 21, 21, 21, 21, 64, 21, 1517,
                          341, 85, 21, 3655};
  irsend.sendGC(gc_test, 75);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture, kLg32Bits, true));
  EXPECT_EQ(LG, irsend.capture.decode_type);
  EXPECT_EQ(kLg32Bits, irsend.capture.bits);
  EXPECT_EQ(0xB4B4AE51, irsend.capture.value);
  EXPECT_EQ(0xB4B, irsend.capture.address);
  EXPECT_EQ(0x4AE5, irsend.capture.command);
  EXPECT_FALSE(irsend.capture.repeat);
}

// Fail to decode a non-LG example via GlobalCache
TEST(TestDecodeLG, FailToDecodeNonLGExample) {
  IRsendTest irsend(4);
  IRrecv irrecv(4);
  irsend.begin();

  irsend.reset();
  // Modified a few entries to unexpected values, based on previous test case.
  uint16_t gc_test[39] = {38000, 1, 1, 322, 162, 20, 61, 20, 61, 20, 20, 20, 20,
                          20, 20, 20, 127, 20, 61, 9, 20, 20, 61, 20, 20, 20,
                          61, 20, 61, 20, 61, 20, 20, 20, 20, 20, 20, 20, 884};
  irsend.sendGC(gc_test, 39);
  irsend.makeDecodeResult();

  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture));
  ASSERT_FALSE(irrecv.decodeLG(&irsend.capture, kLgBits, false));
}

// Tests for sendLG2().

// Test sending typical data only.
TEST(TestSendLG2, SendDataOnly) {
  IRsendTest irsend(0);
  irsend.begin();

  irsend.reset();
  irsend.sendLG2(0x880094D);
  EXPECT_EQ(
      "m3200s9850"
      "m550s1600m550s550m550s550m550s550m550s1600m550s550m550s550m550s550"
      "m550s550m550s550m550s550m550s550m550s550m550s550m550s550m550s550"
      "m550s1600m550s550m550s550m550s1600m550s550m550s1600m550s550m550s550"
      "m550s1600m550s1600m550s550m550s1600"
      "m550s55250", irsend.outputStr());
}

TEST(TestDecodeLG2, SyntheticExample) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  irsend.sendLG2(0x880094D);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture));
  ASSERT_EQ(LG2, irsend.capture.decode_type);
  EXPECT_EQ(kLgBits, irsend.capture.bits);
  EXPECT_EQ(0x880094D, irsend.capture.value);
}


// Verify decoding of LG variant 2 messages.
TEST(TestDecodeLG2, RealLG2Example) {
  IRsendTest irsend(0);
  IRrecv irrecv(0);
  irsend.begin();

  irsend.reset();
  // From issue #548
  uint16_t rawData[59] = {3154, 9834, 520, 1634, 424, 606, 424, 568, 462, 570,
      462, 1564, 508, 568, 458, 544, 500, 546, 508, 530, 508, 532, 506, 566,
      464, 568, 460, 578, 464, 568, 464, 532, 506, 552, 474, 1592, 506, 568,
      460, 570, 462, 1564, 506, 606, 424, 1640, 424, 616, 422, 570, 462, 1616,
      460, 1584, 500, 544, 506, 1598, 490};  // UNKNOWN F6D13AE8
  irsend.sendRaw(rawData, 59, 38000);
  irsend.makeDecodeResult();

  ASSERT_TRUE(irrecv.decodeLG(&irsend.capture));
  ASSERT_EQ(LG2, irsend.capture.decode_type);
  EXPECT_EQ(kLgBits, irsend.capture.bits);
  EXPECT_EQ(0x880094D, irsend.capture.value);
}
