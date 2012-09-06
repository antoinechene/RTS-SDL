#ifndef	__ID_GLIBRARY_H__
#define	__ID_GLIBRARY_H__

#include <stdint.h>
#include <pthread.h>
#include <iostream>
#include <list>
#include <vector>
#include <map>

#ifdef	__SDL
#include <SDL/SDL.h>
#include <SDL/SDL_ttf.h>
#endif



namespace ID
{
 int Init(int flags);
 int InitWrappers(void);
 int DestroyWrappers();
 void Quit(void);
 void About(void);
};
namespace ID
{
 struct Point
 {
  Point(int16_t _x = 0, int16_t _y = 0) : x(_x), y(_y)
  {
  }

  Point(const Point& p) : x(p.x), y(p.y)
  {
  }

  Point& operator=(const Point& p)
  {
   this->x = p.x;
   this->y = p.y;
   return *this;
  }

  int16_t x;
  int16_t y;
 };

 struct Line
 {
  Line(int16_t x1 = 0, int16_t y1 = 0, int16_t x2 = 0, int16_t y2 = 0) : p1(x1, y1), p2(x2, y2)
  {
  }

  Line(Point _p1, Point _p2) : p1(_p1), p2(_p2)
  {
  }

  Line(const Line& l) : p1(l.p1), p2(l.p2)
  {
  }

  Line& operator=(const Line& l)
  {
   this->p1 = l.p1;
   this->p2 = l.p2;
   return *this;
  }

  Point p1;
  Point p2;
 };

 struct Circle
 {
  Circle(int16_t _x = 0, int16_t _y = 0, double _radius = 0) : center(_x, _y), radius(_radius)
  {
  }

  Circle(Point p, double _radius = 0) : center(p), radius(_radius)
  {
  }

  Circle(const Circle& c) : center(c.center), radius(c.radius)
  {
  }

  Circle& operator=(const Circle& c)
  {
   this->center = c.center;
   this->radius = c.radius;
   return *this;
  }

  Point center;
  double radius;
 };

 struct Rect
 {
  Rect(int16_t _x = 0, int16_t _y = 0,
       uint16_t _w = 0, uint16_t _h = 0) : x(_x), y(_y),
        w(_w), h(_h)
  {
  }

  Rect(const Rect& r) : x(r.x), y(r.y),
          w(r.w), h(r.h)
  {
  }

  Rect& operator=(const Rect& r)
  {
   this->x = r.x;
   this->y = r.y;
   this->w = r.w;
   this->h = r.h;
   return *this;
  }

  int16_t x;
  int16_t y;
  uint16_t w;
  uint16_t h;
 };

 typedef struct Cardinal_Size
 {
  Cardinal_Size(uint16_t _top = 0, uint16_t _bot = 0,
         uint16_t _left = 0, uint16_t _right = 0) : top(_top), bot(_bot),
        left(_left), right(_right)
  {
  }

  Cardinal_Size(const Cardinal_Size& cs) : top(cs.top), bot(cs.bot),
        left(cs.left), right(cs.right)
  {
  }

  Cardinal_Size& operator=(const Cardinal_Size& cs)
  {
   this->top = cs.top;
   this->bot = cs.bot;
   this->left = cs.left;
   this->right = cs.right;
   return *this;
  }

  uint16_t top;
  uint16_t bot;
  uint16_t left;
  uint16_t right;
 } Border;

 struct Color
 {
  Color(void) : r(0), g(0), b(0), a(0)
  {
  }

  Color(uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a) : r(_r), g(_g), b(_b), a(_a)
  {
  }

  Color& operator=(const Color& c)
  {
   this->r = c.r;
   this->g = c.g;
   this->b = c.b;
   this->a = c.a;
   return *this;
  }

  Color& operator=(uint32_t& c)
  {
   this->r = c >> 24;
   this->g = c >> 16;
   this->b = c >> 8;
   this->a = c;
   return *this;
  }

  inline uint32_t ConvertToInt(uint8_t rShift = 16,
          uint8_t gShift = 8,
          uint8_t bShift = 0,
          uint8_t aShift = 24) const
  {
   uint32_t c;

   c = (this->r << rShift) |
       (this->g << gShift) |
       (this->b << bShift) |
       (this->a << aShift);
   return c;
  }
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
 };
};



namespace ID
{
 typedef enum
 {




  KEY_UNKNOWN = 0,
  KEY_FIRST = 0,
  KEY_BACKSPACE = 8,
  KEY_TAB = 9,
  KEY_CLEAR = 12,
  KEY_RETURN = 13,
  KEY_PAUSE = 19,
  KEY_ESCAPE = 27,
  KEY_SPACE = 32,
  KEY_EXCLAIM = 33,
  KEY_QUOTEDBL = 34,
  KEY_HASH = 35,
  KEY_DOLLAR = 36,
  KEY_AMPERSAND = 38,
  KEY_QUOTE = 39,
  KEY_LEFTPAREN = 40,
  KEY_RIGHTPAREN = 41,
  KEY_ASTERISK = 42,
  KEY_PLUS = 43,
  KEY_COMMA = 44,
  KEY_MINUS = 45,
  KEY_PERIOD = 46,
  KEY_SLASH = 47,
  KEY_0 = 48,
  KEY_1 = 49,
  KEY_2 = 50,
  KEY_3 = 51,
  KEY_4 = 52,
  KEY_5 = 53,
  KEY_6 = 54,
  KEY_7 = 55,
  KEY_8 = 56,
  KEY_9 = 57,
  KEY_COLON = 58,
  KEY_SEMICOLON = 59,
  KEY_LESS = 60,
  KEY_EQUALS = 61,
  KEY_GREATER = 62,
  KEY_QUESTION = 63,
  KEY_AT = 64,
  KEY_LEFTBRACKET = 91,
  KEY_BACKSLASH = 92,
  KEY_RIGHTBRACKET = 93,
  KEY_CARET = 94,
  KEY_UNDERSCORE = 95,
  KEY_BACKQUOTE = 96,
  KEY_a = 97,
  KEY_b = 98,
  KEY_c = 99,
  KEY_d = 100,
  KEY_e = 101,
  KEY_f = 102,
  KEY_g = 103,
  KEY_h = 104,
  KEY_i = 105,
  KEY_j = 106,
  KEY_k = 107,
  KEY_l = 108,
  KEY_m = 109,
  KEY_n = 110,
  KEY_o = 111,
  KEY_p = 112,
  KEY_q = 113,
  KEY_r = 114,
  KEY_s = 115,
  KEY_t = 116,
  KEY_u = 117,
  KEY_v = 118,
  KEY_w = 119,
  KEY_x = 120,
  KEY_y = 121,
  KEY_z = 122,
  KEY_DELETE = 127,





  KEY_WORLD_0 = 160,
  KEY_WORLD_1 = 161,
  KEY_WORLD_2 = 162,
  KEY_WORLD_3 = 163,
  KEY_WORLD_4 = 164,
  KEY_WORLD_5 = 165,
  KEY_WORLD_6 = 166,
  KEY_WORLD_7 = 167,
  KEY_WORLD_8 = 168,
  KEY_WORLD_9 = 169,
  KEY_WORLD_10 = 170,
  KEY_WORLD_11 = 171,
  KEY_WORLD_12 = 172,
  KEY_WORLD_13 = 173,
  KEY_WORLD_14 = 174,
  KEY_WORLD_15 = 175,
  KEY_WORLD_16 = 176,
  KEY_WORLD_17 = 177,
  KEY_WORLD_18 = 178,
  KEY_WORLD_19 = 179,
  KEY_WORLD_20 = 180,
  KEY_WORLD_21 = 181,
  KEY_WORLD_22 = 182,
  KEY_WORLD_23 = 183,
  KEY_WORLD_24 = 184,
  KEY_WORLD_25 = 185,
  KEY_WORLD_26 = 186,
  KEY_WORLD_27 = 187,
  KEY_WORLD_28 = 188,
  KEY_WORLD_29 = 189,
  KEY_WORLD_30 = 190,
  KEY_WORLD_31 = 191,
  KEY_WORLD_32 = 192,
  KEY_WORLD_33 = 193,
  KEY_WORLD_34 = 194,
  KEY_WORLD_35 = 195,
  KEY_WORLD_36 = 196,
  KEY_WORLD_37 = 197,
  KEY_WORLD_38 = 198,
  KEY_WORLD_39 = 199,
  KEY_WORLD_40 = 200,
  KEY_WORLD_41 = 201,
  KEY_WORLD_42 = 202,
  KEY_WORLD_43 = 203,
  KEY_WORLD_44 = 204,
  KEY_WORLD_45 = 205,
  KEY_WORLD_46 = 206,
  KEY_WORLD_47 = 207,
  KEY_WORLD_48 = 208,
  KEY_WORLD_49 = 209,
  KEY_WORLD_50 = 210,
  KEY_WORLD_51 = 211,
  KEY_WORLD_52 = 212,
  KEY_WORLD_53 = 213,
  KEY_WORLD_54 = 214,
  KEY_WORLD_55 = 215,
  KEY_WORLD_56 = 216,
  KEY_WORLD_57 = 217,
  KEY_WORLD_58 = 218,
  KEY_WORLD_59 = 219,
  KEY_WORLD_60 = 220,
  KEY_WORLD_61 = 221,
  KEY_WORLD_62 = 222,
  KEY_WORLD_63 = 223,
  KEY_WORLD_64 = 224,
  KEY_WORLD_65 = 225,
  KEY_WORLD_66 = 226,
  KEY_WORLD_67 = 227,
  KEY_WORLD_68 = 228,
  KEY_WORLD_69 = 229,
  KEY_WORLD_70 = 230,
  KEY_WORLD_71 = 231,
  KEY_WORLD_72 = 232,
  KEY_WORLD_73 = 233,
  KEY_WORLD_74 = 234,
  KEY_WORLD_75 = 235,
  KEY_WORLD_76 = 236,
  KEY_WORLD_77 = 237,
  KEY_WORLD_78 = 238,
  KEY_WORLD_79 = 239,
  KEY_WORLD_80 = 240,
  KEY_WORLD_81 = 241,
  KEY_WORLD_82 = 242,
  KEY_WORLD_83 = 243,
  KEY_WORLD_84 = 244,
  KEY_WORLD_85 = 245,
  KEY_WORLD_86 = 246,
  KEY_WORLD_87 = 247,
  KEY_WORLD_88 = 248,
  KEY_WORLD_89 = 249,
  KEY_WORLD_90 = 250,
  KEY_WORLD_91 = 251,
  KEY_WORLD_92 = 252,
  KEY_WORLD_93 = 253,
  KEY_WORLD_94 = 254,
  KEY_WORLD_95 = 255,




  KEY_KP0 = 256,
  KEY_KP1 = 257,
  KEY_KP2 = 258,
  KEY_KP3 = 259,
  KEY_KP4 = 260,
  KEY_KP5 = 261,
  KEY_KP6 = 262,
  KEY_KP7 = 263,
  KEY_KP8 = 264,
  KEY_KP9 = 265,
  KEY_KP_PERIOD = 266,
  KEY_KP_DIVIDE = 267,
  KEY_KP_MULTIPLY = 268,
  KEY_KP_MINUS = 269,
  KEY_KP_PLUS = 270,
  KEY_KP_ENTER = 271,
  KEY_KP_EQUALS = 272,




  KEY_UP = 273,
  KEY_DOWN = 274,
  KEY_RIGHT = 275,
  KEY_LEFT = 276,
  KEY_INSERT = 277,
  KEY_HOME = 278,
  KEY_END = 279,
  KEY_PAGEUP = 280,
  KEY_PAGEDOWN = 281,




  KEY_F1 = 282,
  KEY_F2 = 283,
  KEY_F3 = 284,
  KEY_F4 = 285,
  KEY_F5 = 286,
  KEY_F6 = 287,
  KEY_F7 = 288,
  KEY_F8 = 289,
  KEY_F9 = 290,
  KEY_F10 = 291,
  KEY_F11 = 292,
  KEY_F12 = 293,
  KEY_F13 = 294,
  KEY_F14 = 295,
  KEY_F15 = 296,




  KEY_NUMLOCK = 300,
  KEY_CAPSLOCK = 301,
  KEY_SCROLLOCK = 302,
  KEY_RSHIFT = 303,
  KEY_LSHIFT = 304,
  KEY_RCTRL = 305,
  KEY_LCTRL = 306,
  KEY_RALT = 307,
  KEY_LALT = 308,
  KEY_RMETA = 309,
  KEY_LMETA = 310,
  KEY_LSUPER = 311,
  KEY_RSUPER = 312,
  KEY_MODE = 313,
  KEY_COMPOSE = 314,




  KEY_HELP = 315,
  KEY_PRINT = 316,
  KEY_SYSREQ = 317,
  KEY_BREAK = 318,
  KEY_MENU = 319,
  KEY_POWER = 320,
  KEY_EURO = 321,
  KEY_UNDO = 322,

 } Key;

