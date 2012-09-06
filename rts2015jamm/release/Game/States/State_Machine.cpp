#include "State_Machine.h"
#include "State.h"
#include "../Game_Events/GameEvent.hpp"

State_Machine::State_Machine(Unit* u) :__unit(u), __currentState(NULL), __nextState(-1)
{
}

State_Machine::~State_Machine()
{
}

void	State_Machine::Update(void)
{
	try
	{
		if (this->__nextState != -1)
		{
			this->SwitchState(this->__nextState);
			this->__nextState = -1;
		}
		this->__currentState->Update();
	}
	catch (int e)
	{
		std::cout << "Fatal error " << e << ": FSM unknown state." << std::endl;
	}
}

void	State_Machine::SetNextState(uint32_t stateID)
{
	this->__nextState = stateID;
}

void	State_Machine::SwitchState(uint32_t stateID)
{
	this->__currentState->OnLeave();
	this->__currentState = this->GetState(stateID);
	this->__currentState->OnEnter();
}

int	State_Machine::SetStates(uint32_t n, ...)
{
	va_list		args;
	State*		state;
	uint32_t	i;

	va_start(args, n);
	this->__finiteStatesList.resize(n);
	i = 0;
	while (i < n)
	{
		state = va_arg(args, State*);
		state->_motherMachine = this;
		this->__finiteStatesList[i] = state;
		++i;
	}
	va_end(args);
	return 0;
}

void	State_Machine::SetInitialState(uint32_t ID)
{
	this->__currentState = this->GetState(ID);
	this->__currentState->OnEnter();
}

State*	State_Machine::GetState(uint32_t ID)
{
	ID::vector<State*>::iterator	it;
	ID::vector<State*>::iterator	end;

	it = this->__finiteStatesList.begin();
	end = this->__finiteStatesList.end();
	while (it != end)
	{
		if ((*it)->GetID() == ID)
			return *it;
		++it;
	}
	throw 1300;
}
