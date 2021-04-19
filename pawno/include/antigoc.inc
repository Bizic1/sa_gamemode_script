#include <a_samp> //SA-MP TEAM
#include <foreach>

//FGAC - Diaz 1.2

static jackedcar[MAX_PLAYERS],warpwarn[MAX_PLAYERS],legitvehicle[MAX_PLAYERS],ivt[MAX_PLAYERS],vtimer;


new Float:CarPosX[MAX_VEHICLES];
new Float:CarPosY[MAX_VEHICLES];
new Float:CarPosZ[MAX_VEHICLES];

forward AT_Timer();

static
	VehicleDriverPlayerid[MAX_VEHICLES] = { INVALID_PLAYER_ID, ... };

public OnGameModeInit()
{

	vtimer = SetTimer("AT_Timer", 1000, true);
    #if defined Troll_OnGameModeInit
    return Troll_OnGameModeInit();
    #else
    return 1;
    #endif
}

public OnGameModeExit()
{

	KillTimer(vtimer);
    #if defined Troll_OnGameModeExit
    return Troll_OnGameModeExit();
    #else
    return 1;
    #endif
}

public AT_Timer()
{
	foreach(new t: Player)
	{
		if(IsPlayerInAnyVehicle(t))
		{
			if(GetPlayerVehicleID(t) != legitvehicle[t])
			{
				CallLocalFunction("OnPlayerTrollDetected", "i", t);
			}
		}
	}
	return 1;
}

public OnPlayerUpdate(playerid)
{
    if(jackedcar[playerid] != -1 && GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_ENTER_VEHICLE)
    {
        jackedcar[playerid] = -1;
		legitvehicle[playerid] = ivt[playerid];
        return 1;
    }

    #if defined Troll_OnPlayerUpdate
    return Troll_OnPlayerUpdate(playerid);
    #else
    return 1;
    #endif
}

forward OnPlayerTrollDetected(playerid);

public OnPlayerExitVehicle(playerid, vehicleid)
{
    jackedcar[playerid] = -1;
    GetVehiclePos(vehicleid,CarPosX[vehicleid],CarPosY[vehicleid],CarPosZ[vehicleid]);

    #if defined Troll_OnPlayerExitVehicle
    return Troll_OnPlayerExitVehicle(playerid, vehicleid);
    #else
    return 1;
    #endif
}

public OnPlayerStateChange(playerid, newstate, oldstate)
{
	if(oldstate == PLAYER_STATE_PASSENGER && newstate == PLAYER_STATE_DRIVER)
	{
		CallLocalFunction("OnPlayerTrollDetected", "i", playerid);
	}
	if(newstate == PLAYER_STATE_DRIVER)
	{
	    GetVehiclePos(GetPlayerVehicleID(playerid),CarPosX[GetPlayerVehicleID(playerid)],CarPosY[GetPlayerVehicleID(playerid)],CarPosZ[GetPlayerVehicleID(playerid)]);
		if(GetPlayerVehicleID(playerid) == jackedcar[playerid])
		{
		    CallLocalFunction("OnPlayerTrollDetected", "i", playerid);
		}
		new
			vehicleid = GetPlayerVehicleID(playerid);
		if(vehicleid)
		{
			VehicleDriverPlayerid[vehicleid - 1] = playerid;
		}
	}

	#if defined Troll_OnPlayerStateChange
    return Troll_OnPlayerStateChange(playerid, newstate, oldstate);
    #else
    return 1;
    #endif
}


public OnPlayerDeath(playerid, killerid, reason)
{
    if(IsPlayerInAnyVehicle(playerid))
    {
        GetVehiclePos(GetPlayerVehicleID(playerid),CarPosX[GetPlayerVehicleID(playerid)],CarPosY[GetPlayerVehicleID(playerid)],CarPosZ[GetPlayerVehicleID(playerid)]);
    }

	#if defined Troll_OnPlayerDeath
    return Troll_OnPlayerDeath(playerid, killerid, reason);
    #else
    return 1;
    #endif
}

