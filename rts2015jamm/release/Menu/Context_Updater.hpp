#ifndef	__CONTEXT_UPDATER_HPP__
#define	__CONTEXT_UPDATER_HPP__

/*namespace	ID
{
	class	Event;
};
*/
template <typename T>
class	Menu_Context;

template <typename T>
class	Context_Updater
{
	public:
		virtual void	Init(Menu_Context<T>*) = 0;
		virtual void	Update(Menu_Context<T>*) = 0;
		virtual void	ApplyEvent(Menu_Context<T>*, ID::Event*) = 0;
};

#endif
