#include "Map_Drawable.h"

bool	Map_Drawable::_OnClick(ID::Event* event)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true &&
	    event->click.x >= this->_x + this->_offset_x &&
	    event->click.y >= this->_y + this->_offset_y &&
	    event->click.x < this->_x + this->_offset_x + this->GetWidth() &&
	    event->click.y < this->_y + this->_offset_y + this->GetHeight())
		return true;
	return false;
}

bool	Map_Drawable::_OnDoubleClick(ID::Event* event)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true &&
	    event->doubleClick.x >= this->_x + this->_offset_x &&
	    event->doubleClick.y >= this->_y + this->_offset_y &&
	    event->doubleClick.x < this->_x + this->_offset_x + this->GetWidth() &&
	    event->doubleClick.y < this->_y + this->_offset_y + this->GetHeight())
		return true;
	return false;
}

bool	Map_Drawable::_OnClickDown(ID::Event* event)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true &&
	    event->clickDown.x >= this->_x + this->_offset_x &&
	    event->clickDown.y >= this->_y + this->_offset_y &&
	    event->clickDown.x < this->_x + this->_offset_x + this->GetWidth() &&
	    event->clickDown.y < this->_y + this->_offset_y + this->GetHeight())
		return true;
	return false;
}

bool	Map_Drawable::_OnClickUp(ID::Event* event)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true &&
	    event->clickUp.x >= this->_x + this->_offset_x &&
	    event->clickUp.y >= this->_y + this->_offset_y &&
	    event->clickUp.x < this->_x + this->_offset_x + this->GetWidth() &&
	    event->clickUp.y < this->_y + this->_offset_y + this->GetHeight())
		return true;
	return false;
}

bool	Map_Drawable::_OnEnter(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Map_Drawable::_OnLeave(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Map_Drawable::_OnMotion(ID::Event* event)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true &&
	    event->motion.x >= this->_x + this->_offset_x &&
	    event->motion.y >= this->_y + this->_offset_y &&
	    event->motion.x < this->_x + this->_offset_x + this->GetWidth() &&
	    event->motion.y < this->_y + this->_offset_y + this->GetHeight())
		return true;
	return false;
}

bool	Map_Drawable::_OnKeyUp(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Map_Drawable::_OnKeyDown(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Map_Drawable::_OnFocusEnter(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Map_Drawable::_OnFocusLeave(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Map_Drawable::_OnEnableChange(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Map_Drawable::_OnScreenChange(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Map_Drawable::_OnResize(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Map_Drawable::_OnQuit(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Map_Drawable::_OnUserEvent(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

