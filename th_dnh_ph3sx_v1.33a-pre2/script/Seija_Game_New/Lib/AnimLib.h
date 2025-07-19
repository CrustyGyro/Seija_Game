

let ANIM_IDLE = "Idle";
let ANIM_MOVE = "Move";
let ANIM_MOVE_RIGHT = "MoveRight";
let ANIM_MOVE_LEFT = "MoveLeft";
let ANIM_MOVEEND = "MoveToIdle";
let ANIM_MOVEEND_RIGHT = "MoveRightToIdle";
let ANIM_MOVEEND_LEFT = "MoveLeftToIdle";
let ANIM_ATTACK = "Attack";
let ANIM_ATTACKEND = "AttackToIdle";

let ANIM_STANCE_NAME_DEFAULT = "Default";

let fl = false;

//let ANIM_ATTACK_VER = "";

let Xro = 0;
let Yro = 0;



function ObjAnim_SetAnimationPauseA1(objID, animset, ispause){
	Obj_SetValue(objID, "anim_"~animset, ispause);
}

function ObjAnim_SetRectOffsetX(off){
	Xro = off;
}
function ObjAnim_SetRectOffsetY(off){
	Yro = off;
}


function ObjAnim_AddFrameA1(objID, type, left, top, right, bot){
	let aarr = Obj_GetValueD(objID, "anim_"~type, []);
	Obj_SetValue(objID, "anim_"~type, aarr~[[left, top, right, bot, 0, 0]]);
}

function ObjAnim_AddFrameA2(objID, type, left, top, right, bot, xoff, yoff){
	let aarr = Obj_GetValueD(objID, "anim_"~type, []);
	Obj_SetValue(objID, "anim_"~type, aarr~[[left, top, right, bot, xoff, yoff]]);
}

function ObjAnim_SetLoopPointA1(objID, type, frame){
	Obj_SetValue(objID, "anim_"~type~"_loop", frame);
}

function ObjAnim_SetSpeedA1(objID, type, speed){
	Obj_SetValue(objID, "anim_"~type~"_speed", speed);
}

function ObjAnim_FlipMoveImage(objID, cond){
	fl = cond;
}

function ObjAnim_SetNoMoveAnim(ObjID, cond){
	Obj_SetValue(ObjID, "anim_NoMoveAnim", cond);
}

task ObjAnim_ForceStop(objID){
	Obj_SetValue(objID, "anim_forcestop", true);
	yield;
	Obj_SetValue(objID, "anim_forcestop", false);
	
	
}

function ObjAnim_SetAttackA1(objID, status){
	CheckAnimFrameDataStatus(objID);
	Obj_SetValue(objID, "anim_playattack", status);
}

task ObjAnim_SetAttackA2(objID, len){
	CheckAnimFrameDataStatus(objID);
	Obj_SetValue(objID, "anim_playattack", true);
	loop(len){wait(1);}
	Obj_SetValue(objID, "anim_playattack", false);
}

task ObjAnim_SetAttackB2(objID, len, status){
	CheckAnimFrameDataStatus(objID);
	loop(len){wait(1);}
	Obj_SetValue(objID, "anim_playattack", status);
}

function ObjAnim_SetAttackVersionA1(objID, ver){
	Obj_SetValue(objID, "anim_attack_modded_ver", ver);
}

function CheckAnimFrameDataStatus(objID){
	if(Obj_IsDeleted(objID)){return;}
	DidUserSetFrames(objID, ANIM_ATTACK, "Attack");
	DidUserSetFrames(objID, ANIM_ATTACKEND, "Attack To Idle (ATTACKEND)");
	if(!Obj_IsValueExists(objID, "anim_Attack_loop")){
		//RaiseError("Did not specify loop value for attack animation");
	}
}

function ObjAnim_IsAttackPlaying(objID){
	return Obj_GetValueD(objID, "anim_isattackplaying", false);
}

task ObjAnim_StartAnimationA1(objID, bob, bobspeed){
	ObjAnim_StartAnimationFA1(objID, "anim_"~"Default", bob, bobspeed);
}
task ObjAnim_StartAnimationA2(objID, bob, bobspeed){
	ObjAnim_StartAnimationFA2(objID, "anim_"~"Default", bob, bobspeed);
}

