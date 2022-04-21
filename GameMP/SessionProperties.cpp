#include "stdafx.h"
#include "Game.h"

extern FLOAT gam_afEnemyMovementSpeed[5];
extern FLOAT gam_afEnemyAttackSpeed[5];
extern FLOAT gam_afDamageStrength[5];
extern FLOAT gam_afAmmoQuantity[5];
extern FLOAT gam_fManaTransferFactor;
extern FLOAT gam_fExtraEnemyStrength          ;
extern FLOAT gam_fExtraEnemyStrengthPerPlayer ;
extern INDEX gam_iCredits;
extern FLOAT gam_tmSpawnInvulnerability;
extern INDEX gam_iScoreLimit;
extern INDEX gam_iFragLimit;
extern INDEX gam_iTimeLimit;
extern INDEX gam_ctMaxPlayers;
extern INDEX gam_bWaitAllPlayers;
extern INDEX gam_bAmmoStays       ;
extern INDEX gam_bHealthArmorStays;
extern INDEX gam_bAllowHealth     ;
extern INDEX gam_bAllowArmor      ;
extern INDEX gam_bInfiniteAmmo    ;
extern INDEX gam_bRespawnInPlace  ;
extern INDEX gam_bPlayEntireGame;
extern INDEX gam_bWeaponsStay;
extern INDEX gam_bFriendlyFire;
extern INDEX gam_iInitialMana;
extern INDEX gam_iQuickStartDifficulty;
extern INDEX gam_iQuickStartMode;
extern INDEX gam_bQuickStartMP;
extern INDEX gam_iStartDifficulty;
extern INDEX gam_iStartMode;
extern INDEX gam_iBlood;
extern INDEX gam_bGibs;
extern INDEX gam_bUseExtraEnemies;
extern CTString gam_strGameSpyExtras;

// [Cecil] Gameplay Options
extern INDEX gam_bComboMode;
extern INDEX gam_bInfiniteCombos;
extern INDEX gam_bPrintCombos;
extern INDEX gam_bStrongerEnemies;

extern INDEX gam_iIgnoreCollision;
extern INDEX gam_bAutosave;

// [Cecil] Anti-lag Options
extern INDEX al_iDebris;
extern INDEX al_bEffects;

// [Cecil] Mutators
extern INDEX mut_bEnable;
extern FLOAT mut_fSpeedMultiplier;
extern FLOAT mut_fJumpMultiplier;
extern FLOAT mut_fStartHealth;
extern FLOAT mut_fMaxHealth;
extern FLOAT mut_fMaxArmor;
extern FLOAT mut_fEnemyMultiplier;
extern FLOAT mut_fFireSpeed;
extern FLOAT mut_fAmmoMultiplier;
extern INDEX mut_bHeatDamage;
extern INDEX mut_bImpactDamage;
extern FLOAT mut_fEnemyDamageMp;
extern FLOAT mut_fPlayerDamageMp;
extern FLOAT mut_fSelfDamageMp;

// [Cecil] MP Options
extern INDEX mp_bUseWeaponRemoving;
extern INDEX mp_bFillAmmo;

extern INDEX mp_bKnifeEnable;
extern INDEX mp_bChainsawEnable;
extern INDEX mp_bColtEnable;
extern INDEX mp_bDColtEnable;
extern INDEX mp_bShotgunEnable;
extern INDEX mp_bDShotgunEnable;
extern INDEX mp_bTommygunEnable;
extern INDEX mp_bMinigunEnable;
extern INDEX mp_bRLauncherEnable;
extern INDEX mp_bGLauncherEnable;
extern INDEX mp_bFlamerEnable;
extern INDEX mp_bSniperEnable;
extern INDEX mp_bLaserEnable;
extern INDEX mp_bCannonEnable;

extern INDEX mp_iWeaponItems;
extern INDEX mp_iReplaceWIT;