public OnPlayerDisconnect(playerid,reason)
{
    jackedcar[playerid] = -1;
    warpwarn[playerid]=0;
    if(IsPlayerInAnyVehicle(playerid))
    {
        GetVehiclePos(GetPlayerVehicleID(playerid),CarPosX[GetPlayerVehicleID(playerid)],CarPosY[GetPlayerVehicleID(playerid)],CarPosZ[GetPlayerVehicleID(playerid)]);
    }

	#if defined Troll_OnPlayerDisconnect
    return Troll_OnPlayerDisconnect(playerid, reason);
    #else
    return 1;
    #endif
}

public OnPlayerConnect(playerid)
{
    jackedcar[playerid] = -1;
    warpwarn[playerid]=0;

	#if defined Troll_OnPlayerConnect
    return Troll_OnPlayerConnect(playerid);
    #else
    return 1;
    #endif
}

public OnPlayerEnterVehicle(playerid, vehicleid, ispassenger)
{
	ivt[playerid] = vehicleid;
	if(!ispassenger)
	{
	    if(RemoteJacking(playerid,vehicleid))
	    {
			jackedcar[playerid] = vehicleid;
	    }
	    else
		{
            jackedcar[playerid] = -1;
			legitvehicle[playerid] = vehicleid;
		}
	}
	else
	{
		legitvehicle[playerid] = vehicleid;
	}
	#if defined Troll_OnPlayerEnterVehicle
    return Troll_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
    #else
    return 1;
    #endif
}



public OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z)
{
    if(GetVehicleDistanceFromPoint(vehicleid, new_x, new_y, new_z) > 25)
    {
        if(warpwarn[playerid] != 3)
		{
		    warpwarn[playerid]++;
		}
		else
		{
		    //for(new i = GetVehiclePoolSize(); i > 0; i--)
			//{
			//	SetVehicleToRespawn(i); BAGA NA SKILLARENI
			//}
            warpwarn[playerid]=0;
            CallLocalFunction("OnPlayerTrollDetected", "i", playerid);
		}
	}
	CarPosX[vehicleid] = new_x;
 	CarPosY[vehicleid] = new_y;
  	CarPosZ[vehicleid] = new_z;
    return 1;
}


static stock RemoteJacking(playerid,vehicleid)
{
    new Float:fDistance,Float:raX,Float:raY,Float:raZ,Float:Value;
	if(GetVehicleDriver(vehicleid) == INVALID_PLAYER_ID)
	{
	    fDistance = GetPlayerDistanceFromPoint(playerid,CarPosX[vehicleid],CarPosY[vehicleid],CarPosZ[vehicleid]);
	    Value = 13.0;
	}
	else
	{
	    GetPlayerPos(playerid,raX,raY,raZ);
	    fDistance = GetVehicleDistanceFromPoint(vehicleid, raX, raY, raZ);
	    Value = 15.0;
	}
	if(GetVehicleModel(vehicleid) == 577 || GetVehicleModel(vehicleid) == 592 || GetVehicleModel(vehicleid) == 553 || GetVehicleModel(vehicleid) == 511 || GetVehicleModel(vehicleid) == 519) return 0;
	if(!IsVehicleStreamedIn(vehicleid, playerid)) return 0;
	if(fDistance > Value)
	{
		return 1;
	}
	return 0;
}

static stock GetVehicleDriver(vehicleid)
{
	if(vehicleid < 1 || vehicleid > sizeof(VehicleDriverPlayerid)) return INVALID_PLAYER_ID; // invalid array
	if(VehicleDriverPlayerid[vehicleid - 1] != INVALID_PLAYER_ID  && IsPlayerInVehicle(VehicleDriverPlayerid[vehicleid - 1], vehicleid) && GetPlayerVehicleSeat(VehicleDriverPlayerid[vehicleid - 1]) == 0)
		return VehicleDriverPlayerid[vehicleid - 1]; // return playerid vehicledriver
	return VehicleDriverPlayerid[vehicleid - 1] = INVALID_PLAYER_ID;
}

