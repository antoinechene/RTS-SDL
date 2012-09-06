#include "Option_Screen.h"
#include "Main_Screen.h"
#include "../Env.h"
#include "../Constantes.h"
#include "../Game/Game.h"
#include "../Game/Player.h"
#include "../Network/Protocol_Manager.h"
#include "../Menu/Main_Menu.h"
#include <iostream>
#include <sys/stat.h>
#include <fcntl.h>

std::map<int, const char*>	shortcut_textHR(void)
{
	std::map<int, const char*>	sc;

	sc[ID::KEY_UNKNOWN] = "unknow";
	sc[ID::KEY_FIRST] = "first";
	sc[ID::KEY_BACKSPACE] = "backspace";
	sc[ID::KEY_TAB] = "tab";
	sc[ID::KEY_CLEAR] = "clear";
	sc[ID::KEY_RETURN] = "return";
	sc[ID::KEY_PAUSE] = "pause";
	sc[ID::KEY_ESCAPE] = "esc";
	sc[ID::KEY_SPACE] = "space";
	sc[ID::KEY_EXCLAIM] = "!";
	sc[ID::KEY_QUOTEDBL] = "\"";
	sc[ID::KEY_HASH] = "#";
	sc[ID::KEY_DOLLAR] = "$";
	sc[ID::KEY_AMPERSAND] = "%";
	sc[ID::KEY_QUOTE] = "'";
	sc[ID::KEY_LEFTPAREN] = "(";
	sc[ID::KEY_RIGHTPAREN] = ")";
	sc[ID::KEY_ASTERISK] = "*";
	sc[ID::KEY_PLUS] = "+";
	sc[ID::KEY_COMMA] = ",";
	sc[ID::KEY_MINUS] = "-";
	sc[ID::KEY_PERIOD] = "~";
	sc[ID::KEY_SLASH] = "/";
	sc[ID::KEY_0] = "0";
	sc[ID::KEY_1] = "1";
	sc[ID::KEY_2] = "2";
	sc[ID::KEY_3] = "3";
	sc[ID::KEY_4] = "4";
	sc[ID::KEY_5] = "5";
	sc[ID::KEY_6] = "6";
	sc[ID::KEY_7] = "7";
	sc[ID::KEY_8] = "8";
	sc[ID::KEY_9] = "9";
	sc[ID::KEY_COLON] = ":";
	sc[ID::KEY_SEMICOLON] = ";";
	sc[ID::KEY_LESS] = "<";
	sc[ID::KEY_EQUALS] = "=";
	sc[ID::KEY_GREATER] = ">";
	sc[ID::KEY_QUESTION] = "?";
	sc[ID::KEY_AT] = ".";
	sc[ID::KEY_LEFTBRACKET] = "[";
	sc[ID::KEY_BACKSLASH] = "\\";
	sc[ID::KEY_RIGHTBRACKET] = "]";
	sc[ID::KEY_CARET] = "&";
	sc[ID::KEY_UNDERSCORE] = "_";
	sc[ID::KEY_BACKQUOTE] = "backquote";
	sc[ID::KEY_a] = "a";
	sc[ID::KEY_b] = "b";
	sc[ID::KEY_c] = "c";
	sc[ID::KEY_d] = "d";
	sc[ID::KEY_e] = "e";
	sc[ID::KEY_f] = "f";
	sc[ID::KEY_g] = "g";
	sc[ID::KEY_h] = "h";
	sc[ID::KEY_i] = "i";
	sc[ID::KEY_j] = "j";
	sc[ID::KEY_k] = "k";
	sc[ID::KEY_l] = "l";
	sc[ID::KEY_m] = "m";
	sc[ID::KEY_n] = "n";
	sc[ID::KEY_o] = "o";
	sc[ID::KEY_p] = "p";
	sc[ID::KEY_q] = "q";
	sc[ID::KEY_r] = "r";
	sc[ID::KEY_s] = "s";
	sc[ID::KEY_t] = "t";
	sc[ID::KEY_u] = "u";
	sc[ID::KEY_v] = "v";
	sc[ID::KEY_w] = "w";
	sc[ID::KEY_x] = "x";
	sc[ID::KEY_y] = "y";
	sc[ID::KEY_z] = "z";
	sc[ID::KEY_DELETE] = "del";
	sc[ID::KEY_UP] = "up";
	sc[ID::KEY_DOWN] = "down";
	sc[ID::KEY_RIGHT] = "right";
	sc[ID::KEY_LEFT] = "left";
	sc[ID::KEY_INSERT] = "insert";
	sc[ID::KEY_HOME] = "home";
	sc[ID::KEY_END] = "end";
	sc[ID::KEY_PAGEUP] = "page up";
	sc[ID::KEY_PAGEDOWN] = "page down";
	sc[ID::KEY_F1] = "F1";
	sc[ID::KEY_F2] = "F2";
	sc[ID::KEY_F3] = "F3";
	sc[ID::KEY_F4] = "F4";
	sc[ID::KEY_F5] = "F5";
	sc[ID::KEY_F6] = "F6";
	sc[ID::KEY_F7] = "F7";
	sc[ID::KEY_F8] = "F8";
	sc[ID::KEY_F9] = "F9";
	sc[ID::KEY_F10] = "F10";
	sc[ID::KEY_F11] = "F11";
	sc[ID::KEY_F12] = "F12";
	sc[ID::KEY_F13] = "F13";
	sc[ID::KEY_F14] = "F14";
	sc[ID::KEY_F15] = "F15";
	sc[ID::KEY_NUMLOCK] = "numlock";
	sc[ID::KEY_CAPSLOCK] = "capslock";
	sc[ID::KEY_SCROLLOCK] = "scrollock";
	sc[ID::KEY_RSHIFT] = "R shift";
	sc[ID::KEY_LSHIFT] = "L shift";
	sc[ID::KEY_RCTRL] = "R ctrl";
	sc[ID::KEY_LCTRL] = "L ctrl";
	sc[ID::KEY_RALT] = "R alt";
	sc[ID::KEY_LALT] = "L alt";
	sc[ID::KEY_RMETA] = "R meta";
	sc[ID::KEY_LMETA] = "L meta";
	sc[ID::KEY_LSUPER] = "L super";
	sc[ID::KEY_RSUPER] = "R super";
	sc[ID::KEY_MODE] = "mode";
	sc[ID::KEY_COMPOSE] = "compose";
	sc[ID::KEY_HELP] = "help";
	sc[ID::KEY_PRINT] = "print";
	sc[ID::KEY_SYSREQ] = "sysrq";
	sc[ID::KEY_BREAK] = "break";
	sc[ID::KEY_MENU] = "menu";
	sc[ID::KEY_POWER] = "power";
	sc[ID::KEY_EURO] = "euro";
	sc[ID::KEY_UNDO] = "undo";
	return sc;
}