extern INDEX mp_bKnifeItemEnable;
extern INDEX mp_bChainsawItemEnable;
extern INDEX mp_bColtItemEnable;
extern INDEX mp_bShotgunItemEnable;
extern INDEX mp_bDShotgunItemEnable;
extern INDEX mp_bTommygunItemEnable;
extern INDEX mp_bMinigunItemEnable;
extern INDEX mp_bRLauncherItemEnable;
extern INDEX mp_bGLauncherItemEnable;
extern INDEX mp_bFlamerItemEnable;
extern INDEX mp_bSniperItemEnable;
extern INDEX mp_bLaserItemEnable;
extern INDEX mp_bCannonItemEnable;

extern INDEX mp_bRocketExplode;
extern INDEX mp_bTeslaGun;
extern INDEX mp_bExplosiveLaser;

extern INDEX mp_bEnableInvulnerability;
extern INDEX mp_bEnableInvisibility;
extern INDEX mp_bEnableSeriousDamage;
extern INDEX mp_bEnableSeriousSpeed;


static void SetGameModeParameters(CSessionProperties &sp)
{
  sp.sp_gmGameMode = (CSessionProperties::GameMode) Clamp(INDEX(gam_iStartMode), -1L, 3L);

  switch (sp.sp_gmGameMode) {
  default:
    ASSERT(FALSE);
  case CSessionProperties::GM_COOPERATIVE:
    sp.sp_ulSpawnFlags |= SPF_COOPERATIVE;
    break;
  case CSessionProperties::GM_SINGLEPLAYER:
    sp.sp_ulSpawnFlags |= SPF_SINGLEPLAYER|SPF_COOPERATIVE;
    break;
  case CSessionProperties::GM_FLYOVER:
    sp.sp_ulSpawnFlags |= SPF_FLYOVER|SPF_MASK_DIFFICULTY;
    break;
  case CSessionProperties::GM_SCOREMATCH:
  case CSessionProperties::GM_FRAGMATCH:
    sp.sp_ulSpawnFlags |= SPF_DEATHMATCH;
    break;
  }
}
static void SetDifficultyParameters(CSessionProperties &sp)
{
  INDEX iDifficulty = gam_iStartDifficulty;
  if (iDifficulty==4) {
    sp.sp_bMental = TRUE;
    iDifficulty=2;
  } else {
    sp.sp_bMental = FALSE;
  }
  sp.sp_gdGameDifficulty = (CSessionProperties::GameDifficulty) Clamp(INDEX(iDifficulty), -1L, 3L);

  switch (sp.sp_gdGameDifficulty) {
  case CSessionProperties::GD_TOURIST:
    sp.sp_ulSpawnFlags = SPF_EASY;//SPF_TOURIST; !!!!
    sp.sp_fEnemyMovementSpeed = gam_afEnemyMovementSpeed [0];
    sp.sp_fEnemyAttackSpeed   = gam_afEnemyAttackSpeed   [0];
    sp.sp_fDamageStrength     = gam_afDamageStrength     [0];
    sp.sp_fAmmoQuantity       = gam_afAmmoQuantity       [0];
    break;
  case CSessionProperties::GD_EASY:
    sp.sp_ulSpawnFlags = SPF_EASY;
    sp.sp_fEnemyMovementSpeed = gam_afEnemyMovementSpeed [1];
    sp.sp_fEnemyAttackSpeed   = gam_afEnemyAttackSpeed   [1];
    sp.sp_fDamageStrength     = gam_afDamageStrength     [1];
    sp.sp_fAmmoQuantity       = gam_afAmmoQuantity       [1];
    break;
  default:
    ASSERT(FALSE);
  case CSessionProperties::GD_NORMAL:
    sp.sp_ulSpawnFlags = SPF_NORMAL;
    sp.sp_fEnemyMovementSpeed = gam_afEnemyMovementSpeed [2];
    sp.sp_fEnemyAttackSpeed   = gam_afEnemyAttackSpeed   [2];
    sp.sp_fDamageStrength     = gam_afDamageStrength     [2];
    sp.sp_fAmmoQuantity       = gam_afAmmoQuantity       [2];
    break;
  case CSessionProperties::GD_HARD:
    sp.sp_ulSpawnFlags = SPF_HARD;
    sp.sp_fEnemyMovementSpeed = gam_afEnemyMovementSpeed [3];
    sp.sp_fEnemyAttackSpeed   = gam_afEnemyAttackSpeed   [3];
    sp.sp_fDamageStrength     = gam_afDamageStrength     [3];
    sp.sp_fAmmoQuantity       = gam_afAmmoQuantity       [3];
    break;
  case CSessionProperties::GD_EXTREME:
    sp.sp_ulSpawnFlags = SPF_EXTREME;
    sp.sp_fEnemyMovementSpeed = gam_afEnemyMovementSpeed [4];
    sp.sp_fEnemyAttackSpeed   = gam_afEnemyAttackSpeed   [4];
    sp.sp_fDamageStrength     = gam_afDamageStrength     [4];
    sp.sp_fAmmoQuantity       = gam_afAmmoQuantity       [4];
    break;
  }
}

