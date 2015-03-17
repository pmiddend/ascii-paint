#ifndef ASCII_PAINT_BRUSH_H
#define ASCII_PAINT_BRUSH_H

class TCODConsole;
typedef struct {
	TCODColor fore;
	TCODColor back;
	unsigned char symbol;
	bool solid;
	bool walkable;
} Brush;

void applyBrushToCell(int x, int y, Brush &brush, TCODConsole* con);

#endif
