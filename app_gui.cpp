#include <cstdio>
#include "app_gui.h"
#include "app.h"
#include "cell_operation.h"
#include "color_box.h"
#include "export_operation.h"
#include "fill_operation.h"
#include "fov_operation.h"
#include "import_operation.h"
#include "line_operation.h"
#include "new_operation.h"
#include "open_operation.h"
#include "oval_operation.h"
#include "path_operation.h"
#include "pick_operation.h"
#include "quit_operation.h"
#include "rect_operation.h"
#include "redo_operation.h"
#include "save_operation.h"
#include "text_operation.h"
#include "undo_operation.h"

AppGui::AppGui() {
}

AppGui::~AppGui() {
}

void AppGui::build() {
	// Setup gui system
	guiCon = new TCODConsole(app->windowWidth, app->windowHeight);
	guiCon->setKeyColor(TCODColor(255, 0, 255));
	Widget::setConsole(guiCon);

	hideType = NO_HIDE;

	// Status Bar
	new StatusBar(0, 0, app->windowWidth, 1);

	// The left and right toolbar holders
	VBox *vboxLeft = new VBox(0, 2, 1);
	VBox *vboxRight = new VBox(app->windowWidth - 15, 2, 1);

	// Misc for use later
	VBox *vbox = NULL;
	HBox *hbox = NULL;

	RadioButton::setDefaultGroup(1);

	// File toolbar
	ToolBar *file = new ToolBar(0, 0, 10, "File", "Perform file related operations");
	file->addWidget(new Button("New", "Start a new drawing", operationCbk, new NewOperation));
	file->addWidget(new Button("Open", "Open a drawing", operationCbk, new OpenOperation));
	file->addWidget(new Button("Save", "Save current drawing", operationCbk, new SaveOperation));
	file->addWidget(new RadioButton("Import", "Import files to this drawing", operationCbk, new ImportOperation));
	file->addWidget(new RadioButton("Export", "Export this drawing as a .bmp file", operationCbk, new ExportOperation));
	file->addWidget(new Button("Quit", "Quit ASCII Paint", operationCbk, new QuitOperation));

	// Draw toolbar
	ToolBar *draw = new ToolBar(0, 0, 10, "Draw", "Tools to draw the image with");


	cellOperationButton = new RadioButton("Cell", "Draw single ASCII cells", operationCbk, new CellOperation);
	lineOperationButton = new RadioButton("Line", "Draw a line. Hold Shift for straight lines", operationCbk, new LineOperation);
	rectOperationButton = new RadioButton("Rect", "Draw a rectangle. Hold Shift to draw squares", operationCbk, new RectOperation);
	ovalOperationButton = new RadioButton("Oval", "Draw ovals/ellipses. Hold Shift to draw circles", operationCbk, new OvalOperation);
	fillOperationButton = new RadioButton("Fill", "Flood fill in enclosed areas", operationCbk, new FillOperation);
	textOperationButton = new RadioButton("Text", "Draw Text. Click to place text, then type your text and press Enter.", operationCbk, new TextOperation);

	cellOperationButton->select();
	draw->addWidget(cellOperationButton);
	draw->addWidget(lineOperationButton);
	draw->addWidget(rectOperationButton);
	draw->addWidget(ovalOperationButton);
	draw->addWidget(fillOperationButton);
	draw->addWidget(textOperationButton);

	// Tools toolbar
	ToolBar *tools = new ToolBar(0, 0, 10, "Tools", "Misc tools"); // Need a better description
	tools->addWidget(new Button("Pick", "Pick a color from the image", operationCbk, new PickOperation));
	tools->addWidget(new Button("Undo", "Undo the previous operation", operationCbk, new UndoOperation));
	tools->addWidget(new Button("Redo", "Undo the previous undo", operationCbk, new RedoOperation));

	// RL Tools toolbar
	ToolBar *rlTools = new ToolBar(0, 0, 10, "Rogue", "Tools for roguelike developers"); 
	fovOperationButton = new RadioButton("FOV", "Draw a field of view", operationCbk, new FovOperation);
	pathOperationButton = new RadioButton("Path", "Draw the shortest path between two points", operationCbk, new PathOperation);

	rlTools->addWidget(fovOperationButton);
	rlTools->addWidget(pathOperationButton);

	// Info toolbar
	ToolBar *info = new ToolBar(0, 0, 10, "Info", "Information about file and canvas");
	currentPositionLabel = new Label(0, 0, "X: 0, Y: 0", "Coordinates of cursor");
	canvasSizeLabel = new Label(0, 0, "0 x 0", "Dimensions of canvas");
	fpsLabel = new Label(0, 0, "FPS: 0", "The frames per second by which ASCII-Paint is running");
	
	info->addWidget(currentPositionLabel);
	info->addWidget(canvasSizeLabel);
	info->addWidget(fpsLabel);

	// -------- Options
	// Primary Brush Foreground
	ToolBar *brush1Bar = new ToolBar(0, 0, 10, "Primary", "Set the properties of the primary(left mouse button) brush");
	brush1Bar->addSeparator("Foreground", "The foreground color of the primary brush");

	vbox = new VBox(0, 0, 0);

	redSlider1 = new Slider(0, 0, 3, 0.0f, 255.0f, "r", "Red component of brush");
	redSlider1->setFormat("%.0f");
	redSlider1->setValue((float)app->brush1.fore.r);
	redSlider1->setCallback(changeColorCbk, &app->brush1.fore.r);
	vbox->addWidget(redSlider1);

	greenSlider1 = new Slider(0, 0, 3, 0.0f, 255.0f, "g", "Green component of brush");
	greenSlider1->setFormat("%.0f");
	greenSlider1->setValue((float)app->brush1.fore.g);
	greenSlider1->setCallback(changeColorCbk, &app->brush1.fore.g);
	vbox->addWidget(greenSlider1);

	blueSlider1 = new Slider(0, 0, 3, 0.0f, 255.0f, "b", "Blue component of brush");
	blueSlider1->setFormat("%.0f");
	blueSlider1->setValue((float)app->brush1.fore.b);
	blueSlider1->setCallback(changeColorCbk, &app->brush1.fore.b);
	vbox->addWidget(blueSlider1);


	// Add color preview
	hbox = new HBox(0, 0, 1);
	brush1ForeImg = new ColorBox(1, 0, 3, 3, "Primary Foreground Color");
	brush1ForeImg->setBackgroundColor(TCODColor(app->brush1.fore.r, app->brush1.fore.g, app->brush1.fore.b));
	brush1ForeImg->setColorToChange(&app->brush1.fore);

	hbox->addWidget(vbox);
	hbox->addWidget(brush1ForeImg);
	brush1Bar->addWidget(hbox);

	// Primary Brush Background
	brush1Bar->addSeparator("Background", "The background color of the primary brush");

	vbox = new VBox(0, 0, 0);

	redSlider2 = new Slider(0, 0, 3, 0.0f, 255.0f, "r", "Red component of brush");
	redSlider2->setFormat("%.0f");
	redSlider2->setValue((float)app->brush1.back.r);
	redSlider2->setCallback(changeColorCbk, &app->brush1.back.r);
	vbox->addWidget(redSlider2);

	greenSlider2 = new Slider(0, 0, 3, 0.0f, 255.0f, "g", "Green component of brush");
	greenSlider2->setFormat("%.0f");
	greenSlider2->setValue((float)app->brush1.back.g);
	greenSlider2->setCallback(changeColorCbk, &app->brush1.back.g);
	vbox->addWidget(greenSlider2);

	blueSlider2 = new Slider(0, 0, 3, 0.0f, 255.0f, "b", "Blue component of brush");
	blueSlider2->setFormat("%.0f");
	blueSlider2->setValue((float)app->brush1.back.b);
	blueSlider2->setCallback(changeColorCbk, &app->brush1.back.b);
	vbox->addWidget(blueSlider2);

	// Add color preview
	hbox = new HBox(0, 0, 1);
	brush1BackImg = new ColorBox(1, 0, 3, 3, "Primary Background Color");
	brush1BackImg->setBackgroundColor(TCODColor(app->brush1.back.r, app->brush1.back.g, app->brush1.back.b));
	brush1BackImg->setColorToChange(&app->brush1.back);
	hbox->addWidget(vbox);
	hbox->addWidget(brush1BackImg);
	brush1Bar->addWidget(hbox);

	// Solid button
	brush1Bar->addWidget(new Label(0, 0, "", ""));
	solidToggleButton1 = new ToggleButton("Solid", "Whether the cells drawn to should be solid", changeSolidCbk, &app->brush1.solid);
        solidToggleButton1->setPressed(true);
        brush1Bar->addWidget(solidToggleButton1);

	// Primary Brush Symbol
	brush1Bar->addWidget(new Label(0, 0, "", "The symbol of the primary brush"));
	std::string symbolButtonString = "Symbol ";
	symbolButtonString += app->brush1.symbol;
	symbolButton1 = new Button(symbolButtonString.c_str(), "Set the symbol", changeSymbolCallback, &app->brush1.symbol);
	brush1Bar->addWidget(symbolButton1);


	// Secondary Brush Toolbar
	// Secondary Brush Foreground
	ToolBar *brush2Bar = new ToolBar(0, 0, 10, "Secondary", "Set the properties of the secondary(right mouse button) brush");
	brush2Bar->addSeparator("Foreground", "The foreground color of the secondary brush");

	vbox = new VBox(0, 0, 0);

	redSlider3 = new Slider(0, 0, 3, 0.0f, 255.0f, "r", "Red component of brush");
	redSlider3->setFormat("%.0f");
	redSlider3->setValue((float)app->brush2.fore.r);
	redSlider3->setCallback(changeColorCbk, &app->brush2.fore.r);
	vbox->addWidget(redSlider3);

	greenSlider3 = new Slider(0, 0, 3, 0.0f, 255.0f, "g", "Green component of brush");
	greenSlider3->setFormat("%.0f");
	greenSlider3->setValue((float)app->brush2.fore.g);
	greenSlider3->setCallback(changeColorCbk, &app->brush2.fore.g);
	vbox->addWidget(greenSlider3);

	blueSlider3 = new Slider(0, 0, 3, 0.0f, 255.0f, "b", "Blue component of brush");
	blueSlider3->setFormat("%.0f");
	blueSlider3->setValue((float)app->brush2.fore.b);
	blueSlider3->setCallback(changeColorCbk, &app->brush2.fore.b);
	vbox->addWidget(blueSlider3);


	// Add color preview
	hbox = new HBox(0, 0, 1);
	brush2ForeImg = new ColorBox(1, 0, 3, 3, "Secondary Foreground Color");
	brush2ForeImg->setBackgroundColor(TCODColor(app->brush2.fore.r, app->brush2.fore.g, app->brush2.fore.b));
	brush2ForeImg->setColorToChange(&app->brush2.fore);

	hbox->addWidget(vbox);
	hbox->addWidget(brush2ForeImg);
	brush2Bar->addWidget(hbox);

	// Secondary Brush Background
	brush2Bar->addSeparator("Background", "The background color of the secondary brush");

	vbox = new VBox(0, 0, 0);

	redSlider4 = new Slider(0, 0, 3, 0.0f, 255.0f, "r", "Red component of brush");
	redSlider4->setFormat("%.0f");
	redSlider4->setValue((float)app->brush2.back.r);
	redSlider4->setCallback(changeColorCbk, &app->brush2.back.r);
	vbox->addWidget(redSlider4);

	greenSlider4 = new Slider(0, 0, 3, 0.0f, 255.0f, "g", "Green component of brush");
	greenSlider4->setFormat("%.0f");
	greenSlider4->setValue((float)app->brush2.back.g);
	greenSlider4->setCallback(changeColorCbk, &app->brush2.back.g);
	vbox->addWidget(greenSlider4);

	blueSlider4 = new Slider(0, 0, 3, 0.0f, 255.0f, "b", "Blue component of brush");
	blueSlider4->setFormat("%.0f");
	blueSlider4->setValue((float)app->brush2.back.b);
	blueSlider4->setCallback(changeColorCbk, &app->brush2.back.b);
	vbox->addWidget(blueSlider4);

	// Add color preview
	hbox = new HBox(0, 0, 1);
	brush2BackImg = new ColorBox(1, 0, 3, 3, "Secondary Background Color");
	brush2BackImg->setBackgroundColor(TCODColor(app->brush2.back.r, app->brush2.back.g, app->brush2.back.b));
	brush2BackImg->setColorToChange(&app->brush2.back);
	hbox->addWidget(vbox);
	hbox->addWidget(brush2BackImg);
	brush2Bar->addWidget(hbox);

	// Solid button
	brush2Bar->addWidget(new Label(0, 0, "", ""));
        solidToggleButton2 = new ToggleButton("Solid", "Whether the cells drawn to should be solid", changeSolidCbk, &app->brush2.solid);
        solidToggleButton2->setPressed(true);
	brush2Bar->addWidget(solidToggleButton2);

	// Secondary Brush Symbol
	brush2Bar->addWidget(new Label(0, 0, "", "The symbol of the primary brush"));
	symbolButtonString = "Symbol ";
	symbolButtonString += app->brush2.symbol;
	symbolButton2 = new Button(symbolButtonString.c_str(), "Set the symbol", changeSymbolCallback, &app->brush2.symbol);
	brush2Bar->addWidget(symbolButton2);

	// Other options
	ToolBar *options = new ToolBar(0, 0, 10, "Options", "Various other options");

	outlineToggleButton = new ToggleButton("Outline", "Whether there should be an outline for the drawn shape", NULL, NULL);
	outlineToggleButton->setPressed(true);
	filledToggleButton = new ToggleButton("Filled", "Whether the drawn shape should be filled", NULL, NULL);

	useSymbolToggleButton = new ToggleButton("Use Symbol", "If on then the symbol of the brush will be used in drawing", NULL, NULL);
	useForegroundToggleButton = new ToggleButton("Use Fore", "If on then the foreground of the brush will be used in drawing", NULL, NULL);
	useBackgroundToggleButton = new ToggleButton("Use Back", "If on then the background of the brush will be used in drawing", NULL, NULL);
	useSolidToggleButton = new ToggleButton("Use Solid", "If on then the solidness of the brush will be used in drawing", NULL, NULL);

	useSymbolToggleButton->setPressed(true);
	useForegroundToggleButton->setPressed(true);
	useBackgroundToggleButton->setPressed(true);
	useSolidToggleButton->setPressed(true);

	pickSymbolToggleButton = new ToggleButton("Pick Symbol", "If on then the symbol of the cell will be picked", NULL, NULL);
	pickForegroundToggleButton = new ToggleButton("Pick Fore", "If on then the foreground of the cell will be picked", NULL, NULL);
	pickBackgroundToggleButton = new ToggleButton("Pick Back", "If on then the background of the cell will be picked", NULL, NULL);
	pickSolidToggleButton = new ToggleButton("Pick Solid", "If on then the solidness of the cell will be picked", NULL, NULL);

	pickSymbolToggleButton->setPressed(true);
	pickForegroundToggleButton->setPressed(true);
	pickBackgroundToggleButton->setPressed(true);
	pickSolidToggleButton->setPressed(true);

	exportPngButton = new Button("Export PNG", "Export the current image to a .png file", doExportPngCbk, NULL);
	exportBmpButton = new Button("Export BMP", "Export the current image to a .bmp file", doExportBmpCbk, NULL);
	exportTextButton = new Button("Export Text", "Export the symbols of the current image to a .txt file", doExportTextCbk, NULL);
	exportForeButton = new Button("Export Fore", "Export the foreground colors of the image to a .png file", doExportForeCbk, NULL);
	exportBackButton = new Button("Export Back", "Export the background colors of the image to a .png file", doExportBackCbk, NULL);

	importTextButton = new Button("Import Text", "Import the symbols of the current image from a .txt file", doImportTextCbk, NULL);
	importForeButton = new Button("Import Fore", "Import the foreground colors of the image from a .png file", doImportForeCbk, NULL);
	importBackButton = new Button("Import Back", "Import the background colors of the image from a .png file", doImportBackCbk, NULL);

	fovRadiusSlider = new Slider(0, 0, 3, 0.0f, 255.0f, "Radius", "Radius of the field of view. 0 is infinite.");
	fovRadiusSlider->setFormat("%.0f");
	fovRadiusSlider->setValue(10);
	fovLightWallsToggleButton = new ToggleButton("Light Walls", "Whether the wall cells near ground cells in fov must be in fov too.", NULL, NULL);
	fovTypeButton = new Button("Basic", "The type of the field of view. Click to change", fovTypeButtonCbk, NULL);

	pathLengthLabel = new Label(0, 0, "Length: 0", "Length of the path");
	pathDiagonalToggleButton = new ToggleButton("Diagonal", "Whether the path should allow diagonal movements", NULL, NULL);
	pathDiagonalToggleButton->setPressed(true);

	options->addWidget(outlineToggleButton);
	options->addWidget(filledToggleButton);
	options->addWidget(useSymbolToggleButton);
	options->addWidget(useForegroundToggleButton);
	options->addWidget(useBackgroundToggleButton);
	options->addWidget(useSolidToggleButton);
	options->addWidget(pickSymbolToggleButton);
	options->addWidget(pickForegroundToggleButton);
	options->addWidget(pickBackgroundToggleButton);
	options->addWidget(pickSolidToggleButton);
	options->addWidget(exportPngButton);
	options->addWidget(exportBmpButton);
	options->addWidget(exportTextButton);
	options->addWidget(exportForeButton);
	options->addWidget(exportBackButton);
	options->addWidget(importTextButton);
	options->addWidget(importForeButton);
	options->addWidget(importBackButton);
	options->addWidget(fovRadiusSlider);
	options->addWidget(fovLightWallsToggleButton);
	options->addWidget(fovTypeButton);
	options->addWidget(pathLengthLabel);
	options->addWidget(pathDiagonalToggleButton);

	// View toolbar
	ToolBar *view = new ToolBar(0, 0, 10, "View", "Change the current view settings");
	viewImageToggleButton = new ToggleButton("Image", "Whether the image should be viewed", NULL, NULL);
	viewSolidToggleButton = new ToggleButton("Solid", "Whether the solid view should be enabled", NULL, NULL);

	viewImageToggleButton->setPressed(true);

	view->addWidget(viewImageToggleButton);
	view->addWidget(viewSolidToggleButton);

	vboxLeft->addWidget(file);
	vboxLeft->addWidget(draw);
	vboxLeft->addWidget(tools);
	vboxLeft->addWidget(rlTools);
	vboxLeft->addWidget(info);
	vboxRight->addWidget(brush1Bar);
	vboxRight->addWidget(brush2Bar);
	vboxRight->addWidget(options);
	vboxRight->addWidget(view);
}