 typedef enum
 {
  KEYMOD_NONE = 0x0000,
  KEYMOD_LSHIFT= 0x0001,
  KEYMOD_RSHIFT= 0x0002,
  KEYMOD_LCTRL = 0x0040,
  KEYMOD_RCTRL = 0x0080,
  KEYMOD_LALT = 0x0100,
  KEYMOD_RALT = 0x0200,
  KEYMOD_LMETA = 0x0400,
  KEYMOD_RMETA = 0x0800,
  KEYMOD_NUM = 0x1000,
  KEYMOD_CAPS = 0x2000,
  KEYMOD_MODE = 0x4000,
  KEYMOD_RESERVED = 0x8000
 } Mod;

 typedef enum
 {
  BUTTON_NONE = 0,
  BUTTON_LEFT,
  BUTTON_MID,
  BUTTON_RIGHT
 } Mouse_Button;






};
namespace ID
{
 typedef enum
 {
  EVENT_CLICK = 1,
  EVENT_DOUBLE_CLICK,
  EVENT_CLICK_DOWN,
  EVENT_CLICK_UP,
  EVENT_ENTER,
  EVENT_LEAVE,
  EVENT_MOTION,
  EVENT_KEY_UP,
  EVENT_KEY_DOWN,
  EVENT_FOCUS_ENTER,
  EVENT_FOCUS_LEAVE,
  EVENT_ENABLE_CHANGE,
  EVENT_SCREEN_CHANGE,
  EVENT_RESIZE,
  EVENT_QUIT,
  EVENT_USER_EVENT
 } EVENT_TYPE;

 typedef struct
 {
  uint8_t scancode;
  Key sym;
  Mod mod;
 } Keysym;

 typedef struct
 {
  uint8_t type;
  Mouse_Button button;
  uint16_t x;
  uint16_t y;
 } t_Click, t_DoubleClick, t_ClickDown, t_ClickUp;

 typedef struct
 {
  uint8_t type;
 } t_Enter, t_Leave;

 typedef struct
 {
  uint8_t type;
  uint8_t state;
  uint16_t x;
  uint16_t y;
  int16_t x_rel;
  int16_t y_rel;
 } t_Motion;

 typedef struct
 {
  uint8_t type;
  Keysym keysym;
 } t_KeyUp, t_KeyDown;

 typedef struct
 {
  uint8_t type;
 } t_FocusEnter, t_FocusLeave;

 typedef struct
 {
  uint8_t type;
  bool isEnable;
 } t_EnableChange;

 typedef struct
 {
  uint8_t type;
  bool isOnScreen;
 } t_ScreenChange;

 typedef struct
 {
  uint8_t type;
  int w;
  int h;
 } t_Resize;

 typedef struct
 {
  uint8_t type;
 } t_Quit;

 typedef union
 {
  uint8_t type;
  t_Click click;
  t_DoubleClick doubleClick;
  t_ClickDown clickDown;
  t_ClickUp clickUp;
  t_Enter enter;
  t_Leave leave;
  t_Motion motion;
  t_KeyUp keyUp;
  t_KeyDown keyDown;
  t_FocusEnter focusEnter;
  t_FocusLeave focusLeave;
  t_EnableChange enableChange;
  t_ScreenChange screenChange;
  t_Resize resize;
  t_Quit quit;
 } Event;
};
namespace ID
{
 class Surface
 {
  friend class Wrapper_Interface;
  typedef SDL_Surface surface;

  public:
     Surface(void);
     Surface(uint16_t width, uint16_t height, uint32_t flags,
      uint32_t rmask, uint32_t gmask,
      uint32_t bmask, uint32_t amask);
     Surface(uint16_t width, uint16_t height, uint32_t flags,
      bool is_set_video_mode);
     Surface(const char* img_to_load);
     ~Surface();
   Surface& operator=(const Surface& surface);
   int Update(Rect* rect);
   Surface* Clone(void);
   int Resize(uint16_t width, uint16_t height, uint32_t flags = SDL_SWSURFACE,
            uint32_t rmask = 0, uint32_t gmask = 0, uint32_t bmask = 0, uint32_t amask = 0);
   int ResizeVideoMode(uint16_t width, uint16_t height);
   int LoadImage(const char* img_to_load,
        int x = 0, int y = 0,
        bool resize = true);
   void SetPixelTrans(uint16_t x, uint16_t y, Color* c);
   void SetPixelTrans(uint16_t x, uint16_t y, uint32_t color);
   void SetPixel(uint16_t x, uint16_t y, uint32_t color);
   void SetPixel(uint16_t x, uint16_t y, Color* color);
   uint32_t GetPixel(uint16_t x, uint16_t y);
   int FillRect(Rect* rect, Color* color);
   int FillRect(Rect* rect, uint32_t color);
   int BlitSurface(Rect* src_rect,
          Surface* dest,
          int dest_x, int dest_y);
   int SetColorKey(uint32_t flags, uint32_t key);
   int TTF_New_Text(const char* font_path, int font_size,
           const char* text, Color fg, int x,
           int y);
   void GetRGBA(uint32_t color, Color* s_color);
   uint32_t MapRGBA(uint8_t red, uint8_t green,
      uint8_t blue, uint8_t alpha);
   int SetAlpha(uint32_t flags, uint8_t alpha);
   uint32_t GetRmask(void);
   uint32_t GetGmask(void);
   uint32_t GetBmask(void);
   uint32_t GetAmask(void);

  public:
   const uint32_t& flags;
   const uint16_t& w;
   const uint16_t& h;
   const uint16_t& pitch;
   void* pixels;

  private:
   uint32_t __flags;
   uint16_t __w;
   uint16_t __h;
   uint16_t __pitch;
   bool __isSetVideoMode;
   surface* __surface;
 };
}



namespace ID
{
 template <typename T>
 class Singleton
 {
  public:
   inline static T* GetInstance(void)
   {
    if (_instance == 0)
     _instance = new T;
    return _instance;
   };
   static void Kill(void)
   {
    delete _instance;
    _instance = 0;
   };
  protected:
   Singleton<T>(void)
   {

   }
   virtual ~Singleton<T>(void)
   {
   }
  private:
   static T* _instance;
 };
};

template <typename T>
T* ID::Singleton<T>::_instance = 0;

namespace ID
{
 class Font : public Singleton<Font>
 {
  friend class Singleton<Font>;
  public:
   class Police
   {
    public:
     Police(const char* path, int size);
     ~Police();
     TTF_Font* font;
     const char* path;
     int size;
   };
  public:
   Police* LoadFont(const char* path, int size);
   ID::Surface* TextRender(const char* text, Police* font, ID::Color* fg);
   int SizeText(TTF_Font* font, const char* text, int* w, int* h);
  protected:
   Police* _SearchFont(const char* font_path, int font_size);
  private:
   Font();
   ~Font();
  private:
   std::vector<Police*> _polices;
 };
}






