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
	void addButton(const Button& btn) {
		buttons.push_back(btn);
	}
	void addLabel(const Label& lbl) {
		labels.push_back(lbl);
	}
	void draw() {
		DrawRectangle(x, y, padding + width, padding + height, { 255,255,255,120 });
		DrawRectangleLines(x, y, padding + width, padding + height, BLACK);
		for (Button btn : buttons) {
			DrawRectangle(padding + x + btn.x, padding + y + btn.y, btn.width, btn.height, ColorBrightness(btn.bgColor, -0.2));
			DrawRectangle(padding + x + btn.x + 2, padding + y + btn.y + 2, btn.width - 4, btn.height - 4, btn.bgColor);
			int textLength = MeasureText(btn.text.c_str(), btn.fontSize);
			DrawText(TextFormat("ID: %i", btn.id), x + btn.x + 2, y + btn.y + 2, 10, btn.fontColor);
			DrawText(btn.text.c_str(), padding + x + btn.x + (btn.width - textLength) / 2, y + btn.y + (btn.height - 20) / 2, btn.fontSize, btn.fontColor);
		}
		for (Label lbl : labels) {
			DrawText(lbl.text.c_str(), padding + x + lbl.x, y + lbl.y, lbl.fontSize, lbl.fontColor);
		}
	}
	void sizeToFit() {
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

		width = maxWidth;
		height = maxHeight;
	}
	int getHovering(Vector2 mousePosition) {
		for (Button btn : buttons) {
			if (mousePosition.x >= padding + x + btn.x && mousePosition.x <= padding + x + btn.x + btn.width &&
				mousePosition.y >= padding + y + btn.y && mousePosition.y <= padding + y + btn.y + btn.height) {
				return btn.id;
			}
		}
		return -1;
	}
};