task ObjAnim_StartAnimationFA1(objID, animset, bob, bobspeed){
	DidUserSetFrames(objID, ANIM_IDLE, "Idle");
	DidUserSetFrames(objID, ANIM_MOVE, "Move");
	DidUserSetFrames(objID, ANIM_MOVEEND, "Move To Idle (MOVEEND)");
	DidUserSetValidLoop(objID, ANIM_IDLE, "");
	DidUserSetValidLoop(objID, ANIM_MOVE, "");
	if(Obj_IsValueExists(objID, "anim_Attack_loop")){DidUserSetValidLoop(objID, ANIM_ATTACK, "");}
	
	let anim_count = 0;
	let anim_type = 0;
	let anim_rend = 0;
	
	let idle = Obj_GetValueD(objID, "anim_"~ANIM_IDLE~"_speed", 3);
	let move = Obj_GetValueD(objID, "anim_"~ANIM_MOVE~"_speed", 4);
	let attack = Obj_GetValueD(objID, "anim_"~ANIM_ATTACK~"_speed", 6);
	
	let AnimAttack = false;
	
	let x = 0;
	let y = 0;
	let fr = [];
	
	let m = 0;
	
	Obj_SetValue(objID, "anim_isattackplaying", false);
	let ANIM_ATTACK_VER = "";

	while(!Obj_IsDeleted(objID)&&!Obj_GetValueD(objID, "anim_forcestop", false)){
		// while(!Obj_IsDeleted(objID)&&Obj_GetValueD(objID, "anim_"~animset, false)){anim_count = 0;yield;}
		ANIM_ATTACK_VER = Obj_GetValueD(objID, "anim_attack_modded_ver", "");
		AnimAttack = Obj_GetValueD(objID, "anim_playattack", false);
		if(!AnimAttack){
			if(ObjMove_GetSpeed(objID) == 0){
				if(anim_type == 1){
					anim_count = 0;
					anim_type = 2;
				}
			}
			if(ObjMove_GetSpeed(objID) > 0){
				if(!Obj_GetValueD(objID, "anim_NoMoveAnim", false)){
					if(anim_type == 0){
						anim_count = 0;
						anim_type = 1;
					}
				}
			}
			if(anim_type == 3){
				anim_type=4;
				anim_count = 0;
			}
		}else{
			Obj_SetValue(objID, "anim_isattackplaying", true);
			if(anim_type != 3){
				anim_type=3;
				anim_count = 0;
			}
		}
		
		if(CheckDirectionAL1(objID, "right")){ObjRender_SetScaleXYZ(objID, 1-2*fl, 1, 1);}
		if(CheckDirectionAL1(objID, "left")){ObjRender_SetScaleXYZ(objID, -1+2*fl, 1, 1);}
		
			
		if(anim_type==1){ //Moving
			anim_count+=1/move;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_MOVE))-1){anim_count = Obj_GetValue(objID, "anim_"~ANIM_MOVE~"_loop");}
			anim_rend = floor(anim_count);
			fr = EmergencyCheck(ANIM_MOVE);
		}
		if(anim_type==2){ //Moving to idle
			anim_count+=1/move;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_MOVEEND))-1){anim_count = 0;anim_type=0;}
			anim_rend = floor(anim_count);
			fr = EmergencyCheck(ANIM_MOVEEND);
		}
		if(anim_type==3){ //Attack
			anim_count+=1/attack;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_ATTACK~ANIM_ATTACK_VER))-1){anim_count = Obj_GetValueD(objID, "anim_"~ANIM_ATTACK~ANIM_ATTACK_VER~"_loop", 0);}
			anim_rend = floor(anim_count);
			fr = EmergencyCheck(ANIM_ATTACK~ANIM_ATTACK_VER);
		}
		if(anim_type==4){ //Attack to idle
			anim_count+=1/attack;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_ATTACKEND~ANIM_ATTACK_VER))-1){anim_count = 0;anim_type=0;Obj_SetValue(objID, "anim_isattackplaying", false);}
			anim_rend = floor(anim_count);
			fr = EmergencyCheck(ANIM_ATTACKEND~ANIM_ATTACK_VER);
		}
		if(anim_type==0){ //Idle
			anim_count+=1/idle;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_IDLE))-1){anim_count = Obj_GetValueD(objID, "anim_"~ANIM_IDLE~"_loop", 0);}
			anim_rend = floor(anim_count);
			fr = EmergencyCheck(ANIM_IDLE);
		}
		x = fr[2]-fr[0];
		y = fr[3]-fr[1];
		
		m+=bobspeed;
		ObjSprite2D_SetSourceRect(objID, fr[0]+Xro, fr[1]+Yro, fr[2]+Xro, fr[3]+Yro);
		ObjSprite2D_SetDestRect(objID, -x/2+fr[4]/2, -y/2+(bob*sin(m))+fr[5]/2, x/2+fr[4]/2, y/2+(bob*sin(m))+fr[5]/2);
		wait(1);
		while(Obj_GetValueD(objID, "anim_"~animset, false)){anim_count = 0;wait(1);}
	}
	function EmergencyCheck(container){
		if(Obj_IsDeleted(objID)){return;}
		return Obj_GetValue(objID, "anim_"~container)[anim_rend];
	}	
}

