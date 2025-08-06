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
	void addButton(const Button& btn) {
		buttons.push_back(btn);
	}
	void addLabel(const Label& lbl) {
		labels.push_back(lbl);
	}
	void draw() {
		DrawRectangle(0, 0, 800, 800, { 255,255,255,120 });
		for (Button btn : buttons) {
			DrawRectangle(btn.x, btn.y, btn.width, btn.height, ColorBrightness(btn.bgColor, -0.2));
			DrawRectangle(btn.x + 2, btn.y + 2, btn.width - 4, btn.height - 4, btn.bgColor);
			int textLength = MeasureText(btn.text.c_str(), btn.fontSize);
			DrawText(TextFormat("ID: %i", btn.id), btn.x + 2, btn.y + 2, 10, btn.fontColor);
			DrawText(btn.text.c_str(), btn.x + (btn.width - textLength) / 2, btn.y + (btn.height - 20) / 2, btn.fontSize, btn.fontColor);
		}
		for (Label lbl : labels) {
			DrawText(lbl.text.c_str(), lbl.x, lbl.y, lbl.fontSize, lbl.fontColor);
		}
	}
	int getHovering(Vector2 mousePosition) {
		for (Button btn : buttons) {
			if (mousePosition.x >= btn.x && mousePosition.x <= btn.x + btn.width &&
				mousePosition.y >= btn.y && mousePosition.y <= btn.y + btn.height) {
				return btn.id;
			}
		}
		return -1;
	}
};