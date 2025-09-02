#TouhouDanmakufu[Stage]
#Title["FW_Stage_5_Regular"]
#Text["Peenid"]
#ScriptVersion[3]
#Player[ "./../../Player/PL_Reimu.dnh" ]
#System["./../../System/System_Main_Hud.dnh"]

//--------------------------------------------------------------------------------------------------------------------------------------------

#include "./../../System/Common.dnh"
#include "./../../System/Constants.dnh"
#include "./../../Lib/Lib_StageBg.dnh"

//--------------------------------------------------------------------------------------------------------------------------------------------

// Stage Textures

//TODO: Make a proper loading scene for these textures and the meshes.

//let Tex_Water_00 = GetCurrentScriptDirectory() ~ "./../../Resource/Textures/Stage/01/Water_00.png";
let Tex_Vine_Left = GetCurrentScriptDirectory() ~ "./../../Resource/Textures/Stage/STG_05/FW_Vine_L.png";
let Tex_Vine_Right = GetCurrentScriptDirectory() ~ "./../../Resource/Textures/Stage/STG_05/FW_Vine_R.png";
let Tex_Blue = GetCurrentScriptDirectory() ~ "./../../Resource/Textures/Stage/STG_04/FW_Wall_Blue.png";
let Tex_Gradient = GetCurrentScriptDirectory() ~ "./../../Resource/Textures/Stage/Gradient.png";
let Tex_Green = GetCurrentScriptDirectory() ~ "./../../Resource/Textures/Stage/STG_04/FW_Wall_Green.png";
let Tex_Yellow = GetCurrentScriptDirectory() ~ "./../../Resource/Textures/Stage/STG_04/FW_Wall_Yellow.png";

let Tex_Red_Stream = GetCurrentScriptDirectory() ~ "./../../Resource/Textures/Stage/STG_04/FW_Wall_Red_Stream.png";
let Tex_Blue_Stream = GetCurrentScriptDirectory() ~ "./../../Resource/Textures/Stage/STG_04/FW_Wall_Blue_Stream.png";
let Tex_Green_Stream = GetCurrentScriptDirectory() ~ "./../../Resource/Textures/Stage/STG_04/FW_Wall_Green_Stream.png";
let Tex_Yellow_Stream = GetCurrentScriptDirectory() ~ "./../../Resource/Textures/Stage/STG_04/FW_Wall_Yellow_Stream.png";
let Tex_Ground_00 = GetCurrentScriptDirectory() ~ "./../../Resource/Mesh/Ground_00.png";

let Tex_Particle_00 = GetCurrentScriptDirectory() ~ "./../../Resource/Textures/Effect/Particle_00.png";

//--------------------------------------------------------------------------------------------------------------------------------------------

//STAGE_PHASE = 0;

float Camera_Speed = 1; // Camera does not actually move besides the little bobbing animation, this is for scaling the texture scrolling.

float Cam_X = 0;
float Cam_Y = 0;
float Cam_Z = 0;

//--------------------------------------------------------------------------------------------------------------------------------------------

@Loading {
	
    LoadTexture( Tex_Vine_Left );
	LoadTexture( Tex_Blue );
	LoadTexture( Tex_Green );
	LoadTexture( Tex_Yellow );

}


@Initialize{
    
	SetCommonData( "BG_Phase", 1 );
	T_BuildStage();
	T_BG_Phase();
	
}

@MainLoop{ 
	yield; 
}

@Finalize{ }

