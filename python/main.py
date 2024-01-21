import json
import sys
import mysql.connector
from functools import partial

from PyQt6.QtCore import Qt
from PyQt6.QtGui import QCloseEvent
from PyQt6.QtWidgets import QApplication, QWidget, QLabel, QPushButton, QLineEdit, QTextEdit, QFrame, QScrollArea, \
    QGridLayout, QComboBox, QCheckBox, QMessageBox, QHBoxLayout, QVBoxLayout


class Data:
    recipes: list[dict] = []
    username: str
    password: str

    def __init__(self) -> None:
        self.get_login_info()
        self.retrieve_data()

    def get_login_info(self) -> None:
        try:
            with open("login_info.json", "r") as file:
                data = json.load(file)
        except FileNotFoundError:
            print("Error reading login_info.json")
            sys.exit(1)

        self.username = data["username"]
        self.password = data["password"]

    def retrieve_data(self) -> None:
        connection = None
        cursor = None

        try:
            connection = mysql.connector.connect(host="localhost", user=self.username, passwd=self.password,
                                                 database="RecipeApp")
            cursor = connection.cursor()
            query = "SELECT * FROM PythonData"
            cursor.execute(query)
            data = cursor.fetchall()

            for row in data:
                recipe = {
                    "id": row[0],
                    "name": row[1],
                    "ingredients": row[2],
                    "wayOfMaking": row[3],
                    "type": row[4]
                }
                self.recipes.append(recipe)
        except mysql.connector.Error as e:
            print("Error while retrieving data ", e)
            sys.exit(1)
        finally:
            if connection is not None:
                connection.close()
            if cursor is not None:
                cursor.close()

    def save_data(self) -> None:
        connection = None
        cursor = None

        try:
            connection = mysql.connector.connect(host="localhost", user=self.username, passwd=self.password,
                                                 database="RecipeApp")
            cursor = connection.cursor()
            query = "DELETE FROM PythonData"
            cursor.execute(query)

            for recipe in self.recipes:
                query = "INSERT INTO PythonData(name, ingredients, wayOfMaking, type) VALUES (%s, %s, %s, %s)"
                cursor.execute(query, (recipe["name"], recipe["ingredients"], recipe["wayOfMaking"], recipe["type"]))

            connection.commit()
        except mysql.connector.Error as e:
            print("Error while saving the data ", e)
            sys.exit(1)
        finally:
            if connection is not None:
                connection.close()
            if cursor is not None:
                cursor.close()

    def delete_recipe(self, _id: int) -> None:
        for recipe in self.recipes:
            if recipe["id"] == _id:
                self.recipes.remove(recipe)
                break

    def add_recipe(self, name: str, ingredients: str, way_of_making: str, t: str) -> None:
        recipe = {
            "name": name,
            "ingredients": ingredients,
            "wayOfMaking": way_of_making,
            "type": t,
            "id": self.get_new_id()
        }

        self.recipes.append(recipe)

    def get_new_id(self) -> int:
        try:
            return self.recipes[-1]["id"] + 1
        except IndexError:
            return 0

    def change_recipe_name(self, name: str, _id: int) -> None:
        for recipe in self.recipes:
            if recipe["id"] == _id:
                recipe["name"] = name
                break

    def change_recipe_ingredients(self, ingredients: str, _id: int) -> None:
        for recipe in self.recipes:
            if recipe["id"] == _id:
                recipe["ingredients"] = ingredients
                break

    def change_recipe_way_of_making(self, way_of_making: str, _id: int) -> None:
        for recipe in self.recipes:
            if recipe["id"] == _id:
                recipe["wayOfMaking"] = way_of_making
                break

    def change_recipe_type(self, t: str, _id: int) -> None:
        for recipe in self.recipes:
            if recipe["id"] == _id:
                recipe["type"] = t
                break

    def __iter__(self) -> dict:
        for recipe in self.recipes:
            yield recipe


class ReadOnlyCheckBox(QCheckBox):
    def __init__(self) -> None:
        super().__init__()

    def mousePressEvent(self, event):
        pass


