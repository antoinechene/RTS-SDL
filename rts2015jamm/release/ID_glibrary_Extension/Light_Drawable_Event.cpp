#include "Light_Drawable.h"

int	Light_Drawable::Click(ID::Event* e)
{
	if (!this->_OnClick(e))
		return 0;
	std::list<ID::Drawable*>::reverse_iterator	d_rit;
	std::list<ID::Drawable*>::reverse_iterator	d_end;

	d_rit = this->_childrenDrawable.rbegin();
	d_end = this->_childrenDrawable.rend();
	while (d_rit != d_end)
	{
		if ((*d_rit)->Click(e))
			return 1;
		++d_rit;
	}
	if (this->_cbClick != NULL)
		this->_cbClick(this, e, this->__cbClickData);
	return 1;
}

int	Light_Drawable::DoubleClick(ID::Event* e)
{
	if (!this->_OnDoubleClick(e))
		return 0;
	std::list<ID::Drawable*>::reverse_iterator	d_rit;
	std::list<ID::Drawable*>::reverse_iterator	d_end;

	d_rit = this->_childrenDrawable.rbegin();
	d_end = this->_childrenDrawable.rend();
	while (d_rit != d_end)
	{
		if ((*d_rit)->DoubleClick(e))
			return 1;
		++d_rit;
	}
	if (this->_cbDoubleClick != NULL)
		this->_cbDoubleClick(this, e, this->__cbDoubleClickData);
	return 1;
}

int	Light_Drawable::ClickDown(ID::Event* e)
{
	if (!this->_OnClickDown(e))
		return 0;
	this->GetRealWindow()->SetDrawableFocused(this);
	if (this->_cbClickDown != NULL)
		this->_cbClickDown(this, e, this->__cbClickDownData);
	return 1;
}

int	Light_Drawable::ClickUp(ID::Event* e)
{
	if (!this->_OnClickUp(e))
		return 0;
	if (this->_cbClickUp != NULL)
		this->_cbClickUp(this, e, this->__cbClickUpData);
	return 1;
}

int	Light_Drawable::Enter(ID::Event* e)
{
	if (!this->_OnEnter(e))
		return 0;
	if (this->_cbEnter != NULL)
		this->_cbEnter(this, e, this->__cbEnterData);
	return 1;
}

int	Light_Drawable::Leave(ID::Event* e)
{
	if (!this->_OnLeave(e))
		return 0;
	if (this->_cbLeave != NULL)
		this->_cbLeave(this, e, this->__cbLeaveData);
	return 1;
}

int	Light_Drawable::Motion(ID::Event* e)
{
	if (!this->_OnMotion(e))
		return 0;
	this->GetRealWindow()->SetDrawableEnter(this);
	if (this->_cbMotion != NULL)
		this->_cbMotion(this, e, this->__cbMotionData);
	return 1;
}

int	Light_Drawable::KeyUp(ID::Event* e)
{
	if (!this->_OnKeyUp(e))
		return 0;
	if (this->_cbKeyUp != NULL)
		this->_cbKeyUp(this, e, this->__cbKeyUpData);
	return 1;
}

int	Light_Drawable::KeyDown(ID::Event* e)
{
	if (!this->_OnKeyDown(e))
		return 0;
	if (this->_cbKeyDown != NULL)
		this->_cbKeyDown(this, e, this->__cbKeyDownData);
	return 1;
}

int	Light_Drawable::FocusEnter(ID::Event* e)
{
	if (!this->_OnFocusEnter(e))
		return 0;
	if (this->_cbFocusEnter != NULL)
		this->_cbFocusEnter(this, e, this->__cbFocusEnterData);
	return 1;
}

int	Light_Drawable::FocusLeave(ID::Event* e)
{
	if (!this->_OnFocusLeave(e))
		return 0;
	if (this->_cbFocusLeave != NULL)
		this->_cbFocusLeave(this, e, this->__cbFocusLeaveData);
	return 1;
}