void AppGui::update() {
		// IF the gui is not hidden, the current operation is not active, and
		// control is not being pressed (because the user is not dragging the
		// console) THEN update the gui
		if(hideType != TOTAL_HIDE && !app->currentOperation->isActive() && !TCODConsole::isKeyPressed(TCODK_CONTROL))
			Widget::updateWidgets(app->key,TCODMouse::getStatus());
		
		// Pressing TAB will hide/unhide GUI
		if(app->key.vk == TCODK_TAB) {
			if(hideType == TOTAL_HIDE)
				hideType = NO_HIDE;
			else 
				hideType = TOTAL_HIDE;
		}

		//app->brush1.solid = solidToggleButton1->isPressed();
		//app->brush2.solid = solidToggleButton2->isPressed();
                solidToggleButton1->setPressed(app->brush1.solid);
                solidToggleButton2->setPressed(app->brush2.solid);

}

void AppGui::draw() {
	Widget::setConsole(guiCon);
	Widget::setForegroundColor(foregroundColor1, foregroundColor2);
	Widget::setBackgroundColor(backgroundColor1, backgroundColor2);

	guiCon->setDefaultBackground(TCODColor(255, 0, 255));
	guiCon->clear();
	//overlayCon->setBackgroundColor(TCODColor(1, 1, 1));
	//canvasCon->setBackgroundColor(TCODColor(255, 255, 255));

	// Update the slider values
	redSlider1->setValue(app->brush1.fore.r);
	greenSlider1->setValue(app->brush1.fore.g);
	blueSlider1->setValue(app->brush1.fore.b);
	redSlider2->setValue(app->brush1.back.r);
	greenSlider2->setValue(app->brush1.back.g);
	blueSlider2->setValue(app->brush1.back.b);
	redSlider3->setValue(app->brush2.fore.r);
	greenSlider3->setValue(app->brush2.fore.g);
	blueSlider3->setValue(app->brush2.fore.b);
	redSlider4->setValue(app->brush2.back.r);
	greenSlider4->setValue(app->brush2.back.g);
	blueSlider4->setValue(app->brush2.back.b);

	// Update the app->brush color previews
	brush1ForeImg->setBackgroundColor(TCODColor(app->brush1.fore.r, app->brush1.fore.g, app->brush1.fore.b));
	brush1BackImg->setBackgroundColor(TCODColor(app->brush1.back.r, app->brush1.back.g, app->brush1.back.b));
	brush2ForeImg->setBackgroundColor(TCODColor(app->brush2.fore.r, app->brush2.fore.g, app->brush2.fore.b));
	brush2BackImg->setBackgroundColor(TCODColor(app->brush2.back.r, app->brush2.back.g, app->brush2.back.b));

	// Update the app->brush symbol previews
	// Note: I need to learn a better way to store strings and delete
	static char* symbolButtonString = new char[20];
	sprintf(symbolButtonString, "Symbol %c", app->brush1.symbol);

	// If we try printing just one % then the program crashes so we print two % instead
	if(app->brush1.symbol == '%')
		strcat(symbolButtonString, "%");
	symbolButton1->setLabel(symbolButtonString);

	// Now for app->brush2
	sprintf(symbolButtonString, "Symbol %c", app->brush2.symbol);

	// If we try printing just one % then the program crashes so we print two % instead
	if(app->brush2.symbol == '%')
		strcat(symbolButtonString, "%");
	symbolButton2->setLabel(symbolButtonString);


	// Update the current mouse position label
	static char* currentPositionLabelString = new char[20];
	sprintf(currentPositionLabelString, "%i, %i", app->canvasMouseX, app->canvasMouseY);
	currentPositionLabel->setValue(currentPositionLabelString);

	// Update canvas size label
	static char* canvasSizeLabelString = new char[20];
	sprintf(canvasSizeLabelString, "%i x %i", app->canvasWidth, app->canvasHeight);
	canvasSizeLabel->setValue(canvasSizeLabelString);

	// Update canvas size label
	static char* fpsLabelString = new char[20];
	sprintf(fpsLabelString, "FPS: %i", TCODSystem::getFps());
	fpsLabel->setValue(fpsLabelString);

	static int fade = 200;
	if(hideType == NO_HIDE) {
		Widget::renderWidgets();
		TCODConsole::blit(guiCon, 0, 0, app->windowWidth, app->windowHeight, TCODConsole::root, 0, 0, 255);

	} else if(hideType == PARTIAL_HIDE) {
		Widget::renderWidgets();
		// Calculate fade
		if(Widget::focus) { // if in focus
			if(app->mouse.lbutton == false && app->mouse.rbutton == false) // if the user is not drawing
				if(fade < 255) fade += 20;
		} else {
			if(fade > 80) fade -= 20;
		}
		if(fade > 255) fade = 255;

		TCODConsole::blit(guiCon, 0, 0, app->windowWidth, app->windowHeight, TCODConsole::root, 0, 0, fade);

	} else if(hideType == TOTAL_HIDE) {

	}
}

