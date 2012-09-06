#include "AI_AStar.h"
#include "Unit.h"
#include "Game.h"
#include "Map.h"
#include "Move_Handlers/Move_Handler.h"
#include "../Env.h"
#include "../Constantes.h"
#include "../ID_glibrary.h"

std::queue<queueUnit*>	AI_AStar::toProcess = std::queue<queueUnit*>();

AI_AStar::AI_AStar() : game(Game::GetInstance()),
		       map(Map::GetInstance()),
		       env(Env::GetInstance()),
		       unit(NULL),
		       startNode(NULL),
		       endNode(NULL)
//		       openListN(0)
{
}

AI_AStar::~AI_AStar()
{
}

Node*	AI_AStar::Find(Unit* u,
		       double xE, double yE)
{
	Node*	n;
	double	x;
	double	y;

	u->GetPositionXY(&x, &y);
	std::cout << "Find("
		<< x
		<< ";"
		<< y
		<< " -> "
		<< xE
		<< ";"
		<< yE
		<< ")"
		<< std::endl;
	this->unit = u;
	this->n = 0;
	this->openList.clear();
	this->closedList.clear();
	this->GRectList.clear();
	this->endNode = new Node(xE, yE, NULL, NULL);
	if (this->endNode == NULL)
		return NULL;
	this->startNode = new Node(x, y, NULL, this->endNode);
	if (this->startNode == NULL)
	{
		delete this->endNode;
		return NULL;
	}
//	this->openList.push_front(this->startNode);
//	this->openListN = 1;

	this->openList.push_back(0);
	this->openList.push_back(this->startNode);
	this->numberofOpenListItems = 1;
	this->squaresChecked = 1;
//	while (this->openList.empty() == false)
//	while (this->openListN > 0)
	while (this->numberofOpenListItems > 0)
	{
		n = this->__GetFirstOut();
/*		n = this->openList.front();
		this->openList.pop_front();
		--this->openListN;*/
		this->closedList.push_back(n);
		if (*n == *this->endNode)
		{
			this->__ClearNodes(n);
			delete this->endNode;
			return n;
		}
		this->__AppendChildren(n);
	}
	n = this->__GetBestNode();
	this->__ClearNodes(n);
	delete this->endNode;
	return n;
}

int	AI_AStar::PushQueue(Unit* u, double x, double y)
{
	queueUnit*	qU;

	qU = new queueUnit(u, x, y);
	if (qU != NULL)
	{
		u->GetMoveHandler()->ClearPath();
		AI_AStar::toProcess.push(qU);
		return 0;
	}
	return -1;
}

Node*	AI_AStar::__GetBestNode(void)
{
//	std::list<Node*>::iterator	it;
//	std::list<Node*>::iterator	end;
	std::vector<Node*>::iterator	it;
	std::vector<Node*>::iterator	end;
	Node*				best;

	it = this->closedList.begin();
	end = this->closedList.end();
	best = NULL;
	while (it != end)
	{
		if (best == NULL ||
		    (best->h >= (*it)->h))
//		     (*it)->h != 0 &&
//		     best->g >= (*it)->g &&
//		     (*it)->g != 0))
			best = *it;
		++it;
	}
	return best;
}

Node*	AI_AStar::__GetFirstOut(void)
{
	Node*		first;
	Node*		tmp;
	uint32_t	v;
	uint32_t	u;

	first = this->openList[1];
	this->openList[1] = this->openList[this->numberofOpenListItems];
	--this->numberofOpenListItems;

	v = 1;
	while (1)
	{
		u = v;
		if ((u << 1) + 1 <= this->numberofOpenListItems)
		{
			if (this->openList[u]->f >= this->openList[u << 1]->f)
				v = u << 1;
			if (this->openList[v]->f >= this->openList[(u << 1) + 1]->f)
				v = (u << 1) + 1;
		}
		else if (u << 1 <= this->numberofOpenListItems)
		{
			if (this->openList[u]->f >= this->openList[u << 1]->f)
				v = u << 1;
		}

		if (u != v)
		{
			tmp = this->openList[u];
			this->openList[u] = this->openList[v];
			this->openList[v] = tmp;
		}
		else
			break;
	}
	return first;
}

