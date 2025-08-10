#include <functional>
#include <vector>
#include <string>
struct Label {
	int x, y, fontSize, id;
	Color fontColor;
	std::string text;
};
struct Button {
	int x, y, width, height, fontSize;
	Color bgColor;
	Color bgColorSelected;
	Color fontColor;
	std::string text;
	int id;
	bool selected;
};
class GUI {
private:
	std::vector<Label> labels;
	std::vector<Button> buttons;
public:
	int x, y, width, height, padding;
	bool autoSize; // Automatically size to fit buttons and labels
	bool active;
	void addButton(const Button& btn) {
		buttons.push_back(btn);
	}
	void addLabel(const Label& lbl) {
		labels.push_back(lbl);
	}
	Button* getButtonByID(int id) {
		if (id == -1) return nullptr; // Escape quicker
		for (Button& btn : buttons) {
			if (btn.id == id) {
				return &btn;
			}
		}
		return nullptr; // Button not found
	}
	Vector2 getAdjustedPosition() {
		float guiX = fmin(GetScreenWidth() - (width + padding), x);
		float guiY = fmin(GetScreenHeight() - (height + padding), y);
		return { guiX, guiY };
	}
	void draw() {
		if (!active) return; // Don't draw if not active
		Vector2 gui = getAdjustedPosition();
		DrawRectangle(gui.x, gui.y, padding + width, padding + height, { 255,255,255,120 });
		DrawRectangleLines(gui.x, gui.y, padding + width, padding + height, BLACK);
		for (Button btn : buttons) {
			int textLength = MeasureText(btn.text.c_str(), btn.fontSize);
			if(autoSize) {
				btn.width = fmax(btn.width, textLength + 4); // Add padding
				btn.height = fmax(btn.height, btn.fontSize + 4); // Add padding
			}
			btn.width = fmax(btn.width, textLength + 4); // Add padding
			Color bgColor = (btn.selected ? btn.bgColorSelected : btn.bgColor);
			DrawRectangle(padding + gui.x + btn.x, padding + gui.y + btn.y, btn.width, btn.height, ColorBrightness(bgColor, -0.2));
			DrawRectangle(padding + gui.x + btn.x + 2, padding + gui.y + btn.y + 2, btn.width - 4, btn.height - 4, bgColor);
			//DrawText(TextFormat("ID: %i", btn.id), padding + gui.x + btn.x + 2, padding + gui.y + btn.y + 2, 10, btn.fontColor);
			DrawText(btn.text.c_str(), padding + gui.x + btn.x + (btn.width - textLength) / 2, padding + gui.y + btn.y + (btn.height - 20) / 2, btn.fontSize, btn.fontColor);
		}
		for (Label lbl : labels) {
			DrawText(lbl.text.c_str(), padding + gui.x + lbl.x, padding + gui.y + lbl.y, lbl.fontSize, lbl.fontColor);
		}
	}
	void sizeToFit(bool sizeX = true, bool sizeY = true) {
		int maxWidth = 0;
		int maxHeight = 0;

		for (Button btn : buttons) {
			maxWidth = fmax(maxWidth, padding + btn.x + btn.width);
			maxHeight = fmax(maxHeight, padding + btn.y + btn.height);
		}
		for (Label lbl : labels) {
			Vector2 measure = MeasureTextEx(GetFontDefault(), lbl.text.c_str(), lbl.fontSize, 2);
			
			maxWidth = fmax(maxWidth, padding + lbl.x + measure.x);
			maxHeight = fmax(maxHeight, padding + lbl.y + measure.y);
		}

		if(sizeX) width = maxWidth;
		if(sizeY) height = maxHeight;
	}
	void deselectButtons() {
		for (Button& btn : buttons) {
			btn.selected = false;
		}
	}
	void selectButton(int buttonId) {
		if (buttonId == -1) return; // Ignore if not valid button
		Button* button = getButtonByID(buttonId);
		if (button == nullptr) return;
		button->selected = true;
	}
	int getHovering(Vector2 mousePosition) {
		if (!active) return -1; // GUI is not active
		Vector2 adjustedPosition = getAdjustedPosition();
		for (Button btn : buttons) {
			if (mousePosition.x >= padding + adjustedPosition.x + btn.x && mousePosition.x <= padding + adjustedPosition.x + btn.x + btn.width &&
				mousePosition.y >= padding + adjustedPosition.y + btn.y && mousePosition.y <= padding + adjustedPosition.y + btn.y + btn.height) {
				return btn.id;
			}
		}
		return -1;
	}
	int click(Vector2 mousePosition) {
		int buttonId = getHovering(mousePosition);
		selectButton(buttonId); // Select the button if clicked
		return buttonId; // Return the ID of the button clicked, or -1 if none
	}
	void arrangeButtons() {
		int currentX = 0;
		int currentY = 0;
		for (Button& btn : buttons) {
			if (currentX + btn.width + padding > width) {
				currentX = 0; // Reset X position
				currentY += btn.height + padding; // Move to next row
			}
			btn.x = currentX;
			btn.y = currentY;
			currentX += btn.width + padding; // Move to next button position
		}
	}
	void setText(int id, std::string text) {
		for(Label& lbl : labels) {
			if (lbl.id == id) {
				lbl.text = text;
				return;
			}
		}
		printf("Label with ID %d not found.\n", id);
	}
};