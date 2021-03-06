#ifndef ASCII_PAINT_APP_H
#define ASCII_PAINT_APP_H

#include <string>
#include <vector>
#include "app_gui.h"
#include "app_data.h"
#include "libtcod.hpp"
#include "gui/gui.hpp"

#include "brush.h"
#include "color_box.h"
#include "operation.h"

class Operation;

// Basically a vector representing each cell in the canvas/image
// To access the cell at position (x, y) use:
// CanvasImage[x + y * width]
typedef std::vector<Brush> CanvasImage;

class App {
	public:
		App();
		~App();

		int exec();

		void initBrushes();
		void initCanvas();
		void initOperations();

		void changeOperation(Operation *newOperation);
		bool shouldOperationUpdate();

		void setCanvasImage(CanvasImage& canvasImg);
		CanvasImage* getCanvasImage();

		void clearOverlay();
		void applyBrushToOverlayCell(int x, int y, Brush *brush);
		void applyOverlayToCanvas();
		void applyCanvasToOverlay();
                void setOverlayFade(float fade);

		void addUndo();
		void doUndo();
		void doRedo();

		void resetCanvasView();

	public:
		AppGui *gui;
		AppData *data;

		int windowWidth;
		int windowHeight;
		TCODColor windowBackgroundColor;
		std::string fontFilename;
		int fontLayout;
		int fontType;

		bool fullscreen;
		int fpsGoal;
		int currentFps;

		int canvasWidth;
		int canvasHeight;
		
		// The distance between the top-left corner of canvas and the top-left
		// corner of the screen
		int canvasOffsetX;
		int canvasOffsetY;

		// The cell of the canvas that the mouse is currently over
		int canvasMouseX;
		int canvasMouseY;

		std::string filename;
		float mapVersion;
		bool canvasModified;

		bool isCanvasBeingDragged;
		bool quit;

		Operation *currentOperation;
		Operation *previousOperation;

		std::vector<CanvasImage> pastImages; // Undo Images stored here
		std::vector<CanvasImage> futureImages; // Redo Images store here

		TCODConsole *overlayCon; // Drawn over the canvas
		TCODConsole *canvasCon;
		TCODConsole *solidCon;
		TCODConsole *solidOverlayCon; // This canvas holds proposed writes to solidCon

                float overlayFade;
		bool inTypingMode;

		TCOD_key_t key;
		TCOD_mouse_t mouse;

		Brush brush1;
		Brush brush2;
		Brush initBrush1;
		Brush initBrush2;

	private:
		void handleKeyboardShortcuts();
		void handleCanvasDragging();
		void moveMouseWithKeyBoard();

};


#endif
