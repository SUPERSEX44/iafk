/*
  Improved afk.inc by realdiegopoptart
  fixes bugs that I discovered with the include
  Orignal Credits to Miihai

  CHANGES FOR OPEN.RP:
  - made it work for YSI foreach
  - now is compatible with the new YSI hooks
*/
#include <foreach>
#include <YSI_Coding\y_hooks>
static bool:PlayerAFK[MAX_PLAYERS]; // Sets player variable [0-NOT-AFK | 1-AFK]
static AFKDetect[MAX_PLAYERS]; // Sets current date/time to player from OnPlayerUpdate(OPU)
static bool: FunctionCall[2][MAX_PLAYERS]; // Helps to not repeat OnPlayerPause or/and OnPlayerUnpause
static playerAName[MAX_PLAYER_NAME]; //Player name
static AFKTime[MAX_PLAYERS];

//forward
forward OnPlayerPause(playerid);
forward OnPlayerUnpause(playerid);
forward AFKChecker();
forward IsPlayerPaused(playerid);
forward TimeSincePaused(playerid);
 
hook OnPlayerUpdate(playerid)
{
        AFKDetect[playerid] = gettime();
        return 1;
}
hook OnPlayerDisconnect(playerid, reason)
{
        if(PlayerAFK[playerid] == true && FunctionCall[1][playerid] == false)
        {
                GetPlayerName(playerid, playerAName,sizeof(playerAName)),
                PlayerAFK[playerid] = false;
                // printf("[resume] %s is no longer AFK because he's left the game. (%d seconds)",playerAName,gettime()-AFKTime[playerid]);
                FunctionCall[1][playerid] = true;
                FunctionCall[0][playerid] = false;
                AFKTime[playerid] = 0;
                CallLocalFunction("OnPlayerUnpause", "i", playerid);
        }
        return 1;
}
 
hook OnGameModeInit()
{
        SetTimer("AFKChecker",1000, true);
        return 1;
}

public AFKChecker()
{
    foreach(new i : Player)
    {
        if(gettime()-AFKDetect[i] > 5)
        {
            if(PlayerAFK[i] == false && FunctionCall[0][i] == false)
                GetPlayerName(i, playerAName,sizeof(playerAName)),
                PlayerAFK[i] = true,
                CallLocalFunction("OnPlayerPause", "i", i),
                //printf("[pause] %s is now AFK.",playerAName),
                FunctionCall[0][i] = true,
                FunctionCall[1][i] = false,
                AFKTime[i] = gettime();

                // check of AFK_NAMETAG is defined
                #if defined AFK_NAMETAG
                    new nametag[80];
					format(nametag, sizeof(nametag), "Paused for %d seconds", gettime()-AFKTime[i]);
					SetPlayerChatBubble(i, nametag, 0xFF0000FF, 40.0, 2000);
                #endif
        }
        else
        {
        if(PlayerAFK[i] == true && FunctionCall[1][i] == false)
            GetPlayerName(i, playerAName,sizeof(playerAName)),
            PlayerAFK[i] = false,
            CallLocalFunction("OnPlayerUnpause", "i", i),
            //printf("[resume] %s is no longer AFK. (%d seconds)",playerAName,gettime()-AFKTime[i]),
            FunctionCall[1][i] = true,
            FunctionCall[0][i] = false,
            AFKTime[i] = gettime()-AFKTime[i];
        }
    }
    return 1;
}

public IsPlayerPaused(playerid)
        if(PlayerAFK[playerid] == true) return 1;
                else return 0;
 
public TimeSincePaused(playerid)
{
        if(PlayerAFK[playerid] == true) return gettime()-AFKTime[playerid];
        else return AFKTime[playerid];
}
