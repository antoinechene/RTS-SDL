#ifndef __CLASS_RESOURCE_MODEL_H__
#define __CLASS_RESOURCE_MODEL_H__

class	Resource_Model
{
 private:
 	int	ID;
	char*	Name;
 public:
	Resource_Model(char** tab);

	~Resource_Model()
	{
	}

	void	SetID(int id);
	int	GetID();
	void	SetName(char* name);
	char*	GetName();
};

#endif
