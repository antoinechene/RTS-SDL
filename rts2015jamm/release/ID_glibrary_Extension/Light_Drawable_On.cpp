#include "Light_Drawable.h"

bool	Light_Drawable::_OnClick(ID::Event* event)
{
	ID::Rect*	size;

	size = &(this->_animation.GetSpriteAnime()->GetRects()[0]);
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true &&
	    event->click.x >= this->_x + this->_offset_x &&
	    event->click.y >= this->_y + this->_offset_y &&
	    event->click.x < this->_x + this->_offset_x + size->w &&
	    event->click.y < this->_y + this->_offset_y + size->h)
		return true;
	return false;
}

bool	Light_Drawable::_OnDoubleClick(ID::Event* event)
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

bool	Light_Drawable::_OnClickDown(ID::Event* event)
{
	ID::Rect*	size;

	size = &(this->_animation.GetSpriteAnime()->GetRects()[0]);
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true &&
	    event->clickDown.x >= this->_x + this->_offset_x &&
	    event->clickDown.y >= this->_y + this->_offset_y &&
	    event->clickDown.x < this->_x + this->_offset_x + size->w &&
	    event->clickDown.y < this->_y + this->_offset_y + size->h)
		return true;
	return false;
}

bool	Light_Drawable::_OnClickUp(ID::Event* event)
{
	ID::Rect*	size;

	size = &(this->_animation.GetSpriteAnime()->GetRects()[0]);
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true &&
	    event->clickUp.x >= this->_x + this->_offset_x &&
	    event->clickUp.y >= this->_y + this->_offset_y &&
	    event->clickUp.x < this->_x + this->_offset_x + size->w &&
	    event->clickUp.y < this->_y + this->_offset_y + size->h)
		return true;
	return false;
}

bool	Light_Drawable::_OnEnter(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Light_Drawable::_OnLeave(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Light_Drawable::_OnMotion(ID::Event* event)
{
	ID::Rect*	size;

	size = &(this->_animation.GetSpriteAnime()->GetRects()[0]);
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true &&
	    event->motion.x >= this->_x + this->_offset_x &&
	    event->motion.y >= this->_y + this->_offset_y &&
	    event->motion.x < this->_x + this->_offset_x + size->w &&
	    event->motion.y < this->_y + this->_offset_y + size->h)
		return true;
	return false;
}

bool	Light_Drawable::_OnKeyUp(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Light_Drawable::_OnKeyDown(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Light_Drawable::_OnFocusEnter(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Light_Drawable::_OnFocusLeave(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Light_Drawable::_OnEnableChange(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Light_Drawable::_OnScreenChange(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Light_Drawable::_OnResize(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Light_Drawable::_OnQuit(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

bool	Light_Drawable::_OnUserEvent(ID::Event*)
{
	if (this->GetOnScreen() == true &&
	    this->GetEnable() == true)
		return true;
	return false;
}