class SetNameWindow(QWidget):
    wnd: "AddRecipeWindow"
    nameLineEdit: QLineEdit

    def __init__(self, wnd: "AddRecipeWindow") -> None:
        super().__init__()

        # setting attributes to the window
        self.setWindowTitle("Set name")
        self.setGeometry(300, 300, 350, 50)

        # passing the arguments
        self.wnd = wnd

        # creating the widgets
        layout = QHBoxLayout()
        self.nameLineEdit = QLineEdit()
        button_set = QPushButton("Set name")

        # setting attributes and connecting the widgets
        button_set.clicked.connect(self.on_set_button_clicked)

        # adding the widgets
        layout.addWidget(self.nameLineEdit)
        layout.addWidget(button_set)

        self.setLayout(layout)
        self.show()

    def on_set_button_clicked(self) -> None:
        self.wnd.on_name_set(self.nameLineEdit.text())
        self.close()


class SetIngredientsWindow(QWidget):
    wnd: "AddRecipeWindow"
    ingredientsTextEdit: QTextEdit

    def __init__(self, wnd: "AddRecipeWindow") -> None:
        super().__init__()

        # setting attributes to the window
        self.setWindowTitle("Set ingredients")
        self.setGeometry(300, 300, 500, 500)

        # passing the arguments
        self.wnd = wnd

        # creating the widgets
        layout = QVBoxLayout()
        self.ingredientsTextEdit = QTextEdit()
        button_set = QPushButton("Set ingredients")

        # setting attributes and connecting the widgets
        button_set.clicked.connect(self.on_set_button_clicked)

        # adding the widgets
        layout.addWidget(self.ingredientsTextEdit)
        layout.addWidget(button_set)

        self.setLayout(layout)
        self.show()

    def on_set_button_clicked(self) -> None:
        self.wnd.on_ingredient_set(self.ingredientsTextEdit.toPlainText())
        self.close()


class SetWayOfMakingWindow(QWidget):
    wnd: "AddRecipeWindow"
    wayOfMakingTextEdit: QTextEdit

    def __init__(self, wnd: "AddRecipeWindow") -> None:
        super().__init__()

        # setting attributes to the window
        self.setWindowTitle("Set Way of Making")
        self.setGeometry(300, 300, 500, 500)

        # passing the arguments
        self.wnd = wnd

        # creating the widgets
        layout = QVBoxLayout()
        self.wayOfMakingTextEdit = QTextEdit()
        button_set = QPushButton("Set Way of Making")

        # setting attributes and connecting the widgets
        button_set.clicked.connect(self.on_set_button_clicked)

        # adding the widgets
        layout.addWidget(self.wayOfMakingTextEdit)
        layout.addWidget(button_set)

        self.setLayout(layout)
        self.show()

    def on_set_button_clicked(self) -> None:
        self.wnd.on_way_of_making_set(self.wayOfMakingTextEdit.toPlainText())
        self.close()


class SetTypeWindow(QWidget):
    wnd: "AddRecipeWindow"
    types: QComboBox

    def __init__(self, wnd: "AddRecipeWindow") -> None:
        super().__init__()

        # setting attributes to the window:
        self.setWindowTitle("Set Type")
        self.setGeometry(300, 300, 100, 100)

        # passing the arguments
        self.wnd = wnd

        # creating the widgets
        layout = QVBoxLayout()
        self.types = QComboBox()
        button_set = QPushButton("Set Type")

        # setting attributes and connecting the widgets
        for _type in ["Appetizers", "Beverages", "Desserts", "Main courses", "Salads", "Sandwiches",
                      "Soups and stews"]:
            self.types.addItem(_type)
        button_set.clicked.connect(self.on_set_button_clicked)

        # adding the widgets
        layout.addWidget(self.types)
        layout.addWidget(button_set)

        self.setLayout(layout)
        self.show()

    def on_set_button_clicked(self) -> None:
        self.wnd.on_type_set(self.types.currentText())
        self.close()


