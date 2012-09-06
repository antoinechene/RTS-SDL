#include "State.h"
#include <stdlib.h>
#include <iostream>
#include "State_Machine.h"

State::State(uint32_t ID) : _motherMachine(NULL), _ID(ID)
{
}

State::~State()
{
}

void	State::OnEnter(void)
{
}

void	State::OnLeave(void)
{
}