static std::map<int, const char*>	shortcut = shortcut_textHR();

char*	get_welcome_message(Main_Menu* = NULL);

void	__keyHR_CbKeyDown(ID::Control* _tb, ID::Event* e, void* _data)
{
	ID::Textbox*				tb;
	Shortcut_Cmd*				sh;
	std::map<int, const char*>::iterator	it;

	tb = (ID::Textbox*)_tb;
	sh = (Shortcut_Cmd*)_data;

	it = shortcut.find((int)(e->keyDown.keysym.sym));
	tb->SetText((char*)it->second);
	sh->key = e->keyDown.keysym.sym;

	tb->SetInvalidate(true);
//	tb->FocusLeave(e);
}

void	Shortcut_Cmd::SetCb(callback cb)
{
	this->_cbShortcut = cb;
}


void	check_shortcut(ID::Event* e, Option_Screen* os)
{
	if (e->keyDown.keysym.sym == os->talk.key)
	{
		if (os->talk._cbShortcut != NULL)
			os->talk._cbShortcut();
	}
	else if (e->keyDown.keysym.sym == os->teamTalk.key)
	{
		if (os->teamTalk._cbShortcut != NULL)
			os->teamTalk._cbShortcut();
	}
	else if (e->keyDown.keysym.sym == os->cameraUp.key)
	{
		if (os->cameraUp._cbShortcut != NULL)
			os->cameraUp._cbShortcut();
	}
	else if (e->keyDown.keysym.sym == os->cameraDown.key)
	{
		if (os->cameraDown._cbShortcut != NULL)
			os->cameraDown._cbShortcut();
	}
	else if (e->keyDown.keysym.sym == os->cameraLeft.key)
	{
		if (os->cameraLeft._cbShortcut != NULL)
			os->cameraLeft._cbShortcut();
	}
	else if (e->keyDown.keysym.sym == os->cameraRight.key)
	{
		if (os->cameraRight._cbShortcut != NULL)
			os->cameraRight._cbShortcut();
	}
	else if (e->keyDown.keysym.sym == os->move.key)
	{
		if (os->move._cbShortcut != NULL)
			os->move._cbShortcut();
	}
	else if (e->keyDown.keysym.sym == os->attack.key)
	{
		if (os->attack._cbShortcut != NULL)
			os->attack._cbShortcut();
	}
	else if (e->keyDown.keysym.sym == os->patrol.key)
	{
		if (os->patrol._cbShortcut != NULL)
			os->patrol._cbShortcut();
	}
	else if (e->keyDown.keysym.sym == os->hold.key)
	{
		if (os->hold._cbShortcut != NULL)
			os->hold._cbShortcut();
	}
	else if (e->keyDown.keysym.sym == os->assist.key)
	{
		if (os->assist._cbShortcut != NULL)
			os->assist._cbShortcut();
	}
	else if (e->keyDown.keysym.sym == os->stop.key)
	{
		if (os->stop._cbShortcut != NULL)
			os->stop._cbShortcut();
	}
	else if (e->keyDown.keysym.sym == os->hidleWorker.key)
	{
		if (os->hidleWorker._cbShortcut != NULL)
			os->hidleWorker._cbShortcut();
	}
	else if (e->keyDown.keysym.sym == 38 || e->keyDown.keysym.sym == 49)
	{
		if (Game::GetInstance()->IsGameRunning() == true)
		{
			if (e->keyDown.keysym.mod & ID::KEYMOD_LCTRL)
				Protocol_Manager::GetInstance()->prot.AddCmdGameSetGroup(1);
			else
				Protocol_Manager::GetInstance()->prot.AddCmdGameUseGroup(1, Game::GetInstance()->myID);
		}
	}
	else if (e->keyDown.keysym.sym == 233 || e->keyDown.keysym.sym == 50)
	{
		if (Game::GetInstance()->IsGameRunning() == true)
		{
			if (e->keyDown.keysym.mod & ID::KEYMOD_LCTRL)
				Protocol_Manager::GetInstance()->prot.AddCmdGameSetGroup(2);
			else
				Protocol_Manager::GetInstance()->prot.AddCmdGameUseGroup(2, Game::GetInstance()->myID);
		}
	}
	else if (e->keyDown.keysym.sym == 34 || e->keyDown.keysym.sym == 51)
	{
		if (Game::GetInstance()->IsGameRunning() == true)
		{
			if (e->keyDown.keysym.mod & ID::KEYMOD_LCTRL)
				Protocol_Manager::GetInstance()->prot.AddCmdGameSetGroup(3);
			else
				Protocol_Manager::GetInstance()->prot.AddCmdGameUseGroup(3, Game::GetInstance()->myID);
		}
	}
	std::cout << e->keyDown.keysym.sym << "\n";
}