class AddRecipeWindow(QWidget):
    wnd: "Window"
    data: Data
    name: str
    ingredients: str
    wayOfMaking: str
    t: str

    nameChecked: ReadOnlyCheckBox
    ingredientsChecked: ReadOnlyCheckBox
    wayOfMakingChecked: ReadOnlyCheckBox
    typeChecked: ReadOnlyCheckBox
    inputted = [False, False, False, False]

    setNameWindow: SetNameWindow
    setIngredientsWindow: SetIngredientsWindow
    setWayOfMakingWindow: SetWayOfMakingWindow
    setTypeWindow: SetTypeWindow

    def __init__(self, wnd: "Window", data: Data) -> None:
        super().__init__()

        # setting attributes to the window
        self.setWindowTitle("Add Recipe Window")
        self.setGeometry(200, 200, 200, 200)

        # passing the arguments
        self.wnd = wnd
        self.data = data

        # creating the widgets
        layout = QGridLayout()
        button_set_name = QPushButton("Set name")
        button_set_ingredients = QPushButton("Set ingredients")
        button_set_way_of_making = QPushButton("Set way of making")
        button_set_type = QPushButton("Set type")
        button_add_recipe = QPushButton("Add recipe")

        self.nameChecked = ReadOnlyCheckBox()
        self.ingredientsChecked = ReadOnlyCheckBox()
        self.wayOfMakingChecked = ReadOnlyCheckBox()
        self.typeChecked = ReadOnlyCheckBox()

        # setting attributes and connecting the widgets
        button_set_name.clicked.connect(self.on_button_set_name_clicked)
        button_set_ingredients.clicked.connect(self.on_button_set_ingredients_clicked)
        button_set_way_of_making.clicked.connect(self.on_button_set_way_of_making_clicked)
        button_set_type.clicked.connect(self.on_button_set_type_clicked)
        button_add_recipe.clicked.connect(self.on_button_add_recipe_clicked)

        # adding the widgets
        layout.addWidget(button_set_name, 0, 0)
        layout.addWidget(self.nameChecked, 0, 1)
        layout.addWidget(button_set_ingredients, 1, 0)
        layout.addWidget(self.ingredientsChecked, 1, 1)
        layout.addWidget(button_set_way_of_making, 2, 0)
        layout.addWidget(self.wayOfMakingChecked, 2, 1)
        layout.addWidget(button_set_type, 3, 0)
        layout.addWidget(self.typeChecked, 3, 1)
        layout.addWidget(button_add_recipe, 4, 0)

        self.setLayout(layout)
        self.show()

    def on_button_set_name_clicked(self) -> None:
        self.setNameWindow = SetNameWindow(self)

    def on_name_set(self, name: str) -> None:
        self.name = name
        self.nameChecked.setChecked(True)
        self.inputted[0] = True

    def on_button_set_ingredients_clicked(self) -> None:
        self.setIngredientsWindow = SetIngredientsWindow(self)

    def on_ingredient_set(self, ingredient: str) -> None:
        self.ingredients = ingredient
        self.ingredientsChecked.setChecked(True)
        self.inputted[1] = True

    def on_button_set_way_of_making_clicked(self) -> None:
        self.setWayOfMakingWindow = SetWayOfMakingWindow(self)

    def on_way_of_making_set(self, way_of_making: str) -> None:
        self.wayOfMaking = way_of_making
        self.wayOfMakingChecked.setChecked(True)
        self.inputted[2] = True

    def on_button_set_type_clicked(self) -> None:
        self.setTypeWindow = SetTypeWindow(self)

    def on_type_set(self, t: str) -> None:
        self.t = t
        self.typeChecked.setChecked(True)
        self.inputted[3] = True

    def on_button_add_recipe_clicked(self) -> None:
        if not self.inputted[0]:
            QMessageBox.warning(self, "Error", "Please enter the name")
            return
        elif not self.inputted[1]:
            QMessageBox.warning(self, "Error", "Please enter the ingredients")
            return
        elif not self.inputted[2]:
            QMessageBox.warning(self, "Error", "Please enter the way of the making")
            return
        elif not self.inputted[3]:
            QMessageBox.warning(self, "Error", "Please enter the type")
            return

        self.data.add_recipe(self.name, self.ingredients, self.wayOfMaking, self.t)
        self.wnd.on_search()
        self.close()


