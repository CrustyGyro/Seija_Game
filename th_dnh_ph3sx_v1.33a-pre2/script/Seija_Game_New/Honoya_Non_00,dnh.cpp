#TouhouDanmakufu[Single]
#ScriptVersion[3]
#Title["Honoya_Non_00"]
#Background["./Stage/STG_01/STG_BG.dnh"];
#Player[ "./Player/PL_Reimu.dnh", "./Player/EGoA_Reimu_A.dnh" ]
#System["./System/System_Main_Hud.dnh"]

//--------------------------------------------------Variables------------------------------------------------------

let bossObj;
let objScene = GetEnemyBossSceneObjectID();
let bossX = 0;
let bossY = 0;

//SetIntersectionVisualization( true );

let BossTex = GetCurrentScriptDirectory() ~ "./Resource/Textures/Boss/Test_Boss.png";
let Prism_Tex = GetCurrentScriptDirectory() ~ "./Resource/Textures/Boss/Prism.png";
let CutinTex = GetCurrentScriptDirectory() ~ "./Resource/Textures/Boss/Cutin/Hinoya.png";

//---------------------------------------------------Include-------------------------------------------------------

	#include "./lib/Index_Boss.dnh"
	#include "./lib/Lib_Spell.dnh"
	#include "./lib/Lib_SpellCutin.dnh"

//----------------------------------------------------Start--------------------------------------------------------

@Loading{
	
	LoadTexture( CutinTex );
	
	
}	

@Initialize {

	bossObj = ObjEnemy_Create(OBJ_ENEMY_BOSS);	
	
	ObjEnemy_Regist( bossObj );	

	T_Lifebar( bossObj );
	ObjEnemy_SetDamageRate( bossObj, 0, 0 );	
	//ObjPrim_SetTexture( bossObj, BossTex );
	Render_Boss_Chimi( bossObj );
	//Render_WheelGhost_A1( bossObj, ENEMY_RED );
	//ObjSprite2D_SetSourceRect( bossObj, 0, 0, 128, 128 );
	//ObjSprite2D_SetDestCenter( bossObj );
	//ObjRender_SetScaleX( bossObj, 2 );
	//ObjRender_SetScaleY( bossObj, 2);	
	//T_Render_AtInt( bossObj, Resolution );
	
	ObjRender_SetTextureFilter( bossObj, FILTER_POINT, FILTER_POINT, FILTER_POINT );
	
	ObjMove_SetDestAtWeight( bossObj, GetStgFrameWidth/2, Get_CenterY/2 + 16, 15, 8 );
	
	
	SetPlayerLife( 8 );
	T_Main;
	T_End;
	
}

@Event { 

	alternative(GetEventType()) 
	case( EV_REQUEST_LIFE ) { 
		SetScriptResult( 3800 ); // 2500
	}
	case( EV_REQUEST_TIMER ) { 
		SetScriptResult( 45 );
	}	


} 

@MainLoop {

	bossX = ObjMove_GetX( bossObj );
	bossY = ObjMove_GetY( bossObj );

	ObjEnemy_SetIntersectionCircleToShot( bossObj, bossX, bossY, 16 );
	ObjEnemy_SetIntersectionCircleToPlayer( bossObj, bossX, bossY, 8 );
	
	yield;
}

@Finalize {
} 

//----------------------------------------------------Patterns--------------------------------------------------------