stock PutPlayerInVehicleEx(playerid, vehicleid, seatid)
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		jackedcar[playerid] = -1;
		new Float:Acc,Float:Acd,Float:Acy;
		GetPlayerPos(playerid,Acc,Acd,Acy);
		SetPlayerPos(playerid,Acc,Acd,Acy);
	}
    if(PutPlayerInVehicle(playerid, vehicleid, seatid))
    {
        jackedcar[playerid] = -1;
		legitvehicle[playerid] = vehicleid;
		return 1;
    }
	return 0;
}

#if defined _ALS_PutPlayerInVehicle
    #undef PutPlayerInVehicle
#else
    #define _ALS_PutPlayerInVehicle
#endif

#define PutPlayerInVehicle PutPlayerInVehicleEx



stock RemovePlayerFromVehicleEx(playerid)
{
    if(RemovePlayerFromVehicle(playerid))
    {
        jackedcar[playerid] = -1;
		return 1;
    }
    return 0;
}

#if defined _ALS_RemovePlayerFromVehicle
    #undef RemovePlayerFromVehicle
#else
    #define _ALS_RemovePlayerFromVehicle
#endif

#define RemovePlayerFromVehicle RemovePlayerFromVehicleEx

stock AddStaticVehicleExHook(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:angle, color1, color2, respawn_delay)
{
    new vehicleid = AddStaticVehicleEx(modelid, spawn_x, spawn_y, spawn_z, angle, color1, color2, respawn_delay);
    {
        CarPosX[vehicleid] = spawn_x; CarPosY[vehicleid] = spawn_y; CarPosZ[vehicleid] = spawn_z;
    }
    return vehicleid;
}

#if defined _ALS_AddStaticVehicleEx
    #undef AddStaticVehicleEx
#else
    #define _ALS_AddStaticVehicleEx
#endif

#define AddStaticVehicleEx AddStaticVehicleExHook



stock AddStaticVehicleHook(modelid, Float:spawn_x, Float:spawn_y, Float:spawn_z, Float:angle, color1, color2)
{
    new vehicleid = AddStaticVehicle(modelid, spawn_x, spawn_y, spawn_z, angle, color1, color2);
    if(vehicleid)
    {
        CarPosX[vehicleid] = spawn_x; CarPosY[vehicleid] = spawn_y; CarPosZ[vehicleid] = spawn_z;
	}
    return vehicleid;
}

#if defined _ALS_AddStaticVehicle
    #undef AddStaticVehicle
#else
    #define _ALS_AddStaticVehicle
#endif

#define AddStaticVehicle AddStaticVehicleHook



stock CreateVehicleEx(vehicletype, Float:x, Float:y, Float:z, Float:rotation, color1, color2, respawn_delay)
{
    new vehicleid = CreateVehicle(vehicletype, x, y, z, rotation, color1, color2, respawn_delay);
    if(vehicleid)
    {
    	CarPosX[vehicleid] = x; CarPosY[vehicleid] = y; CarPosZ[vehicleid] = z;
    }
    return vehicleid;
}



#if defined _ALS_CreateVehicle
    #undef CreateVehicle
#else
    #define _ALS_CreateVehicle
#endif

#define CreateVehicle CreateVehicleEx


stock SetVehiclePosEx(vehicleid, Float:x, Float:y, Float:z)
{
    if(SetVehiclePos(vehicleid, Float:x, Float:y, Float:z))
    {
        CarPosX[vehicleid] = x; CarPosY[vehicleid] = y; CarPosZ[vehicleid] = z;
		return 1;
    }
    return 0;
}

#if defined _ALS_SetVehiclePos
    #undef SetVehiclePos