task T_BG_Phase(){
	
	int Current_BG_Phase = 0;
	
	while( true ){
		
		while( GetCommonData( "BG_Phase", 0 ) == Current_BG_Phase ){ yield; } 
		Current_BG_Phase = GetCommonData( "BG_Phase", 0);
		
		alternative( Current_BG_Phase )
		case( 1 ){
			//Initial Stage Reveal
			T_Cam_Phase_1();
			T_Fog_Phase_1();
		}
		case( 2 ){
			//Initial Stage Reveal
			//T_Cam_Phase_1();
			T_Fog_Phase_2();
		}		
		yield;	
	}
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------

task T_BuildStage{

	int Render_P = 20;
	
	int Cliff_Tiling = 1;
	float Cliff_Speed = -1;
	
	int BG_Color = Render_3D_Sprite_A1( Tex_Blue, [ 0, 0, 512, 512 ], 0, 0, 1900, Render_P, [ 5, 5, 5 ] );


	descent( i in 0..8 ){
		T_Repeating_Vines_A( Tex_Vine_Left, -132, 256, i, 255, -1, 700, 6, 4 );
		T_Repeating_Vines_A( Tex_Vine_Right, -100, 0, i - 0.5, 255, 1, 700, 6, -6 );
	}

	descent( i in 0..8 ){
		T_Repeating_Vines_A( Tex_Vine_Right, 100, 0, i, 255, -1, 700, 6, 6 );
		T_Repeating_Vines_A( Tex_Vine_Left, 132, 256, i - 0.5, 255, 1, 700, 6, -4 );
	}	
		
	
	int BG_Color_Gradient = Render_3D_Sprite_A1( Tex_Gradient, [ 0, 0, 512, 512 ], 0, 0, 900, Render_P + 2, [ 2.2, 2.2, 2.2 ] );
	ObjRender_SetAngleZ( BG_Color_Gradient, 180 );
	//ObjRender_SetColor( BG_Color_Gradient, 60, 125, 125 );
	//ObjRender_SetColor( BG_Color_Gradient, 0, 0, 0 );
	//ObjRender_SetColor( BG_Color_Gradient, 25, 0, 0 );
	ObjRender_SetAlpha( BG_Color_Gradient, 25 );
	
	//T_Scroll_A1( Cliff_B, [ 0, 0, 512*Cliff_Tiling, 512*Cliff_Tiling ], Cliff_Speed, Camera_Speed );
	
	yield;
	
	
	while( GetCommonData( "BG_Phase", 0 ) < 3 ){ 
	
		//T_Mist( 128, rand( -100, 32 ), rand( 48, 128 ), 24, BLEND_SUBTRACT );
		
		ascent( i in 0..16 ){
			
			T_Particles;	
			//yield;
			
		}	
		
		
		wait( 60 );
		yield; 
	}
	
	//Obj_Delete( Water_A );
	//Obj_Delete( Water_B );

	//SetCommonData( "BG_Phase", 1 );

	//----------------------------------------------------------------------------------------------------
	
	//----------------------------------------------------------------------------------------------------
	
	
	task T_Particles{
		
		float Frame = 0;
		
		let Particle_Obj = ObjPrim_Create( OBJ_SPRITE_3D );
		ObjPrim_SetTexture( Particle_Obj, Tex_Particle_00 );
		ObjSprite3D_SetSourceDestRect( Particle_Obj, 0, 0, 64, 64 );
		ObjRender_SetBlendType( Particle_Obj, BLEND_ADD_ARGB );
		
		Obj_SetRenderPriorityI( Particle_Obj, 24);
		ObjRender_SetAngleZ( Particle_Obj, 180 );
		
		ObjRender_SetX( Particle_Obj, prand( -32, 32 ) );	
		ObjRender_SetY( Particle_Obj, prand( -256, 256 ) );
		ObjRender_SetZ( Particle_Obj, prand( 128, 256 ) );
		
		let Angle_X = rand( -2,2 );
		let Angle_Y = rand( -2,2 );
		let Angle_Z = prand( 0,360 );
		let Scale_RNG = 0.05;
		
		T_ManageParticle();

		ObjRender_SetColor( Particle_Obj, 190, 50, 50 );
		ObjRender_SetScaleXYZ( Particle_Obj, Scale_RNG, Scale_RNG, Scale_RNG );
		ObjSprite3D_SetBillboard( Particle_Obj, true );	

		//ObjRender_SetScale( Particle_Obj, rand( 0.5, 1.5 ) );
		
		while( !Obj_IsDeleted( Particle_Obj )){
			
			//ObjRender_SetX( Particle_Obj, ObjRender_GetX( Particle_Obj ) + 0.4 * sin( Frame*5 ) );
			ObjRender_SetZ( Particle_Obj, ObjRender_GetZ( Particle_Obj ) - 1.3 );
			ObjRender_SetY( Particle_Obj, ObjRender_GetY( Particle_Obj ) + 0.1 );
			//ObjRender_SetAngleX( Particle_Obj, ObjRender_GetAngleX( Particle_Obj ) + Angle_X);
			//ObjRender_SetAngleY( Particle_Obj, ObjRender_GetAngleY( Particle_Obj ) + Angle_Y);
			ObjRender_SetAngleZ( Particle_Obj, Angle_Z );
			//ObjRender_SetScaleXYZ( Particle_Obj, Scale_RNG - 1/200, Scale_RNG - 1/200 , Scale_RNG - 1/200  );
			
			//Angle_X += 0.8;
			//Angle_Y += 0.8;
			Angle_Z += 1;
			Frame++;
		
			yield;
		
		}
		
		task T_ManageParticle(){
			
			T_FadeIn_A1( Particle_Obj, 160, 255 );
			
			wait( 100 );
			
			T_FadeOut_Delete( Particle_Obj, prand( 60, 100 ), 255 );
			
		}	
		
		//Obj_Delete( Particle_Obj );
		
	}		
		
	//----------------------------------------------------------------------------------------------------
	
}

task T_Repeating_Vines_A( Texture, X, Y, Z, Alpha, Scale, Gap, Tiling, Tilt ){
	
	let Obj =  Render_3D_Sprite_A1( Texture, [ 0, 0, 512, 512 * Tiling ], X, Y, Z, 22, [ Scale, Scale, Scale ]);
	ObjRender_SetAngleXYZ( Obj, 0, 0, 180 + Tilt );	
	
	//ObjRender_SetColor( Obj, Color[0], Color[1], Color[2] );
	ObjRender_SetAlpha( Obj, Alpha );

	//ObjRender_SetFogEnable( Obj, false );
	//ObjSprite3D_SetBillboard( Obj, true);
	//ObjRender_SetTextureFilter( Obj, FILTER_ANISOTROPIC, FILTER_ANISOTROPIC, FILTER_ANISOTROPIC );
	
	float pos = 0;
	
	loop{
		if(pos >= Gap){pos = 0;}
		
		ObjRender_SetZ(Obj, Z*Gap - pos);

		pos+= 2 *Camera_Speed;
		
		yield;
	}	
		
}	

//----------------------------------------------------------------------

task T_Fog_Phase_1{
	
	float Reveal_Time = 500;
	
	// ---- Phase 0; ----
	// Transition from BG phase 0 to Phase 1.
	
	let Fog_Color_Initial = [ 10, 0, 10 ];
	//let Fog_Color_Reveal =[ 255, 60, 60 ];
	let Fog_Color_Reveal =[ 60, 125, 125 ];
	
	let Fog_Range_Inital = [ 1, 200 ];
	let Fog_Range_Reveal = [ 512, 2000 ];	
	
	float Current_BG_Phase = 0;
	
	// --------------------
	
	SetFogParam( Fog_Range_Inital[0], Fog_Range_Inital[1], Fog_Color_Initial[0], Fog_Color_Initial[1], Fog_Color_Initial[2] );
	
	ascent( time in 0..Reveal_Time ){
		
		float[] Color = Interpolate_Smoother( Fog_Color_Initial, Fog_Color_Reveal, time/Reveal_Time );
		float[] Range = Interpolate_Smoother( Fog_Range_Inital, Fog_Range_Reveal, time/Reveal_Time );		
		
		
		SetFogParam( Range[0], Range[1], Color[0], Color[1], Color[2] );
		
		yield;
		
	}	
	
	wait( 200 );
	SetCommonData( "BG_Phase", 2 );	
	//SetCommonData( "BG_Phase", 2 );
	
}	

task T_Fog_Phase_2{
	
	float Reveal_Time = 500;
	
	// ---- Phase 0; ----
	// Transition from BG phase 0 to Phase 1.
	
	let Fog_Color_Initial = [ 60, 125, 125];
	let Fog_Color_Reveal =[ 255, 60, 60 ];
	//let Fog_Color_Reveal =[ 60, 125, 125 ];
	
	let Fog_Range_Inital = [ 1, 200 ];
	let Fog_Range_Reveal = [ 512, 2000 ];	
	
	float Current_BG_Phase = 0;
	
	// --------------------
	
	SetFogParam( Fog_Range_Inital[0], Fog_Range_Inital[1], Fog_Color_Initial[0], Fog_Color_Initial[1], Fog_Color_Initial[2] );
	
	ascent( time in 0..Reveal_Time ){
		
		float[] Color = Interpolate_Smoother( Fog_Color_Initial, Fog_Color_Reveal, time/Reveal_Time );
		//float[] Range = Interpolate_Smoother( Fog_Range_Inital, Fog_Range_Reveal, time/Reveal_Time );		
		
		
		SetFogParam( 512, 2000, Color[0], Color[1], Color[2] );
		
		yield;
		
	}	
	
//	wait( 200 );
//	SetCommonData( "BG_Phase", 2 );	
	//SetCommonData( "BG_Phase", 2 );
	
}

//----------------------------------------------------------------------

task T_Cam_Phase_1{
	
	Cam_X = 0;
	Cam_Y = 0;
	Cam_Z = 0;	
	
	SetCameraPerspectiveClip( -1, 2000 );
	SetCameraPosEye( Cam_X, Cam_Y, Cam_Z );
	SetCameraAzimuthAngle( -90 );
	SetCameraElevationAngle( 0 );
	SetCameraRadius( 64 );
	
	float Cam_Roll = 0;
		
	loop{
		
		//float Cam_Roll = 0;
		
		SetCameraRoll( Cam_Roll );
		
		Cam_Roll += 0.02;
		yield;
	}	
		
	yield;
	
}

//--------------------------------------------------------------------------------------------------------------------------------------------