task ObjAnim_StartAnimationFA2(objID, animset, bob, bobspeed){
	DidUserSetFrames(objID, ANIM_IDLE, "Idle");
	DidUserSetFrames(objID, ANIM_MOVE_RIGHT, "Move (right)");
	DidUserSetFrames(objID, ANIM_MOVE_LEFT, "Move (left)");
	DidUserSetFrames(objID, ANIM_MOVEEND_RIGHT, "Move To Idle (MOVEEND_RIGHT)");
	DidUserSetFrames(objID, ANIM_MOVEEND_LEFT, "Move To Idle (MOVEEND_LEFT)");
	DidUserSetValidLoop(objID, ANIM_IDLE, "");
	DidUserSetValidLoop(objID, ANIM_MOVE_RIGHT, " (right)");
	DidUserSetValidLoop(objID, ANIM_MOVE_LEFT, " (left)");
	if(Obj_IsValueExists(objID, "anim_Attack_loop")){DidUserSetValidLoop(objID, ANIM_ATTACK, "");}
	
	let anim_count = 0;
	let anim_type = 0;
	let anim_rend = 0;
	
	let idle = Obj_GetValueD(objID, "anim_"~ANIM_IDLE~"_speed", 3);
	let mover = Obj_GetValueD(objID, "anim_"~ANIM_MOVE_RIGHT~"_speed", 4);
	let movel = Obj_GetValueD(objID, "anim_"~ANIM_MOVE_LEFT~"_speed", 4);
	let attack = Obj_GetValueD(objID, "anim_"~ANIM_ATTACK~"_speed", 6);
	
	let AnimAttack = false;
	
	let x = 0;
	let y = 0;
	let fr = [];
	
	let m = 0;
	let LastDirection;
			
	Obj_SetValue(objID, "anim_isattackplaying", false);
	
	let ANIM_ATTACK_VER = "";
	
	while(!Obj_IsDeleted(objID)&&!Obj_GetValueD(objID, "anim_forcestop", false)){
		// while(!Obj_IsDeleted(objID)&&Obj_GetValueD(objID, "anim_"~animset, false)){yield;}
		
		ANIM_ATTACK_VER = Obj_GetValueD(objID, "anim_attack_modded_ver", "");
		AnimAttack = Obj_GetValueD(objID, "anim_playattack", false);
		if(!AnimAttack){
			if(ObjMove_GetSpeed(objID) == 0){
				if(anim_type == 1){
					anim_count = 0;
					anim_type = 2;
				}
			}
			if(ObjMove_GetSpeed(objID) > 0){
				if(!Obj_GetValueD(objID, "anim_NoMoveAnim", false)){
					if(anim_type == 0){
						anim_count = 0;
						anim_type = 1;
					}
				}
			}
			if(anim_type == 3){
				anim_type=4;
				anim_count = 0;
			}
		}else{
			Obj_SetValue(objID, "anim_isattackplaying", true);
			if(anim_type != 3){
				anim_type=3;
				anim_count = 0;
			}
		}
		
		//ObjRender_SetScaleXYZ(objID, 1-2*fl, 1, 1);
		if(CheckDirectionAL1(objID, "right")){LastDirection = "Right";}
		if(CheckDirectionAL1(objID, "left")){LastDirection = "Left";}
		
			
		if(anim_type==1){ //Moving
			if(LastDirection == "Right"){
				anim_count+=1/mover;
				if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_MOVE_RIGHT))-1){anim_count = Obj_GetValue(objID, "anim_"~ANIM_MOVE_RIGHT~"_loop");}
				anim_rend = floor(anim_count);
				fr = EmergencyCheck(ANIM_MOVE_RIGHT);
			}
			else if(LastDirection == "Left"){
				anim_count+=1/movel;
				if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_MOVE_LEFT))-1){anim_count = Obj_GetValue(objID, "anim_"~ANIM_MOVE_LEFT~"_loop");}
				anim_rend = floor(anim_count);
				fr = EmergencyCheck(ANIM_MOVE_LEFT);
			}
			else{RaiseError("An error occurred when animating. If you cannot resolve the issue, please send the affected script to gtbot so he may fix it.");}
		}
		if(anim_type==2){ //Moving to idle
			if(LastDirection == "Right"){
				anim_count+=1/mover;
				if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_MOVEEND_RIGHT))-1){anim_count = 0;anim_type=0;}
				anim_rend = floor(anim_count);
				fr = EmergencyCheck(ANIM_MOVEEND_RIGHT);
			}
			else if(LastDirection == "Left"){
				anim_count+=1/movel;
				if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_MOVEEND_LEFT))-1){anim_count = 0;anim_type=0;}
				anim_rend = floor(anim_count);
				fr = EmergencyCheck(ANIM_MOVEEND_LEFT);
			}
			else{RaiseError("An error occurred when animating. If you cannot resolve the issue, please send the affected script to gtbot so he may fix it.");}
		}
		if(anim_type==3){ //Attack
			anim_count+=1/Obj_GetValueD(objID, "anim_"~ANIM_ATTACK~ANIM_ATTACK_VER~"_speed", 6);
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_ATTACK~ANIM_ATTACK_VER))-1){anim_count = Obj_GetValueD(objID, "anim_"~ANIM_ATTACK~ANIM_ATTACK_VER~"_loop", 0);}
			anim_rend = floor(anim_count);
			fr = EmergencyCheck(ANIM_ATTACK~ANIM_ATTACK_VER);
		}
		if(anim_type==4){ //Attack to idle
			anim_count+=1/Obj_GetValueD(objID, "anim_"~ANIM_ATTACK~ANIM_ATTACK_VER~"_speed", 6);;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_ATTACKEND~ANIM_ATTACK_VER))-1){anim_count = 0;anim_type=0;Obj_SetValue(objID, "anim_isattackplaying", false);}
			anim_rend = floor(anim_count);
			fr = EmergencyCheck(ANIM_ATTACKEND~ANIM_ATTACK_VER);
		}
		if(anim_type==0){ //Idle
			anim_count+=1/idle;
			if(floor(anim_count)>length(Obj_GetValue(objID, "anim_"~ANIM_IDLE))-1){anim_count = Obj_GetValueD(objID, "anim_"~ANIM_IDLE~"_loop", 0);}
			anim_rend = floor(anim_count);
			fr = EmergencyCheck(ANIM_IDLE);
		}
		x = fr[2]-fr[0];
		y = fr[3]-fr[1];
		
		m+=bobspeed;
		ObjSprite2D_SetSourceRect(objID, fr[0]+Xro, fr[1]+Yro, fr[2]+Xro, fr[3]+Yro);
		Obj_SetValue(objID, "AnimRect", [fr[0]+Xro, fr[1]+Yro, fr[2]+Xro, fr[3]+Yro]);
		ObjSprite2D_SetDestRect(objID, -x/2+fr[4]/2, -y/2+(bob*sin(m))+fr[5]/2, x/2+fr[4]/2, y/2+(bob*sin(m))+fr[5]/2);
		wait(1);
		while(Obj_GetValueD(objID, "anim_"~animset, false)){wait(1);}
	}
	function EmergencyCheck(container){
		if(Obj_IsDeleted(objID)){return;}
		return Obj_GetValue(objID, "anim_"~container)[anim_rend];
	}	
}