// set properties for a single player session
void CGame::SetSinglePlayerSession(CSessionProperties &sp)
{
  // clear
  memset(&sp, 0, sizeof(sp));

  SetDifficultyParameters(sp);
  SetGameModeParameters(sp);
  sp.sp_ulSpawnFlags&=~SPF_COOPERATIVE;

  sp.sp_bEndOfGame = FALSE;

  sp.sp_ctMaxPlayers = 1;
  sp.sp_bWaitAllPlayers = FALSE;
  sp.sp_bQuickTest = FALSE;
  sp.sp_bCooperative = TRUE;
  sp.sp_bSinglePlayer = TRUE;
  sp.sp_bUseFrags = FALSE;

  // [Cecil] Gameplay Options
  sp.sp_bComboMode = 0;
  sp.sp_bInfiniteCombos = 0;
  sp.sp_bPrintCombos = 1;
  sp.sp_bStrongerEnemies = 0;
  
  sp.sp_iIgnoreCollision = 0;
  sp.sp_bAutosave = FALSE;

  // [Cecil] Anti-lag Options
  sp.sp_iDebris = 2;
  sp.sp_bEffects = 1;

  // [Cecil] Mutators
  sp.sp_fSpeedMultiplier = 1;
  sp.sp_fJumpMultiplier = 1;
  sp.sp_fStartHealth = 100;
  sp.sp_fMaxHealth = 200;
  sp.sp_fMaxArmor = 200;
  sp.sp_fEnemyMultiplier = 1;
  sp.sp_fFireSpeed = 1;
  sp.sp_fAmmoMultiplier = 1;
  sp.sp_bHeatDamage = 1;
  sp.sp_bImpactDamage = 1;
  sp.sp_bMutators = 0;
  sp.sp_fEnemyDamageMp = 1;
  sp.sp_fPlayerDamageMp = 1;
  sp.sp_fSelfDamageMp = 1;

  // [Cecil] MP Options
  sp.sp_bUseWeaponRemoving = 0;
  sp.sp_bFillAmmo = 0;
  sp.sp_bKnifeEnable = 1;
  sp.sp_bChainsawEnable = 0;
  sp.sp_bColtEnable = 1;
  sp.sp_bDColtEnable = 0;
  sp.sp_bShotgunEnable = 0;
  sp.sp_bDShotgunEnable = 0;
  sp.sp_bTommygunEnable = 0;
  sp.sp_bMinigunEnable = 0;
  sp.sp_bRLauncherEnable = 0;
  sp.sp_bGLauncherEnable = 0;
  sp.sp_bFlamerEnable = 0;
  sp.sp_bSniperEnable = 0;
  sp.sp_bLaserEnable = 0;
  sp.sp_bCannonEnable = 0;

  sp.sp_iWeaponItems = 5;
  sp.sp_iReplaceWIT = 0;

  sp.sp_bKnifeItemEnable = 1;
  sp.sp_bChainsawItemEnable = 1;
  sp.sp_bColtItemEnable = 1;
  sp.sp_bShotgunItemEnable = 1;
  sp.sp_bDShotgunItemEnable = 1;
  sp.sp_bTommygunItemEnable = 1;
  sp.sp_bMinigunItemEnable = 1;
  sp.sp_bRLauncherItemEnable = 1;
  sp.sp_bGLauncherItemEnable = 1;
  sp.sp_bFlamerItemEnable = 1;
  sp.sp_bSniperItemEnable = 1;
  sp.sp_bLaserItemEnable = 1;
  sp.sp_bCannonItemEnable = 1;
  sp.sp_bRocketExplode = 1;
  sp.sp_bTeslaGun = 0;
  sp.sp_bExplosiveLaser = 0;
  
  sp.sp_bEnableInvulnerability = 1;
  sp.sp_bEnableInvisibility = 1;
  sp.sp_bEnableSeriousDamage = 1;
  sp.sp_bEnableSeriousSpeed = 1;

  // other

  sp.sp_iScoreLimit = 0;
  sp.sp_iFragLimit  = 0; 
  sp.sp_iTimeLimit  = 0; 

  sp.sp_ctCredits     = 0;
  sp.sp_ctCreditsLeft = 0;
  sp.sp_tmSpawnInvulnerability = 0;

  sp.sp_bTeamPlay = FALSE;
  sp.sp_bFriendlyFire = FALSE;
  sp.sp_bWeaponsStay = FALSE;
  sp.sp_bPlayEntireGame = TRUE;

  sp.sp_bAmmoStays        = FALSE;
  sp.sp_bHealthArmorStays = FALSE;
  sp.sp_bAllowHealth = TRUE;
  sp.sp_bAllowArmor = TRUE;
  sp.sp_bInfiniteAmmo = FALSE;
  sp.sp_bRespawnInPlace = FALSE;
  sp.sp_fExtraEnemyStrength          = 0;
  sp.sp_fExtraEnemyStrengthPerPlayer = 0;

  sp.sp_iBlood = Clamp( gam_iBlood, 0L, 3L);
  sp.sp_bGibs  = gam_bGibs;
}

