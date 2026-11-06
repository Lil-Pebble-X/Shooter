#include "ShooterWeaponCatalog.h"
#include "ShooterWeaponData.h"

bool UShooterWeaponCatalog::TryMakeLoadout(int32 PrimaryIndex, int32 SecondaryIndex, FShooterLoadout& OutLoadout) const
{
	if (!Weapons.IsValidIndex(PrimaryIndex)|| !Weapons.IsValidIndex(SecondaryIndex))
	{
		return false;
	}

	UShooterWeaponData* PrimaryWeapon = Weapons[PrimaryIndex];
	UShooterWeaponData* SecondaryWeapon = Weapons[SecondaryIndex];

	if (!IsValid(PrimaryWeapon)|| !IsValid(SecondaryWeapon))
	{
		return false;
	}

	OutLoadout.PrimaryWeapon = PrimaryWeapon;
	OutLoadout.SecondaryWeapon = SecondaryWeapon;

	return true;
}