void	AI_AStar::__AppendChildren(Node* n)
{
	int	vX;
	int	vY;

	vY = -1;
	DEBUG("n("<< n->x <<";"<< n->y <<")<" << n->f <<
		" = " << n->g << " + " << n->h <<
		"\n")
	while (vY <= 1)
	{
		vX = -1;
		while (vX <= 1)
		{
			if (vY == 0 && vX == 0)
			{
				DEBUG("		")
				vX = 1;
			}
			if (n->x + vX >= 0 &&
			    n->x + vX < this->map->w &&
			    n->y + vY >= 0 &&
			    n->y + vY < this->map->h &&
			    this->__CheckDiag(n, vX, vY) &&
			    this->unit->GetMoveHandler()->CanMove(n->x + vX, n->y + vY))
			{
				this->__AddNode(n, n->x + vX, n->y + vY);
				++this->n;
				DEBUG("		")
			}
			else
				DEBUG("X		")
			++vX;
		}
		++vY;
		DEBUG("\n")
	}
	DEBUG("----------------------\n")
}

bool	AI_AStar::__CheckDiag(Node* n, int vX, int vY)
{
	if (vX == 0 || vY == 0)
		return true;
	if (!this->unit->GetMoveHandler()->CanMove(n->x, n->y + vY) ||
	    !this->unit->GetMoveHandler()->CanMove(n->x + vX, n->y))
		return false;
	return true;
}

void	AI_AStar::__ClearNodes(Node* n)
{
	//std::list<Node*>::iterator	it;
	std::vector<Node*>::iterator	it;
	std::vector<Node*>::iterator	end;
	uint32_t			i;
	Node*				tmp;
	bool				avoid;

//	std::cout << "N=" <<this->n<< std::endl;

	it = this->openList.begin();
	i = 0;
	while (i < this->numberofOpenListItems)
//	while (it != this->openList.end())
	{
/*		tmp = n;
		avoid = false;
		while (tmp != NULL)
		{
			if (tmp == *it)
			{
				avoid = true;
				break ;
			}
			tmp = tmp->parent;
		}
		if (avoid == false)
*/			delete *it;
		++it;
		++i;
	}

	it = this->closedList.begin();
	end = this->closedList.end();
	while (it != end)
	{
		tmp = n;
		avoid = false;
		while (tmp != NULL)
		{
			if (tmp == *it)
			{
				avoid = true;
				break ;
			}
			tmp = tmp->parent;
		}
		if (avoid == false)
			delete *it;
		++it;
	}
	tmp = n;
	while (tmp->parent != NULL)
	{
		if (tmp->parent->g == 0)
		{
			delete tmp->parent;
			tmp->parent = NULL;
			return ;
		}
		tmp = tmp->parent;
	}
}

int	AI_AStar::__IsThereBetterNode(Node* n)
{
//	std::list<Node*>::iterator	it;
//	std::vector<Node*>::iterator	it;
	uint32_t			i;

	i = this->__FindNodeInList(n, &(this->openList), this->numberofOpenListItems);
	if (i != 0)
	{
//		if (n->g < (*it)->g)
		if (n->g < this->openList[i]->g)
		{
			//(*it)->RecalculG(n->parent);
			this->openList[i]->RecalculG(n->parent);
			//n = *it;
//			DEBUG("E(" << (*it)->x << ";" << (*it)->y << ")")
//			this->openList.erase(it);
//			DEBUG("A(" << n->x << ";" << n->y << ")")
			//this->__AddNodeOpenList(n);
			this->__SwapToTop(i);
			return 1;
		}
		return 0;
	}
	return -1;
}