namespace ID
{
 class Keyboard_Manager : public Singleton<Keyboard_Manager>
 {
  friend class Singleton<Keyboard_Manager>;
  public:
   void Write(char* str, Mod mod, Key sym, unsigned int maxLength);

  private:
   Keyboard_Manager();
   ~Keyboard_Manager();
 };
};







namespace ID
{
 const Color COLOR_BLACK = Color(0, 0, 0, 255);
 const Color COLOR_WHITE = Color(0xFF, 0xFF, 0xFF, 255);
 const Color COLOR_GREY = Color(0x80, 0x80, 0x80, 255);
 const Color COLOR_RED = Color(0xFF, 0, 0, 255);
 const Color COLOR_GREEN = Color(0, 0x80, 0, 255);
 const Color COLOR_BLUE = Color(0, 0, 0xFF, 255);
 const Color COLOR_BROWN = Color(0x80, 0, 0, 255);
 const Color COLOR_PURPLE = Color(0x80, 0, 0x80, 255);
 const Color COLOR_SILVER = Color(0xC0, 0xC0, 0xC0, 255);
 const Color COLOR_YELLOW = Color(0xFF, 0xFF, 0, 255);
 const Color COLOR_PINK = Color(0xFF, 0xC0, 0xCB,255);
 const Color COLOR_ORANGE = Color(0xFF, 0xA5, 0, 255);
 const Color COLOR_GOLD = Color(0xFF, 0xD7, 0, 255);
};

namespace ID
{
 class Surface;
 class Draw_Geometry_Rect : public Singleton<Draw_Geometry_Rect>
 {
  friend class Singleton<Draw_Geometry_Rect>;
  public:
   void Draw(Surface* surface,
          Rect* line,
          bool fulfil,
          uint32_t color = COLOR_BLACK.ConvertToInt());
   void Draw(Surface* surface,
          int16_t x, int16_t y,
          int16_t width, int16_t height,
          bool fulfil,
          uint32_t color = COLOR_BLACK.ConvertToInt());
  private:
    Draw_Geometry_Rect();
    ~Draw_Geometry_Rect();
private:
inline uint8_t __Alpha(uint8_t src, uint8_t dst, uint8_t a)
{
 return (src * a + (256 - a) * dst) >> 8;
}

inline void __Rect(int16_t x, int16_t y,
         int16_t w, int16_t h,
         bool f)
{
 int16_t minX;
 int16_t minY;
 int16_t maxX;
 int16_t maxY;
 uint8_t* minP;
 uint8_t* maxP;
 uint8_t* tmp_saver;
 int32_t bpp;

 if (0 <= w)
 {
  minX = x;
  maxX = x + w;
 }
 else
 {
  minX = x + w + 1;
  maxX = x + 1;
 }
 if (0 <= h)
 {
  minY = y;
  maxY = y + h;
 }
 else
 {
  minY = y + h + 1;
  maxY = y + 1;
 }

 bpp = 4;
 x = minX;
 --maxY;
 minP = (uint8_t*)this->__target->pixels +
        minY * this->__target->pitch +
        minX * bpp;

 tmp_saver = minP;
 maxP = (uint8_t*)minP + (maxY - minY) * this->__target->pitch;
 while (x < maxX)
 {
  *(uint32_t*)minP = this->__color;
  *(uint32_t*)maxP = this->__color;
  minP = minP + bpp;
  maxP = maxP + bpp;
  ++x;
 }
 maxP = (uint8_t*)minP + this->__target->pitch - bpp;
 minP = tmp_saver + this->__target->pitch;
 ++minY;
 --maxY;
 --maxX;
 while (minY <= maxY)
 {
  *(uint32_t*)minP = this->__color;
    *(uint32_t*)maxP = this->__color;
    maxP = maxP + this->__target->pitch;
  if (f == false)
  {
   minP = minP + this->__target->pitch;
   ++minY;
   continue ;
  }
  tmp_saver = minP;
  x = minX + 1;
  minP = (uint8_t*)minP + bpp;
  while (x < maxX)
  {

   minP[0] = this->__Alpha(this->__color & 0xFF,
      minP[0],
      this->__color >> 24 & 0xFF);
   minP[1] = this->__Alpha(this->__color >> 8 & 0xFF,
      minP[1],
      this->__color >> 24 & 0xFF);
   minP[2] = this->__Alpha(this->__color >> 16 & 0xFF,
      minP[2],
      this->__color >> 24 & 0xFF);
   minP = minP + bpp;
   ++x;
  }
  ++minY;
  minP = tmp_saver + this->__target->pitch;
 }
}

  private:
   Surface* __target;
   uint32_t __color;
 };
};







namespace ID
{
 class Surface;
 class Draw_Geometry_Line : public Singleton<Draw_Geometry_Line>
 {
  friend class Singleton<Draw_Geometry_Line>;
  public:
   void Draw(Surface* surface,
          Line* line, int size,
          Color* color = (Color*)&(COLOR_BLACK));
   void Draw(Surface* surface,
          int16_t x1, int16_t y1,
          int16_t x2, int16_t y2,
          int size,
          Color* color = (Color*)&(COLOR_BLACK));
  private:
    Draw_Geometry_Line();
    ~Draw_Geometry_Line();

  private:
   void __Line(int x1, int y1, int x2, int y2);
   void __PixelBrush(int x_center, int y_center);
   void __PixelBrush_1(int x_center, int y_center);
   void __PixelBrush_2(int x_center, int y_center);
   void __PixelBrush_3(int x_center, int y_center);
   void __PixelBrush_4(int x_center, int y_center);
   void __PixelBrush_5(int x_center, int y_center);
   void __PixelBrush_6(int x_center, int y_center);
   void __PixelBrush_7(int x_center, int y_center);
   void __PixelBrush_8(int x_center, int y_center);
  private:
   int __lastX;
   int __lastY;
   int __size;
   Surface* __target;
   Color* __color;
 };
};







namespace ID
{
 class Surface;
 class Draw_Geometry_Circle : public Singleton<Draw_Geometry_Circle>
 {
  friend class Singleton<Draw_Geometry_Circle>;
  public:
   void Draw(Surface* surface,
          Circle* line,
          bool fulfil,
          Color* color = (Color*)&(COLOR_BLACK));
   void Draw(Surface* surface,
          int16_t x, int16_t y,
          int radius,
          bool fulfil,
          Color* color = (Color*)&(COLOR_BLACK));
  private:
    Draw_Geometry_Circle();
    ~Draw_Geometry_Circle();
   void __Circle(int x, int y, int radius,
      bool fulfil);
   float __Distance(int xa, int ya,
        int xb, int yb);
   float __Sqrt(float n);

  private:
   Surface* __target;
   Color* __color;
 };
};


























namespace ID
{
 class Wrapper_Event_Template
 {
 public:
  static void* WrapEvent(void*);
  virtual int Init(void) = 0;
  virtual void Destroy(void) = 0;
  virtual int WaitEvent(ID::Event* event) = 0;
  virtual int PollEvent(ID::Event* event) = 0;
 private:
  pthread_t __id_thread;
 private:
  pthread_mutex_t __mutex;
 };
};


namespace ID
{
 class Control;
 class Wrapper_Event : public Wrapper_Event_Template,
    public Singleton<ID::Wrapper_Event>
 {
   friend class Singleton<ID::Wrapper_Event>;

  public:
   static void* WrapEvent(void*);
   int Init(void);
   void Destroy(void);
   int WaitEvent(Event* event);
   int PollEvent(Event* event);
   void AddKeyUpControl(Control* control);
   void AddKeyDownControl(Control* control);
   void DelKeyUpControl(Control* control);
   void DelKeyDownControl(Control* control);
   void ApplyKeyUp(ID::Event* e);
   void ApplyKeyDown(ID::Event* e);

  private:
   Wrapper_Event(void);
   ~Wrapper_Event(void);

  private:
   pthread_t __id_thread;
   bool __alive;
   pthread_mutex_t __mutex;
   Event* __events_queue;
   int __events_size;
   std::list<Control*> __keyDownControl;
   std::list<Control*> __keyUpControl;
 };
};


namespace ID
{
 class Drawable;
 class Window;
 class Control;
 class Wrapper_Event;
 typedef void (*cbEvent)(Control* sender, Event* event, void* data);
 typedef void (*cb)(Control* sender, void* data);
 class Control
 {
 public:
    Control(void);
    Control(const Control&);
  virtual ~Control(void);
  Control& operator=(const Control&);

  virtual int Click(Event* event);
  virtual int DoubleClick(Event* event);
  virtual int ClickDown(Event* event);
  virtual int ClickUp(Event* event);
  virtual int Enter(Event* event);
  virtual int Leave(Event* event);
  virtual int Motion(Event* event);
  virtual int KeyUp(Event* event);
  virtual int KeyDown(Event* event);
  virtual int FocusEnter(Event* event);
  virtual int FocusLeave(Event* event);
  virtual int EnableChange(Event* event);
  virtual int ScreenChange(Event* event);
  virtual int Resize(Event* event);
  virtual int Quit(Event* event);
  virtual int UserEvent(Event* event);

  void SetCbClick(cbEvent, void* data);
  void SetCbDoubleClick(cbEvent, void* data);
  void SetCbClickDown(cbEvent, void* data);
  void SetCbClickUp(cbEvent, void* data);
  void SetCbEnter(cbEvent, void* data);
  void SetCbLeave(cbEvent, void* data);
  void SetCbMotion(cbEvent, void* data);
  void SetCbKeyUp(cbEvent, void* data, bool global = false);
  void SetCbKeyDown(cbEvent, void* data, bool global = false);
  void SetCbFocusEnter(cbEvent, void* data);
  void SetCbFocusLeave(cbEvent, void* data);
  void SetCbEnableChange(cbEvent, void* data);
  void SetCbScreenChange(cbEvent, void* data);
  void SetCbResize(cbEvent, void* data);
  void SetCbQuit(cbEvent, void* data);
  void SetCbUserEvent(cbEvent, void* data);
  void SetCbUpdate(cb, void* data);

