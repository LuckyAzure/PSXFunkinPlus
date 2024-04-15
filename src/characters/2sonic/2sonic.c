#include "2sonic.h"

#include "../../psx/mem.h"
#include "../../psx/archive.h"
#include "../../scenes/stage/stage.h"
#include "../../main.h"

//2Sonic character structure
enum
{
	Sonic_ArcMain_idle0,
	Sonic_ArcMain_idle1,
	Sonic_ArcMain_idle2,
	Sonic_ArcMain_left0,
	Sonic_ArcMain_down0,
	Sonic_ArcMain_up0,
	Sonic_ArcMain_right0,
	Sonic_ArcMain_spindash0,
	Sonic_ArcMain_spindash1,
	Sonic_ArcMain_spindash2,
	Sonic_ArcMain_spindash3,
	Sonic_ArcMain_spindash4,
	Sonic_ArcMain_spindash5,
	Sonic_ArcMain_spindash6,
	Sonic_ArcMain_spindash7,
	Sonic_ArcMain_spindash8,
	Sonic_ArcMain_spindash9,
	Sonic_ArcMain_spindash10,
	Sonic_ArcMain_spindash11,
	Sonic_ArcMain_spindash12,
	Sonic_ArcMain_spindash14,
	Sonic_ArcMain_spindash15,
	Sonic_ArcMain_spindash16,
	Sonic_ArcMain_spindash19,

