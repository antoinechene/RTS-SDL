#ifndef	__SCRIPT_MANAGER_H__
#define	__SCRIPT_MANAGER_H__

#include "../../Singleton.hpp"
#include <vector>

class	Script_Model;

enum	SCRIPT_ID
{
	SCRIPT_NONE = 0,
	SCRIPT_RESOURCE,
	SCRIPT_ONE_SHOT
};

class	Script_Manager : public Singleton<Script_Manager>
{
	friend class	Singleton<Script_Manager>;
	public:
				Script_Manager();
				~Script_Manager();
		int		Init(void);
		Script_Model*	GetModel(SCRIPT_ID ID);

	private:
		std::vector<Script_Model*>	__models;
};

#endif
