#ifndef	__MOVE_HANDLER_H__
#define	__MOVE_HANDLER_H__

class	Node;
class	Unit;

class	Move_Handler
{
	public:
			Move_Handler(Unit*);
			~Move_Handler();
		virtual bool	CanMove(double x, double y) = 0;
		virtual int	Move() = 0;
		void		ClearPath(void);
		inline void	SetPath(Node* p)
		{
			this->ClearPath();
			this->_path = p;
		}
		inline Node*	GetPath(void)  const
		{
			return this->_path;
		}
	protected:
		Node*	_path;
		Unit*	_unit;
};

#endif
