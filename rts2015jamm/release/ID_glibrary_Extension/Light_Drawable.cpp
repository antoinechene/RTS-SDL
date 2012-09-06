#include "Light_Drawable.h"
#include "../Game/Cam.h"
#include "../Constantes.h"

void	__Light_Drawable__CbClickDown(ID::Control*, ID::Event*, void*)
{
	std::cout << "LD_ClickDown" << std::endl;
}

Light_Drawable::Light_Drawable() : _inMap(false), _lastIndex(-1)
{
	this->SetName("Light Drawable");
	this->SetCbClickDown(__Light_Drawable__CbClickDown, NULL);
	this->SetEnable(true);
}

Light_Drawable::~Light_Drawable()
{
}

int	Light_Drawable::RefreshToSurface(ID::Surface* s,
					 int x, int y)
{
	if (this->_animation.GetSpriteAnime() == NULL)
		return 0;
	int	index;

	index = this->_animation.Animate();
	if (index != this->_lastIndex)
	{
		int	halfCasePixels;

		halfCasePixels = CASE_SIDE_PIXELS >> 1;
		Cam::GetInstance()->InvalidateCases((float)(x + this->_x + halfCasePixels) / CASE_SIDE_PIXELS,
						    (float)(y + this->_y + halfCasePixels) / CASE_SIDE_PIXELS,
						    halfCasePixels);
		this->_lastIndex = index;
	}

	if (this->_animation.
		GetSpriteAnime()->
		GetSprite() != NULL &&
	    this->_animation.
		GetSpriteAnime()->
		GetSprite()->
		BlitSurface(&(this->_animation.
			      GetSpriteAnime()->
			      GetRects()[index]),
			    s,
			    x + this->_x,
			    y + this->_y) == -1)
		return -1;
	this->SetInvalidate(false);
	return 0;
}
