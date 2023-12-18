import json
from functools import partial
from PyQt6.QtCore import Qt
from PyQt6.QtGui import QCloseEvent
from PyQt6.QtWidgets import QApplication, QWidget, QLabel, QPushButton, QGridLayout, QTextEdit, QComboBox
import sys

PATH = "/Users/simonesamardzhiev/Desktop/My projects/Recipes App/python/data.json"


class Data:
    data: dict

    def __init__(self) -> None:
        self.read_data()

    def read_data(self) -> None:
        with open(PATH, "r") as file:
            self.data = json.load(file)

    def write_data(self) -> None:
        self.reindex_data()
        with open(PATH, "w") as file:
            json.dump(self.data, file, indent=4)

    def reindex_data(self) -> None:
        for key in self.data.keys():
            for i in range(len(self.data[key])):
                self.data[key][i]['index'] = i

    def delete_recipe(self, index: int, key: str) -> None:
        self.data[key] = [recipe for recipe in self.data[key] if recipe['index'] != index]

    def get_new_index(self, key: str) -> int:
        return self.data[key][-1]['index'] + 1

    def add_recipe(self, key: str, recipe: dict) -> None:
        self.data[key].append(recipe)

    def iter_over_keys(self):
        for key in self.data:
            yield key

    def iter_over_recipes(self, key: str) -> list[dict]:
        for recipe in self.data[key]:
            yield recipe


class NewRecipeWindow(QWidget):
    data: Data
    mainWindow: "Window"

    layout: QGridLayout
    label_name: QLabel
    label_ingredients: QLabel
    label_recipe: QLabel
    text_name: QTextEdit
    text_ingredients: QTextEdit
    text_recipe: QTextEdit
    type_select: QComboBox
    button: QPushButton

    def __init__(self, main_window, data: Data) -> None:
        super().__init__()
        self.setWindowTitle("Recipe")
        self.setGeometry(100, 100, 500, 500)

        # passing the arguments
        self.mainWindow = main_window
        self.data = data

        # creating widgets
        self.layout = QGridLayout()
        self.label_name = QLabel("Enter the name of the recipe :")
        self.label_ingredients = QLabel("Enter the ingredients of the recipe :")
        self.label_recipe = QLabel("Enter the recipe :")
        self.text_name = QTextEdit()
        self.text_ingredients = QTextEdit()
        self.text_recipe = QTextEdit()
        self.type_select = QComboBox()
        self.button = QPushButton("Add the recipe")

        # changing the attributes to the widgets
        for key in self.data.iter_over_keys():
            self.type_select.addItem(key)
        self.button.clicked.connect(self.on_add_pressed)
        # adding the widgets
        self.layout.addWidget(self.label_name, 0, 0)
        self.layout.addWidget(self.text_name, 0, 1)
        self.layout.addWidget(self.label_ingredients, 1, 0)
        self.layout.addWidget(self.text_ingredients, 1, 1)
        self.layout.addWidget(self.label_recipe, 2, 0)
        self.layout.addWidget(self.text_recipe, 2, 1)
        self.layout.addWidget(self.type_select, 3, 0)
        self.layout.addWidget(self.button, 4, 0)

        self.setLayout(self.layout)
        self.show()

    def on_add_pressed(self):
        recipe = {
            "name": self.text_name.toPlainText(),
            "ingredients": self.text_ingredients.toPlainText(),
            "recipe": self.text_recipe.toPlainText(),
            "index": self.data.get_new_index(self.type_select.currentText())
        }
        self.data.add_recipe(self.type_select.currentText(), recipe)
        self.mainWindow.render_recipes()
        self.close()


