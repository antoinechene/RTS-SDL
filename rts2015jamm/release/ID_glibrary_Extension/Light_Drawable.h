#ifndef	__LIGHT_DRAWABLE_H__
#define	__LIGHT_DRAWABLE_H__

#include "../ID_glibrary.h"
#include "../Game/Sprite_Anime.h"

class	Light_Drawable : public ID::Drawable
{
	public:
		Light_Drawable();
		~Light_Drawable();

		inline void	SetInMap(bool data)
		{
			this->_inMap = data;
		}
		inline bool	GetInMap(void) const
		{
			return this->_inMap;
		}
		inline void	SetPosInvalid(int x, int y)
		{
			this->SetInvalidate(true);
			this->_AddUpdateRect();
			this->_x = x;
			this->_y = y;
			this->_AddUpdateRect();
			this->SetInvalidate(true);
		}
		inline void	SetPos(int x, int y)
		{
			this->_x = x;
			this->_y = y;
		}
		inline void	SetPicture(Animation* p)
		{
			this->_animation = *p;
			this->_w = this->GetWidth();
			this->_h = this->GetHeight();
			this->_AddUpdateRect();
			this->SetInvalidate(true);
		}
		inline Animation*	GetPicture()
		{
			return &(this->_animation);
		}
		inline uint16_t	GetWidth(void) const
		{
			return (this->_animation.GetSpriteAnime()->GetSprite() != 0) ? this->_animation.GetSpriteAnime()->GetSprite()->w : 0;
//			return this->_animation.GetSpriteAnime()->GetSprite()->w;
		}
		inline uint16_t	GetHeight(void) const
		{
			return (this->_animation.GetSpriteAnime()->GetSprite()->h != 0) ? this->_animation.GetSpriteAnime()->GetSprite()->h : 0;
//			return this->_animation.GetSpriteAnime()->GetSprite()->h;
		}

		virtual int	RefreshToSurface(ID::Surface* surface,
						 int x, int y);

		virtual int 	Click(ID::Event* event);
		virtual int 	DoubleClick(ID::Event* event);
		virtual int 	ClickDown(ID::Event* event);
		virtual int 	ClickUp(ID::Event* event);
		virtual int 	Enter(ID::Event* event);
		virtual int 	Leave(ID::Event* event);
		virtual int 	Motion(ID::Event* event);
		virtual int 	KeyUp(ID::Event* event);
		virtual int 	KeyDown(ID::Event* event);
		virtual int 	FocusEnter(ID::Event* event);
		virtual int 	FocusLeave(ID::Event* event);
		virtual int 	EnableChange(ID::Event* event);
		virtual int 	ScreenChange(ID::Event* event);
		virtual int 	Resize(ID::Event* event);
		virtual int 	Quit(ID::Event* event);
		virtual int 	UserEvent(ID::Event* event);

		virtual bool	_OnClick(ID::Event* event);
		virtual bool	_OnDoubleClick(ID::Event* event);
		virtual bool	_OnClickDown(ID::Event* event);
		virtual bool	_OnClickUp(ID::Event* event);
		virtual bool	_OnEnter(ID::Event* event);
		virtual bool	_OnLeave(ID::Event* event);
		virtual bool	_OnMotion(ID::Event* event);
		virtual bool	_OnKeyUp(ID::Event* event);
		virtual bool	_OnKeyDown(ID::Event* event);
		virtual bool	_OnFocusEnter(ID::Event* event);
		virtual bool	_OnFocusLeave(ID::Event* event);
		virtual bool	_OnEnableChange(ID::Event* event);
		virtual bool	_OnScreenChange(ID::Event* event);
		virtual bool	_OnResize(ID::Event* event);
		virtual bool	_OnQuit(ID::Event* event);
		virtual bool	_OnUserEvent(ID::Event* event);

	protected:
		inline void	_AddUpdateRect(void)
		{
			ID::Window*	w;

			w = this->GetRealWindow();
			if (w != NULL)
			{
				ID::Rect*	size;

				size = this->_animation.GetSpriteAnime()->GetRects();
				ID::Rect	rect(this->_offset_x + this->_x - (size->w >> 1),
						     this->_offset_y + this->_y - (size->h >> 1),
						     size->w,
						     size->h);
				w->AddUpdate(&rect);
			}
		}
		inline void	_AddUpdateRectAnime(void)
		{
			ID::Window*	w;

			w = this->GetRealWindow();
			if (w != NULL)
			{
				ID::Rect*	size;

				size = this->_animation.GetSpriteAnime()->GetRects();
				ID::Rect	rect(this->_offset_x + this->_x - (size->w),
						     this->_offset_y + this->_y - (size->h >> 1),
						     size->w,
						     size->h);
				w->AddUpdate(&rect);
			}
		}
	protected:
		bool		_inMap;
		Animation	_animation;
		int		_lastIndex;
};

#endif