class ChangeNameWindow(QWidget):
    nameLineEdit: QLineEdit
    wnd: "RecipeInfoWindow"
    data: Data
    recipe: dict
    recipe: dict

    def __init__(self, wnd: "RecipeInfoWindow", data: Data, recipe: dict) -> None:
        super().__init__()

        # setting attributes to the window
        self.setGeometry(250, 250, 300, 50)
        self.setWindowTitle("Change name")

        # passing the arguments
        self.wnd = wnd
        self.data = data
        self.recipe = recipe

        # creating the widgets
        layout = QHBoxLayout()
        self.nameLineEdit = QLineEdit()
        save_button = QPushButton("Save")

        # setting attributes and connecting the widgets
        self.nameLineEdit.setText(recipe["name"])
        save_button.clicked.connect(self.on_save_button_clicked)

        # adding the widgets
        layout.addWidget(self.nameLineEdit)
        layout.addWidget(save_button)

        self.setLayout(layout)
        self.show()

    def on_save_button_clicked(self) -> None:
        self.wnd.on_change_name(self.nameLineEdit.text())
        self.close()


class ChangeIngredientsWindow(QWidget):
    ingredientsTextEdit: QTextEdit
    wnd: "RecipeInfoWindow"
    data: Data
    recipe: dict

    def __init__(self, wnd: "RecipeInfoWindow", data: Data, recipe: dict) -> None:
        super().__init__()

        # setting attributes to the window
        self.setGeometry(250, 250, 400, 400)
        self.setWindowTitle("Change ingredients")

        # passing the arguments
        self.wnd = wnd
        self.data = data
        self.recipe = recipe

        # creating the widgets
        layout = QVBoxLayout()
        self.ingredientsTextEdit = QTextEdit()
        save_button = QPushButton("Save")

        # setting attributes and connecting the widgets
        self.ingredientsTextEdit.setText(recipe["ingredients"])
        save_button.clicked.connect(self.on_save_button_clicked)

        # adding the widgets
        layout.addWidget(self.ingredientsTextEdit)
        layout.addWidget(save_button)

        self.setLayout(layout)
        self.show()

    def on_save_button_clicked(self) -> None:
        self.wnd.on_change_ingredients(self.ingredientsTextEdit.toPlainText())
        self.close()


class ChangeWayOfMakingWindow(QWidget):
    wayOfMakingTextEdit: QTextEdit
    wnd: "RecipeInfoWindow"
    data: Data
    recipe: dict

    def __init__(self, wnd: "RecipeInfoWindow", data: Data, recipe: dict) -> None:
        super().__init__()

        # setting attributes to the window
        self.setGeometry(250, 250, 400, 400)
        self.setWindowTitle("Change way of making")

        # passing the arguments
        self.wnd = wnd
        self.data = data
        self.recipe = recipe

        # creating the widgets
        layout = QVBoxLayout()
        self.wayOfMakingTextEdit = QTextEdit()
        save_button = QPushButton("Save")

        # setting attributes and connecting the widgets
        self.wayOfMakingTextEdit.setText(recipe["wayOfMaking"])
        save_button.clicked.connect(self.on_save_button_clicked)

        # adding the widgets
        layout.addWidget(self.wayOfMakingTextEdit)
        layout.addWidget(save_button)

        self.setLayout(layout)
        self.show()

    def on_save_button_clicked(self) -> None:
        self.wnd.on_change_way_of_making(self.wayOfMakingTextEdit.toPlainText())
        self.close()