// set properties for a quick start session
void CGame::SetQuickStartSession(CSessionProperties &sp)
{
  gam_iStartDifficulty = gam_iQuickStartDifficulty;
  gam_iStartMode = gam_iQuickStartMode;

  // same as single player
  if (!gam_bQuickStartMP) {
    SetSinglePlayerSession(sp);
  } else {
    SetMultiPlayerSession(sp);
  }
  // quick start type
  sp.sp_bQuickTest = TRUE;

}

// set properties for a multiplayer session
void CGame::SetMultiPlayerSession(CSessionProperties &sp)
{
  // clear
  memset(&sp, 0, sizeof(sp));

  SetDifficultyParameters(sp);
  SetGameModeParameters(sp);
  sp.sp_ulSpawnFlags&=~SPF_SINGLEPLAYER;

  sp.sp_bEndOfGame = FALSE;

  sp.sp_bQuickTest = FALSE;
  sp.sp_bCooperative = (sp.sp_gmGameMode==CSessionProperties::GM_COOPERATIVE || sp.sp_gmGameMode==CSessionProperties::GM_SINGLEPLAYER);
  sp.sp_bSinglePlayer = FALSE;
  sp.sp_bPlayEntireGame = gam_bPlayEntireGame;
  sp.sp_bUseFrags = sp.sp_gmGameMode==CSessionProperties::GM_FRAGMATCH;
  sp.sp_bWeaponsStay = gam_bWeaponsStay;
  sp.sp_bFriendlyFire = gam_bFriendlyFire;
  sp.sp_ctMaxPlayers = gam_ctMaxPlayers;
  sp.sp_bWaitAllPlayers = gam_bWaitAllPlayers;

  sp.sp_bAmmoStays        = gam_bAmmoStays       ;
  sp.sp_bHealthArmorStays = gam_bHealthArmorStays;
  sp.sp_bAllowHealth      = gam_bAllowHealth     ;
  sp.sp_bAllowArmor       = gam_bAllowArmor      ;
  sp.sp_bInfiniteAmmo     = gam_bInfiniteAmmo    ;
  sp.sp_bRespawnInPlace   = gam_bRespawnInPlace  ;

  // [Cecil] Gameplay Options
  sp.sp_bComboMode = gam_bComboMode;
  sp.sp_bInfiniteCombos = gam_bInfiniteCombos;
  sp.sp_bPrintCombos = gam_bPrintCombos;
  sp.sp_bStrongerEnemies = gam_bStrongerEnemies;
  
  sp.sp_iIgnoreCollision = gam_iIgnoreCollision;
  sp.sp_bAutosave = gam_bAutosave;

  // [Cecil] Anti-lag Options
  sp.sp_iDebris = al_iDebris;
  sp.sp_bEffects = al_bEffects;

  // [Cecil] Mutators
  if (mut_bEnable) {
    sp.sp_fSpeedMultiplier = mut_fSpeedMultiplier;
    sp.sp_fJumpMultiplier = mut_fJumpMultiplier;
    sp.sp_fStartHealth = mut_fStartHealth;
    sp.sp_fMaxHealth = mut_fMaxHealth;
    sp.sp_fMaxArmor = mut_fMaxArmor;
    sp.sp_fEnemyMultiplier = mut_fEnemyMultiplier;
    sp.sp_fFireSpeed = mut_fFireSpeed;
    sp.sp_fAmmoMultiplier = mut_fAmmoMultiplier;
    sp.sp_bHeatDamage = mut_bHeatDamage;
    sp.sp_bImpactDamage = mut_bImpactDamage;
    sp.sp_fEnemyDamageMp = mut_fEnemyDamageMp;
    sp.sp_fPlayerDamageMp = mut_fPlayerDamageMp;
    sp.sp_fSelfDamageMp = mut_fSelfDamageMp;
  } else {
    sp.sp_fSpeedMultiplier = 1;
    sp.sp_fJumpMultiplier = 1;
    sp.sp_fStartHealth = 100;
    sp.sp_fMaxHealth = 200;
    sp.sp_fMaxArmor = 200;
    sp.sp_fEnemyMultiplier = 1;
    sp.sp_fFireSpeed = 1;
    sp.sp_fAmmoMultiplier = 1;
    sp.sp_bHeatDamage = 1;
    sp.sp_bImpactDamage = 1;
    sp.sp_fEnemyDamageMp = 1;
    sp.sp_fPlayerDamageMp = 1;
    sp.sp_fSelfDamageMp = 1;
  }
  sp.sp_bMutators = mut_bEnable;

  // [Cecil] MP Options
  sp.sp_bUseWeaponRemoving = mp_bUseWeaponRemoving;
  sp.sp_bFillAmmo = mp_bFillAmmo;
  sp.sp_bKnifeEnable = mp_bKnifeEnable;
  sp.sp_bChainsawEnable = mp_bChainsawEnable;
  sp.sp_bColtEnable = mp_bColtEnable;
  sp.sp_bDColtEnable = mp_bDColtEnable;
  sp.sp_bShotgunEnable = mp_bShotgunEnable;
  sp.sp_bDShotgunEnable = mp_bDShotgunEnable;
  sp.sp_bTommygunEnable = mp_bTommygunEnable;
  sp.sp_bMinigunEnable = mp_bMinigunEnable;
  sp.sp_bRLauncherEnable = mp_bRLauncherEnable;
  sp.sp_bGLauncherEnable = mp_bGLauncherEnable;
  sp.sp_bFlamerEnable = mp_bFlamerEnable;
  sp.sp_bSniperEnable = mp_bSniperEnable;
  sp.sp_bLaserEnable = mp_bLaserEnable;
  sp.sp_bCannonEnable = mp_bCannonEnable;

  sp.sp_iWeaponItems = mp_iWeaponItems;
  sp.sp_iReplaceWIT = mp_iReplaceWIT;

  sp.sp_bKnifeItemEnable = mp_bKnifeItemEnable;
  sp.sp_bChainsawItemEnable = mp_bChainsawItemEnable;
  sp.sp_bColtItemEnable = mp_bColtItemEnable;
  sp.sp_bShotgunItemEnable = mp_bShotgunItemEnable;
  sp.sp_bDShotgunItemEnable = mp_bDShotgunItemEnable;
  sp.sp_bTommygunItemEnable = mp_bTommygunItemEnable;
  sp.sp_bMinigunItemEnable = mp_bMinigunItemEnable;
  sp.sp_bRLauncherItemEnable = mp_bRLauncherItemEnable;
  sp.sp_bGLauncherItemEnable = mp_bGLauncherItemEnable;
  sp.sp_bFlamerItemEnable = mp_bFlamerItemEnable;
  sp.sp_bSniperItemEnable = mp_bSniperItemEnable;
  sp.sp_bLaserItemEnable = mp_bLaserItemEnable;
  sp.sp_bCannonItemEnable = mp_bCannonItemEnable;
  sp.sp_bRocketExplode = mp_bRocketExplode;
  sp.sp_bTeslaGun = mp_bTeslaGun;
  sp.sp_bExplosiveLaser = mp_bExplosiveLaser;

  sp.sp_bEnableInvulnerability = mp_bEnableInvulnerability;
  sp.sp_bEnableInvisibility = mp_bEnableInvisibility;
  sp.sp_bEnableSeriousDamage = mp_bEnableSeriousDamage;
  sp.sp_bEnableSeriousSpeed = mp_bEnableSeriousSpeed;

  sp.sp_fManaTransferFactor = gam_fManaTransferFactor;
  sp.sp_fExtraEnemyStrength          = gam_fExtraEnemyStrength         ;
  sp.sp_fExtraEnemyStrengthPerPlayer = gam_fExtraEnemyStrengthPerPlayer;
  sp.sp_iInitialMana        = gam_iInitialMana;

  sp.sp_iBlood = Clamp( gam_iBlood, 0L, 3L);
  sp.sp_bGibs  = gam_bGibs;
  sp.sp_tmSpawnInvulnerability = gam_tmSpawnInvulnerability;

  sp.sp_bUseExtraEnemies = gam_bUseExtraEnemies;

  // set credits and limits
  if (sp.sp_bCooperative) {
    sp.sp_ctCredits     = gam_iCredits;
    sp.sp_ctCreditsLeft = gam_iCredits;
    sp.sp_iScoreLimit = 0;
    sp.sp_iFragLimit  = 0;
    sp.sp_iTimeLimit  = 0;
    sp.sp_bAllowHealth = TRUE;
    sp.sp_bAllowArmor  = TRUE;

  } else {
    sp.sp_ctCredits     = -1;
    sp.sp_ctCreditsLeft = -1;
    sp.sp_iScoreLimit = gam_iScoreLimit;
    sp.sp_iFragLimit  = gam_iFragLimit;
    sp.sp_iTimeLimit  = gam_iTimeLimit;
    sp.sp_bWeaponsStay = FALSE;
    sp.sp_bAmmoStays = FALSE;
    sp.sp_bHealthArmorStays = FALSE;
    if (sp.sp_bUseFrags) {
      sp.sp_iScoreLimit = 0;
    } else {
      sp.sp_iFragLimit = 0;
    }
  }
}