void	__Option_Screen__toggleFSCbClickDown(ID::Control*, ID::Event*, void*)
{
	ID::Wrapper_Interface::GetInstance()->WM_ToggleFullScreen(Env::GetInstance()->window->GetSurface());
}

void	__Option_Screen__validateOptionsCbClickDown(ID::Control*, ID::Event*, void* d)
{
	std::cout << "Validate options" << std::endl;
	int		fd;
	Env*		env;
	Option_Screen*	os;
	char*		nick;

	os = (Option_Screen*)d;
	env = Env::GetInstance();
	nick = os->inputNickname.GetText();
	strncpy(env->nickname, nick, 20);

	fd = open("Resources/options", O_WRONLY | O_CREAT | O_TRUNC);
	if (fd == -1)
		std::cout << "Cannot open the option file, wtf have you done to it ?!" << std::endl;
	else
	{
		write(fd, env->nickname, strlen(env->nickname));
		write(fd, "\n", 1);
		write(fd, env->optionScreen->talk.keyHR.GetText(), strlen(env->optionScreen->talk.keyHR.GetText()));
		write(fd, "\n", 1);
		write(fd, env->optionScreen->teamTalk.keyHR.GetText(), 1);
		write(fd, "\n", 1);
		write(fd, env->optionScreen->cameraUp.keyHR.GetText(), 1);
		write(fd, "\n", 1);
		write(fd, env->optionScreen->cameraDown.keyHR.GetText(), 1);
		write(fd, "\n", 1);
		write(fd, env->optionScreen->cameraLeft.keyHR.GetText(), 1);
		write(fd, "\n", 1);
		write(fd, env->optionScreen->cameraRight.keyHR.GetText(), 1);
		write(fd, "\n", 1);
		write(fd, env->optionScreen->move.keyHR.GetText(), 1);
		write(fd, "\n", 1);
		write(fd, env->optionScreen->attack.keyHR.GetText(), 1);
		write(fd, "\n", 1);
		write(fd, env->optionScreen->patrol.keyHR.GetText(), 1);
		write(fd, "\n", 1);
		write(fd, env->optionScreen->hold.keyHR.GetText(), 1);
		write(fd, "\n", 1);
		write(fd, env->optionScreen->assist.keyHR.GetText(), 1);
		write(fd, "\n", 1);
		write(fd, env->optionScreen->stop.keyHR.GetText(), 1);
		write(fd, "\n", 1);
		write(fd, env->optionScreen->hidleWorker.keyHR.GetText(), 1);
		write(fd, "\n", 1);
		env->mainScreen->mainMenu->welcomeTitle.SetText(get_welcome_message());
	}
	close(fd);
}

