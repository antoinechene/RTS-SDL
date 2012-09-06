NAME =	id_dtc
LIBRARY_NAME = ID_glibrary.a
SRC = \
      	main.cpp\
	Buffer.cpp\
	Env.cpp\
	Tools.cpp\
\
	Database/Database.cpp\
\
	Network/Protocol_Manager.cpp\
	Network/Protocol_Manager_Reader.cpp\
	Network/Protocol_Manager_Writer.cpp\
	Network/Protocol_Reader_Context.cpp\
	Network/Order_Network.cpp\
	Network/Sockets_manager.cpp\
	Network/Socket.cpp\
\
	ID_glibrary_Extension/Light_Drawable.cpp\
	ID_glibrary_Extension/Light_Drawable_On.cpp\
	ID_glibrary_Extension/Light_Drawable_Event.cpp\
	ID_glibrary_Extension/Map_Drawable.cpp\
	ID_glibrary_Extension/Map_Drawable_On.cpp\
	ID_glibrary_Extension/Map_Drawable_Event.cpp\
\
	Game/Math_tools.cpp\
	Game/Data_Access.cpp\
	Game/Sprite_Anime.cpp\
	Game/AI_AStar.cpp\
	Game/Cam.cpp\
	Game/Game.cpp\
	Game/HUDEventsCallbacks.cpp\
	Game/Unit.cpp\
	Game/Unit_Model.cpp\
	Game/Production_DB.cpp\
	Game/Resource_Model.cpp\
	Game/Player.cpp\
	Game/Group.cpp\
	Game/Map.cpp\
	Game/ChecksCollision.cpp\
	Game/Building.cpp\
\
	Game/Attack_Handlers/Attack_Handler.cpp\
	Game/Attack_Handlers/Ground_Attack_Handler.cpp\
\
	Game/Move_Handlers/Move_Handler.cpp\
	Game/Move_Handlers/Ground_Move_Handler.cpp\
	Game/Move_Handlers/Air_Move_Handler.cpp\
\
	Game/States/FSM_Idle.cpp\
	Game/States/FSM_Attacker.cpp\
	Game/States/State_Machine.cpp\
	Game/States/State.cpp\
	Game/States/State_Idle.cpp\
	Game/States/State_Idle_Attack.cpp\
	Game/States/State_Move.cpp\
	Game/States/State_Move_Attack_Focus.cpp\
	Game/States/State_Move_Attack_Auto.cpp\
	Game/States/State_Follow_Attack_Auto.cpp\
	Game/States/State_Attack_Focus.cpp\
	Game/States/State_Attack_Auto.cpp\
\
	Game/Scripts/Script_Manager.cpp\
	Game/Scripts/Gather.cpp\
	Game/Scripts/One_Shot.cpp\
	Game/Scripts/Actions/Bullet_Hit_Model.cpp\
	Game/Scripts/Actions/Gather_Model.cpp\
	Game/Scripts/Actions/Action.cpp\
\
	Game/Game_Events/Change_Selection.cpp\
	Game/Game_Events/Create_Units.cpp\
	Game/Game_Events/Delete_Units.cpp\
	Game/Game_Events/Orders_Attack.cpp\
	Game/Game_Events/Orders_Move.cpp\
	Game/Game_Events/Orders_Follow.cpp\
	Game/Game_Events/Orders_Hold.cpp\
	Game/Game_Events/Set_Group.cpp\
	Game/Game_Events/Use_Group.cpp\
\
	Threads/Thread_Display.cpp\
	Threads/Thread_Gameplay.cpp\
\
	Menu/Main_Menu.cpp\
	Menu/Network_Menu.cpp\
	Menu/Nicks_Menu.cpp\
\
	Screens/Main_Screen.cpp\
	Screens/Host_Screen.cpp\
	Screens/Join_Screen.cpp\
	Screens/Credit_Screen.cpp\
	Screens/Option_Screen.cpp\
	Screens/Lobby_Screen.cpp\
	Screens/Game_Screen.cpp\
\
	Sound/Sounds_mix.cpp

SRCC =	Screens/Main_Screen.cpp\
	Screens/Host_Screen.cpp\
	Screens/Join_Screen.cpp\
	Screens/Credit_Screen.cpp\
	Screens/Option_Screen.cpp\
	Screens/Lobby_Screen.cpp\
	Screens/Game_Screen.cpp

CC	= g++
LDFLAGS	+= -lpthread -lSDL -lSDL_ttf -lm -lefence -lSDL_mixer
CXXFLAGS+= -Wall -W -D__SDL -g3
OBJ	= $(SRC:.cpp=.o)
OBJJ	= $(SRCC:.cpp=.o)

$(NAME): $(OBJ)
	${CC} -o ${NAME} ${OBJ} ${LIBRARY_NAME} ${CXXFLAGS} ${LDFLAGS}

clean:
	rm -f ${OBJ}

distclean: clean
	rm -rf $(NAME)

rebuild: distclean $(NAME)

debug1:	CXXFLAGS += -D__DEBUG
debug1:	rebuild

debug2:	CXXFLAGS += -pg
debug2:	LDFLAGS += -pg
debug2:	debug1

screens: $(OBJJ)
	@(echo '')
#.cpp.o:
#	${CC} ${CXXFLAGS} -c $*.cpp -o $*.o
