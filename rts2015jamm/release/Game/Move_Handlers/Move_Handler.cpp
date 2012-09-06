#include "Move_Handler.h"
#include "../AI_AStar.h"

Move_Handler::Move_Handler(Unit* u) : _path(NULL),
				      _unit(u)
{
}

Move_Handler::~Move_Handler()
{
}

void	Move_Handler::ClearPath(void)
{
	Node*	tmp;

	while (this->_path != NULL)
	{
		tmp = this->_path;
		this->_path = this->_path->parent;
		delete tmp;
	}
}