#else
    #define _ALS_SetVehiclePos
#endif

#define SetVehiclePos SetVehiclePosEx



stock UserName(playerid)
{
	new name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, name, MAX_PLAYER_NAME);
	return name;
}


//Callback Hooks

#if defined _ALS_OnGameModeInit
#undef OnGameModeInit
#else
#define _ALS_OnGameModeInit
#endif

#define OnGameModeInit Troll_OnGameModeInit
#if defined Troll_OnGameModeInit
forward Troll_OnGameModeInit();
#endif

#if defined _ALS_OnGameModeExit
#undef OnGameModeExit
#else
#define _ALS_OnGameModeExit
#endif

#define OnGameModeExit Troll_OnGameModeExit
#if defined Troll_OnGameModeExit
forward Troll_OnGameModeExit();
#endif

#if defined _ALS_OnPlayerUpdate
#undef OnPlayerUpdate
#else
#define _ALS_OnPlayerUpdate
#endif

#define OnPlayerUpdate Troll_OnPlayerUpdate
#if defined Troll_OnPlayerUpdate
forward Troll_OnPlayerUpdate(playerid);
#endif

#if defined _ALS_OnPlayerExitVehicle
#undef OnPlayerExitVehicle
#else
#define _ALS_OnPlayerExitVehicle
#endif

#define OnPlayerExitVehicle Troll_OnPlayerExitVehicle
#if defined Troll_OnPlayerExitVehicle
forward Troll_OnPlayerExitVehicle(playerid, vehicleid);
#endif

#if defined _ALS_OnPlayerStateChange
#undef OnPlayerStateChange
#else
#define _ALS_OnPlayerStateChange
#endif

#define OnPlayerStateChange Troll_OnPlayerStateChange
#if defined Troll_OnPlayerStateChange
forward Troll_OnPlayerStateChange(playerid, newstate, oldstate);
#endif

#if defined _ALS_OnPlayerDeath
#undef OnPlayerDeath
#else
#define _ALS_OnPlayerDeath
#endif

#define OnPlayerDeath Troll_OnPlayerDeath
#if defined Troll_OnPlayerDeath
forward Troll_OnPlayerDeath(playerid, killerid, reason);
#endif

#if defined _ALS_OnPlayerDisconnect
#undef OnPlayerDisconnect
#else
#define _ALS_OnPlayerDisconnect
#endif

#define OnPlayerDisconnect Troll_OnPlayerDisconnect
#if defined Troll_OnPlayerDisconnect
forward Troll_OnPlayerDisconnect(playerid, reason);
#endif


#if defined _ALS_OnPlayerConnect
#undef OnPlayerConnect
#else
#define _ALS_OnPlayerConnect
#endif

#define OnPlayerConnect Troll_OnPlayerConnect
#if defined Troll_OnPlayerConnect
forward Troll_OnPlayerConnect(playerid);
#endif

#if defined _ALS_OnPlayerEnterVehicle
#undef OnPlayerEnterVehicle
#else
#define _ALS_OnPlayerEnterVehicle
#endif

#define OnPlayerEnterVehicle Troll_OnPlayerEnterVehicle
#if defined Troll_OnPlayerEnterVehicle
forward Troll_OnPlayerEnterVehicle(playerid, vehicleid, ispassenger);
#endif


#if defined _ALS_OnUnoccupiedVehicleUpdate
#undef OnUnoccupiedVehicleUpdate
#else
#define _ALS_OnUnoccupiedVehicleUpdate
#endif

#define OnUnoccupiedVehicleUpdate Troll_OnUnoccupiedVehicleUpdate
#if defined Troll_OnUnoccupiedVehicleUpdate
forward Troll_OnUnoccupiedVehicleUpdate(vehicleid, playerid, passenger_seat, Float:new_x, Float:new_y, Float:new_z, Float:vel_x, Float:vel_y, Float:vel_z);
#endif
