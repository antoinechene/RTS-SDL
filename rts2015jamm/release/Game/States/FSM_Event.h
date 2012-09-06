#ifndef	__FSM_EVENT_H__
#define	__FSM_EVENT_H__

#include <stdint.h>

class	Unit;

enum	FSM_EVENT
{
	FSM_ORDER_MOVE = 0,
	FSM_ORDER_FOLLOW,
	FSM_ORDER_HOLD,
	FSM_ORDER_ATTACK,
};

typedef struct
{
	uint8_t	type;
	double	x;
	double	y;
}		FSM_Event_Order_Move;

typedef struct
{
	uint8_t	type;
	Unit*	unit;
}		FSM_Event_Order_Follow;

typedef struct
{
	uint8_t	type;
	bool	hold;
}		FSM_Event_Order_Hold;

typedef struct
{
	uint8_t	type;
	double	x;
	double	y;
	Unit*	unit;
}		FSM_Event_Order_Attack;

typedef union
{
	uint8_t			type;
	FSM_Event_Order_Move	move;
	FSM_Event_Order_Follow	follow;
	FSM_Event_Order_Hold	hold;
	FSM_Event_Order_Attack	attack;
}	FSM_Event;

#endif