class ChangeTypeWindow(QWidget):
    types: QComboBox
    wnd: "RecipeInfoWindow"
    data: Data

    def __init__(self, wnd: "RecipeInfoWindow", data: Data) -> None:
        super().__init__()

        # setting attributes to the window
        self.setGeometry(250, 250, 300, 300)
        self.setWindowTitle("Change type of recipe")

        # passing the arguments
        self.wnd = wnd
        self.data = data

        # creating the widgets
        layout = QVBoxLayout()
        self.types = QComboBox()
        save_button = QPushButton("Save")

        # setting attributes and connecting the widgets
        for _type in ["Appetizers", "Beverages", "Desserts", "Main courses", "Salads", "Sandwiches",
                      "Soups and stews"]:
            self.types.addItem(_type)
        save_button.clicked.connect(self.on_save_button_clicked)

        # adding the widgets
        layout.addWidget(self.types)
        layout.addWidget(save_button)

        self.setLayout(layout)
        self.show()

    def on_save_button_clicked(self) -> None:
        self.wnd.on_change_type(self.types.currentText())
        self.close()


class RecipeInfoWindow(QWidget):
    wnd: "Window"
    data: Data
    id: int
    labelIngredients: QLabel
    labelWayOfMaking: QLabel

    changeNameWindow: ChangeNameWindow
    changeIngredientsWindow: ChangeIngredientsWindow
    changeWayOfMakingWindow: ChangeWayOfMakingWindow
    changeTypeWindow: ChangeTypeWindow

    def __init__(self, wdn: "Window", data: Data, recipe: dict) -> None:
        super().__init__()

        # setting attributes to the window
        self.setWindowTitle(recipe["name"])
        self.setGeometry(200, 200, 500, 500)

        # passing the arguments
        self.wnd = wdn
        self.data = data
        self.recipe = recipe

        # creating the widgets
        layout = QGridLayout()
        change_name_button = QPushButton("Change Name")
        change_ingredients_button = QPushButton("Change Ingredients")
        change_way_of_making_button = QPushButton("Change Way Of Making")
        change_type_button = QPushButton("Change Type")
        delete_recipe_button = QPushButton("Delete Recipe")
        scroll_area_holder = QFrame()
        scroll_area_holed_layout = QHBoxLayout()
        self.labelIngredients = QLabel(f"Ingredients:\n{recipe['ingredients']}")
        self.labelWayOfMaking = QLabel(f"Way Of Making:\n{recipe['wayOfMaking']}")
        scroll_area1 = QScrollArea()
        scroll_area2 = QScrollArea()

        # setting attributes and connecting the widgets
        change_name_button.clicked.connect(self.on_change_name_button_clicked)
        change_ingredients_button.clicked.connect(self.on_change_ingredients_button_clicked)
        change_way_of_making_button.clicked.connect(self.on_change_way_of_making_button_clicked)
        change_type_button.clicked.connect(self.on_change_type_button_clicked)
        delete_recipe_button.clicked.connect(self.on_delete_recipe_button_clicked)
        scroll_area_holder.setLayout(scroll_area_holed_layout)
        self.labelIngredients.setStyleSheet("QLabel { padding-left: 20px; }")
        self.labelWayOfMaking.setStyleSheet("QLabel { padding-left: 20px; }")
        scroll_area1.setWidgetResizable(True)
        scroll_area1.setWidget(self.labelIngredients)
        scroll_area2.setWidgetResizable(True)
        scroll_area2.setWidget(self.labelWayOfMaking)

        # adding the widgets
        scroll_area_holed_layout.addWidget(scroll_area1)
        scroll_area_holed_layout.addWidget(scroll_area2)

        layout.addWidget(change_name_button, 0, 0)
        layout.addWidget(change_ingredients_button, 0, 1)
        layout.addWidget(change_way_of_making_button, 0, 2)
        layout.addWidget(change_type_button, 0, 3)
        layout.addWidget(delete_recipe_button, 0, 4)
        layout.addWidget(scroll_area_holder, 1, 0, 5, 0)

        self.setLayout(layout)
        self.show()

    def on_change_name_button_clicked(self) -> None:
        self.changeNameWindow = ChangeNameWindow(self, self.data, self.recipe)

    def on_change_name(self, name: str) -> None:
        self.setWindowTitle(name)
        self.data.change_recipe_name(name, self.recipe["id"])
        self.wnd.on_search()

    def on_change_ingredients_button_clicked(self) -> None:
        self.changeIngredientsWindow = ChangeIngredientsWindow(self, self.data, self.recipe)

    def on_change_ingredients(self, ingredients: str) -> None:
        self.labelIngredients.setText(f"Ingredients:\n {ingredients}")
        self.data.change_recipe_ingredients(ingredients, self.recipe["id"])
        self.wnd.on_search()

    def on_change_way_of_making_button_clicked(self) -> None:
        self.changeWayOfMakingWindow = ChangeWayOfMakingWindow(self, self.data, self.recipe)

    def on_change_way_of_making(self, way_of_making: str) -> None:
        self.labelWayOfMaking.setText(f"Way of making:\n {way_of_making}")
        self.data.change_recipe_way_of_making(way_of_making, self.recipe["id"])
        self.wnd.on_search()

    def on_change_type_button_clicked(self) -> None:
        self.changeTypeWindow = ChangeTypeWindow(self, self.data)

    def on_change_type(self, _type: str) -> None:
        self.data.change_recipe_type(_type, self.recipe["id"])
        self.wnd.on_search()

    def on_delete_recipe_button_clicked(self) -> None:
        self.data.delete_recipe(self.recipe["id"])
        self.wnd.on_search()
        self.close()