  cbEvent GetCbClick(void) const;
  cbEvent GetCbDoubleClick(void) const;
  cbEvent GetCbClickDown(void) const;
  cbEvent GetCbClickUp(void) const;
  cbEvent GetCbEnter(void) const;
  cbEvent GetCbLeave(void) const;
  cbEvent GetCbMotion(void) const;
  cbEvent GetCbKeyUp(void) const;
  cbEvent GetCbKeyDown(void) const;
  cbEvent GetCbFocusEnter(void) const;
  cbEvent GetCbFocusLeave(void) const;
  cbEvent GetCbEnableChange(void) const;
  cbEvent GetCbScreenChange(void) const;
  cbEvent GetCbResize(void) const;
  cbEvent GetCbQuit(void) const;
  cbEvent GetCbUserEvent(void) const;
  cb GetCbUpdate(void) const;

  virtual void AddChild(Control* control);
  virtual void AddChild(Drawable* drawable);
  virtual int DelChild(Control* control);
  virtual int DelChild(Drawable* drawable);
  int MoveChildFront(Drawable* drawable);
  int MoveChildBack(Drawable* drawable);
  void ClearChildren(void);
  void SetWindow(Window* window);
  void SetParent(Drawable* drawable);
  Window* GetWindow(void) const;
  Drawable* GetParent(void) const;
  std::list<Control*>* GetChildrenControl(void);
  std::list<Drawable*>* GetChildrenDrawable(void);

  inline void SetName(const char* name)
  {
   this->__name = name;
  }
  inline const char* GetName(void) const
  {
   return this->__name;
  }

 protected:
  virtual bool _OnClick(Event* event);
  virtual bool _OnDoubleClick(Event* event);
  virtual bool _OnClickDown(Event* event);
  virtual bool _OnClickUp(Event* event);
  virtual bool _OnEnter(Event* event);
  virtual bool _OnLeave(Event* event);
  virtual bool _OnMotion(Event* event);
  virtual bool _OnKeyUp(Event* event);
  virtual bool _OnKeyDown(Event* event);
  virtual bool _OnFocusEnter(Event* event);
  virtual bool _OnFocusLeave(Event* event);
  virtual bool _OnEnableChange(Event* event);
  virtual bool _OnScreenChange(Event* event);
  virtual bool _OnResize(Event* event);
  virtual bool _OnQuit(Event* event);
  virtual bool _OnUserEvent(Event* event);

 protected:
  Window* _window;
  Drawable* _parent;
  std::list<Control*> _childrenControl;
  std::list<Drawable*> _childrenDrawable;
  cbEvent _cbClick;
  cbEvent _cbDoubleClick;
  cbEvent _cbClickDown;
  cbEvent _cbClickUp;
  cbEvent _cbEnter;
  cbEvent _cbLeave;
  cbEvent _cbMotion;
  cbEvent _cbKeyUp;
  cbEvent _cbKeyDown;
  cbEvent _cbFocusEnter;
  cbEvent _cbFocusLeave;
  cbEvent _cbEnableChange;
  cbEvent _cbScreenChange;
  cbEvent _cbResize;
  cbEvent _cbQuit;
  cbEvent _cbUserEvent;
  cb _cbUpdate;

 protected:
  void* __cbClickData;
  void* __cbDoubleClickData;
  void* __cbClickDownData;
  void* __cbClickUpData;
  void* __cbEnterData;
  void* __cbLeaveData;
  void* __cbMotionData;
  void* __cbKeyUpData;
  void* __cbKeyDownData;
  void* __cbFocusEnterData;
  void* __cbFocusLeaveData;
  void* __cbEnableChangeData;
  void* __cbScreenChangeData;
  void* __cbResizeData;
  void* __cbQuitData;
  void* __cbUserEventData;
  void* __cbUpdateData;
  const char* __name;
 };
};








namespace ID
{
 class Menubar;
 class Menu;
 class Main_Control;
 class Drawable;
 class Window : public Control
 {
  public:
     Window(uint16_t width, uint16_t height, uint32_t flags);
     Window(const Window& window);
   virtual ~Window(void);
   Window& operator=(const Window& window);

   virtual int Click(Event* event);
   virtual int DoubleClick(Event* event);
   virtual int ClickDown(Event* event);
   virtual int ClickUp(Event* event);
   virtual int Enter(Event* event);
   virtual int Leave(Event* event);
   virtual int Motion(Event* event);
   virtual int KeyUp(Event* event);
   virtual int KeyDown(Event* event);
   virtual int FocusEnter(Event* event);
   virtual int FocusLeave(Event* event);
   virtual int EnableChange(Event* event);
   virtual int ScreenChange(Event* event);
   virtual int Resize(Event* event);
   virtual int Quit(Event* event);
   virtual int UserEvent(Event* event);

public:
inline void SetInvalidate(bool i)
{
 this->__isInvalidate = i;


}

inline void SetEnableBg(bool ebg)
{
 this->__enableBg = ebg;


}

inline int SetBackground(ID::Surface* surface)
{
 this->__bgSurface = surface;
 this->SetInvalidate(true);
 return 0;
}

inline int SetBackground(ID::Color* struct_c)
{
 this->__bgColor = *struct_c;
 this->__bgSurface = NULL;
 this->SetInvalidate(true);
 return 0;
}

inline int SetBackground(uint32_t c)
{
 this->__bgColor = c;
 this->__bgSurface = NULL;
 this->SetInvalidate(true);
 return 0;
}
inline void SetToReorganize(bool toReorganize)
{
 this->__toReorganize = toReorganize;
}

inline Main_Control* GetMainControl(void) const
{
 return this->__mainControl;
}

inline bool GetInvalidate(void) const
{
 return this->__isInvalidate;
}

inline bool GetEnableBg(void) const
{
 return this->__enableBg;
}

inline uint16_t GetWidth(void) const
{
 return this->__window.w;
}

inline uint16_t GetHeight(void) const
{
 return this->__window.h;
}

inline ID::Drawable* GetDrawableFocused(void) const
{
 return this->__drawableFocused;
}

inline ID::Drawable* GetDrawableEnter(void) const
{
 return this->__drawableEnter;
}

inline ID::Surface* GetSurface(void)
{
 return &(this->__window);
}

inline std::vector<ID::Menu*>* GetDeveloppedMenus(void)
{
 return &(this->__developpedMenus);
}

inline bool GetToReorganize(void) const
{
 return this->__toReorganize;
}
inline void AddUpdate(ID::Rect* r)
{
 if (r == 0 || (r->x == 0 && r->y == 0 && r->w == 0 && r->h == 0))
 {
  this->__fullscreenUpdate = true;
  this->__updateRects.clear();
 }
 else
 {
  if (r->w == 0 || r->h == 0)
   return ;
  if (r->x < 0)
  {
   r->w = r->w + r->x;
   r->x = 0;
  }
  if (r->y < 0)
  {
   r->h = r->h + r->y;
   r->y = 0;
  }
  if (r->w + r->x >= this->GetWidth())
  {
   r->w = this->GetWidth() - r->x;
  }
  if (r->h + r->y >= this->GetHeight())
  {
   r->h = this->GetHeight() - r->y;
  }
  this->__updateRects.push_back(*r);
 }
}

inline int Update(void)
{
 if (this->__fullscreenUpdate == true)
 {
  std::cout << "U(full)\n";
  this->__updateRects.clear();
  this->__window.Update(0);
  this->__fullscreenUpdate = false;
 }
 else
 {
  std::vector<ID::Rect>::iterator it;
  std::vector<ID::Rect>::iterator end;

  it = this->__updateRects.begin();
  end = this->__updateRects.end();
  while (it != end)
  {

   if (this->__window.Update(&(*it)) == -1)
    return -1;
   ++it;
  }
  this->__updateRects.clear();
 }
 return 0;
}

inline int Update(ID::Rect* r)
{
 return this->__window.Update(r);
}

inline int Resize(uint16_t w, uint16_t h)
{
 return this->__window.ResizeVideoMode(w, h);
}
protected:
inline void _RefreshBackground(void)
{
 if (this->__bgSurface != NULL)
  this->__bgSurface->BlitSurface(NULL, &(this->__window), 0, 0);
 else
  this->__window.FillRect(NULL, &(this->__bgColor));
}






  public:
   void SetDrawableFocused(Drawable* focused);
   void SetDrawableEnter(Drawable* enter);
   int RefreshSurface(void);





   void AddMenubar(Menubar* menubar);
   int DelMenubar(Menubar* menubar);
   virtual void AddChild(Control* control);
   virtual void AddChild(Drawable* drawable);

  protected:

   int _Reorganize(void);


  protected:
   virtual bool _OnClick(Event* event);
   virtual bool _OnDoubleClick(Event* event);
   virtual bool _OnClickDown(Event* event);
   virtual bool _OnClickUp(Event* event);
   virtual bool _OnEnter(Event* event);
   virtual bool _OnLeave(Event* event);
   virtual bool _OnMotion(Event* event);
   virtual bool _OnKeyUp(Event* event);
   virtual bool _OnKeyDown(Event* event);
   virtual bool _OnFocusEnter(Event* event);
   virtual bool _OnFocusLeave(Event* event);
   virtual bool _OnEnableChange(Event* event);
   virtual bool _OnScreenChange(Event* event);
   virtual bool _OnResize(Event* event);
   virtual bool _OnQuit(Event* event);
   virtual bool _OnUserEvent(Event* event);

  private:
   Surface __window;
   Main_Control* __mainControl;
   bool __isInvalidate;
   bool __enableBg;
   Surface* __bgSurface;
   Color __bgColor;
   Drawable* __drawableFocused;
   Drawable* __drawableEnter;
   std::vector<Rect> __updateRects;
   bool __fullscreenUpdate;
   bool __toReorganize;
   std::vector<Menu*> __developpedMenus;
 };
}

