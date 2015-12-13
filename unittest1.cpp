#include "stdafx.h"
#include "CppUnitTest.h"
#include "TestClasses.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace RyansTests
{	
	TEST_CLASS(RandomNumberTest)
	{
	public:
		TEST_METHOD(RandomNum)
		{
			int RandomNum = Random(0, 2);
			Assert::IsTrue(RandomNum >= 0);
			Assert::IsTrue(RandomNum < 2);

			RandomNum = Random(0, 0);
			Assert::AreEqual(RandomNum, 0);
		}
	};


	TEST_CLASS(RectClassTest)
	{
	public:
		
		TEST_METHOD(RectContains)
		{
			Rect Temp(0, 0, 10, 10);
			Assert::IsTrue(Temp.ContainsPoint(3, 3));

			Assert::IsTrue(Temp.ContainsPoint(10, 10));

			Assert::IsTrue(Temp.ContainsPoint(0, 0));

			Assert::IsFalse(Temp.ContainsPoint(11, 11));

			Assert::IsFalse(Temp.ContainsPoint(-1, -1));
		}

		TEST_METHOD(RectInsectsRectTrue)
		{
			Rect One(0, 0, 5, 5);
			Rect Two(0, 0, 10, 10);
			Assert::IsTrue(One.Intersects(Two));

			One = Rect(10, 10, 15, 15);
			Assert::IsTrue(One.Intersects(Two));

			One = Rect(0, 0, 8, 8);
			Two = Rect(5, 5, 10, 10);
			Assert::IsTrue(One.Intersects(Two));

			One = Rect(0, 0, 12, 12);
			Assert::IsTrue(One.Intersects(Two));

			One = Rect(11, 11, 15, 15);
		}
	};
}
