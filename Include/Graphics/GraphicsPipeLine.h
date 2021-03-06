#pragma once
#include <vector>
#include <./Graphics/GraphicStatus.h>
#include <./Graphics/GraphicObject.h>

class GraphicsPipeLine
{
public:
	GraphicsPipeLine();
	GraphicsPipeLine(unsigned long size);
	~GraphicsPipeLine();

	void setStatus();
	void Release();
	virtual bool Draw();
	virtual bool Attach(GraphicStatus* ptr);
	virtual bool Remove(GraphicStatus* ptr);

protected:
private:
	std::vector<GraphicStatus*> status;
	GraphicObject* object;
};