namespace ID
{
 enum REL_POS_TYPE
 {
  REL_POS_NONE = 1,
  REL_POS_TOP = 2,
  REL_POS_BOT = 4,
  REL_POS_LEFT = 8,
  REL_POS_RIGHT = 16,
  REL_POS_CENTER = REL_POS_TOP | REL_POS_BOT | REL_POS_LEFT | REL_POS_RIGHT
 };

 class Drawable : public Control
 {
  public:
    Drawable(void);
    Drawable(uint16_t width, uint16_t height, uint32_t flags,
      uint32_t rmask, uint32_t gmask,
      uint32_t bmask, uint32_t amask);
    Drawable(const Drawable&);
  virtual ~Drawable(void);
  Drawable& operator=(const Drawable&);

  virtual int Click(Event* event);
  virtual int DoubleClick(Event* event);
  virtual int ClickDown(Event* event);
  virtual int ClickUp(Event* event);
  virtual int Enter(Event* event);
  virtual int Leave(Event* event);
  virtual int Motion(Event* event);
  virtual int KeyUp(Event* event);
  virtual int KeyDown(Event* event);
  virtual int FocusEnter(Event* event);
  virtual int FocusLeave(Event* event);
  virtual int EnableChange(Event* event);
  virtual int ScreenChange(Event* event);
  virtual int Resize(Event* event);
  virtual int Quit(Event* event);
  virtual int UserEvent(Event* event);



inline void SetOnScreen(bool visible)
{
 ID::Event event;

 this->_onScreen = visible;
 event.type = EVENT_SCREEN_CHANGE;
 event.screenChange.type = EVENT_SCREEN_CHANGE;
 event.screenChange.isOnScreen = visible;
 this->ScreenChange(&event);
 this->SetInvalidate(true);


}

inline void SetEnable(bool enable)
{
 ID::Event event;

 this->_isEnable = enable;
 event.type = EVENT_ENABLE_CHANGE;
 event.enableChange.type = EVENT_ENABLE_CHANGE;
 event.enableChange.isEnable = enable;
 this->EnableChange(&event);
 if (this->GetParent() != NULL)
  this->GetParent()->SetInvalidate(true);
}

inline void SetInvalidate(bool invalidate)
{
 this->_SetInvalidate(invalidate);
 if (invalidate == true)
  this->_AddUpdateRect();
}

inline void SetAbstractParent(bool ap)
{
 this->_abstractParent = ap;
 this->SetInvalidate(true);
}

inline void SetEnableBg(bool ebg)
{
 this->_enableBg = ebg;
 this->SetInvalidate(true);
}

inline void SetSurface(ID::Surface* new_surface)
{
 this->_surface = new_surface;
 this->_w = this->_surface->w;
 this->_h = this->_surface->h;
 this->SetInvalidate(true);
}

inline void SetPos(int16_t new_x, int16_t new_y)
{
 this->SetInvalidate(true);
 this->_relPos = ID::REL_POS_NONE;
 this->_x = new_x;
 this->_y = new_y;
 this->_RefreshOffset();
 this->SetInvalidate(true);
}

inline void SetPos(ID::REL_POS_TYPE relPos)
{
 this->SetInvalidate(true);
 this->_relPos = relPos;
 this->_CalculRelativePosition(&(this->_x), &(this->_y));
 this->_RefreshOffset();
 this->SetInvalidate(true);
}

inline int SetBackground(ID::Surface* surface)
{
 this->_bgSurface = surface;
 this->SetInvalidate(true);
 return 0;
}

inline int SetBackground(ID::Color* struct_c)
{
 this->_bgColor = *struct_c;
 this->_bgSurface = NULL;
 this->SetInvalidate(true);
 return 0;
}

inline int SetBackground(uint32_t c)
{
 this->_bgColor = c;
 this->_bgSurface = NULL;
 this->SetInvalidate(true);
 return 0;
}

inline void SetVisibleRect(Rect* rect)
{
 if (this->_visibleRect != NULL)
  delete this->_visibleRect;
 this->_visibleRect = new ID::Rect(*rect);
 this->SetInvalidate(true);
}

inline void SetBgRect(Rect* rect)
{
 if (this->_bgRect != NULL)
  delete this->_bgRect;
 this->_bgRect = new ID::Rect(*rect);
 this->SetInvalidate(true);
}

inline int SetBorderColor(ID::Color* struct_c)
{
 this->_borderColor = *struct_c;
 this->SetInvalidate(true);
 return 0;
}

inline int SetBorderColor(uint32_t c)
{
 this->_borderColor = c;
 this->SetInvalidate(true);
 return 0;
}

inline void SetBorder(Border* b)
{
 this->_border = *b;
 this->SetInvalidate(true);
}

inline void SetBorder(uint16_t all)
{
 this->_border.top = all;
 this->_border.bot = all;
 this->_border.left = all;
 this->_border.right = all;
 this->SetInvalidate(true);
}

inline void SetBorder(uint16_t top, uint16_t bot,
      uint16_t left, uint16_t right)
{
 this->_border.top = top;
 this->_border.bot = bot;
 this->_border.left = left;
 this->_border.right = right;
 this->SetInvalidate(true);
}

inline bool GetOnScreen(void) const
{
 return this->_onScreen;
}

inline bool GetEnable(void) const
{
 return this->_isEnable;
}

inline bool GetInvalidate(void) const
{
 return this->_isInvalidate;

}

inline bool GetAbstractParent(void) const
{
 return this->_abstractParent;
}

inline bool GetEnableBg(void) const
{
 return this->_enableBg;
}

inline ID::Surface* GetSurface(void) const
{
 return this->_surface;
}

inline void GetPos(int16_t* x, int16_t* y) const
{
 if (x != NULL)
  *x = this->_x;
 if (y != NULL)
  *y = this->_y;
}

inline ID::REL_POS_TYPE GetRelPos(void) const
{
 return this->_relPos;
}

inline void GetOffset(int* offset_x, int* offset_y) const
{
 if (offset_x != NULL)
  *offset_x = this->_offset_x;
 if (offset_y != NULL)
  *offset_y = this->_offset_y;
}

inline ID::Color GetBorderColor(void) const
{
 return this->_borderColor;
}

inline ID::Border GetBorder(void) const
{
 return this->_border;
}

inline void GetBorder(uint16_t* top, uint16_t* bot,
      uint16_t* left, uint16_t* right)
{
 if (top != NULL)
  *top = this->_border.top;
 if (bot != NULL)
  *bot = this->_border.bot;
 if (left != NULL)
  *left = this->_border.left;
 if (right != NULL)
  *right = this->_border.right;
}

inline uint16_t GetWidth(void) const
{

 return (this->_surface != 0) ? this->_surface->w : this->_w;
}

inline uint16_t GetHeight(void) const
{

 return (this->_surface != 0) ? this->_surface->h : this->_h;
}

inline ID::Rect* GetVisibleRect(void) const
{
 return this->_visibleRect;
}

inline ID::Rect* GetBgRect(void) const
{
 return this->_bgRect;
}

inline ID::Window* GetRealWindow(void)
{
 ID::Drawable* draw;

 draw = this;
 while (draw->GetParent() != NULL)
  draw = draw->GetParent();
 return draw->GetWindow();
}

inline int BlitSurface(ID::Rect* rect, ID::Surface* target, int x, int y)
{
 return this->_surface->BlitSurface(rect, target, x, y);
}

inline virtual int RefreshToSurface(ID::Surface* s, int x, int y)
{
 if (this->GetOnScreen() == false ||
     (this->_surface == NULL && this->GetAbstractParent() == false))
  return 0;
 std::list<Drawable*>::iterator it;
 std::list<Drawable*>::iterator end;

 it = this->_childrenDrawable.begin();
 end = this->_childrenDrawable.end();
 if (this->GetAbstractParent() == true)
 {
  while (it != end)
  {
   if ((*it)->RefreshToSurface(s, x + this->_x, y + this->_y) == -1)
    return -1;
   ++it;
  }
 }
 else
 {
  if (this->GetInvalidate() == true)
  {
   if (this->GetEnableBg() == true)
    this->_RefreshBackground();
   while (it != end)
   {
    if ((*it)->RefreshToSurface(this->GetSurface(), 0, 0) == -1)
     return -1;
    ++it;
   }
  }
  if (this->_surface->BlitSurface(this->_visibleRect,
      s,
      x + this->_x,
      y + this->_y) == -1)
   return -1;
 }
 if (this->_cbUpdate != NULL)
  this->_cbUpdate(this, this->__cbUpdateData);
 this->SetInvalidate(false);
 return 0;
}

inline virtual int Resize(uint16_t w, uint16_t h,
          uint32_t f = SDL_SWSURFACE | SDL_SRCALPHA,
          uint32_t r = 0, uint32_t g = 0,
          uint32_t b = 0, uint32_t a = 0)
{
 if (this->_surface == NULL)
 {
  this->_surface = new ID::Surface(w, h, f,
       r, g,
       b, a);
  if (this->_surface == 0)
   return -1;
 }
 else if (this->_surface->Resize(w, h, f, r, g, b, a) == -1)
  return -1;
 this->_w = w;
 this->_h = h;
 return 0;
}

protected:
inline void _RefreshBackground(void)
{
 ID::Rect bg;

 if (this->_surface == NULL)
  return ;
 if (this->_bgSurface != NULL)
 {
  this->_bgSurface->BlitSurface(this->_bgRect, this->_surface, this->_border.top, this->_border.left);
  return ;
 }
 else
  this->_surface->FillRect(NULL, &(this->_bgColor));
 if (this->_border.top != 0)
 {
  bg.x = 0;
  bg.y = 0;
  bg.w = this->GetWidth();
  bg.h = this->_border.top;
  this->_surface->FillRect(&bg, &(this->_borderColor));
 }
 if (this->_border.bot != 0)
 {
  bg.x = 0;
  bg.y = this->GetHeight() - this->_border.bot;
  bg.w = this->GetWidth();
  bg.h = this->_border.bot;
  this->_surface->FillRect(&bg, &(this->_borderColor));
 }
 if (this->_border.left != 0)
 {
  bg.x = 0;
  bg.y = 0;
  bg.w = this->_border.left;
  bg.h = this->GetHeight();
  this->_surface->FillRect(&bg, &(this->_borderColor));
 }
 if (this->_border.right != 0)
 {
  bg.x = this->GetWidth() - this->_border.right;
  bg.y = 0;
  bg.w = this->_border.right;
  bg.h = this->GetHeight();
  this->_surface->FillRect(&bg, &(this->_borderColor));
 }
}

inline virtual void _SetInvalidate(bool invalidate)
{
 if (invalidate == true)
 {
  if (this->_parent != NULL &&
      this->_parent->GetInvalidate() == false)
   this->_parent->_SetInvalidate(true);
 }
 this->_isInvalidate = invalidate;
}

inline void _AddUpdateRect()
{
 ID::Window* w;

 w = this->GetRealWindow();
 if (w != NULL)
 {
  ID::Rect rect(this->_offset_x + this->_x,
         this->_offset_y + this->_y,
         this->GetWidth(), this->GetHeight());
  w->AddUpdate(&rect);
 }
}

inline void _RefreshOffset()
{
 ID::Drawable* dad;
 std::list<Drawable*>::iterator it;
 std::list<Drawable*>::iterator end;

 dad = this->_parent;
 this->_offset_x = 0;
 this->_offset_y = 0;
 while (dad != 0)
 {
  this->_offset_x += dad->_x;
  this->_offset_y += dad->_y;
  dad = dad->_parent;
 }
 it = this->_childrenDrawable.begin();
 end = this->_childrenDrawable.end();
 while (it != end)
 {
  (*it)->_RefreshOffset();
  ++it;
 }
}

inline void _CalculRelativePosition(int16_t* x, int16_t* y)
{
 int parentWidth;
 int parentHeight;

 if (this->_relPos & ID::REL_POS_NONE)
 {
  *x = this->_x;
  *y = this->_y;
  return ;
 }
 if (this->_parent != NULL)
 {
  parentWidth = this->_parent->GetWidth();
  parentHeight = this->_parent->GetHeight();
 }
 else if (this->_window != NULL)
 {
  parentWidth = this->_window->GetWidth();
  parentHeight = this->_window->GetHeight();
 }
 *x = 0;
 *y = 0;
 if (!(this->_relPos & (ID::REL_POS_BOT | ID::REL_POS_TOP)))
  *y = (parentHeight - this->GetHeight()) >> 1;
 else
 {
  if (this->_relPos & ID::REL_POS_BOT)
   *y = parentHeight - this->GetHeight();
  if (this->_relPos & ID::REL_POS_TOP && *y != 0)
   *y = *y >> 1;
 }
 if (!(this->_relPos & (ID::REL_POS_LEFT | ID::REL_POS_RIGHT)))
  *x = (parentWidth - this->GetWidth()) >> 1;
 else
 {
  if (this->_relPos & ID::REL_POS_RIGHT)
   *x = parentWidth - this->GetWidth();
  if (this->_relPos & ID::REL_POS_LEFT && *x != 0)
   *x = *x >> 1;
 }
}
  virtual bool _OnClick(Event* event);
  virtual bool _OnDoubleClick(Event* event);
  virtual bool _OnClickDown(Event* event);
  virtual bool _OnClickUp(Event* event);
  virtual bool _OnEnter(Event* event);
  virtual bool _OnLeave(Event* event);
  virtual bool _OnMotion(Event* event);
  virtual bool _OnKeyUp(Event* event);
  virtual bool _OnKeyDown(Event* event);
  virtual bool _OnFocusEnter(Event* event);
  virtual bool _OnFocusLeave(Event* event);
  virtual bool _OnEnableChange(Event* event);
  virtual bool _OnScreenChange(Event* event);
  virtual bool _OnResize(Event* event);
  virtual bool _OnQuit(Event* event);
  virtual bool _OnUserEvent(Event* event);