void	AI_AStar::__AddNode(Node* n, int x, int y)
{
	Node*		newNode;

	newNode = new Node(x, y, n, this->endNode);
	if (newNode == NULL)
		return ;
	DEBUG(newNode->g << "-" << newNode->h << "	" << newNode)
	if (this->__IsThereBetterNode(newNode) == -1 &&
	    this->__FindNodeInList(newNode,
				   &(this->closedList), this->closedList.size()) == 0)
/*	if (this->__IsThereBetterNode(newNode) == -1 &&
	    this->__FindNodeInList(newNode,
				   &(this->closedList)) == this->closedList.end())
*/	{
		this->__AddNodeOpenList(newNode);
//		++this->openListN;
	}
	else
		delete newNode;
}

void	AI_AStar::__AddNodeOpenList(Node* n)
{
	++this->squaresChecked;
	++this->numberofOpenListItems;
	if (this->openList.size() > this->numberofOpenListItems)
		this->openList[this->numberofOpenListItems] = n;
	else
		this->openList.push_back(n);
	this->__SwapToTop(this->numberofOpenListItems);
}

void	AI_AStar::__SwapToTop(uint32_t m)
{
	Node*	tmp;

	while (m != 1)
	{
		if (this->openList[m]->f <= this->openList[m >> 1]->f)
		{
			tmp = this->openList[m >> 1];
			this->openList[m >> 1] = this->openList[m];
			this->openList[m] = tmp;
			m = m >> 1;
		}
		else
			break;
	}
	return ;
/*	std::list<Node*>::iterator	it;
	std::list<Node*>::iterator	end;
	std::list<Node*>::iterator	closest;

	it = this->openList.begin();
	end = this->openList.end();
	closest = it;
//	DEBUG("Add(" << n->x << ";" << n->y << ")")
	while (it != end)
	{
		if ((*it)->f > n->f)
		{
			closest = it;
			break ;
		}
		++it;
		if (it == end &&
		    (*closest)->f < n->f)
			closest = it;
	}
	this->openList.insert(closest, n);*/
}

//std::list<Node*>::iterator
//	AI_AStar::__FindNodeInList(Node* n, std::list<Node*>* l)
uint32_t
	AI_AStar::__FindNodeInList(Node* n, std::vector<Node*>* l, uint32_t max)
{
	//std::list<Node*>::iterator	it;
	//std::list<Node*>::iterator	end;
	uint32_t			i;
//	uint32_t			max;

	i = 0;
	if ((*l)[0] == 0)
		i = 1;
//	max = l->size();
/*	it = l->begin();
	if (*it == 0)
		++it;
	end = l->end();
	while (it != end)
*/	while (i < max)
	{
		if (n->x == (*l)[i]->x &&
		    n->y == (*l)[i]->y)
			return i;
//		++it;
		++i;
	}
	return 0;
}

Node::Node(double x, double y,
	   Node* p, Node* t) : parent(p),
			       x(x), y(y),
			       f(0), g(0), h(0)
{
	Map*	map;
	int	dX;
	int	dY;
	double	slow;

	map = Map::GetInstance();
	slow = map->cases[(int)this->y * map->w + (int)this->x].
		animation.
		GetSpriteAnime()->
		caract->
		slow;
	if (p != NULL)
	{
		if (this->x != p->x &&
		    this->y != p->y)
			this->g = p->g + 141 * slow;
		else
			this->g = p->g + 100 * slow;
	}
	if (t != NULL)
	{
		dX = ABS(this->x - t->x);
		dY = ABS(this->y - t->y);
		if (dX > dY)
			this->h = 141 * dY + 100 * (dX - dY);
		else
			this->h = 141 * dX + 100 * (dY - dX);
	}

	this->f = this->g + this->h;
}

Node::~Node()
{
}

bool	Node::operator==(const Node& n) const
{
	if (this->x == n.x && this->y == n.y)
		return true;
	return false;
}

void	Node::RecalculG(Node* p)
{
	this->parent = p;
	if (this->x != this->parent->x &&
	    this->y != this->parent->y)
		this->g = this->parent->g + 14;
	else
		this->g = this->parent->g + 10;
	this->f = this->g + this->h;
}