function GetAnimationFrames(objID, type){
	return Obj_GetValue(objID, "anim_"~type);
}

function DidUserSetFrames(objID, type, str){
	if(!(length(Obj_GetValueD(objID, "anim_"~type, []))>0)){
		RaiseError("Did not specify frames for "~str~" Animation");
	}
}

function DidUserSetValidLoop(objID, type, str){
	if(Obj_IsDeleted(objID)){return;}
	if(!Obj_IsValueExists(objID, "anim_"~type)){
		RaiseError("Did not specify loop value for movement ("~str~") animation");
	}
	let loopvar = Obj_GetValueD(objID, "anim_"~type~"_loop", 0);
	let loopnum = length(Obj_GetValue(objID, "anim_"~type))-1;
	if(loopvar>loopnum){
		RaiseError("Specified loop value of "~IntToString(loopvar)~" for anim_"~type~"_loop "~str~" exceeds total number of frame indices: "~IntToString(loopnum));
	}
	else if(loopvar<0){
		RaiseError("Specified loop value of "~IntToString(loopvar)~" for anim_"~type~"_loop "~str~" is invalid: cannot be less than 0");
	}
}

function CheckDirectionAL1(checkid, direc){
	let direction = false;
	let enemyang = absolute(ObjMove_GetAngle(checkid));
	let enemyspd = absolute(ObjMove_GetSpeed(checkid));
	
	
	while(enemyang>360){
		enemyang = enemyang-360;
	}

	if(enemyspd > 0){
		if(direc=="right"){
			if(enemyang > 270 || enemyang-270 < 90){
				direction=true;
			}
		}
		if(direc=="left"){
			if(enemyang < 270 && enemyang > 90){
				direction=true;
			}
		}
	}
	if(direc=="none"){
		if(enemyspd == 0){
			direction=true;
		}
	}
	
	return direction;
}

function Animate(anim_count){
	return max(0, floor(anim_count));
}

