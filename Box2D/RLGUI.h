#include <functional>
#include <vector>
#include <string>
struct Label {
	int x, y, fontSize;
	Color fontColor;
	std::string text;
};
struct Button {
	int x, y, width, height, fontSize;
	Color bgColor;
	Color fontColor;
	std::string text;
	int id;
};
class GUI {
private:
	std::vector<Label> labels;
	std::vector<Button> buttons;
public:
	int x, y, width, height, padding;
	bool active;
	void addButton(const Button& btn) {
		buttons.push_back(btn);
	}
	void addLabel(const Label& lbl) {
		labels.push_back(lbl);
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
			DrawRectangle(padding + gui.x + btn.x, padding + gui.y + btn.y, btn.width, btn.height, ColorBrightness(btn.bgColor, -0.2));
			DrawRectangle(padding + gui.x + btn.x + 2, padding + gui.y + btn.y + 2, btn.width - 4, btn.height - 4, btn.bgColor);
			int textLength = MeasureText(btn.text.c_str(), btn.fontSize);
			DrawText(TextFormat("ID: %i", btn.id), padding + gui.x + btn.x + 2, padding + gui.y + btn.y + 2, 10, btn.fontColor);
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
};