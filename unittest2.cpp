#include "stdafx.h"
#include "CppUnitTest.h"
#include "AI.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace JamesTests
{		
	TEST_CLASS(AIStatTests)
	{
	public:

		TEST_METHOD(InitialStatTest)
		{
			AI ai_test(MELEE, 5, 5, 20, 8);

			Assert::AreEqual(ai_test.GetXPosition(), (float)0);
			Assert::AreEqual(ai_test.GetYPosition(), (float)0);
			Assert::AreEqual(ai_test.GetSpeed(), 5);
			Assert::AreEqual(ai_test.GetFace(), (int)S);
			Assert::AreEqual(ai_test.GetType(), (int)MELEE);
			Assert::AreEqual(ai_test.GetState(), (int)IDLE);
		}
		TEST_METHOD(MovementTest)
		{
			AI ai_test(MELEE, 5, 5, 20, 8);

			// Initial position (0,0), speed (5), and face (S)

			// Move up
			ai_test.MoveUp();
			Assert::AreEqual(ai_test.GetXPosition(), (float)0);
			Assert::AreEqual(ai_test.GetYPosition(), (float)-5);
			Assert::AreEqual(ai_test.GetFace(), (int)N);
			// Move down
			ai_test.MoveDown();
			Assert::AreEqual(ai_test.GetXPosition(), (float)0);
			Assert::AreEqual(ai_test.GetYPosition(), (float)0);
			Assert::AreEqual(ai_test.GetFace(), (int)S);
			// Move left
			ai_test.MoveLeft();
			Assert::AreEqual(ai_test.GetXPosition(), (float)-5);
			Assert::AreEqual(ai_test.GetYPosition(), (float)0);
			Assert::AreEqual(ai_test.GetFace(), (int)W);
			// Move right
			ai_test.MoveRight();
			Assert::AreEqual(ai_test.GetXPosition(), (float)0);
			Assert::AreEqual(ai_test.GetYPosition(), (float)0);
			Assert::AreEqual(ai_test.GetFace(), (int)E);

			// MoveTowardTarget tests
			// Move up and left
			ai_test.MoveTowardTarget(-5, -5);
			Assert::AreEqual(ai_test.GetXPosition(), (float)-5);
			Assert::AreEqual(ai_test.GetYPosition(), (float)-5);
			// Move down and right
			ai_test.MoveTowardTarget(0, 0);
			Assert::AreEqual(ai_test.GetXPosition(), (float)0);
			Assert::AreEqual(ai_test.GetYPosition(), (float)0);

			// MoveTowardTarget tests for when the AI passes the goal due to high speed
			// Move up and left
			ai_test.MoveTowardTarget(-3, -3);
			Assert::AreEqual(ai_test.GetXPosition(), (float)-3);
			Assert::AreEqual(ai_test.GetYPosition(), (float)-3);
			// Move down and right
			ai_test.MoveTowardTarget(0, 0);
			Assert::AreEqual(ai_test.GetXPosition(), (float)0);
			Assert::AreEqual(ai_test.GetYPosition(), (float)0);
		}

		TEST_METHOD(BuffTest)
		{
			AI ai_melee_test(BOSS_MELEE, 5, 5, 20, 8);
			AI ai_ranger_test(BOSS_RANGER, 5, 5, 20, 10);

			// Test melee buff
			ai_melee_test.EnableMeleeBossBuff(4);
			Assert::AreEqual(ai_melee_test.GetSpeed(), 10);
			Assert::AreEqual(ai_melee_test.GetATK(), 12);
			ai_melee_test.DisableMeleeBossBuff(4);
			Assert::AreEqual(ai_melee_test.GetSpeed(), 5);
			Assert::AreEqual(ai_melee_test.GetATK(), 8);

			// Test ranger buff
			ai_ranger_test.EnableRangerBossBuff();
			Assert::AreEqual(ai_ranger_test.GetSpeed(), 4);
			Assert::AreEqual(ai_ranger_test.GetATK(), 20);
			ai_ranger_test.DisableRangerBossBuff();
			Assert::AreEqual(ai_ranger_test.GetSpeed(), 5);
			Assert::AreEqual(ai_ranger_test.GetATK(), 10);
		}

		TEST_METHOD(BoundPointTests)
		{
			AI ai_test(MELEE, 5, 5, 20, 8);

			// Position is (0,0), bound_x is 48, and bound_y is 64

			// North
			Assert::AreEqual(ai_test.GetXNorthBoundPoint(), 0);
			Assert::AreEqual(ai_test.GetYNorthBoundPoint(), -32);
			// South
			Assert::AreEqual(ai_test.GetXSouthBoundPoint(), 0);
			Assert::AreEqual(ai_test.GetYSouthBoundPoint(), 32);
			// West
			Assert::AreEqual(ai_test.GetXWestBoundPoint(), -24);
			Assert::AreEqual(ai_test.GetYWestBoundPoint(), 0);
			// East
			Assert::AreEqual(ai_test.GetXEastBoundPoint(), 24);
			Assert::AreEqual(ai_test.GetYEastBoundPoint(), 0);
			// Northwest
			Assert::AreEqual(ai_test.GetXNorthWestBoundPoint(), -24);
			Assert::AreEqual(ai_test.GetYNorthWestBoundPoint(), -32);
			// Northeast
			Assert::AreEqual(ai_test.GetXNorthEastBoundPoint(), 24);
			Assert::AreEqual(ai_test.GetYNorthEastBoundPoint(), -32);
			// Southwest
			Assert::AreEqual(ai_test.GetXSouthWestBoundPoint(), -24);
			Assert::AreEqual(ai_test.GetYSouthWestBoundPoint(), 32);
			// Southeast
			Assert::AreEqual(ai_test.GetXSouthEastBoundPoint(), 24);
			Assert::AreEqual(ai_test.GetYSouthEastBoundPoint(), 32);
		}

	};
}