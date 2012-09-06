#ifndef	__ARC_HPP__
#define	__ARC_HPP__

#include "../ID_glibrary.h"
#include "Context_Updater.hpp"
#include "Menu_Manager.hpp"
#include <sys/time.h>
#include <math.h>

#define	PI	3.141592

template <typename T>
class	Arc_Updater : public Context_Updater<T>
{
 public:
	struct timeval	last;
	struct timeval	interval;
	int		step;
	float		angleTargeted;
	int		lastFocus;
	int16_t		targetX;
	int16_t		targetY;

 public:
	Arc_Updater() : lastFocus(-1), targetX(0), targetY(0)
	{
		this->last.tv_sec = 0;
		this->last.tv_usec = 0;
		this->interval.tv_sec = 0;
		this->interval.tv_usec = 10000;
		this->step = 3;
		this->angleTargeted = 45;
	}

	void	Init(Menu_Context<T>* mc)
	{
		int	pW;
		int	pH;
		int	w;
		int	h;
		int	i;
		int	x;
		int	y;
		ID::Font::Police*	modelP;
		ID::Font::Police*	newP;
		ID::Font*		f;

		f = ID::Font::GetInstance();
		pW = mc->GetMenu(mc->focus)->GetParent()->GetWidth();
		pH = mc->GetMenu(mc->focus)->GetParent()->GetHeight();
		w = mc->GetMenu(mc->focus)->GetWidth();
		h = mc->GetMenu(mc->focus)->GetHeight();

		x = pW >> 1;
		y = pH >> 1;
		mc->GetMenu(mc->focus)->SetPos(x, y);

		modelP = mc->GetMenu(mc->focus)->GetPolice();
		i = 0;
		while (i < mc->focus)
		{
			x = x - (w >> 2);
			y = y - h - 30;
			newP = f->LoadFont(modelP->path,
					   modelP->size - (6 * (mc->focus - i)));
			mc->GetMenu(i)->SetPolice(newP);
			mc->GetMenu(i)->SetPos(x, y);
			++i;
			std::cout << x << "|" << y << "\n";
		}
		x = pW >> 1;
		y = pH >> 1;
		std::cout << "---" << x << "|" << y << "\n";
		i = mc->focus;
		while (i < mc->GetMaxMenus() - 1)
		{
			++i;
			newP = f->LoadFont(modelP->path,
					   modelP->size - (6 * i));
			x = x - (w >> 2);
			y = y + h + 30;
			mc->GetMenu(i)->SetPolice(newP);
			mc->GetMenu(i)->SetPos(x, y);
			std::cout << i << ">" << x << "|" << y << ">"<< newP->size << "\n";
		}
	}

	void	Update(Menu_Context<T>* mc)
	{
		struct timeval	tv;
		if (gettimeofday(&tv, NULL) != -1)
		{
			if (timercmp(&tv, &(this->last), >))
			{
				this->last = tv;
				this->last.tv_sec += this->interval.tv_sec;
				this->last.tv_usec += this->interval.tv_usec;
			}
			else
				return ;
		}
		else
			return ;
		int16_t	x;
		int16_t	y;

		mc->GetMenu(mc->focus)->GetPos(&x, &y);
		if (x == this->targetX &&
		    y == this->targetY &&
		    this->lastFocus != -1)
			return ;

		int	i;
		float	xD;
		float	yD;
		float	angleRefereee;
		int	angleReferee;
		int	size;
		ID::Font::Police*	modelP;
		ID::Font::Police*	newP;
		ID::Font*		f;

		f = ID::Font::GetInstance();
		modelP = mc->GetMenu(mc->focus)->GetPolice();
		xD = x;
		yD = y;
		angleRefereee = atan(yD / xD);
		angleReferee = 180 * angleRefereee / PI;
		if (angleReferee - this->angleTargeted > 0)
		{
			angleReferee -= this->step;
			if (angleReferee < this->angleTargeted)
				angleReferee = this->angleTargeted;
		}
		else if (angleReferee - this->angleTargeted < 0)
		{
			angleReferee += this->step;
			if (angleReferee > this->angleTargeted)
				angleReferee = this->angleTargeted;
		}
		else
			return ;
		if (this->lastFocus != mc->focus)
		{
			angleRefereee = PI * (this->angleTargeted / 180);
			targetX = 500 * cos(angleRefereee);
			targetY = 500 * sin(angleRefereee);
			this->lastFocus = mc->focus;
		}
		i = 0;
//		mc->GetMenu(i)->GetRealWindow()->AddUpdate(0);
		while (i < mc->GetMaxMenus())
		{
			size = angleReferee + (i - mc->focus) * 11;
			angleRefereee = PI * size / 180;
			x = 500 * cos(angleRefereee);
			y = 500 * sin(angleRefereee);
			mc->GetMenu(i)->SetPos(x, y);
			if (this->angleTargeted - size >= 0)
				size = 40 - ((this->angleTargeted - size) * 0.7);
			else
				size = 40 - ((size - this->angleTargeted) * 0.7);
			if (size <= 1)
			{
				if (mc->GetMenu(i)->GetOnScreen() == true)
					mc->GetMenu(i)->SetOnScreen(false);
				++i;
				continue ;
			}
			newP = f->LoadFont(modelP->path,
					   size);
			if (newP != NULL)
			{
				if (mc->GetMenu(i)->GetOnScreen() == false)
					mc->GetMenu(i)->SetOnScreen(true);
				mc->GetMenu(i)->SetPolice(newP);
			}
			++i;
		}
	}
	void	ApplyEvent(Menu_Context<T>* mc, ID::Event* e)
	{
		if (e->type == ID::EVENT_KEY_DOWN)
		{
			if (e->keyDown.keysym.sym == 273)
			{
				if (mc->focus > 0)
					--mc->focus;
			}
			else if (e->keyDown.keysym.sym == 274)
			{
				if (mc->focus + 1 < mc->GetMaxMenus())
					++mc->focus;
			}
			else
			{
				mc->GetMenu(mc->focus)->UserEvent(e);
			}
		}
	}
};

#endif