class RecipeWindow(QWidget):
    data: Data
    recipe: dict
    key: str
    mainWindow: "Window"

    layout: QGridLayout
    label_name: QLabel
    label_recipe: QLabel
    label_ingredients: QLabel
    text_name: QTextEdit
    text_way: QTextEdit
    text_ingredients: QTextEdit
    button_save: QPushButton
    button_delete: QPushButton

    def __init__(self, main_window, recipe: dict, key: str, data: Data) -> None:
        super().__init__()
        self.setGeometry(50, 50, 400, 400)
        self.setWindowTitle(recipe['name'])

        # passing the arguments
        self.data = data
        self.recipe = recipe
        self.key = key
        self.mainWindow = main_window

        # creating the widgets
        self.layout = QGridLayout()
        self.label_name = QLabel("Name:")
        self.label_recipe = QLabel("Recipe:")
        self.label_ingredients = QLabel("Ingredients:")
        self.text_name = QTextEdit()
        self.text_way = QTextEdit()
        self.text_ingredients = QTextEdit()
        self.button_save = QPushButton("Save")
        self.button_delete = QPushButton("Delete")

        # changing the attributes of the widgets
        self.text_name.setText(self.recipe['name'])
        self.text_way.setText(self.recipe['way'])
        self.text_ingredients.setText(self.recipe['ingredients'])
        self.button_save.clicked.connect(self.on_save_pressed)
        self.button_delete.clicked.connect(self.on_delete_pressed)

        # adding the widgets
        self.layout.addWidget(self.label_name, 0, 0)
        self.layout.addWidget(self.text_name, 0, 1)
        self.layout.addWidget(self.label_ingredients, 1, 0)
        self.layout.addWidget(self.text_ingredients, 1, 1)
        self.layout.addWidget(self.label_recipe, 2, 0)
        self.layout.addWidget(self.text_way, 2, 1)
        self.layout.addWidget(self.button_save, 3, 0)
        self.layout.addWidget(self.button_delete, 4, 0)

        self.setLayout(self.layout)
        self.show()

    def on_save_pressed(self):
        self.recipe['name'] = self.text_name.toPlainText()
        self.recipe['ingredients'] = self.text_ingredients.toPlainText()
        self.recipe['way'] = self.text_way.toPlainText()
        self.mainWindow.render_recipes()
        self.close()

    def on_delete_pressed(self):
        self.data.delete_recipe(self.recipe['index'], self.key)
        self.mainWindow.render_recipes()
        self.close()


class Window(QWidget):
    labels: list[QLabel] = []
    data: Data
    layout: QGridLayout
    buttonForAdding: QPushButton
    windows: list[QWidget] = []
    buttons: list[QPushButton] = []

    def __init__(self) -> None:
        super().__init__()
        # setting window attributes
        self.setWindowTitle("Recipes App")
        self.setGeometry(100, 100, 600, 600)

        # creating basis widgets
        self.data = Data()
        self.layout = QGridLayout()
        for key in self.data.iter_over_keys():
            label = QLabel(key)
            label.setAlignment(Qt.AlignmentFlag.AlignCenter)
            self.labels.append(label)
        self.buttonForAdding = QPushButton("Add recipe")
        self.buttonForAdding.clicked.connect(self.on_add_pressed)

        # putting the widgets into the layout
        self.layout.addWidget(self.buttonForAdding, 0, 3)
        for i in range(len(self.labels)):
            self.layout.addWidget(self.labels[i], 1, i)
        self.render_recipes()

        self.setLayout(self.layout)

    def render_recipes(self) -> None:
        for button in self.buttons:
            print(button)
            self.layout.removeWidget(button)
        self.buttons.clear()

        col = 0
        for key in self.data.iter_over_keys():
            row = 2
            for recipe in self.data.iter_over_recipes(key):
                button = QPushButton(recipe['name'])
                button.pressed.connect(partial(self.on_recipe_pressed, recipe, key))
                self.buttons.append(button)
                self.layout.addWidget(button, row, col)
                row += 1
            col += 1

    def on_recipe_pressed(self, recipe: dict, key: str) -> None:
        self.windows.clear()
        self.windows.append(RecipeWindow(self, recipe, key, self.data))

    def on_add_pressed(self):
        self.windows.clear()
        self.windows.append(NewRecipeWindow(self, self.data))

    def closeEvent(self, event: QCloseEvent) -> None:
        self.windows.clear()
        self.data.write_data()
        event.accept()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(app.exec())
