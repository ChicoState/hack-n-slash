#include "stdafx.h"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

#include "BowWeapon.h"
#include "Camera.h"
#include "FoodPickup.h"
#include "GUILayer.h"
#include "Inventory.h"
#include "PickupObject.h"
#include "Player.h"
#include "PlayerScoreCalculator.h"
#include "Projectile.h"
#include "SpeedPickup.h"
#include "StrengthPickup.h"
#include "SwordWeapon.h"
#include "Weapon.h"

namespace ScottTests
{		
	TEST_CLASS(UnitTest1)
	{
	public:

		TEST_METHOD(BowWeaponTest)
		{
			BowWeapon TestBowWeapon;

			TestBowWeapon.EventHandler();

			for(int count = 0; count < 10; count++)
			{
				TestBowWeapon.Draw(rand() % 999999, rand() % 999999, rand() % 999999, rand() % 999999);
			}
		}

		TEST_METHOD(CameraTest)
		{
			int TestScreenWidth = rand() % 999999;
			int TestScreenHeight = rand() % 999999;

			Camera CameraTest(TestScreenWidth, TestScreenHeight);

			for(int count = 0; count < 10; count++)
			{
				CameraTest.EventHandler(rand() % 999999, rand() % 999999);
			}
		}

		TEST_METHOD(GUILayerTest)
		{
			int TestScreenWidth = rand() % 999999;
			int TestScreenHeight = rand() % 999999;

			GUILayer TestGUILayer(TestScreenWidth, TestScreenHeight);

			Camera TempTestCamera(TestScreenWidth, TestScreenHeight);

			for(int count = 0; count < 10; count++)
			{
				TestGUILayer.DrawPlayerInformation(TempTestCamera, rand() % 999999, rand() % 999999);
			}

			for(int count = 0; count < 10; count++)
			{
				TestGUILayer.DrawScoreInformation(TempTestCamera, rand() % 999999);
			}

			for(int count = 0; count < 10; count++)
			{
				TestGUILayer.DrawGameTimeInformation(TempTestCamera, rand() % 999999);
			}

			for(int count = 0; count < 10; count++)
			{
				TestGUILayer.DrawFinalScoreScreen(rand() % 999999);
			}
		}

		TEST_METHOD(InventoryTestfewfew)
		{
			Inventory* TestInventory = new Inventory();

			Weapon* TestWeapon1 = new BowWeapon();
			Weapon* TestWeapon2 = new BowWeapon();
			Weapon* TestWeapon3 = new BowWeapon();
			Weapon* TestWeapon4 = new BowWeapon();
			Weapon* TestWeapon5 = new BowWeapon();
			Weapon* TestWeapon6 = new BowWeapon();
			Weapon* TestWeapon7 = new BowWeapon();
			Weapon* TestWeapon8 = new BowWeapon();
			Weapon* TestWeapon9 = new BowWeapon();
			Weapon* TestWeapon10 = new BowWeapon();

			TestInventory->AddWeapon(TestWeapon1);
			TestInventory->AddWeapon(TestWeapon2);
			TestInventory->AddWeapon(TestWeapon3);
			TestInventory->AddWeapon(TestWeapon4);
			TestInventory->AddWeapon(TestWeapon5);
			TestInventory->AddWeapon(TestWeapon6);
			TestInventory->AddWeapon(TestWeapon7);
			TestInventory->AddWeapon(TestWeapon8);
			TestInventory->AddWeapon(TestWeapon9);
			TestInventory->AddWeapon(TestWeapon10);

			TestInventory->AddWeapon(NULL);

			for(int count = 0; count < 10; count++)
			{
				Weapon* TempWeapon = TestInventory->GetNextCycledWeapon();
			}

			for(int count = 0; count < 10; count++)
			{
				Weapon* TempWeapon = TestInventory->GetWeaponFromSlot(rand() % 999999);
			}
		}

		TEST_METHOD(ObjectPickupTest)
		{
			int TestFoodPickupXPos = rand() % 999999;
			int TestFoodPickupYPos = rand() % 999999;

			FoodPickup TestFoodPickup(TestFoodPickupXPos, TestFoodPickupYPos);
			
			Assert::AreEqual(TestFoodPickup.IsPickupDead(), false);

			Assert::AreEqual(TestFoodPickup.GetXBound(), 44);
			Assert::AreEqual(TestFoodPickup.GetYBound(), 44);

			Assert::AreEqual(TestFoodPickup.GetXPosition(), TestFoodPickupXPos);
			Assert::AreEqual(TestFoodPickup.GetYPosition(), TestFoodPickupYPos);

			int TestSpeedPickupXPos = rand() % 999999;
			int TestSpeedPickupYPos = rand() % 999999;

			SpeedPickup TestSpeedPickup(TestSpeedPickupXPos, TestSpeedPickupYPos);

			Assert::AreEqual(TestSpeedPickup.IsPickupDead(), false);

			Assert::AreEqual(TestSpeedPickup.GetXBound(), 44);
			Assert::AreEqual(TestSpeedPickup.GetYBound(), 44);

			Assert::AreEqual(TestSpeedPickup.GetXPosition(), TestSpeedPickupXPos);
			Assert::AreEqual(TestSpeedPickup.GetYPosition(), TestSpeedPickupYPos);

			int TestStrengthPickupXPos = rand() % 999999;
			int TestStrengthPickupYPos = rand() % 999999;

			StrengthPickup TestStrengthPickup(TestStrengthPickupXPos, TestStrengthPickupYPos);

			Assert::AreEqual(TestStrengthPickup.IsPickupDead(), false);

			Assert::AreEqual(TestStrengthPickup.GetXBound(), 44);
			Assert::AreEqual(TestStrengthPickup.GetYBound(), 44);

			Assert::AreEqual(TestStrengthPickup.GetXPosition(), TestStrengthPickupXPos);
			Assert::AreEqual(TestStrengthPickup.GetYPosition(), TestStrengthPickupYPos);
		}
		
		TEST_METHOD(PlayerTest)
		{
			Player* TestPlayer = new Player();

			for(int count = 0; count < 10; count++)
			{
				TestPlayer->EventHandler(rand() % 999999, rand() % 999999);
			}

			TestPlayer->DrawPlayer();

			TestPlayer->MovementCollidingBoundOne();
			TestPlayer->MovementCollidingBoundTwo();

			Assert::AreEqual(TestPlayer->IsDead(), false);
			Assert::AreEqual(TestPlayer->GetCurrentLevel(), 1);
			Assert::AreEqual(TestPlayer->GetCurrentHealth(), 100);
			Assert::AreEqual(TestPlayer->GetCurrentScore(), 0);
			TestPlayer->ResetPlayer();
			Assert::AreEqual(TestPlayer->IsDead(), false);
			Assert::AreEqual(TestPlayer->GetCurrentLevel(), 1);
			Assert::AreEqual(TestPlayer->GetCurrentHealth(), 100);
			Assert::AreEqual(TestPlayer->GetCurrentScore(), 0);

			TestPlayer->GiveExperience();
			TestPlayer->ResetPlayer();

			Assert::AreEqual(TestPlayer->GetCurrentLevel(), 1);
			TestPlayer->AddPlayerLevel();
			Assert::AreEqual(TestPlayer->GetCurrentLevel(), 2);
			TestPlayer->ResetPlayer();

			Assert::AreEqual(TestPlayer->GetCurrentHealth(), 100);
			TestPlayer->DealDamage(50);
			Assert::AreEqual(TestPlayer->GetCurrentHealth(), 50);
			TestPlayer->HealPlayer();
			Assert::AreEqual(TestPlayer->GetCurrentHealth(), 55);
			TestPlayer->ResetPlayer();

			Assert::AreEqual(TestPlayer->IsDead(), false);
			TestPlayer->DealDamage(999999);
			Assert::AreEqual(TestPlayer->IsDead(), true);
			TestPlayer->ResetPlayer();

			Assert::AreEqual(TestPlayer->GetCurrentScore(), 0);
	
			TestPlayer->GetTag();

			Assert::AreEqual(TestPlayer->GetXBound(), 88);
			Assert::AreEqual(TestPlayer->GetYBound(), 88);

			TestPlayer->GetXNorthBoundPoint();
			TestPlayer->GetYNorthBoundPoint();
			TestPlayer->GetXSouthBoundPoint();
			TestPlayer->GetYSouthBoundPoint();
			TestPlayer->GetXEastBoundPoint();
			TestPlayer->GetYEastBoundPoint();
			TestPlayer->GetXWestBoundPoint();
			TestPlayer->GetYWestBoundPoint();
			TestPlayer->GetNorthEastXBoundPoint();
			TestPlayer->GetNorthEastYBoundPoint();
			TestPlayer->GetNorthWestXBoundPoint();
			TestPlayer->GetNorthWestYBoundPoint();
			TestPlayer->GetSouthEastXBoundPoint();
			TestPlayer->GetSouthEastYBoundPoint();
			TestPlayer->GetSouthWestXBoundPoint();
			TestPlayer->GetSouthWestYBoundPoint();

			Assert::AreEqual(TestPlayer->GetXPosition(), 0);
			Assert::AreEqual(TestPlayer->GetYPosition(), 0);

			TestPlayer->GetHitBoxXBoundOne();
			TestPlayer->GetHitBoxYBoundOne();
			TestPlayer->GetHitBoxXBoundTwo();
			TestPlayer->GetHitBoxYBoundTwo();

			TestPlayer->GetCollisionXBoundOne();
			TestPlayer->GetCollisionYBoundOne();
			TestPlayer->GetCollisionXBoundTwo();
			TestPlayer->GetCollisionYBoundTwo();

			TestPlayer->GetWeaponHitBoxXBoundOne();
			TestPlayer->GetWeaponHitBoxYBoundOne();
			TestPlayer->GetWeaponHitBoxXBoundTwo();
			TestPlayer->GetWeaponHitBoxYBoundTwo();

			Assert::AreEqual(TestPlayer->GetFinalTimedScore(60), 0);

			for(int count = 0; count < 10; count++)
			{
				int TestPlayerXPos = rand() % 999999;
				int TestPlayerYPos = rand() % 999999;

				TestPlayer->SetXPosition(TestPlayerXPos);
				TestPlayer->SetYPosition(TestPlayerYPos);

				Assert::AreEqual(TestPlayer->GetXPosition(), TestPlayerXPos);
				Assert::AreEqual(TestPlayer->GetYPosition(), TestPlayerYPos);
			}

			for(int count = 0; count < 10; count++)
			{
				TestPlayer->ScaleGameUp(rand() % 999999);
			}
		}

		TEST_METHOD(ProjectileTest)
		{
			Projectile TestProjectile(rand() % 999999, rand() % 999999, rand() % 999999, rand() % 999999, rand() % 999999, rand() % 999999, rand() % 999999);

			for(int count = 0; count < 10; count++)
			{
				TestProjectile.SendProjecile(rand() % 999999, rand() % 999999, rand() % 999999, rand() % 999999);
			}

			TestProjectile.UpdatePosition();
			TestProjectile.GetHitBoxXBoundOne();
			TestProjectile.GetHitBoxYBoundOne();
			TestProjectile.GetHitBoxXBoundTwo();
			TestProjectile.GetHitBoxYBoundTwo();

			Assert::AreEqual(TestProjectile.IsAtRestingPosition(), false);

			TestProjectile.ResetProjectile();

			Assert::AreEqual(TestProjectile.IsAtRestingPosition(), true);
		}
		
		TEST_METHOD(PlayerScoreCalculatorTest)
		{
			PlayerScoreCalculator TestCalc;

			Assert::AreEqual(TestCalc.GetCurrentPlayerScore(), 0);

			TestCalc.AddBossKilledScore();

			Assert::AreEqual(TestCalc.GetCurrentPlayerScore(), 500);

			TestCalc.AddEnemyKilledScore();

			Assert::AreEqual(TestCalc.GetCurrentPlayerScore(), 550);

			TestCalc.AddLevelUpScore();

			Assert::AreEqual(TestCalc.GetCurrentPlayerScore(), 800);

			Assert::AreEqual(TestCalc.CalculateTimedScore(120, 2), 799);

			TestCalc.ResetScore();

			Assert::AreEqual(TestCalc.GetCurrentPlayerScore(), 0);
		}

		TEST_METHOD(SwordWeaponTest)
		{
			SwordWeapon TestSwordWeapon;

			TestSwordWeapon.EventHandler();

			for(int count = 0; count < 10; count++)
			{
				TestSwordWeapon.Draw(rand() % 999999, rand() % 999999, rand() % 999999, rand() % 999999);
			}
		}

		TEST_METHOD(WeaponTest)
		{
			BowWeapon TestWeapon;

			TestWeapon.EventHandler();

			for(int count = 0; count < 10; count++)
			{
				TestWeapon.Draw(rand() % 999999, rand() % 999999, rand() % 999999, rand() % 999999);
			}

			TestWeapon.ResetWeapon();
			TestWeapon.IncreaseRangedAttackTime();

			Assert::AreEqual(TestWeapon.IsRangedWeapon(), true);

			Assert::AreEqual(TestWeapon.IsActive(), false);

			TestWeapon.Attack();

			Assert::AreEqual(TestWeapon.GetXBound(), 16);
			Assert::AreEqual(TestWeapon.GetYBound(), 16);

			TestWeapon.GetHitBoxXBoundOne();
			TestWeapon.GetHitBoxYBoundOne();
			TestWeapon.GetHitBoxXBoundTwo();
			TestWeapon.GetHitBoxYBoundTwo();

			Assert::AreEqual(TestWeapon.GetDamage(), 6.0f);

			TestWeapon.SetDamageModifier(2.0f);

			Assert::AreEqual(TestWeapon.GetDamage(), 12.0f);
		}

	};
}