#if defined _abd_included
	#endinput
#endif
#define _abd_included

#include <a_samp>

static
	g_TimesDetected[ MAX_PLAYERS ]
;

public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
	if(hittype == BULLET_HIT_TYPE_PLAYER && !IsPlayerInAnyVehicle(playerid))
 	{
     	static
			g_surf,
			g_tsurf
		;
		g_surf = GetPlayerSurfingVehicleID(playerid);
		g_tsurf = GetPlayerSurfingVehicleID(hitid);

		if(g_surf == INVALID_VEHICLE_ID && g_tsurf == INVALID_VEHICLE_ID && !IsPlayerInAnyVehicle(hitid))
		{
	    	static
				Float:g_targetpkl,
				Float:g_playerpkl
			;
			g_targetpkl = NetStats_PacketLossPercent(hitid);
		 	g_playerpkl = NetStats_PacketLossPercent(playerid);

		    if(g_targetpkl < 0.7 && g_playerpkl < 0.7)
		    {
			    static
					Float:g_fOriginX, Float:g_fOriginY, Float:g_fOriginZ,
					Float:g_fHitPosX, Float:g_fHitPosY, Float:g_fHitPosZ
				;
				GetPlayerLastShotVectors(playerid, g_fOriginX, g_fOriginY, g_fOriginZ, g_fHitPosX, g_fHitPosY, g_fHitPosZ);

				if(!IsPlayerInRangeOfPoint(hitid, 3.0, g_fHitPosX, g_fHitPosY, g_fHitPosZ))
    			{
					g_TimesDetected[ playerid ]++;
					if(g_TimesDetected[ playerid ] >= 3)
					{
						CallRemoteFunction("OnAimbotDetect", "ii", playerid, hitid);
					}
				}
				else
				{
				    if(g_TimesDetected[ playerid ] > 0)
					{
						g_TimesDetected[ playerid ]--;
					}
				}
			}
        }
	}
	#if defined abd_OnPlayerWeaponShot
        abd_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
 	#endif
    return 1;
}

#if defined abd_OnPlayerWeaponShot
    forward abd_OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ);
#endif

#if defined _ALS_OnPlayerWeaponShot
    #undef OnPlayerWeaponShot
#else
    #define _ALS_OnPlayerWeaponShot
#endif
#define OnPlayerWeaponShot abd_OnPlayerWeaponShot