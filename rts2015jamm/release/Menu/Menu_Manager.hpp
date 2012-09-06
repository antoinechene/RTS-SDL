#ifndef	__MENU_MANAGER_H__
#define	__MENU_MANAGER_H__

#include "../Singleton.hpp"
#include "Context_Updater.hpp"
#include <vector>

template <typename T>
class	Menu_Context
{
	public:
		Menu_Context(int n) : focus(0),
				      __maxMenus(n),
				      __updater(NULL)
		{
			this->__menus = new T*[n];
			if (this->__menus == NULL)
				throw 127;
		}
		~Menu_Context()
		{
			delete[] this->__menus;
		}
		inline T*	GetMenu(int n)
		{
			return this->__menus[n];
		}
		inline void	SetMenu(int index, T* m)
		{
			this->__menus[index] = m;
		}
		inline void	SetUpdater(Context_Updater<T>* u)
		{
			this->__updater = u;
		}
		inline int	GetMaxMenus()
		{
			return this->__maxMenus;
		}
		inline void	ApplyEvent(ID::Event* e)
		{
			if (this->__updater != NULL)
			{
				this->__updater->ApplyEvent(this, e);
			}
		}
		inline void	Update(void)
		{
			if (this->__updater != NULL)
			{
				this->__updater->Update(this);
			}
		}

	public:
		int	focus;

	private:
		T**			__menus;
		int			__maxMenus;
		Context_Updater<T>*	__updater;
};

template <typename T>
class	Menu_Manager : public Singleton< Menu_Manager<T> >
{
	friend class	Singleton< Menu_Manager<T> >;
	public:
		Menu_Manager()
		{}
		~Menu_Manager()
		{
		}
		Menu_Context<T>*	AddContext(int n)
		{
			Menu_Context<T>*	c;

			c = new Menu_Context<T>(n);
			if (c == NULL)
				return NULL;
			this->menus.push_back(c);
			return c;
		}

	public:
		std::vector<Menu_Context<T>*>	menus;
};

#endif
