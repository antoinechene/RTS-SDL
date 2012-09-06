#ifndef	__LINE_UPDATER_HPP__
#define	__LINE_UPDATER_HPP__

#include "../ID_glibrary.h"
#include "Context_Updater.hpp"
#include "Menu_Manager.hpp"
#include <sys/time.h>
#include <math.h>

#define	PI	3.141592

template <typename T>
class	Line_Updater : public Context_Updater<T>
{
 public:
	struct timeval	last;
	struct timeval	interval;
	int		step;
	double		angleTargeted;
	int		lastFocus;
	int16_t		targetX;
	int16_t		targetY;

 public:
	Line_Updater() : targetX(353), targetY(353)
	{
		this->last.tv_sec = 0;
		this->last.tv_usec = 0;
		this->interval.tv_sec = 0;
		this->interval.tv_usec = 4000;
		this->step = 16;
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
		int	i;
		int	delta;

		mc->GetMenu(mc->focus)->GetPos(&x, &y);
		if (x == this->targetX && y == this->targetY)
			return ;
		if (x < this->targetX)
		{
			x = x + this->step;
			y = y + this->step;
			if (x > this->targetX)
			{
				x = this->targetX;
				y = this->targetY;
			}
		}
		else
		{
			x = x - this->step;
			y = y - this->step;
			if (x < this->targetX)
			{
				x = this->targetX;
				y = this->targetY;
			}

		}
		delta = 120;
		i = 0;
		while (i < mc->GetMaxMenus())
		{
			mc->GetMenu(i)->SetPos(x + (delta * (i - mc->focus)),
					       y + (delta * (i - mc->focus)));
			++i;
		}
	}

	void	ApplyEvent(Menu_Context<T>* mc, ID::Event* e)
	{
		if (e->type == ID::EVENT_KEY_DOWN)
		{
			if (e->keyDown.keysym.sym == 273)
			{
				if (mc->focus > 1)
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