BOOL IsMenuEnabled(const CTString &strMenuName)
{
  if (strMenuName=="Single Player") {
    return FALSE;
  } else if (strMenuName=="Network"      ) {
    return TRUE;
  } else if (strMenuName=="Split Screen" ) {
    return TRUE;
  } else if (strMenuName=="High Score"   ) {
    return FALSE;
  } else if (strMenuName=="Training"   ) {
    return FALSE;
  } else if (strMenuName=="Technology Test") {
    return FALSE;
  } else {
    return TRUE;
  }
}

CTString GetGameTypeName(INDEX iMode)
{
  switch (iMode) {
  default:
    return "";
    break;
  case CSessionProperties::GM_COOPERATIVE:
    return TRANS("Cooperative (Coop Maps)");
    break;
  case CSessionProperties::GM_SINGLEPLAYER:
    return TRANS("Cooperative (SP Maps)");
    break;
  case CSessionProperties::GM_FLYOVER:
    return TRANS("Flyover");
    break;
  case CSessionProperties::GM_SCOREMATCH:
    return TRANS("Scorematch");
    break;
  case CSessionProperties::GM_FRAGMATCH:
    return TRANS("Fragmatch");
    break;
  }
}
CTString GetCurrentGameTypeName()
{
  const CSessionProperties &sp = *GetSP();
  return GetGameTypeName(sp.sp_gmGameMode);
}