void	__Option_Screen__backCbClickDown(ID::Control*, ID::Event*, void*)
{
	std::cout << "back\n";
	Env*		e;

	e = Env::GetInstance();
	e->SwitchScreen(e->mainScreen);
}

Option_Screen::Option_Screen() : Abstract_Screen(UI_WIDTH, UI_HEIGHT),
			     optionTitle(DEFAULT_FONT_PATH, 40,
				       (char*)"Options",
				       (ID::Color*)&(ID::COLOR_GOLD)),
			     nickname(DEFAULT_FONT_PATH, 30,
				     (char*)"Nickname",
				     (ID::Color*)&(ID::COLOR_GOLD)),
			     inputNickname(350, 30, UI_FLAGS, UI_RMASK,
				     UI_GMASK, UI_BMASK, UI_AMASK,
				     DEFAULT_FONT_PATH, 28,
				     (ID::Color*)&ID::COLOR_ORANGE),
			     talk((char*)"Talk", (char*)"t", 25, 200),
			     teamTalk((char*)"Team Talk", (char*)"y", 25, 230),
			     cameraUp((char*)"Camera-Up", (char*)" ", 25, 260),
			     cameraDown((char*)"Camera-Down", (char*)" ", 25, 290),
			     cameraLeft((char*)"Camera-Left", (char*)" ", 25, 320),
			     cameraRight((char*)"Camera-Right", (char*)" ", 25, 350),
			     move((char*)"Order-Move", (char*)"m", 25, 380),
			     attack((char*)"Order-Attack", (char*)"a", 25, 410),
			     patrol((char*)"Order-Patrol", (char*)"p", 25, 440),
			     hold((char*)"Order-Hold", (char*)"h", 25, 470),
			     assist((char*)"Order-Assist", (char*)"h", 25, 500),
			     stop((char*)"Order-Stop", (char*)"s", 25, 530),
			     hidleWorker((char*)"Hidle Worker", (char*)"w", 25, 560),
			     toggleFullScreen(250, 30, UI_FLAGS, UI_RMASK,
					      UI_GMASK, UI_BMASK, UI_AMASK,
					      DEFAULT_FONT_PATH, 17,
					      (char*)"Toggle Full Screen",
					      (ID::Color*)&(ID::COLOR_GOLD)),
			     validateOptions(250, 100, UI_FLAGS, UI_RMASK,
				       UI_GMASK, UI_BMASK, UI_AMASK,
				       DEFAULT_FONT_PATH, 40,
				       (char*)"Validate",
				       (ID::Color*)&(ID::COLOR_GOLD)),
			     back(80, 30, UI_FLAGS, UI_RMASK,
				  UI_GMASK, UI_BMASK, UI_AMASK,
				  DEFAULT_FONT_PATH, 20, (char*)"Back",
				  (ID::Color*)&(ID::COLOR_GOLD))
{
	Env*	env;
	env = Env::GetInstance();

	this->SetName("Option_Screen");

	this->optionTitle.SetPos(15, 15);
	this->AddChild(&(this->optionTitle));

	this->nickname.SetPos(25, 150);
	this->AddChild(&(this->nickname));

	this->inputNickname.SetPos(225, 150);
	this->inputNickname.SetBackground(0xFFFF0000);
	this->inputNickname.SetMaxLength(20);
	this->inputNickname.SetText(env->nickname);
	this->AddChild(&(this->inputNickname));

	this->toggleFullScreen.SetPos(env->width - 250, env->height - 300);
	this->toggleFullScreen.SetCbClickDown(__Option_Screen__toggleFSCbClickDown, this);
	this->AddChild(&(this->toggleFullScreen));

	this->validateOptions.SetPos(env->width - 250, env->height - 100);
	this->validateOptions.SetCbClickDown(__Option_Screen__validateOptionsCbClickDown, this);
	this->AddChild(&(this->validateOptions));

	this->back.SetPos(ID::REL_POS_BOT);
	this->back.SetCbClickDown(__Option_Screen__backCbClickDown, NULL);
	this->AddChild(&(this->back));

	this->talk.keyHR.SetCbKeyDown(__keyHR_CbKeyDown, &(this->talk));
	this->teamTalk.keyHR.SetCbKeyDown(__keyHR_CbKeyDown, &(this->teamTalk));
	this->cameraUp.keyHR.SetCbKeyDown(__keyHR_CbKeyDown, &(this->cameraUp));
	this->cameraDown.keyHR.SetCbKeyDown(__keyHR_CbKeyDown, &(this->cameraDown));
	this->cameraLeft.keyHR.SetCbKeyDown(__keyHR_CbKeyDown, &(this->cameraLeft));
	this->cameraRight.keyHR.SetCbKeyDown(__keyHR_CbKeyDown, &(this->cameraRight));
	this->move.keyHR.SetCbKeyDown(__keyHR_CbKeyDown, &(this->move));
	this->attack.keyHR.SetCbKeyDown(__keyHR_CbKeyDown, &(this->attack));
	this->patrol.keyHR.SetCbKeyDown(__keyHR_CbKeyDown, &(this->patrol));
	this->hold.keyHR.SetCbKeyDown(__keyHR_CbKeyDown, &(this->hold));
	this->assist.keyHR.SetCbKeyDown(__keyHR_CbKeyDown, &(this->assist));
	this->stop.keyHR.SetCbKeyDown(__keyHR_CbKeyDown, &(this->stop));
	this->hidleWorker.keyHR.SetCbKeyDown(__keyHR_CbKeyDown, &(this->hidleWorker));

	this->AddChild(&(this->talk.name));
	this->AddChild(&(this->talk.keyHR));
	this->AddChild(&(this->teamTalk.name));
	this->AddChild(&(this->teamTalk.keyHR));
	this->AddChild(&(this->cameraUp.name));
	this->AddChild(&(this->cameraUp.keyHR));
	this->AddChild(&(this->cameraDown.name));
	this->AddChild(&(this->cameraDown.keyHR));
	this->AddChild(&(this->cameraLeft.name));
	this->AddChild(&(this->cameraLeft.keyHR));
	this->AddChild(&(this->cameraRight.name));
	this->AddChild(&(this->cameraRight.keyHR));
	this->AddChild(&(this->move.name));
	this->AddChild(&(this->move.keyHR));
	this->AddChild(&(this->attack.name));
	this->AddChild(&(this->attack.keyHR));
	this->AddChild(&(this->patrol.name));
	this->AddChild(&(this->patrol.keyHR));
	this->AddChild(&(this->hold.name));
	this->AddChild(&(this->hold.keyHR));
	this->AddChild(&(this->assist.name));
	this->AddChild(&(this->assist.keyHR));
	this->AddChild(&(this->stop.name));
	this->AddChild(&(this->stop.keyHR));
	this->AddChild(&(this->hidleWorker.name));
	this->AddChild(&(this->hidleWorker.keyHR));
}

