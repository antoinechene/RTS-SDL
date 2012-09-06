#ifndef __SINGLETON_HPP__
#define __SINGLETON_HPP__

template <typename T>
class	Singleton
{
	public:
		inline static T*	GetInstance(void)
		{
			if (_instance == 0)
				_instance = new T;
			return _instance;
		};
		static void		Kill(void)
		{
			delete _instance;
			_instance = 0;
		};
	protected:
		Singleton<T>(void)
		{
			//hihihi
		}
		virtual ~Singleton<T>(void)
		{
		}
	private:
		static	T*	_instance;
};

template <typename T>
T*	Singleton<T>::_instance = 0;

#endif