class Window(QWidget):
    data: Data = Data()
    searchBar: QLineEdit
    typesBox: QComboBox
    resultsLayout: QGridLayout
    recipeInfoWindow: RecipeInfoWindow
    addRecipeWindow: AddRecipeWindow

    def __init__(self) -> None:
        super().__init__()

        # setting attributes to the window
        self.setGeometry(300, 300, 500, 500)
        self.setWindowTitle("Recipe app")

        # creating the widgets
        layout = QGridLayout()
        self.searchBar = QLineEdit()
        self.typesBox = QComboBox()
        results_frame = QFrame()
        results_scroll_area = QScrollArea()
        self.resultsLayout = QGridLayout()
        button_add_recipe = QPushButton("Add recipe")

        # setting attributes and connecting the widgets
        self.searchBar.returnPressed.connect(self.on_search)

        for _type in ["All", "Appetizers", "Beverages", "Desserts", "Main courses", "Salads", "Sandwiches",
                      "Soups and stews"]:
            self.typesBox.addItem(_type)

        button_add_recipe.clicked.connect(self.on_button_add_recipe_clicked)

        results_frame.setLayout(self.resultsLayout)

        results_scroll_area.setWidget(results_frame)
        results_scroll_area.setWidgetResizable(True)
        results_scroll_area.setVerticalScrollBarPolicy(Qt.ScrollBarPolicy.ScrollBarAlwaysOff)

        # adding the widgets
        layout.addWidget(self.searchBar, 0, 0)
        layout.addWidget(self.typesBox, 0, 1)
        layout.addWidget(results_scroll_area, 1, 0, 1, 2)
        layout.addWidget(button_add_recipe, 2, 0, 1, 2)

        self.setLayout(layout)

    def on_button_add_recipe_clicked(self) -> None:
        self.addRecipeWindow = AddRecipeWindow(self, self.data)

    def on_search(self) -> None:
        for i in reversed(range(self.resultsLayout.count())):
            self.resultsLayout.itemAt(i).widget().setParent(None)

        text = self.searchBar.text().strip().lower()
        _type = self.typesBox.currentText()
        row = 0

        for recipe in self.data:
            if _type == "All":
                if text in recipe["name"]:
                    result = QPushButton(recipe["name"])
                    result.clicked.connect(partial(self.on_results_clicked, recipe))
                    self.resultsLayout.addWidget(result, row, 0)
                    row += 1
            else:
                if text in recipe["name"] and recipe["type"] == _type:
                    result = QPushButton(recipe["name"])
                    result.clicked.connect(partial(self.on_results_clicked, recipe))
                    self.resultsLayout.addWidget(result, row, 0)
                    row += 1

    def on_results_clicked(self, recipe: dict) -> None:
        self.recipeInfoWindow = RecipeInfoWindow(self, self.data, recipe)

    def closeEvent(self, event: QCloseEvent) -> None:
        self.data.save_data()
        event.accept()


if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = Window()
    window.show()
    sys.exit(app.exec())
