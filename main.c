#include <string.h>

#include "huffman.h"

static int getFileSize(const char* fileName)
{
	int fileSize =0;
	FILE* pFile = fopen(fileName, "rb");
	fseek(pFile, 0, SEEK_END);
	fileSize = ftell(pFile);
	fclose(pFile);

	return fileSize;
}

int main(int argc, char** argv)
{
	char* pInput = malloc(1024*1024*16);
	BitFile* pOutput = NULL;
	int debugOutputModel = 0;
	int i;
	const char* const outputFileName = "jake.bin";
	int inputSize = 0;
	int outputSize = 0;
	float ratio = 0.0f;

	for (i = 0; i < argc; i++)
	{
		if (strcmp(argv[i], "-d") == 0)
		{
			debugOutputModel = 1;
		}
	}

strcat(pInput,"ui_icon_ctf_seize""TAKE");
strcat(pInput,"ui_icon_ctf_kill""KILL");
strcat(pInput,"ui_icon_ctf_base""BASE");
strcat(pInput,"ui_icon_cs_defend""DEFEND");
strcat(pInput,"ui_icon_cs_clear""CLEAR");
strcat(pInput,"ui_icon_cs_capture""CAPTURE");
strcat(pInput,"ui_icon_ex_defend""DEFEND");
strcat(pInput,"ui_icon_ex_escort""ESCORT");
strcat(pInput,"ui_icon_ex_seize""TAKE");
strcat(pInput,"ui_icon_ex_kill""KILL");
strcat(pInput,"ui_icon_ex_extraction""EXTRACTION");
strcat(pInput,"ui_icon_pr_defend""DEFEND");
strcat(pInput,"ui_icon_pr_take""TAKE");
strcat(pInput,"ui_icon_pr_reach""REACH");
strcat(pInput,"ui_icon_pr_protect""PROTECT");
strcat(pInput,"ui_leaving_battle_area""RETURN TO BATTLE AREA");
strcat(pInput,"ui_msg_gl_visible_on_radar""VISIBLE ON RADAR");
strcat(pInput,"ui_msg_gl_become_marked_target""YOU ARE THE MARKED TARGET");
strcat(pInput,"ui_msg_hr_become_hunter""YOU ARE THE HUNTER");
strcat(pInput,"ui_msg_hr_lose_hunter""YOU HAVE JOINED THE MARINES");
strcat(pInput,"ui_msg_hr_other_becoming_hunter""%1 IS THE HUNTER");
strcat(pInput,"ui_msg_hr_status_kill_hunter""KILL THE HUNTER");
strcat(pInput,"ui_msg_hr_status_kill_marines""KILL THE MARINES");
strcat(pInput,"ui_msg_hr_disruptor_available""A SUIT DISRUPTOR IS AVAILABLE");
strcat(pInput,"ui_msg_hr_disruptor_activated""SUIT DISRUPTOR ACTIVATED");
strcat(pInput,"ui_msg_hr_disruptor_deactivated""SUIT DISRUPTOR DEACTIVATED");
strcat(pInput,"ui_msg_hr_hunter_killed""HUNTER ELIMINATED");
strcat(pInput,"ui_msg_sp_spear_captured""SPEAR CAPTURED");
strcat(pInput,"ui_msg_sp_spear_lost""SPEAR LOST");
strcat(pInput,"ui_msg_sp_initial""CAPTURE THE SPEARS");
strcat(pInput,"ui_msg_sp_enemy_capturing""OUR SPEAR IS BEING CAPTURED");
strcat(pInput,"ui_msg_sp_friendly_capturing""WE ARE CAPTURING A SPEAR");
strcat(pInput,"ui_msg_sp_no_spears""WE HAVE NO SPEARS");
strcat(pInput,"ui_msg_sp_one_spear""WE HAVE 1 SPEAR");
strcat(pInput,"ui_msg_sp_multiple_spears""WE HAVE %1 SPEARS");
strcat(pInput,"ui_msg_sp_new_spear""A NEW SPEAR IS EMERGING");
strcat(pInput,"ui_msg_sp_neutral_retract""A SPEAR IS RETRACTING");
strcat(pInput,"ui_msg_sp_friendly_retract""OUR SPEAR IS RETRACTING");
strcat(pInput,"ui_msg_sp_enemy_retract""A HOSTILE SPEAR IS RETRACTING");
strcat(pInput,"ui_msg_sp_new_spears""NEW SPEARS ARE EMERGING");
strcat(pInput,"ui_msg_sp_spears_retract""THE SPEARS ARE RETRACTING");
strcat(pInput,"ui_msg_sp_winning""WE HAVE TAKEN THE LEAD");
strcat(pInput,"ui_msg_sp_losing""WE HAVE LOST THE LEAD");
strcat(pInput,"ui_msg_sp_our_energy_20""ENERGY COLLECTION AT 20%");
strcat(pInput,"ui_msg_sp_our_energy_40""ENERGY COLLECTION AT 40%");
strcat(pInput,"ui_msg_sp_our_energy_60""ENERGY COLLECTION AT 60%");
strcat(pInput,"ui_msg_sp_our_energy_80""ENERGY COLLECTION AT 80%");
strcat(pInput,"ui_msg_sp_our_energy_90""ENERGY COLLECTION AT 90%");
strcat(pInput,"ui_msg_sp_enemy_energy_20""HOSTILE ENERGY LEVEL AT 20%");
strcat(pInput,"ui_msg_sp_enemy_energy_40""HOSTILE ENERGY LEVEL AT 40%");
strcat(pInput,"ui_msg_sp_enemy_energy_60""HOSTILE ENERGY LEVEL AT 60%");
strcat(pInput,"ui_msg_sp_enemy_energy_80""HOSTILE ENERGY LEVEL AT 80%");
strcat(pInput,"ui_msg_sp_enemy_energy_90""HOSTILE ENERGY LEVEL AT 90%");
strcat(pInput,"ui_msg_sp_capture_contribution""CAPTURE CONTRIBUTION");
strcat(pInput,"ui_msg_sp_max_suit_charged""Maximum Suit Charged");
strcat(pInput,"ui_msg_sp_max_suit_detected""Maximum Suit Detected");
strcat(pInput,"ui_msg_sp_final_spear_emerging""Final Spear Emerging");
strcat(pInput,"ui_msg_gl_friendly_waypoint""ALLIES HAVE REACHED A WAYPOINT");
strcat(pInput,"ui_msg_gl_hostile_waypoint""MARINES HAVE REACHED A WAYPOINT");
strcat(pInput,"ui_msg_gl_picked_up""OBJECTIVE PICKED UP");
strcat(pInput,"ui_msg_gl_dropped""OBJECTIVE DROPPED");
strcat(pInput,"ui_scorepopup_gladiator_survive_waypoint""SURVIVED TO A WAYPOINT");
strcat(pInput,"ui_scorepopup_gladiator_revive_allies""REACHED A WAYPOINT");
strcat(pInput,"ui_interaction_vehiclekill""%1 Hijack Pinger");
strcat(pInput,"ui_msg_pr_cant_spawn""RESPAWNING NEXT WAVE");
strcat(pInput,"ui_msg_pr_reach_waypoint""REACH THE WAYPOINT");
strcat(pInput,"ui_msg_pr_protect_carrier""PROTECT THE OBJECTIVE CARRIER");
strcat(pInput,"ui_msg_pr_collect_objective""SURVIVE");
strcat(pInput,"ui_msg_pr_kill_carrier""ELIMINATE THE OBJECTIVE CARRIER");
strcat(pInput,"ui_msg_pr_kill_soldiers""ELIMINATE THE MARINES");
strcat(pInput,"ui_msg_pr_being_targeted""BEING TARGETED");
strcat(pInput,"ui_msg_pr_start_you_are_a""You Are A");
strcat(pInput,"ui_msg_pr_start_soldier""MARINE");
strcat(pInput,"ui_msg_pr_start_predator""HUNTER");
strcat(pInput,"ui_msg_pr_attempt_failed""ATTEMPT FAILED");
strcat(pInput,"ui_msg_pr_num_retries""%1 ATTEMPTS REMAINING");
strcat(pInput,"ui_msg_pr_num_retries_1""FINAL ATTEMPT");
strcat(pInput,"ui_msg_pr_wave_defeated""WAVE DEFEATED");
strcat(pInput,"ui_msg_pr_num_waves""%1 WAVES REMAINING");
strcat(pInput,"ui_msg_pr_num_waves_1""FINAL WAVE");
strcat(pInput,"ui_msg_pr_tries""MARINES: %1");
strcat(pInput,"ui_msg_summon_gunship""[act] %1 Summon Ceph Gunship");
strcat(pInput,"mp_vote_initialized_kick""Kick vote initiated against %1");
strcat(pInput,"mp_vote_initialized_kick_update""Kick %1. %2 votes needed");
strcat(pInput,"mp_vote_initialized_kick_success""Vote ended. Kicked %1");
strcat(pInput,"mp_vote_initialized_kick_fail""Vote ended. Not enough votes");
strcat(pInput,"mp_vote_cannotvote_alreadyactive""Cannot initiate a vote because there is one in progress");
strcat(pInput,"mp_vote_cannotvote_enoughplayers""Cannot initiate a vote because there are not enough players");
strcat(pInput,"mp_vote_cannotvote_cooldown""Cannot initiate a vote because you recently initated one. Time left %1.");
strcat(pInput,"mp_vote_cannotvote_warning""WARNING");
strcat(pInput,"mp_vote_key_vote_for""YES - %1");
strcat(pInput,"mp_vote_key_vote_against""IGNORE - %1");
strcat(pInput,"mp_vote_start_kick""KICK");
strcat(pInput,"mp_vote_small_for""YES (%1)");
strcat(pInput,"mp_vote_small_against""IGNORE (%1)");
strcat(pInput,"mp_vote_small_name""KICK %1?");
strcat(pInput,"mp_vote_start_name""%1?");
strcat(pInput,"mp_vote_start_initiate""VOTE KICK INITIATED");
strcat(pInput,"mp_vote_player_kicked""Kicked %1.");
strcat(pInput,"mp_vote_success""VOTE SUCCESSFUL!");
strcat(pInput,"mp_vote_failed""VOTE FAILED!");
strcat(pInput,"menu_nav_caption_kick_vote""KICK");
strcat(pInput,"mp_prematch_warning""PRE MATCH");
strcat(pInput,"mp_prematch_playercount_warning""WAITING FOR %1 MORE PLAYERS");
strcat(pInput,"mp_prematch_oneplayer_warning""WAITING FOR 1 MORE PLAYER");
strcat(pInput,"mp_spec_killedby_title""KILLED BY");
strcat(pInput,"mp_spec_killedby_personal_battle""PERSONAL BATTLE");
strcat(pInput,"ui_msg_pred_restart_weapons_win""All Marines have been eliminated");
strcat(pInput,"ui_msg_pred_restart_weapons_lose""Failed to kill all Marines");
strcat(pInput,"ui_msg_pred_restart_time_win""Marine's have survived the round");
strcat(pInput,"ui_msg_pred_restart_time_lose""Failed to kill all Marines");
strcat(pInput,"ui_msg_pred_restart_time_limit_win""TIME LIMIT %1");
strcat(pInput,"ui_msg_pred_restart_time_limit_lose""OUT OF TIME");
strcat(pInput,"ui_msg_pred_restart_obj_win""TIME TAKEN %1");
strcat(pInput,"ui_msg_pred_restart_obj_lose""TIME SURVIVED %1");
strcat(pInput,"ui_msg_pred_restart_time_taken""TIME TAKEN %1");
strcat(pInput,"ui_msg_hunter_start_attack""Eliminate all the Marines");
strcat(pInput,"ui_msg_hunter_start_defend""Survive the Hunter attack");

	inputSize = (int)strlen(pInput);
	pOutput = binaryOpenOutputFile(outputFileName);
	compressInput(pInput, inputSize, pOutput, debugOutputModel);
	binaryCloseFile(pOutput);

	outputSize = getFileSize(outputFileName);

	ratio = (100.0f*(float)outputSize/(float)inputSize);
	printf("InputSize %d OutputSize %d Ratio %.2f Predict Size %.2fMB Saving %.2fMB\n", inputSize, outputSize, ratio, 
					(ratio*4.0f/100.0f), (4.0f-ratio*4.0f/100.0f));

	return 1;
}