	Sonic_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[Sonic_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_2Sonic;

//2Sonic character definitions
static const u16 char_2sonic_icons[2][4] = {
	{0,0,41,25},
	{42,0,41,27}
};

static const CharFrame char_2sonic_frame[] = {
	{Sonic_ArcMain_idle0,{0,0,95,137},{46,124}}, //0 Idle
	{Sonic_ArcMain_idle0,{98,0,96,138},{46,125}}, //1 Idle
	{Sonic_ArcMain_idle1,{0,0,95,140},{43,127}}, //2 Idle
	{Sonic_ArcMain_idle1,{98,0,94,141},{41,128}}, //3 Idle
	{Sonic_ArcMain_idle2,{0,0,93,142},{41,128}}, //4 Idle
	
	{Sonic_ArcMain_left0,{0,0,104,136},{50,124}}, //5 Left
	{Sonic_ArcMain_left0,{107,0,103,138},{46,126}}, //6 Left
	
	{Sonic_ArcMain_down0,{0,0,93,125},{40,111}}, //7 Down
	{Sonic_ArcMain_down0,{95,0,91,127},{39,113}}, //8 Down
	
	{Sonic_ArcMain_up0,{0,0,81,152},{33,136}}, //9 Up
	{Sonic_ArcMain_up0,{84,0,82,150},{34,135}}, //10 Up
	
	{Sonic_ArcMain_right0,{0,0,103,143},{31,128}}, //11 Right
	{Sonic_ArcMain_right0,{106,0,101,144},{33,129}}, //12 Right
	
	{Sonic_ArcMain_spindash0,{0,0,96,144},{42,128}}, //13 SpinDash
	{Sonic_ArcMain_spindash1,{0,0,96,144},{42,128}}, //14 SpinDash
	{Sonic_ArcMain_spindash2,{0,0,96,144},{42,128}}, //15 SpinDash
	{Sonic_ArcMain_spindash3,{0,0,98,144},{42,128}}, //16 SpinDash
	{Sonic_ArcMain_spindash4,{0,0,101,142},{42,127}}, //17 SpinDash
	{Sonic_ArcMain_spindash5,{0,0,104,142},{45,127}}, //18 SpinDash
	{Sonic_ArcMain_spindash6,{0,0,100,140},{47,126}}, //19 SpinDash
	{Sonic_ArcMain_spindash7,{0,0,104,138},{50,124}}, //20 SpinDash
	{Sonic_ArcMain_spindash8,{0,0,108,140},{52,124}}, //21 SpinDash
	{Sonic_ArcMain_spindash9,{0,0,110,142},{52,125}}, //22 SpinDash
	{Sonic_ArcMain_spindash10,{0,0,108,142},{52,125}}, //23 SpinDash
	{Sonic_ArcMain_spindash11,{0,0,108,142},{52,125}}, //24 SpinDash
	{Sonic_ArcMain_spindash12,{0,0,108,142},{53,125}}, //25 SpinDash
	{Sonic_ArcMain_spindash14,{0,0,105,134},{60,115}}, //26 SpinDash
	{Sonic_ArcMain_spindash15,{0,0,154,109},{61,93}}, //27 SpinDash
	{Sonic_ArcMain_spindash15,{0,111,151,96},{61,93}}, //28 SpinDash
	{Sonic_ArcMain_spindash16,{0,0,194,80},{71,75}}, //29 SpinDash
	{Sonic_ArcMain_spindash16,{0,83,194,85},{90,80}}, //30 SpinDash
	{Sonic_ArcMain_spindash19,{0,0,222,91},{141,80}}, //31 SpinDash
	{Sonic_ArcMain_spindash19,{0,94,222,89},{265,80}}, //32 SpinDash
	{Sonic_ArcMain_spindash19,{0,0,10,10},{76,80}}, //33 SpinDash
	{Sonic_ArcMain_spindash19,{0,0,10,10},{76,80}}, //34 SpinDash
	{Sonic_ArcMain_spindash19,{0,0,10,10},{76,80}}, //35 SpinDash
	{Sonic_ArcMain_spindash19,{0,0,10,10},{76,80}}, //36 SpinDash
	{Sonic_ArcMain_spindash19,{0,0,10,10},{76,80}}, //37 SpinDash
	{Sonic_ArcMain_spindash19,{0,0,10,10},{76,80}}, //38 SpinDash
	{Sonic_ArcMain_spindash19,{0,0,10,10},{76,80}}, //39 SpinDash
	{Sonic_ArcMain_spindash19,{0,0,10,10},{76,80}}, //40 SpinDash
	

};

static const Animation char_2sonic_anim[CharAnim_Max] = {
	{12, (const u8[]){ 0, 1, 2, 3, 4,  ASCR_BACK, 1}},		//CharAnim_Idle
	{12, (const u8[]){ 5, 6,  ASCR_BACK, 1}},		//CharAnim_Left
	{12, (const u8[]){ 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,  ASCR_BACK, 1}},		//CharAnim_LeftAlt
	{12, (const u8[]){ 7, 8,  ASCR_BACK, 1}},		//CharAnim_Down
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_DownAlt
	{12, (const u8[]){ 9, 10,  ASCR_BACK, 1}},		//CharAnim_Up
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_UpAlt
	{12, (const u8[]){ 11, 12,  ASCR_BACK, 1}},		//CharAnim_Right
	{0, (const u8[]){ASCR_CHGANI, CharAnim_Idle}},		//CharAnim_RightAlt

};

//2Sonic character functions
void Char_2Sonic_SetFrame(void *user, u8 frame)
{
	Char_2Sonic *this = (Char_2Sonic*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_2sonic_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_2Sonic_Tick(Character *character)
{
	Char_2Sonic *this = (Char_2Sonic*)character;
	
	//Perform idle dance
	if ((character->pad_held & (stage.prefs.control_keys[0] | stage.prefs.control_keys[1] | stage.prefs.control_keys[2] | stage.prefs.control_keys[3])) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_2Sonic_SetFrame);
	Character_Draw(character, &this->tex, &char_2sonic_frame[this->frame]);
}

void Char_2Sonic_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_2Sonic_Free(Character *character)
{
	Char_2Sonic *this = (Char_2Sonic*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_2Sonic_New(fixed_t x, fixed_t y)
{
	//Allocate 2sonic object
	Char_2Sonic *this = Mem_Alloc(sizeof(Char_2Sonic));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_2Sonic_New] Failed to allocate 2sonic object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_2Sonic_Tick;
	this->character.set_anim = Char_2Sonic_SetAnim;
	this->character.free = Char_2Sonic_Free;
	
	Animatable_Init(&this->character.animatable, char_2sonic_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	memcpy(this->character.health_i, char_2sonic_icons, sizeof(char_2sonic_icons));
	
	//health bar color
	this->character.health_bar = 0xFF808080;
	
	this->character.focus_x = FIXED_DEC(30,1);
	this->character.focus_y = FIXED_DEC(-90,1);
	this->character.focus_zoom = FIXED_DEC(100,100);
	
	this->character.size = FIXED_DEC(100,100);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\2SONIC.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim",
		"idle1.tim",
		"idle2.tim",
		"left0.tim",
		"down0.tim",
		"up0.tim",
		"right0.tim",
		"spindash0.tim",
		"spindash1.tim",
		"spindash2.tim",
		"spindash3.tim",
		"spindash4.tim",
		"spindash5.tim",
		"spindash6.tim",
		"spindash7.tim",
		"spindash8.tim",
		"spindash9.tim",
		"spindash10.tim",
		"spindash11.tim",
		"spindash12.tim",
		"spindash14.tim",
		"spindash15.tim",
		"spindash16.tim",
		"spindash19.tim",

		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