 protected:
  Surface* _surface;
  Surface* _bgSurface;
  Color _bgColor;
  bool _onScreen;
  bool _isEnable;
  bool _isInvalidate;
  bool _abstractParent;
  bool _enableBg;
  REL_POS_TYPE _relPos;
  int16_t _x;
  int16_t _y;
  Rect* _visibleRect;
  Rect* _bgRect;
  int16_t _offset_x;
  int16_t _offset_y;
  Color _borderColor;
  Border _border;

  uint16_t _w;
  uint16_t _h;
 };
}





namespace ID
{
 class GPolygon : public Drawable
 {
  public:
     GPolygon(int size, bool fulfil,
       Color* color = (Color*)&(COLOR_BLACK));
     ~GPolygon();

   void AddPoint(int16_t x, int16_t y);
   void AddPoint(Point* point);
   void SetFulfil(bool fulfil);
   void SetColor(uint32_t color);
   void SetColor(Color color);

   std::vector<Point>* GetPoints(void);
   bool GetFulfil(void);
   Color GetColor(void);

   virtual int RefreshToSurface(Surface* surface, int x, int y);
  private:
   std::vector<Point> __points;
   bool __size;
   bool __fulfil;
   Color __color;
 };
};
namespace ID
{
 class Surface;
 class Draw_Geometry_Polygon : public Singleton<Draw_Geometry_Polygon>
 {
  friend class Singleton<Draw_Geometry_Polygon>;
  public:
   void Draw(Surface* surface,
          int16_t x, int16_t y,
          GPolygon* polygon,
          int size,
          bool fulfil,
          Color* color = (Color*)&(COLOR_BLACK));
   void Draw(Surface* surface,
          int16_t x, int16_t y,
          std::vector<Point>* v,
          int size,
          bool fulfil,
          Color* color = (Color*)&(COLOR_BLACK));
  private:
    Draw_Geometry_Polygon();
    ~Draw_Geometry_Polygon();
   void __Polygon(int16_t originX, int16_t originY, std::vector<Point>* list, bool fulfil);
   void __Fill(void);
   void __SetPixelTrans(uint16_t x, uint16_t y);
   void __Line(int x1, int y1, int x2, int y2);
   void __PixelBrush(int x_center, int y_center);
   void __PixelBrush_1(int x_center, int y_center);
   void __PixelBrush_2(int x_center, int y_center);
   void __PixelBrush_3(int x_center, int y_center);
   void __PixelBrush_4(int x_center, int y_center);
   void __PixelBrush_5(int x_center, int y_center);
   void __PixelBrush_6(int x_center, int y_center);
   void __PixelBrush_7(int x_center, int y_center);
   void __PixelBrush_8(int x_center, int y_center);

  private:
   int __lastX;
   int __lastY;
   int __size;
   Surface* __target;
   Color* __color;
   std::map<int, std::list<int> > __points;
 };
};




namespace ID
{
 class Container : public Drawable
 {
  public:
   Container(void);
   Container(uint16_t w, uint16_t h, uint32_t flags,
      uint32_t rmask, uint32_t gmask,
      uint32_t bmask, uint32_t amask);
  virtual ~Container(void);
 };
};




namespace ID
{
 class GLine : public Drawable
 {
  public:
     GLine(int16_t x, int16_t y,
           int size,
           Color* color = (Color*)&(COLOR_BLACK));
     ~GLine();

   void SetVector(int16_t vX, int16_t vY);
   void SetSize(uint16_t size);
   void SetColor(uint32_t color);
   void SetColor(Color color);

   Point GetVector(void);
   uint16_t GetSize(void);
   Color GetColor(void);

   virtual int RefreshToSurface(Surface* surface, int x, int y);
  private:
   Point __point;
   int __size;
   Color __color;
 };
};




namespace ID
{
 class GCircle : public Drawable
 {
  public:
     GCircle(uint32_t radius,
      bool fulfil = true,
      Color* color = (Color*)&(COLOR_BLACK));
     ~GCircle();

   void SetColor(uint32_t color);
   void SetColor(Color* color);
   void SetRadius(uint32_t radius);
   void SetFulfil(bool fulfil);

   Color GetColor(void);
   uint32_t GetRadius(void);
   bool GetFulfil(void);

   virtual int RefreshToSurface(Surface* surface, int x, int y);
  private:
   uint32_t __radius;
   Color __color;
   bool __fulfil;
 };
};




namespace ID
{
 class GRect : public Drawable
 {
  public:
     GRect(int16_t w, int16_t h,
           bool fulfil,
           Color* color = (Color*)&(COLOR_BLACK));
     ~GRect();






     void SetDimension(int16_t w, int16_t h);
   void SetFulfil(bool fulfil);
   void SetColor(uint32_t color);
   void SetColor(Color color);
     inline bool GetFulfil(void) const
   {
    return this->__fulfil;
   }
   inline Color GetColor(void) const
   {
    return this->__color;
   }

   virtual int RefreshToSurface(Surface* surface, int x, int y);
  protected:
  private:


   bool __fulfil;
   Color __color;
 };
};










namespace ID
{
 class Button : public Drawable
 {
 public:
    Button(void);
    Button(uint16_t w, uint16_t h, uint16_t f,
           uint32_t rmask, uint32_t gmask,
           uint32_t bmask, uint32_t amask);
    Button(const ID::Button& button);
  virtual ~Button(void);
  virtual int RefreshToSurface(Surface* surface, int x, int y);
 };
}




namespace ID
{
 class Label : public Drawable
 {
  public:
     Label(const char* font_path, int font_size, char* text,
           Color* fg = (Color*)&COLOR_BLACK);
     Label(const ID::Label& b);
   virtual ~Label();
   char* GetText(void) const;
   void SetText(char* Text);
   void SetColor(Color fg);
   void SetColor(uint32_t fg);
   Color GetColor(void) const;
   void SetPolice(Font::Police* police);
   Font::Police* GetPolice(void) const;
   virtual int RefreshToSurface(Surface* surface, int x, int y);

  private:
   char* __text;
   Color __color;
   Font::Police* __police;
 };
}

