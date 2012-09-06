#ifndef	__AI_ASTAR_H__
#define	__AI_ASTAR_H__

#include <stdint.h>
#include <list>
#include <vector>
#include <queue>
#include "../Constantes.h"

namespace	ID
{
	class	Drawable;
	class	GRect;
};
class	Game;
class	Map;
class	Env;
class	Unit;

class	Node
{
	public:
		Node(double x, double y,
		     Node* parent,
		     Node* target);
		~Node();
		bool	operator==(const Node&) const;
		void	RecalculG(Node* parent);

	public:
		Node*		parent;
		double		x;
		double		y;
		uint16_t	f;
		uint16_t	g;
		uint16_t	h;
};

class	queueUnit
{
	public:
		queueUnit(Unit* u, double x, double y) : u(u),
						       tX(x),
						       tY(y)
		{
		}

	public:
		Unit*	u;
		double	tX;
		double	tY;
};

class	AI_AStar
{
	uint32_t	n;
	public:
				AI_AStar();
				~AI_AStar();
		Node*		Find(Unit*,
				     double xTarget, double yTarget);
		static int	PushQueue(Unit* u, double x, double y);

	public:
		static std::queue<queueUnit*>	toProcess;

	protected:
		Game*			game;
		Map*			map;
		Env*			env;
		Unit*			unit;
		Node*			startNode;
		Node*			endNode;
//		int			openListN;
//		std::list<Node*>	openList;
//		std::list<Node*>	closedList;
		std::list<ID::GRect*>	GRectList;

		uint32_t		numberofOpenListItems;
		uint32_t		squaresChecked;
		std::vector<uint32_t>	items;
		std::vector<Node*>	openList;
		std::vector<Node*>	closedList;

	private:
		Node*	__GetFirstOut(void);
		Node*	__GetBestNode(void);
		void	__AppendChildren(Node* n);
		bool	__CheckDiag(Node* n, int vX, int vY);
		void	__ClearNodes(Node* node);
		int	__IsThereBetterNode(Node* n);
		void	__AddNode(Node* node, int x, int y);
		void	__AddNodeOpenList(Node* n);
		void	__SwapToTop(uint32_t m);
//		std::list<Node*>::iterator	__FindNodeInList(Node*, std::list<Node*>* list);
		uint32_t	__FindNodeInList(Node*, std::vector<Node*>* list, uint32_t);
};

#endif