void AppGui::hideAllOptions() {
	outlineToggleButton->setVisible(false);
	filledToggleButton->setVisible(false);

	useSymbolToggleButton->setVisible(false);
	useForegroundToggleButton->setVisible(false);
	useBackgroundToggleButton->setVisible(false);
	useSolidToggleButton->setVisible(false);

	pickSymbolToggleButton->setVisible(false);
	pickForegroundToggleButton->setVisible(false);
	pickBackgroundToggleButton->setVisible(false);
	pickSolidToggleButton->setVisible(false);

	exportPngButton->setVisible(false);
	exportBmpButton->setVisible(false);
	exportTextButton->setVisible(false);
	exportForeButton->setVisible(false);
	exportBackButton->setVisible(false);

	importTextButton->setVisible(false);
	importForeButton->setVisible(false);
	importBackButton->setVisible(false);

	fovRadiusSlider->setVisible(false);
	fovLightWallsToggleButton->setVisible(false);
	fovTypeButton->setVisible(false);

	pathLengthLabel->setVisible(false);
	pathDiagonalToggleButton->setVisible(false);
}

// -------- Callbacks
void operationCbk(Widget *wid, void *data) {
	Operation *op = reinterpret_cast<Operation*>(data);
	op->getApp()->changeOperation(op);
}