namespace ID
{
 class Button_Label : public Button
 {
  public:
    Button_Label(uint16_t width, uint16_t height, uint32_t flags,
          uint32_t rmask, uint32_t gmask,
          uint32_t bmask, uint32_t amask);
    Button_Label(uint16_t width, uint16_t height, uint32_t flags,
          uint32_t rmask, uint32_t gmask,
          uint32_t bmask, uint32_t amask,
          const char* font_path, int font_size, char* label,
          Color* fg = (Color*)&COLOR_BLACK);
    Button_Label(const Button_Label& button);
  virtual ~Button_Label(void);
  void SetLabel(Label* label);
  Label* GetLabel(void) const;
  void SetLabelPos(int16_t x, int16_t y);
  void SetLabelPos(REL_POS_TYPE rel_pos);
  void GetLabelPos(int16_t* x, int16_t* y)const;
  REL_POS_TYPE GetLabelRelPos(void)const;
  virtual int RefreshToSurface(Surface* surface, int x, int y);

  private:
   ID::Label* __label;
 };
}

namespace ID
{
 enum MENU_TYPE
 {
  MENU_VERTICAL = 1,
  MENU_HORIZONTAL = 2
 };

 enum ALIGNMENT_TYPE
 {
  ALIGNMENT_TOP = 1,
  ALIGNMENT_LEFT = 1,
  ALIGNMENT_BOT = 2,
  ALIGNMENT_RIGHT = 2,
  ALIGNMENT_CENTER = 3
 };

 enum SIDE_POSITION_TYPE
 {
  SIDE_POSITION_TOP = 1,
  SIDE_POSITION_BOT,
  SIDE_POSITION_LEFT,
  SIDE_POSITION_RIGHT
 };

 class Menu : public Drawable
 {
  friend class Menubar;
  public:
     Menu(uint16_t w, uint16_t h, uint32_t flags,
          uint32_t rmask, uint32_t gmask,
          uint32_t bmask, uint32_t amask,
          const char* font_path, int font_size,
          char* text, Color* color = (Color*)&COLOR_BLACK);
   virtual ~Menu(void);

   void SetMenuType(MENU_TYPE menu_type);
   void SetChildrenSidePosition(SIDE_POSITION_TYPE side_position_type);
   void SetChildrenAlignment(ALIGNMENT_TYPE alignment_type);
   void SetMustBeOnScreen(bool mustBeOnScreen);
   void SetLabel(Label* label);

   MENU_TYPE GetMenuType(void) const;
   std::list<Menu*> GetChildrenMenu(void) const;
   SIDE_POSITION_TYPE GetChildrenSidePosition(void) const;
   ALIGNMENT_TYPE GetChildrenAlignment(void) const;
   bool GetMustBeOnScreen(void) const;
   Label* GetLabel(void) const;

   void ShowAll();
   void HideAll();

   virtual int Click(Event* event);
   virtual int DoubleClick(Event* event);
   virtual int ClickDown(Event* event);
   virtual int ClickUp(Event* event);
   virtual int Enter(Event* event);
   virtual int Leave(Event* event);
   virtual int Motion(Event* event);
   virtual int KeyUp(Event* event);
   virtual int KeyDown(Event* event);
   virtual int FocusEnter(Event* event);
   virtual int FocusLeave(Event* event);
   virtual int EnableChange(Event* event);
   virtual int ScreenChange(Event* event);
   virtual int Resize(Event* event);
   virtual int Quit(Event* event);
   virtual int UserEvent(Event* event);

   int ActionEvent(void);
   void SetCbActionEvent(cbEvent, void* data);
   cbEvent GetCbActionEvent(void);

   virtual void AddChild(Menu* menu);
   virtual int DelChild(Menu* menu);
   virtual int RefreshToSurface(Surface* surface, int x, int y);

  protected:
   void _SetToReorganize(bool toReorganize);
   bool _GetToReorganize(void);

   void _Reorganize(void);
   int _BlitToWindow(void);

   virtual bool _OnClick(Event* event);
   virtual bool _OnDoubleClick(Event* event);
   virtual bool _OnClickDown(Event* event);
   virtual bool _OnClickUp(Event* event);
   virtual bool _OnEnter(Event* event);
   virtual bool _OnLeave(Event* event);
   virtual bool _OnMotion(Event* event);
   cbEvent _cbActionEvent;

  protected:
   Label* _label;

  private:
   void __ReorganizeVertical(void);
   void __ReorganizeHorizontal(void);
   int __GetChildrenMaxWidth(void);
   int __GetChildrenTotalWidth(void);
   int __GetChildrenMaxHeight(void);
   int __GetChildrenTotalHeight(void);
   void __CalculChildrenSidePosition(int* x, int* y);
   void __CalculChildrenAlignment(int* x, int* y,
          int minX, int minY);

  private:
   MENU_TYPE __menuType;
   std::list<Menu*> __childrenMenu;
   SIDE_POSITION_TYPE __childrenSidePosition;
   ALIGNMENT_TYPE __childrenAlignment;
   bool __mustBeOnScreen;
   bool __toReorganize;
   void* __cbActionEventData;
 };
};




namespace ID
{
 class Menubar : public Drawable
 {
  public:
     Menubar(uint16_t w, uint16_t h, uint32_t flags,
      uint32_t rmask, uint32_t gmask,
      uint32_t bmask, uint32_t amask,
      REL_POS_TYPE relPos = REL_POS_TOP);
   virtual ~Menubar(void);

   void SetOnScreen(bool visible);
   virtual void AddChild(Menu* menu);
   virtual int RefreshToSurface(Surface* surface, int x, int y);

   virtual int Click(Event* event);
   virtual int DoubleClick(Event* event);
   virtual int ClickDown(Event* event);
   virtual int ClickUp(Event* event);
   virtual int Enter(Event* event);
   virtual int Leave(Event* event);
   virtual int Motion(Event* event);
   virtual int KeyUp(Event* event);
   virtual int KeyDown(Event* event);
   virtual int FocusEnter(Event* event);
   virtual int FocusLeave(Event* event);
   virtual int EnableChange(Event* event);
   virtual int ScreenChange(Event* event);
   virtual int Resize(Event* event);
   virtual int Quit(Event* event);
   virtual int UserEvent(Event* event);

  protected:
   void _Reorganize(void);
   virtual void _SetInvalidate(bool invalidate);
   virtual bool _OnClick(ID::Event* event);
   virtual bool _OnDoubleClick(ID::Event* event);
   virtual bool _OnClickDown(ID::Event* event);
   virtual bool _OnClickUp(ID::Event* event);
   virtual bool _OnEnter(ID::Event* event);
   virtual bool _OnLeave(ID::Event* event);
   virtual bool _OnMotion(ID::Event* event);

  private:
   std::list<Menu*> __childrenMenu;
 };
};




namespace ID
{
 class Main_Control : public Drawable
 {
  public:
     Main_Control(uint16_t width, uint16_t height, uint32_t flags,
           uint32_t rmask, uint32_t gmask,
           uint32_t bmask, uint32_t amask);
     Main_Control(const Main_Control& mc);
   virtual ~Main_Control();
   Main_Control& operator=(const Main_Control& mc);

   bool GetInvalidate(void) const;
   virtual int RefreshToSurface(Surface* surface, int x, int y);

  protected:
   virtual void _SetInvalidate(bool invalidate);
 };
};




void __id_textboxClickDown(ID::Control* _tb, ID::Event*, void*);
void __id_textboxKeyDown(ID::Control* _tb, ID::Event* e, void*);

namespace ID
{
 class Textbox : public Drawable
 {
  friend void ::__id_textboxKeyDown(ID::Control*, ID::Event*, void*);
  public:
     Textbox(void);
     Textbox(uint16_t w, uint16_t h, uint32_t f,
      uint32_t rmask, uint32_t gmask,
      uint32_t bmask, uint32_t amask,
      const char* font_path, int font_size,
      Color* fg = (Color*)&COLOR_BLACK);
     Textbox(const ID::Textbox& button);
   virtual ~Textbox(void);
   void SetText(char* text);
   char* GetText(void);
   void SetColor(Color fg);
   void SetColor(uint32_t fg);
   Color GetColor(void) const;
   void SetPolice(Font::Police* police);
   Font::Police* GetPolice(void) const;
   int SetMaxLength(unsigned int maxLength);
   unsigned int GetMaxLength(void) const;
   virtual int RefreshToSurface(Surface* surface, int x, int y);

   virtual int Action(Event*);
   void SetCbAction(ID::cbEvent, void* data);
   ID::cbEvent GetCbAction(void) const;

  protected:
   ID::cbEvent _cbAction;
   void* _cbActionData;

  private:
   unsigned int __maxLength;
   char* __text;
   Color __color;
   Font::Police* __police;

 };
};




namespace ID
{
 class Button_Image : public Button
 {
 public:
    Button_Image(uint16_t w, uint16_t h, uint16_t f,
          uint32_t rmask, uint32_t gmask,
          uint32_t bmask, uint32_t amask);
    Button_Image(const char* image_path);
    Button_Image(const Button_Image& button_image);
  virtual ~Button_Image(void);
  void SetImage(Surface* surface);
  Surface* GetImage(void) const;
  void SetImagePosition(int x, int y);
  int SetImageVisible(Rect* rect);
  void GetImageVisible(Rect* rect) const;
  virtual int RefreshToSurface(Surface* surface, int x, int y);
 private:
  Surface* __image;
  int __imgX;
  int __imgY;
  Rect* __imgRect;
 };
}







namespace ID
{
 class Surface;
 class Wrapper_Interface_Template
 {
 public:
  virtual int Init(void) = 0;
  virtual void Destroy(void) = 0;
  virtual int Update(Surface* s, ID::Rect* rect) = 0;