Option_Screen::~Option_Screen()
{
}

void	Option_Screen::ApplyEvent(ID::Event* e)
{
/*	Com_protocol		pm;
	Protocol_Manager	Pm;
	Protocol	p;
	int		fd;
	char		buf[512];

	if (Sockets_manager::GetInstance()->GetSockets()->begin() != Sockets_manager::GetInstance()->GetSockets()->end())
	{
		fd = (*Sockets_manager::GetInstance()->GetSockets()->begin())->fd;
	p.create = new cmd_Create();
	p.create->qty_mod = 1;
	p.create->mods = new t_Create_model[1];
	p.create->mods[0].qty_uni = 1;
	p.create->mods[0].id_model = 14;
	p.create->mods[0].units = new t_Create_unit[1];
	p.create->mods[0].units[0].id_parent = 73;

	p.order_attack = new cmd_Order_attack();
	p.order_attack->qty = 1;
	p.order_attack->attack = new t_Attack[1];
	p.order_attack->attack[0].id_target = 5;
	p.order_attack->attack[0].id_player = 3;
	p.order_attack->attack[0].qty = 1;
	p.order_attack->attack[0].id_unit = new uint32_t[1];
	p.order_attack->attack[0].id_unit[0] = 86;
	}
*/	if (e->type == ID::EVENT_KEY_DOWN)
	{
		if (e->keyDown.keysym.sym == ID::KEY_RETURN ||
		    e->keyDown.keysym.sym == ID::KEY_KP_ENTER)
			__Option_Screen__validateOptionsCbClickDown(0, 0, this);
		else if (e->keyDown.keysym.sym == ID::KEY_ESCAPE)
			__Option_Screen__backCbClickDown(0, 0, 0);
/*		else if (e->keyDown.keysym.sym == ID::KEY_a)
		{
			if (Sockets_manager::GetInstance()->GetSockets()->begin() != Sockets_manager::GetInstance()->GetSockets()->end())
			{
				pm.write_prot(fd, p);
			}
		}
*/	}
}

void	Option_Screen::Refresh()
{
}

void	Option_Screen::Enter()
{
}

void	Option_Screen::Leave()
{
}

Shortcut_Cmd::Shortcut_Cmd(char* nameKey, char* defaultKey, int x, int y) : name(DEFAULT_FONT_PATH, 20, nameKey,
								(ID::Color*)&(ID::COLOR_GOLD)),
					     		    keyHR(155, 25, UI_FLAGS, UI_RMASK,
								  UI_GMASK, UI_BMASK, UI_AMASK,
				  				  DEFAULT_FONT_PATH, 20,
				    				  (ID::Color*)&ID::COLOR_ORANGE)

{
	(void)defaultKey;
	this->name.SetPos(x, y);

	this->keyHR.SetPos(x + 200, y);
	this->keyHR.SetBackground(0x0FFFFFF);
	this->keyHR.SetMaxLength(10);
	this->keyHR.SetText(defaultKey);

	this->_cbShortcut = NULL;
}
