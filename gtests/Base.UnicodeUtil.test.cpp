#include <gtest/gtest.h>

#include <Base/UnicodeUtil.h>

TEST(BaseTest, UnicodeUtil)
{
	std::string utf8Str = u8"���{��";
	std::wstring wStr = L"���{��";

	EXPECT_EQ(utf8Str, saba::ToUtf8String(wStr));
	EXPECT_EQ(wStr, saba::ToWString(utf8Str));
}