  virtual int CreateSurface(Surface* surface,
            uint16_t width, uint16_t height,
            uint32_t flags,
            uint32_t rmask, uint32_t gmask,
            uint32_t bmask, uint32_t amask) = 0;
  virtual int SetVideoMode(Surface* surface,
           uint16_t width, uint16_t height,
           uint32_t flags) = 0;
  virtual void DeleteSurface(Surface* surface) = 0;
  virtual uint32_t GetPixel(Surface* surface, uint16_t x, uint16_t y) = 0;
  virtual void SetPixel(Surface* surface,
       uint16_t x, uint16_t y, uint32_t color) = 0;
  virtual int FillRect(Surface* surface, Rect* rect,
       uint32_t color) = 0;
  virtual int BlitSurface(Surface* surface,
          Rect* src_rect,
          Surface* dest,
          int dest_x, int dest_y) = 0;
 private:
  pthread_t __id_thread;
 private:
  pthread_mutex_t __mutex;
 };
};

namespace ID
{
 class Wrapper_Interface : public Wrapper_Interface_Template,
        public Singleton<ID::Wrapper_Interface>
 {
  friend class Singleton<ID::Wrapper_Interface>;
 public:
  typedef SDL_Surface wrapper_surface;



inline virtual int Init(void)
{
 return 0;
}

inline virtual void Destroy(void)
{
}

inline virtual int Update(Surface* s, ID::Rect* r)
{
 if (r == 0)
  SDL_UpdateRect(s->__surface, 0, 0, 0, 0);
 else
  SDL_UpdateRect(s->__surface, r->x, r->y, r->w, r->h);
 return 0;
}

inline virtual int LoadImage(Surface* s, const char* path,
      int x, int y, bool )
{

 ID::Rect size(x, y);




 s->__surface = SDL_LoadBMP(path);
 if (s->__surface == NULL)
  return (-1);
 s->__w = s->__surface->w;
 s->__h = s->__surface->h;
 s->__pitch = s->__surface->pitch;
 s->pixels = s->__surface->pixels;
 size.w = s->w;
 size.h = s->h;
 return (0);
}

inline virtual int CreateSurface(Surface* s, uint16_t w,
          uint16_t h, uint32_t f,
          uint32_t rmask, uint32_t gmask,
          uint32_t bmask, uint32_t amask)
{
 s->__surface = SDL_CreateRGBSurface(f, w, h, 32,
         rmask, gmask,
         bmask, amask);
 if (s->__surface == NULL)
  return -1;
 s->__flags = f;
 s->__w = w;
 s->__h = h;
 s->__pitch = s->__surface->pitch;
 s->pixels = s->__surface->pixels;
 return 0;
}

inline virtual int SetVideoMode(Surface* s, uint16_t w, uint16_t h, uint32_t f)
{
 s->__surface = SDL_SetVideoMode(w, h, 32, f);
 if (s->__surface == NULL)
  return -1;
 s->__flags = f;
 s->__w = w;
 s->__h = h;
 s->__pitch = s->__surface->pitch;
 s->pixels = s->__surface->pixels;
 return 0;
}

inline virtual void DeleteSurface(Surface* s)
{
 SDL_FreeSurface(s->__surface);
}

inline virtual uint32_t GetPixel(Surface* s, uint16_t x, uint16_t y)
{
 int bpp;
 uint8_t* p;

 bpp = s->__surface->format->BytesPerPixel;
 p = (uint8_t*)s->__surface->pixels + y * s->__surface->pitch + x * bpp;
 if (bpp == 1)
  return *p;
 if (bpp == 2)
  return *(uint16_t*)p;
 if (bpp == 3)
 {
  if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
   return p[0] << 16 | p[1] << 8 | p[2];
  else
   return p[0] | p[1] << 8 | p[2] << 16;
 }
 if (bpp == 4)
  return *(uint32_t*)p;
 return 0;
}

inline virtual void SetPixel(Surface* s,
     uint16_t x, uint16_t y, uint32_t c)
{
 int bpp;
 uint8_t* p;

 bpp = s->__surface->format->BytesPerPixel;
 p = (uint8_t*)s->pixels + y * s->__pitch + x * bpp;
 if (bpp == 1)
  *p = c;
 else if (bpp == 2)
  *(uint16_t*)p = c;
 else if (bpp == 3)
 {
  if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
  {
   p[0] = (c >> 16) & 0xff;
   p[1] = (c >> 8) & 0xff;
   p[2] = c & 0xff;
  }
  else
  {
   p[0] = c & 0xff;
   p[1] = (c >> 8) & 0xff;
   p[2] = (c >> 16) & 0xff;
  }
 }
 else
  *(uint32_t*)p = c;
}

inline virtual int FillRect(Surface* s, ID::Rect* r,
     uint32_t c)
{
 return SDL_FillRect(s->__surface, (SDL_Rect*)r, c);
}

inline virtual int SetColorKey(Surface* s, uint32_t f, uint32_t k)
{
 return SDL_SetColorKey(s->__surface, f, k);
}

inline virtual int BlitSurface(Surface* s, ID::Rect* src_r,
        Surface* dest, int dest_x, int dest_y)
{
 SDL_Rect dst_r;

 dst_r.x = dest_x;
 dst_r.y = dest_y;
 return SDL_BlitSurface(s->__surface, (SDL_Rect*)src_r, dest->__surface, &dst_r);
}

inline void GetRGBA(Surface* surface,
    uint32_t color, ID::Color* s_color)
{
 SDL_GetRGBA(color, surface->__surface->format,
      &(s_color->r), &(s_color->g), &(s_color->b), &(s_color->a));
}

inline uint32_t MapRGBA(Surface* surface,
    uint8_t red, uint8_t green,
    uint8_t blue, uint8_t alpha)
{
 return SDL_MapRGBA(surface->__surface->format, red, green, blue, alpha);
}

inline Surface* Morph_SDL_to_ID(SDL_Surface* SDL_s)
{
 Surface* ID_s;

 ID_s = new Surface(SDL_s->w, SDL_s->h, SDL_s->flags,
          0xFF0000, 0xFF00, 0xFF, 0xFF000000);
 if (ID_s == 0)
  return (0);
 this->CopyPixelsByByte(SDL_s, ID_s);
 return ID_s;
}

inline uint8_t GetColorWithAlpha(uint8_t src_color, uint8_t dst_color, uint8_t alpha)
{
 return (src_color * alpha + (256 - alpha) * dst_color) >> 8;
}

inline void SetPixelWithAlpha(uint8_t* dst, uint8_t* src, int sbpp)
{
 uint32_t src_color;

 src_color = *(uint32_t*)src;
 if (sbpp == 4)
 {
  if ((src_color >> 24 & 0xFF) == 0xFF)
   *(uint32_t*)dst = src_color;
  else if ((src_color >> 24 & 0xFF) == 0)
   return ;
  else if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
  {
   dst[0] = this->GetColorWithAlpha(src_color >> 16 & 0xFF,
        dst[0],
        src_color >> 24 & 0xFF);
   dst[1] = this->GetColorWithAlpha(src_color >> 8 & 0xFF,
        dst[1],
        src_color >> 24 & 0xFF);
   dst[2] = this->GetColorWithAlpha(src_color & 0xFF,
        dst[2],
        src_color >> 24 & 0xFF);
  }
  else
  {
   dst[0] = this->GetColorWithAlpha(src_color & 0xFF,
        dst[0],
        src_color >> 24 & 0xFF);
   dst[1] = this->GetColorWithAlpha(src_color >> 8 & 0xFF,
        dst[1],
        src_color >> 24 & 0xFF);
   dst[2] = this->GetColorWithAlpha(src_color >> 16 & 0xFF,
        dst[2],
        src_color >> 24 & 0xFF);
  }
 }
 else if (sbpp == 1)
  *dst = *src;
 else if (sbpp == 2)
  *(uint16_t*)dst = *(uint16_t*)src;
 else if (sbpp == 3)
 {
  if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
  {
   dst[0] = (src_color >> 16) & 0xff;
   dst[1] = (src_color >> 8) & 0xff;
   dst[2] = src_color & 0xff;
  }
  else
  {
   dst[0] = src_color & 0xff;
   dst[1] = (src_color >> 8) & 0xff;
   dst[2] = (src_color >> 16) & 0xff;
  }
 }
}

inline void CopyPixelsByByte(SDL_Surface* SDL, Surface* ID)
{
 int x;
 int y;
 int w;
 int h;
 int sbpp;
 int dbpp;
 uint8_t* dp;
 uint8_t* sp;
 uint16_t spitch;
 uint16_t dpitch;

 sbpp = SDL->format->BytesPerPixel;
 spitch = SDL->pitch;
 h = SDL->h;
 w = SDL->w;
 sp = (uint8_t*)(SDL->pixels);
 if (h > ID->h)
  h = ID->h;
 if (w > ID->w)
  w = ID->w;
 if (h <= 0 || w <= 0)
  return ;
 y = 0;
 dbpp = ID->__surface->format->BytesPerPixel;
 dpitch = ID->__surface->pitch;
 dp = (uint8_t*)(ID->__surface->pixels);
 uint32_t c;
 while (y < h)
 {
  x = 0;
  while (x < w)
  {

   c = *(uint32_t*)sp;
   *(uint32_t*)dp = c;
   ++x;
   dp = (uint8_t*)dp + dbpp;
   sp = (uint8_t*)sp + sbpp;
  }
  ++y;
  dp = (uint8_t*)dp + dpitch - dbpp * x;
  sp = (uint8_t*)sp + spitch - sbpp * x;
 }
}


inline int SetAlpha(Surface* surface, uint32_t flags, uint8_t alpha)
{
 return SDL_SetAlpha(surface->__surface, flags, alpha);
}

inline void WM_SetCaption(const char* title, const char* icon)
{
 SDL_WM_SetCaption(title, icon);
}

inline int WM_IconifyWindow(void)
{
 if (SDL_WM_IconifyWindow() != 0)
  return 0;
 return -1;
}

inline int WM_ToggleFullScreen(Surface* surface)
{
 return !SDL_WM_ToggleFullScreen(surface->__surface);
}

inline void WM_SetIcon(Surface* icon, uint8_t* mask)
{
 SDL_WM_SetIcon(icon->__surface, mask);
}
 private:
  Wrapper_Interface(void);
  ~Wrapper_Interface(void);
 };
};

#endif
