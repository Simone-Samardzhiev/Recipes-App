from functions import Data
from tkinter import Tk, Label, Button, Listbox, Text, Entry

data = Data()


class NewRecipe(Tk):
    def __init__(self, function):
        super().__init__()

        self.title("Add new recipe")

        Label(self, text="Enter the name of your recipe").grid(row=0, column=0, pady=5)

        self.entryForName = Entry(self)
        self.entryForName.grid(row=0, column=1, pady=5)

        self.options = Listbox(self, selectmode="SINGLE")
        self.options.grid(row=1, column=0, padx=5, pady=5)
        for key in data.iterOverKeys():
            self.options.insert("end", key)

        Label(self, text="Enter the ingredients").grid(row=2, column=0, padx=5, pady=5)

        self.textForIngredients = Text(self, height=10)
        self.textForIngredients.grid(row=3, column=0, padx=5, pady=5)

        Label(self, text="Enter how its made").grid(row=4, column=0, padx=5, pady=5)

        self.textForWay = Text(self, height=10)
        self.textForWay.grid(row=5, column=0, padx=5, pady=5)

        Button(self, text="Create", command=lambda: self.createRecipe(function)).grid(row=6, column=0, padx=5, pady=5)

        self.mainloop()

    def createRecipe(self, function) -> None:
        key = self.options.get(self.options.curselection())
        recipe = {
            "way": self.textForWay.get("1.0", "end-1c"),
            "index": data.getIndex(key),
            "ingredients": self.textForIngredients.get("1.0", "end-1c"),
            "name": self.entryForName.get()
        }

        function(key, recipe)


# class that will open a widow to show info about the clicked recipe
class RecipeInfo(Tk):
    def __init__(self, dish: dict, function) -> None:
        super().__init__()

        self.geometry("400x400+500+500")
        self.title(dish["name"])
        self.columnconfigure(0, weight=1)

        text = (f"Name : {dish['name']}\n"
                f"Ingredients :\n{dish['ingredients']} \n"
                f"Recipe :\n{dish['way']}")
        Label(self, text=text, justify='left', font=20).grid(row=0, column=0)

        Button(self, text="delete", command=lambda: self.deleteButtonPressed(function)).grid(row=1, column=0)

        self.mainloop()

    def deleteButtonPressed(self, function) -> None:
        function()
        self.destroy()


class Window(Tk):
    def __init__(self) -> None:
        super().__init__()

        self.geometry("850x600+200+200")
        self.title("Recipe app")
        self.protocol("WM_DELETE_WINDOW", self.onClose)

        self.renderGUI()

    # function that will redner the GUI
    def renderGUI(self) -> None:
        for child in self.winfo_children():
            child.destroy()

        Button(self, text="add new dish", command=lambda: NewRecipe(self.addNewRecipe)).grid(row=0, column=3, padx=10,
                                                                                             pady=10)

        col = 0
        for key in data.iterOverKeys():
            Label(self, text=key, font=20).grid(row=1, column=col, padx=10, pady=10)
            col += 1

        col = 0
        for key in data.iterOverKeys():
            row = 2
            for dish in data.iterOverDishes(key):
                Button(self,
                       text=dish["name"],
                       command=lambda k=key, idx=dish["index"]: RecipeInfo(dish, lambda: self.deleteButtonPressed(k,
                                                                                                                  idx))).grid(
                    row=row, column=col, padx=10, pady=10)
                row += 1
            col += 1

    # function that will delete a dish from the data and the GUI
    def deleteButtonPressed(self, key: str, index: int) -> None:
        data.deleteDish(key, index)
        self.renderGUI()

    # function that will be activated when adding a new dish
    def addNewRecipe(self, key: str, recipe: dict) -> None:
        data.addNewDish(key, recipe)
        self.renderGUI()

    # function that will update the data when the app is closed
    def onClose(self) -> None:
        data.writeData()
        self.destroy()


def main():
    wnd = Window()
    wnd.mainloop()


if __name__ == '__main__':
    main()