int	Light_Drawable::EnableChange(ID::Event* e)
{
	if (!this->_OnEnableChange(e))
		return 0;
	std::list<ID::Drawable*>::reverse_iterator	d_rit;
	std::list<ID::Control*>::reverse_iterator	c_rit;
	std::list<ID::Drawable*>::reverse_iterator	d_end;
	std::list<ID::Control*>::reverse_iterator	c_end;

	c_rit = this->_childrenControl.rbegin();
	c_end = this->_childrenControl.rend();
	while (c_rit != c_end)
	{
		if ((*c_rit)->EnableChange(e))
			return 1;
		++c_rit;
	}

	d_rit = this->_childrenDrawable.rbegin();
	d_end = this->_childrenDrawable.rend();
	while (d_rit != d_end)
	{
		if ((*d_rit)->EnableChange(e))
			return 1;
		++d_rit;
	}
	if (this->_cbEnableChange != NULL)
		this->_cbEnableChange(this, e, this->__cbEnableChangeData);
	return 1;
}

int	Light_Drawable::ScreenChange(ID::Event* e)
{
	if (!this->_OnScreenChange(e))
		return 0;
	std::list<ID::Drawable*>::reverse_iterator	d_rit;
	std::list<ID::Drawable*>::reverse_iterator	d_end;

	d_rit = this->_childrenDrawable.rbegin();
	d_end = this->_childrenDrawable.rend();
	while (d_rit != d_end)
	{
		if ((*d_rit)->ScreenChange(e))
			return 1;
		++d_rit;
	}
	if (this->_cbScreenChange != NULL)
		this->_cbScreenChange(this, e, this->__cbScreenChangeData);
	return 1;
}

int	Light_Drawable::Resize(ID::Event* e)
{
	if (!this->_OnResize(e))
		return 0;
	std::list<ID::Drawable*>::reverse_iterator	d_rit;
	std::list<ID::Drawable*>::reverse_iterator	d_end;

	d_rit = this->_childrenDrawable.rbegin();
	d_end = this->_childrenDrawable.rend();
	while (d_rit != d_end)
	{
		if ((*d_rit)->Resize(e))
			return 1;
		++d_rit;
	}
	if (this->_cbResize != NULL)
		this->_cbResize(this, e, this->__cbResizeData);
	return 1;
}

int	Light_Drawable::Quit(ID::Event* e)
{
	if (!this->_OnQuit(e))
		return 0;
	std::list<ID::Drawable*>::reverse_iterator	d_rit;
	std::list<ID::Control*>::reverse_iterator	c_rit;
	std::list<ID::Drawable*>::reverse_iterator	d_end;
	std::list<ID::Control*>::reverse_iterator	c_end;

	c_rit = this->_childrenControl.rbegin();
	c_end = this->_childrenControl.rend();
	while (c_rit != c_end)
	{
		if ((*c_rit)->Quit(e))
			return 1;
		++c_rit;
	}

	d_rit = this->_childrenDrawable.rbegin();
	d_end = this->_childrenDrawable.rend();
	while (d_rit != d_end)
	{
		if ((*d_rit)->Quit(e))
			return 1;
		++d_rit;
	}
	if (this->_cbQuit != NULL)
		this->_cbQuit(this, e, this->__cbQuitData);
	return 1;
}

int	Light_Drawable::UserEvent(ID::Event* e)
{
	if (!this->_OnUserEvent(e))
		return 0;
	std::list<ID::Drawable*>::reverse_iterator	d_rit;
	std::list<ID::Control*>::reverse_iterator	c_rit;
	std::list<ID::Drawable*>::reverse_iterator	d_end;
	std::list<ID::Control*>::reverse_iterator	c_end;

	c_rit = this->_childrenControl.rbegin();
	c_end = this->_childrenControl.rend();
	while (c_rit != c_end)
	{
		if ((*c_rit)->UserEvent(e))
			return 1;
		++c_rit;
	}

	d_rit = this->_childrenDrawable.rbegin();
	d_end = this->_childrenDrawable.rend();
	while (d_rit != d_end)
	{
		if ((*d_rit)->UserEvent(e))
			return 1;
		++d_rit;
	}
	if (this->_cbUserEvent != NULL)
		this->_cbUserEvent(this, e, this->__cbUserEventData);
	return 1;
}