CTString GetGameSpyRulesInfo(void)
{
  CTString strOut;
	CTString strKey;
  const CSessionProperties &sp = *GetSP();


  CTString strDifficulty;
  if (sp.sp_bMental) {
    strDifficulty = TRANS("Mental");
  } else {
    switch(sp.sp_gdGameDifficulty) {
    case CSessionProperties::GD_TOURIST:
      strDifficulty = TRANS("Tourist");
      break;
    case CSessionProperties::GD_EASY:
      strDifficulty = TRANS("Easy");
      break;
    default:
      ASSERT(FALSE);
    case CSessionProperties::GD_NORMAL:
      strDifficulty = TRANS("Normal");
      break;
    case CSessionProperties::GD_HARD:
      strDifficulty = TRANS("Hard");
      break;
    case CSessionProperties::GD_EXTREME:
      strDifficulty = TRANS("Serious");
      break;
    }
  }

	strKey.PrintF("\\difficulty\\%s", (const char*)strDifficulty);
	strOut+=strKey;

  strKey.PrintF("\\friendlyfire\\%d", sp.sp_bFriendlyFire?0:1);
	strOut+=strKey;
  
  strKey.PrintF("\\weaponsstay\\%d", sp.sp_bWeaponsStay?0:1);
	strOut+=strKey;

  strKey.PrintF("\\ammostays\\%d", sp.sp_bAmmoStays                   ?0:1);	strOut+=strKey;
  strKey.PrintF("\\healthandarmorstays\\%d", sp.sp_bHealthArmorStays  ?0:1);	strOut+=strKey;
  strKey.PrintF("\\allowhealth\\%d", sp.sp_bAllowHealth               ?0:1);	strOut+=strKey;
  strKey.PrintF("\\allowarmor\\%d", sp.sp_bAllowArmor                 ?0:1);	strOut+=strKey;
  strKey.PrintF("\\infiniteammo\\%d", sp.sp_bInfiniteAmmo             ?0:1);	strOut+=strKey;
  strKey.PrintF("\\respawninplace\\%d", sp.sp_bRespawnInPlace         ?0:1);	strOut+=strKey;

  if (sp.sp_bCooperative) {
    if (sp.sp_ctCredits<0) {
      strKey.PrintF("\\credits\\infinite");
  	  strOut+=strKey;
    } else if (sp.sp_ctCredits>0) {
      strKey.PrintF("\\credits\\%d", sp.sp_ctCredits);
  	  strOut+=strKey;
      strKey.PrintF("\\credits_left\\%d", sp.sp_ctCreditsLeft);
  	  strOut+=strKey;
    }
  } else {
    if (sp.sp_bUseFrags && sp.sp_iFragLimit>0) {
      strKey.PrintF("\\fraglimit\\%d", sp.sp_iFragLimit);
  	  strOut+=strKey;
    }
    if (!sp.sp_bUseFrags && sp.sp_iScoreLimit>0) {
      strKey.PrintF("\\fraglimit\\%d", sp.sp_iScoreLimit);
  	  strOut+=strKey;
    }
    if (sp.sp_iTimeLimit>0) {
      strKey.PrintF("\\timelimit\\%d", sp.sp_iTimeLimit);
  	  strOut+=strKey;
    }
  }

  strOut+=gam_strGameSpyExtras;
  return strOut;
}

ULONG GetSpawnFlagsForGameType(INDEX iGameType)
{
  switch(iGameType) {
  default:
    ASSERT(FALSE);
  case CSessionProperties::GM_COOPERATIVE:  return SPF_COOPERATIVE;
  case CSessionProperties::GM_SINGLEPLAYER: return SPF_SINGLEPLAYER;
  case CSessionProperties::GM_SCOREMATCH:   return SPF_DEATHMATCH|SPF_COOPERATIVE;
  case CSessionProperties::GM_FRAGMATCH:    return SPF_DEATHMATCH|SPF_COOPERATIVE;
  };
}