void changeColorCbk(Widget *wid, float val, void *data) {
	// data points to some color component such as &app->brush1.back.r
	*(reinterpret_cast<uint8*>(data)) = val;
}

void changeSolidCbk(Widget *wid, void *data) {
	// data points to the solid component of a brush &app->brush1.solid
	ToggleButton *toggleButton = reinterpret_cast<ToggleButton *>(wid);
	*(reinterpret_cast<bool *>(data)) = toggleButton->isPressed();
}

void changeSymbolCallback(Widget *wid, void *data) {
	// data should be a pointer to a brush symbol eg &app->brush1.symbol

	// Draw a grid of ASCII characters
	int xOffset = (AppUser::app->windowWidth - 35)/2;
	int yOffset = (AppUser::app->windowHeight - 35)/2;
	int spacing = 2;
	TCODConsole::root->setDefaultBackground(TCODColor::black);
	TCODConsole::root->setDefaultForeground(TCODColor::white);
	// Prio #1
        TCODConsole::root->printFrame(xOffset - 2, yOffset - 2, 35, 35, true, TCOD_BKGND_SET, "Select symbol");

	for(int x = 0; x < 16; x++) {
		for(int y = 0; y < 16; y++) {
			TCODConsole::root->putChar(xOffset + spacing * x, yOffset + spacing * y, x * 16 + y);
		}
	}

	TCODConsole::root->flush();

	TCOD_key_t key;
	TCOD_mouse_t mouse;
	char newSymbol;

	// Loop until the user presses a valid or the user presses the left mouse button
	do {
		key = TCODConsole::checkForKeypress();
		mouse = TCODMouse::getStatus();

		if(mouse.lbutton_pressed) {
			break;
		}

		int x = (mouse.cx - xOffset) / (float)spacing;
		int y = (mouse.cy - yOffset) / (float)spacing;

		// If the user is hovering over a symbol
		if(x >= 0 && x < 16 && y >= 0 && y < 16) {

			// First give all the symbols a dark background
			for(int tx = 0; tx < 16; tx++) {
				for(int ty = 0; ty < 16; ty++) {
					TCODConsole::root->setCharBackground(xOffset + spacing * tx, yOffset + spacing * ty, TCODColor(0, 0, 0));
				}
			}

			TCODConsole::root->setCharBackground(xOffset + spacing * x, yOffset + spacing * y, TCODColor(50, 50, 50));
			TCODConsole::root->flush();
		}

		// Keep looping until we get a printable character
	} while(key.vk == TCODK_NONE || key.c == 0);

	if(mouse.lbutton_pressed) {
		int x = (mouse.cx - xOffset) / spacing;
		int y = (mouse.cy - yOffset) / spacing;
		if(x >= 0 && x < 16 && y >= 0 && y < 16) {
			newSymbol = x * 16 + y;
		} else {
			// If the user has clicked outside the box then use the previous symbol
			newSymbol = *reinterpret_cast<unsigned char*>(data);
		}
	} else {
		// If the user has not clicked it means they pressed a key
		// Pressing escape will set newSymbol to be the previous symbol
		if(key.vk == TCODK_ESCAPE)
			newSymbol = *reinterpret_cast<unsigned char*>(data);
		else
			newSymbol = key.c;
	}

	*reinterpret_cast<unsigned char*>(data) = newSymbol;
}

