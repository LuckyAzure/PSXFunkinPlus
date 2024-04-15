#define XA_LENGTH(x) (((u64)(x) * 75) / 100 * IO_SECT_SIZE) //Centiseconds to sectors in bytes (w)

typedef struct
{
	XA_File file;
	u32 length;
} XA_TrackDef;

static const XA_TrackDef xa_tracks[] = {
	//MENU.XA
	{XA_Menu, XA_LENGTH(11300)}, //XA_GettinFreaky
	{XA_Menu, XA_LENGTH(3800)},  //XA_GameOver
	//1.XA
	{XA_1, XA_LENGTH(5735)}, //XA_Bopeebo
	{XA_1, XA_LENGTH(28500)}, //XA_Fresh
};

static const char *xa_paths[] = {
	"\\MUSIC\\MENU.XA;1",   //XA_Menu
	"\\MUSIC\\1.XA;1", //XA_Week1A
	NULL,
};

typedef struct
{
	const char *name;
	boolean vocal;
} XA_Mp3;

static const XA_Mp3 xa_mp3s[] = {
	//MENU.XA
	{"freakymenu", false},   //XA_GettinFreaky
	{"gameover", false}, //XA_GameOver
	//WEEK1A.XA
	{"2torial", true}, //XA_Bopeebo
	{"free-4-me", true},   //XA_Fresh
	
	{NULL, false}
};