task T_Main {

	
	float ShotAngle = 0;
	float Radius = 64;
	float WaitTime = 10;
	float Dir = 3;
	float Wvel = 2;
	
	float X = 0;
	float Y = 0;
	
	NotifyEventAll( EV_BOSS_EFFECT, bossObj, CC_RED );
	
	//let Color = [ DOT_GRAY, DOT_RED, DOT_YELLOW, DOT_CYAN, DOT_PURPLE, DOT_GREEN ];

	yield;

	wait( 45 );	
	
	ObjEnemy_SetDamageRate( bossObj, 1, 1 );
	//ObjEnemyBossScene_StartSpell( objScene );
	
	//T_CallSpell_A1( "Fire Sign \"Fugue of Chrysanthemum and Peony\"", 0, 1, objScene );
	//T_SpellBG_Hinoya();
	//T_Call_Spell_Cutin( CutinTex, [ 0, 0, 969, 1084 ], 0.4, CC_ORANGE );
	
	//wait( 200 );	
		
	
	while( Boss_Is_Alive( bossObj )){
		
		T_PlaySE( SE_CHARGE_00, 30 );
		T_Charge_A1( bossObj, 20, "Petal", CC_YELLOW );	
		
		wait( 60 );
		
		
		T_Boss_Shot_A( 45, 2, SHOT_ORB_YELLOW, 60 );
		//descent( i in 0..4 ){ T_Boss_Shot_A(  bossX + 32 + i * bossX/4, bossY + 144 - i*24, 60, 64*i/1.5 );  }
		//descent( i in 0..4 ){ T_Boss_Shot_A(  bossX - 32 - i * bossX/4, bossY + 144 - i*24, 60, 64*i/1.5 ); }

		//descent( i in 0..4 ){ T_Boss_Shot_A(  bossX - 48 - i * bossX/4, bossY + 144 - i*24, 60, 64 ); }		

		wait( 60 );
		
		ObjMove_SetDestAtWeight( bossObj, Get_CenterX + rand_int( -32, 32 ), Get_MaxY/3 + rand_int( -20, 20), 15, 1 );

		//wait( 200 );
		
		//T_Charge_A1( bossObj, 20, "Petal", CC_CYAN );	
		
		//wait( 20 );		
		
		//T_Boss_Shot_B();
		
		wait( 300 );
		
	}	
	
	/*
	while(Boss_Is_Alive( bossObj )){
		
		//T_PlaySE( SE_CHARGE_00, 30 );
		//T_Charge_A1( bossObj, 20, "Petal", CC_YELLOW );	
		
		T_Boss_Shot_A( bossX + rand( -256, 256 ), bossY + 128 , 60, 64 );
		
		wait( 100 );
		
	}		
	*/
}	

task T_Boss_Shot_A( Angle, Speed, Color, Delay ){
	
	//let Color_Array = [ BALL_S_RED, BALL_S_RED, BALL_S_RED, BALL_M_RED ];
	
	//int Way = 1;
	//int Stack = 1;
	
	T_PlaySE( SE_CHIME_02, 35 );
	
	let Firework_A = ShotA1( bossX, bossY Speed, Angle, Color, 30 );
	ObjMove_AddPatternC2( Firework_A, Delay, NO_CHANGE, NO_CHANGE, NO_CHANGE, 0.05, 0, -4, 0, 0  ); 
	
	wait( Delay );
	
	T_PlaySE( SE_FIREWORKS_01, 30 );	
	

}	

//-----------------------------------------------------Death---------------------------------------------------------

task T_End {
	
    while( ObjEnemy_GetInfo( bossObj, INFO_LIFE ) > 0 ) {
	     yield;
	}
	
	if( ObjEnemyBossScene_GetInfo( GetEnemyBossSceneObjectID, INFO_IS_LAST_STEP ) ){

		NotifyEventAll( EV_CLEAR_ENEMY_SHOTS, GetPlayerObjectID(), 60, 12 );
		//NotifyEventAll( EV_CLEAR_ENEMIES ); // WIll add proper functionality to this later - Seija
		//NotifyEventAll( EV_CLEAR_ENEMY_SHOTS, bossObj, 60, 14 );
		
		yield;
		
		SetAutoDeleteObject(true);
		
		//wait( 60 );
		
		Obj_Delete( bossObj );	
		
		ENEMY_CLEAR = true;
		
		yield;
		
		ENEMY_CLEAR = false;
		
		wait( 360 );
		
		CloseScript( GetOwnScriptID() );
		
	}	
	else{
	
		//DeleteShotAll( TYPE_ALL, TYPE_ITEM );
		
		yield;
		
		SetAutoDeleteObject(true);
		
		Obj_Delete( bossObj );	
		
		wait( 200 );
		
		CloseScript( GetOwnScriptID() );
	
	}
	
	//SetAutoDeleteObject(true);
	
	//Obj_Delete( bossObj );	
	//CloseScript( GetOwnScriptID() );
	
}

