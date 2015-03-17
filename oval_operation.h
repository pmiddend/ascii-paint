#ifndef ASCII_PAINT_OVAL_OPERATION_H
#define ASCII_PAINT_OVAL_OPERATION_H

#include "operation.h"

class OvalOperation : public Operation {
	public:
		OvalOperation();
		virtual ~OvalOperation();
		virtual void start();
		virtual void update();
		virtual void end();

	private:
		void setCircleEndPoint(int* ovalEndX, int* ovalEndY);
		int ovalStartX;
		int ovalStartY;
		bool justCleared;

};

#endif

